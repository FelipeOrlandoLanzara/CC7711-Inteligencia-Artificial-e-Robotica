/*
 * File:          Controlador.c
 * Description:   Controlador de movimento do e-puck com leitura de caixas do mundo
 */

#include <stdio.h>
#include <string.h>
#include <webots/robot.h>
#include <webots/motor.h>
#include <webots/distance_sensor.h>
#include <webots/led.h>
#include <webots/supervisor.h>  // NOVO

#define TIME_STEP 256
#define QtddSensoresProx 8
#define QtddLeds 10
#define QtddCaixa 18  // NOVO

int main(int argc, char **argv) {
  int i = 0;
  char texto[256];
  double LeituraSensorProx[QtddSensoresProx];
  double AceleradorDireito = 1.0, AceleradorEsquerdo = 1.0;

  for (i = 0; i < 257; i++) texto[i] = '0';

  wb_robot_init();

  // Motores
  WbDeviceTag MotorEsquerdo = wb_robot_get_device("left wheel motor");
  WbDeviceTag MotorDireito = wb_robot_get_device("right wheel motor");
  wb_motor_set_position(MotorEsquerdo, INFINITY);
  wb_motor_set_position(MotorDireito, INFINITY);
  wb_motor_set_velocity(MotorEsquerdo, 0);
  wb_motor_set_velocity(MotorDireito, 0);

  // Sensores de Proximidade
  WbDeviceTag SensorProx[QtddSensoresProx] = {
    wb_robot_get_device("ps0"), wb_robot_get_device("ps1"),
    wb_robot_get_device("ps2"), wb_robot_get_device("ps3"),
    wb_robot_get_device("ps4"), wb_robot_get_device("ps5"),
    wb_robot_get_device("ps6"), wb_robot_get_device("ps7")
  };

  for (i = 0; i < QtddSensoresProx; i++)
    wb_distance_sensor_enable(SensorProx[i], TIME_STEP);

  // LEDs
  WbDeviceTag Leds[QtddLeds] = {
    wb_robot_get_device("led0"), wb_robot_get_device("led1"),
    wb_robot_get_device("led2"), wb_robot_get_device("led3"),
    wb_robot_get_device("led4"), wb_robot_get_device("led5"),
    wb_robot_get_device("led6"), wb_robot_get_device("led7"),
    wb_robot_get_device("led9")
  };

  for (i = 0; i < 9; i++)
    wb_led_set(Leds[i], -1);

  // Caixas (Supervisor)
  WbNodeRef caixa[QtddCaixa];
  char nomeCaixa[10] = {0};

  for (i = 0; i < QtddCaixa; i++) {
    sprintf(nomeCaixa, "CAIXA%02d", i);
    caixa[i] = wb_supervisor_node_get_from_def(nomeCaixa);
    if (caixa[i] != NULL)
      printf("%2d. %s  -  %p\n", i, nomeCaixa, (void *)caixa[i]);
    else
      printf("Falha ao carregar a posição da %s\n", nomeCaixa);
  }
  printf("\n\n CAIXAS OK  \n\n");

  // Lógica de movimento
  int girou = 0;
  int contador = 0;

  while (wb_robot_step(TIME_STEP) != -1) {
    for (i = 0; i < 256; i++) texto[i] = 0;

    for (i = 0; i < QtddSensoresProx; i++) {
      LeituraSensorProx[i] = wb_distance_sensor_get_value(SensorProx[i]) - 60;
      sprintf(texto, "%s|%d: %5.2f  ", texto, i, LeituraSensorProx[i]);
    }

    printf("%s\n", texto);
    wb_led_set(Leds[0], wb_led_get(Leds[0]) * -1);

    if ((LeituraSensorProx[0] > 1400 || LeituraSensorProx[7] > 1400) && girou == 0) {
      printf("Parede detectada. Parando e girando...\n");

      if (contador % 2 == 0) {
        wb_motor_set_velocity(MotorEsquerdo, 0);
        wb_motor_set_velocity(MotorDireito, 0);
        wb_robot_step(TIME_STEP);

        int passosGiro = (int)((1000.0 / TIME_STEP) * 2.0);
        wb_motor_set_velocity(MotorEsquerdo, 6.28);
        wb_motor_set_velocity(MotorDireito, -6.28);

        for (int j = 0; j < passosGiro; j++) wb_robot_step(TIME_STEP);

        wb_motor_set_velocity(MotorEsquerdo, 0);
        wb_motor_set_velocity(MotorDireito, 0);
        wb_robot_step(TIME_STEP);

        int passosAndar = (int)((1000.0 / TIME_STEP) * 2.0);
        int passosGiro2 = (int)((1000.0 / TIME_STEP) * 2.1);

        wb_motor_set_velocity(MotorEsquerdo, 6.28);
        wb_motor_set_velocity(MotorDireito, 6.28);
        for (int j = 0; j < passosAndar; j++) wb_robot_step(TIME_STEP);

        wb_motor_set_velocity(MotorEsquerdo, 0);
        wb_motor_set_velocity(MotorDireito, 0);
        wb_robot_step(TIME_STEP);

        wb_motor_set_velocity(MotorEsquerdo, 6.28);
        wb_motor_set_velocity(MotorDireito, -6.28);
        for (int j = 0; j < passosGiro2; j++) wb_robot_step(TIME_STEP);

        wb_motor_set_velocity(MotorEsquerdo, 0);
        wb_motor_set_velocity(MotorDireito, 0);
        wb_robot_step(TIME_STEP);

        girou = 1;
      } else {
        wb_motor_set_velocity(MotorEsquerdo, 0);
        wb_motor_set_velocity(MotorDireito, 0);
        wb_robot_step(TIME_STEP);

        int passosGiro = (int)((1000.0 / TIME_STEP) * 2.0);
        wb_motor_set_velocity(MotorEsquerdo, -6.28);
        wb_motor_set_velocity(MotorDireito, 6.28);

        for (int j = 0; j < passosGiro; j++) wb_robot_step(TIME_STEP);

        wb_motor_set_velocity(MotorEsquerdo, 0);
        wb_motor_set_velocity(MotorDireito, 0);
        wb_robot_step(TIME_STEP);

        int passosAndar = (int)((1000.0 / TIME_STEP) * 2.0);
        int passosGiro2 = (int)((1000.0 / TIME_STEP) * 2.0);

        wb_motor_set_velocity(MotorEsquerdo, 6.28);
        wb_motor_set_velocity(MotorDireito, 6.28);
        for (int j = 0; j < passosAndar; j++) wb_robot_step(TIME_STEP);

        wb_motor_set_velocity(MotorEsquerdo, 0);
        wb_motor_set_velocity(MotorDireito, 0);
        wb_robot_step(TIME_STEP);

        wb_motor_set_velocity(MotorEsquerdo, -6.28);
        wb_motor_set_velocity(MotorDireito, 6.28);
        for (int j = 0; j < passosGiro2; j++) wb_robot_step(TIME_STEP);

        wb_motor_set_velocity(MotorEsquerdo, 0);
        wb_motor_set_velocity(MotorDireito, 0);
        wb_robot_step(TIME_STEP);

        girou = 1;
      }

      contador += 1;
      printf("Contador = %d\n", contador);
    } else if (LeituraSensorProx[0] <= 1400 && LeituraSensorProx[7] <= 1400) {
      wb_motor_set_velocity(MotorEsquerdo, 6.28);
      wb_motor_set_velocity(MotorDireito, 6.28);
      girou = 0;
    }
    
    

    // Leitura das posições das caixas
    printf("           X       Y      Z\n");
    for (i = 0; i < QtddCaixa; i++) {
      if (caixa[i] != NULL) {
        const double *PosicaoCaixa = wb_supervisor_node_get_position(caixa[i]);
        printf("CAIXA%02d %5.2f   %5.2f  %5.2f\n", i + 1, PosicaoCaixa[0], PosicaoCaixa[1], PosicaoCaixa[2]);
      }
    }
  }

  wb_robot_cleanup();
  return 0;
}
