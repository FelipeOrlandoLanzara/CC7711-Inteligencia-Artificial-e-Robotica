/*
 * File:          e-puck_wander_with_boxes.c
 * Description:   Vagueio aleatório para e-puck no Webots,
 *                com giro imediato, fase de “escape” e leitura de posições
 *                de caixas via Supervisor.
 */

#include <stdio.h>                 // printf, sprintf
#include <stdlib.h>                // srand, rand, RAND_MAX
#include <time.h>                  // time (para seed de rand)
#include <string.h>                // strcpy, etc.
#include <webots/robot.h>          // wb_robot_init, wb_robot_step, wb_robot_get_time, wb_robot_cleanup
#include <webots/motor.h>          // wb_motor_* (motores)
#include <webots/distance_sensor.h>// wb_distance_sensor_* (sensores de proximidade)
#include <webots/led.h>            // wb_led_* (se precisar de LEDs)
#include <webots/supervisor.h>     // wb_supervisor_* (Supervisor)
#include <math.h>

/* --- Configurações de simulação e robô --- */
#define TIME_STEP         64      // ms, passo de simulação
#define N_SENSORS         8       // ps0…ps7
#define WHEEL_RADIUS      0.0205  // m
#define V_LINEAR          0.06    // m/s desejado
#define MAX_VELOCITY      (V_LINEAR / WHEEL_RADIUS)  // ≈ 2.93 rad/s
#define VELOCITY_TURN     MAX_VELOCITY              // rad/s para giro
#define OBST_THRESHOLD    100.0   // leitura que indica obstáculo
#define ESCAPE_TIME       0.2     // s que avança após giro
#define QtddCaixa         18      // número de caixas no mundo


int main(int argc, char **argv) {
  double posicao_original_caixas[QtddCaixa];
   
   
  typedef struct{
    double x;
    double y;
    double z;
  }PosicaoCaixas;
  
  int posicao_caixas_adicionada = 0;
  wb_robot_init();
  srand(time(NULL));

  /* --- 1) Inicializa referências de caixas (Supervisor) --- */
  WbNodeRef caixa[QtddCaixa];
  char nomeCaixa[10];
  for (int i = 0; i < QtddCaixa; i++) {
    sprintf(nomeCaixa, "CAIXA%02d", i);
    caixa[i] = wb_supervisor_node_get_from_def(nomeCaixa);
    if (caixa[i] != NULL)
      printf("%2d. %s encontrada em %p\n", i, nomeCaixa, (void*)caixa[i]);
    else
      printf("Falha ao carregar a posição da %s\n", nomeCaixa);
  }
  
  printf("\n== CAIXAS OK ==\n\n");
  
  PosicaoCaixas originais[QtddCaixa];
  
  printf("POSIÇÃO ORIGINAL DAS CAIXAS:\n");
  for(int i = 0 ; i < QtddCaixa; i++){
        const double *pos = wb_supervisor_node_get_position(caixa[i]);
        
        
        
        originais[i].x = pos[0];
        originais[i].y = pos[1];
        originais[i].z = pos[2];
        
        printf("CAIXA%02d: %5.2f, %5.2f, %5.2f\n",
               i, originais[i].x, originais[i].y, originais[i].z);
  }
  
  

  /* --- 2) Inicialização dos sensores de proximidade --- */
  WbDeviceTag ps[N_SENSORS];
  char ps_name[5];
  for (int i = 0; i < N_SENSORS; i++) {
    sprintf(ps_name, "ps%d", i);
    ps[i] = wb_robot_get_device(ps_name);
    wb_distance_sensor_enable(ps[i], TIME_STEP);
  }

  /* --- 3) Inicialização dos motores de roda --- */
  WbDeviceTag left_motor  = wb_robot_get_device("left wheel motor");
  WbDeviceTag right_motor = wb_robot_get_device("right wheel motor");
  wb_motor_set_position(left_motor,  INFINITY);
  wb_motor_set_position(right_motor, INFINITY);
  wb_motor_set_velocity(left_motor,  0.0);
  wb_motor_set_velocity(right_motor, 0.0);

  /* --- 4) Loop principal --- */
  while (wb_robot_step(TIME_STEP) != -1) {
    // 4.1) Detecta obstáculo pelos sensores de proximidade
    int obstacle = 0;
    for (int i = 0; i < N_SENSORS; i++) {
      if (wb_distance_sensor_get_value(ps[i]) > OBST_THRESHOLD) {
        obstacle = 1;
        break;
      }
    }

    if (!obstacle) {
      // 4.2) Sem obstáculo: segue em frente
      wb_motor_set_velocity(left_motor,  MAX_VELOCITY);
      wb_motor_set_velocity(right_motor, MAX_VELOCITY);
    } else {
      // 4.3) Obstáculo: gira imediatamente
      int dir = (rand() % 2) ? +1 : -1;                // +1 = esquerda, -1 = direita
      double duration = 0.5 + ((double)rand() / RAND_MAX); // 0.5–1.5 s
      double start_time = wb_robot_get_time();
      // Executa giro
      while (wb_robot_step(TIME_STEP) != -1 &&
             wb_robot_get_time() - start_time < duration) {
        wb_motor_set_velocity(left_motor,  dir * VELOCITY_TURN);
        wb_motor_set_velocity(right_motor, -dir * VELOCITY_TURN);
      }
      // 4.4) Fase de “escape”: avança um pouco para sair do obstáculo
      double escape_start = wb_robot_get_time();
      while (wb_robot_step(TIME_STEP) != -1 &&
             wb_robot_get_time() - escape_start < ESCAPE_TIME) {
        wb_motor_set_velocity(left_motor,  MAX_VELOCITY);
        wb_motor_set_velocity(right_motor, MAX_VELOCITY);
      }
      
    }
    
    
    
    
    

    /* 4.5) A cada passo, imprime posições das caixas */
    
    /*
    printf("   Posicoes das caixas (X, Y, Z):\n");
    for (int i = 0; i < QtddCaixa; i++) {
      const double *pos = wb_supervisor_node_get_position(caixa[i]);
      
      if (pos) {
        printf("CAIXA%02d: %5.2f, %5.2f, %5.2f\n",
               i, pos[0], pos[1], pos[2]);
               
      } else {
        printf("CAIXA%02d: sem referencia\n", i);
      }
    }
    printf("\n");
    */
    
    for (int i = 0; i < QtddCaixa; i++) {
       
       if(caixa[i] == NULL) continue;
       const double *pos = wb_supervisor_node_get_position(caixa[i]);
       double dx = fabs(pos[0] - originais[i].x);
       double dy = fabs(pos[1] - originais[i].y);
       double dz = fabs(pos[2] - originais[i].z);
       
       if (dx > 1e-6 || dy > 1e-6 || dz > 1e-6) {
        printf("CAIXA%02d mudou!   antiga=(%.2f,%.2f,%.2f)   nova=(%.2f,%.2f,%.2f)\n",
               i,
               originais[i].x, originais[i].y, originais[i].z,
               pos[0], pos[1], pos[2]);
       
        }
    
    }
  }

  /* --- 5) Cleanup --- */
  wb_robot_cleanup();
  return 0;
}