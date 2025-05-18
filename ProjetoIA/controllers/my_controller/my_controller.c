#include <stdio.h>                 // printf, sprintf
#include <stdlib.h>                // srand, rand, RAND_MAX
#include <time.h>                  // time (para seed de rand)
#include <string.h>                // strcpy, etc.
#include <webots/robot.h>          // wb_robot_init, wb_robot_step, wb_robot_get_time, wb_robot_cleanup
#include <webots/motor.h>          // wb_motor_* (motores)
#include <webots/distance_sensor.h>// wb_distance_sensor_* (sensores de proximidade)
#include <webots/led.h>            // wb_led_* (LEDs do e-puck)
#include <webots/supervisor.h>     // wb_supervisor_* (Supervisor)
#include <math.h>                  // fabs

#define TIME_STEP         64      // ms, intervalo de cada passo de simulação
#define N_SENSORS         8       // quantidade de sensores de proximidade
#define WHEEL_RADIUS      0.0205  // m, raio das rodas
#define V_LINEAR          0.06    // m/s de velocidade linear desejada
#define MAX_VELOCITY      (V_LINEAR / WHEEL_RADIUS)  // conversão para rad/s
#define VELOCITY_TURN     MAX_VELOCITY              // rad/s para giro
#define OBST_THRESHOLD    100.0   // limiar de leitura para detectar obstáculo
#define ESCAPE_TIME       0.2     // s de fase de “escape” após giro
#define QtddCaixa         18      // número de caixas no mundo
#define QtddLeds          8       // número de LEDs no e-puck

typedef struct {                    // armazena a posição XYZ de cada caixa
  double x;
  double y;
  double z;
} PosicaoCaixas;

int main(int argc, char **argv) {
  wb_robot_init();                  // inicializa o robô
  srand(time(NULL));                // semente para rand()

  // --- Inicializa LEDs ---
  WbDeviceTag leds[QtddLeds];
  char led_name[8];
  for (int i = 0; i < QtddLeds; i++) {
    sprintf(led_name, "led%d", i);          // monta o nome "led0"..."led7"
    leds[i] = wb_robot_get_device(led_name); // obtém a tag do LED
  }

  // --- Carrega referências das caixas pelo DEF ---
  WbNodeRef caixa[QtddCaixa];
  char nomeCaixa[10];
  for (int i = 0; i < QtddCaixa; i++) {
    sprintf(nomeCaixa, "CAIXA%02d", i);              // DEF: CAIXA00, CAIXA01...
    caixa[i] = wb_supervisor_node_get_from_def(nomeCaixa);
    if (caixa[i] != NULL) {
      printf("%2d. %s encontrada em %p\n", i, nomeCaixa, (void*)caixa[i]);
    } else {
      printf("Falha ao carregar a posição da %s\n", nomeCaixa);
    }
  }
  printf("\n== CAIXAS OK ==\n\n");

  // --- Armazena posições iniciais ---
  PosicaoCaixas originais[QtddCaixa];
  printf("POSIÇÃO ORIGINAL DAS CAIXAS:\n");
  for (int i = 0; i < QtddCaixa; i++) {
    const double *pos = wb_supervisor_node_get_position(caixa[i]); // lê posição
    originais[i].x = pos[0];
    originais[i].y = pos[1];
    originais[i].z = pos[2];
    printf("CAIXA%02d: %5.2f, %5.2f, %5.2f\n",
           i, originais[i].x, originais[i].y, originais[i].z);
  }

  // --- Sensores de proximidade ---
  WbDeviceTag ps[N_SENSORS];
  char ps_name[5];
  for (int i = 0; i < N_SENSORS; i++) {
    sprintf(ps_name, "ps%d", i);
    ps[i] = wb_robot_get_device(ps_name);
    wb_distance_sensor_enable(ps[i], TIME_STEP); // habilita sensor
  }

  // --- Motores de roda ---
  WbDeviceTag left_motor  = wb_robot_get_device("left wheel motor");
  WbDeviceTag right_motor = wb_robot_get_device("right wheel motor");
  wb_motor_set_position(left_motor,  INFINITY);  // controle por velocidade
  wb_motor_set_position(right_motor, INFINITY);
  wb_motor_set_velocity(left_motor,  0.0);       // inicializa parado
  wb_motor_set_velocity(right_motor, 0.0);

  // --- Loop principal ---
  while (wb_robot_step(TIME_STEP) != -1) {
    // 1) Detecta obstáculo
    int obstacle = 0;
    for (int i = 0; i < N_SENSORS; i++) {
      if (wb_distance_sensor_get_value(ps[i]) > OBST_THRESHOLD) {
        obstacle = 1;         // encontrou obstáculo
        break;
      }
    }

    // 2) Movimento ou desvio
    if (!obstacle) {
      wb_motor_set_velocity(left_motor,  MAX_VELOCITY);   // vai em frente
      wb_motor_set_velocity(right_motor, MAX_VELOCITY);
    } else {
      int dir = (rand() % 2) ? +1 : -1;                    // escolhe direção
      double duration = 0.5 + ((double)rand() / RAND_MAX); // duração aleatória
      double start_time = wb_robot_get_time();
      // gira até completar 'duration'
      while (wb_robot_step(TIME_STEP) != -1 &&
             wb_robot_get_time() - start_time < duration) {
        wb_motor_set_velocity(left_motor,  dir * VELOCITY_TURN);
        wb_motor_set_velocity(right_motor, -dir * VELOCITY_TURN);
      }
      // escape: avança um pouco
      double escape_start = wb_robot_get_time();
      while (wb_robot_step(TIME_STEP) != -1 &&
             wb_robot_get_time() - escape_start < ESCAPE_TIME) {
        wb_motor_set_velocity(left_motor,  MAX_VELOCITY);
        wb_motor_set_velocity(right_motor, MAX_VELOCITY);
      }
    }

    // 3) Verifica se alguma caixa mudou de posição
    for (int i = 0; i < QtddCaixa; i++) {
      if (caixa[i] == NULL) {
        continue;        // pula se nó inválido
      }
      const double *pos = wb_supervisor_node_get_position(caixa[i]);
      double dx = fabs(pos[0] - originais[i].x);
      double dy = fabs(pos[1] - originais[i].y);
      double dz = fabs(pos[2] - originais[i].z);

      if (dx > 1e-6 || dy > 1e-6 || dz > 1e-6) {
        // detectou movimento real da caixa
        printf("CAIXA%02d mudou!   antiga=(%.5f,%.5f,%.5f)   nova=(%.5f,%.5f,%.5f)\n",
               i,
               originais[i].x, originais[i].y, originais[i].z,
               pos[0], pos[1], pos[2]);

        // para o robô
        wb_motor_set_velocity(left_motor,  0.0);
        wb_motor_set_velocity(right_motor, 0.0);

        // acende todos os LEDs
        for (int j = 0; j < QtddLeds; j++) {
          wb_led_set(leds[j], 1);
        }

        // gira indefinidamente
        while (wb_robot_step(TIME_STEP) != -1) {
          wb_motor_set_velocity(left_motor,   VELOCITY_TURN);
          wb_motor_set_velocity(right_motor, -VELOCITY_TURN);
        }
      }
    }
  }

  wb_robot_cleanup();  // finaliza o controlador
  return 0;
}
