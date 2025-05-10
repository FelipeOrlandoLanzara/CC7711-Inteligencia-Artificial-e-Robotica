/*

 * File:          Controlador.c

 * Date:

 * Description:

 * Author:

 * Modifications:

 */



/*

 * You may need to add include files like <webots/distance_sensor.h> or

 * <webots/motor.h>, etc.

 */

#include <stdio.h>

#include <webots/robot.h>

#include <webots/motor.h>

#include <webots/distance_sensor.h>

#include <webots/led.h>



/*

 * You may want to add macros here.

 */

#define TIME_STEP 256

#define QtddSensoresProx 8

#define QtddLeds 10



/*

 * This is the main program.

 * The arguments of the main function can be specified by the

 * "controllerArgs" field of the Robot node

 */
 
 


int main(int argc, char **argv) {

  

  int i=0;

  char texto[256];

  double LeituraSensorProx[QtddSensoresProx];

  double AceleradorDireito=1.0, AceleradorEsquerdo=1.0;

  /* necessary to initialize webots stuff */

  for(i=0;i<257;i++) texto[i]='0';

  

  wb_robot_init();


  

  //configurei MOTORES

  WbDeviceTag MotorEsquerdo, MotorDireito;

  

  MotorEsquerdo = wb_robot_get_device("left wheel motor");

  MotorDireito = wb_robot_get_device("right wheel motor");

  

  wb_motor_set_position(MotorEsquerdo, INFINITY);

  wb_motor_set_position(MotorDireito, INFINITY);

  

  wb_motor_set_velocity(MotorEsquerdo,0);

  wb_motor_set_velocity(MotorDireito,0);

  

  

   //configurei Sensores de Proximidade

   WbDeviceTag SensorProx[QtddSensoresProx];

   

   SensorProx[0] = wb_robot_get_device("ps0");

   SensorProx[1] = wb_robot_get_device("ps1");

   SensorProx[2] = wb_robot_get_device("ps2");

   SensorProx[3] = wb_robot_get_device("ps3");

   SensorProx[4] = wb_robot_get_device("ps4");

   SensorProx[5] = wb_robot_get_device("ps5");

   SensorProx[6] = wb_robot_get_device("ps6");

   SensorProx[7] = wb_robot_get_device("ps7");

   

   wb_distance_sensor_enable(SensorProx[0],TIME_STEP);

   wb_distance_sensor_enable(SensorProx[1],TIME_STEP);

   wb_distance_sensor_enable(SensorProx[2],TIME_STEP);

   wb_distance_sensor_enable(SensorProx[3],TIME_STEP);

   wb_distance_sensor_enable(SensorProx[4],TIME_STEP);

   wb_distance_sensor_enable(SensorProx[5],TIME_STEP);

   wb_distance_sensor_enable(SensorProx[6],TIME_STEP);

   wb_distance_sensor_enable(SensorProx[7],TIME_STEP);



    //config leds

    WbDeviceTag Leds[QtddLeds];

    Leds[0] = wb_robot_get_device("led0");
    Leds[1] = wb_robot_get_device("led1");
    Leds[2] = wb_robot_get_device("led2");
    Leds[3] = wb_robot_get_device("led3");
    Leds[4] = wb_robot_get_device("led4");
    Leds[5] = wb_robot_get_device("led5");
    Leds[6] = wb_robot_get_device("led6");
    Leds[7] = wb_robot_get_device("led7");
    Leds[8] = wb_robot_get_device("led9");

    wb_led_set(Leds[0],-1);
    wb_led_set(Leds[1],-1);
    wb_led_set(Leds[2],-1);
    wb_led_set(Leds[3],-1);
    wb_led_set(Leds[4],-1);
    wb_led_set(Leds[5],-1);
    wb_led_set(Leds[6],-1);
    wb_led_set(Leds[7],-1);
    wb_led_set(Leds[8],-1);
    

  

  

  /*

   * You should declare here WbDeviceTag variables for storing

   * robot devices like this:

   *  WbDeviceTag my_sensor = wb_robot_get_device("my_sensor");

   *  WbDeviceTag my_actuator = wb_robot_get_device("my_actuator");

   */



  /* main loop

   * Perform simulation steps of TIME_STEP milliseconds

   * and leave the loop when the simulation is over

   */

  int girou = 0;  // Flag para saber se o robô já girou
  int contador = 0;
  
while (wb_robot_step(TIME_STEP) != -1) {
  
  for(i=0;i<256;i++) texto[i]=0;

  for(i=0;i<QtddSensoresProx;i++){
    LeituraSensorProx[i]= wb_distance_sensor_get_value(SensorProx[i]) - 60;
    sprintf(texto,"%s|%d: %5.2f  ",texto,i,LeituraSensorProx[i]);
  }

  printf("%s\n", texto);
  wb_led_set(Leds[0], wb_led_get(Leds[0]) * -1);  // Pisca LED 0

  // Se detectar parede e ainda não girou
  if((LeituraSensorProx[0] > 1400 || LeituraSensorProx[7] > 1400) && girou == 0) {
    contador += 1;
    printf("Contador = %d\n",contador);
    printf("oi");
    printf("Parede detectada. Parando e girando...\n");
    
    if(contador % 2 == 0){
      wb_motor_set_velocity(MotorEsquerdo, 0);
    wb_motor_set_velocity(MotorDireito, 0);
    wb_robot_step(TIME_STEP);  // Espera um tempo
    
    // Espera tempo suficiente para girar 90 graus
    int passosGiro = (int)((1000.0 / TIME_STEP) * 2.0);  // ~1 segundo
    
    // Gira 90 graus no sentido anti-horário (esquerda para trás, direita para frente)
    wb_motor_set_velocity(MotorEsquerdo, -6.28);
    wb_motor_set_velocity(MotorDireito, 6.28);
    
    for (int j = 0; j < passosGiro; j++) {
      wb_robot_step(TIME_STEP);
    }
    
    // Para após o giro
    wb_motor_set_velocity(MotorEsquerdo, 0);
    wb_motor_set_velocity(MotorDireito, 0);
    wb_robot_step(TIME_STEP);
    
    // Anda para frente por um curto tempo (~0.5 segundos)
    int passosAndar = (int)((1000.0 / TIME_STEP) * 2.0);  // meio segundo
    
    wb_motor_set_velocity(MotorEsquerdo, 6.28);
    wb_motor_set_velocity(MotorDireito, 6.28);
    
    for (int j = 0; j < passosAndar; j++) {
      wb_robot_step(TIME_STEP);
    }
    
    // Para o robô antes do segundo giro
    wb_motor_set_velocity(MotorEsquerdo, 0);
    wb_motor_set_velocity(MotorDireito, 0);
    wb_robot_step(TIME_STEP);
    
    // Gira mais 90 graus no sentido anti-horário (esquerda)
    wb_motor_set_velocity(MotorEsquerdo, -6.28);
    wb_motor_set_velocity(MotorDireito, 6.28);
    for (int j = 0; j < passosGiro; j++) {
      wb_robot_step(TIME_STEP);
    }
    
    // Para após o segundo giro
    wb_motor_set_velocity(MotorEsquerdo, 0);
    wb_motor_set_velocity(MotorDireito, 0);
    wb_robot_step(TIME_STEP);
    
    girou = 1;  // Marca que já girou, pra não repetir o giro
    
    
    
    
    
      
    }else{
      // Para o robô
    wb_motor_set_velocity(MotorEsquerdo, 0);
    wb_motor_set_velocity(MotorDireito, 0);
    wb_robot_step(TIME_STEP);  // Espera um tempo

    
    // Espera tempo suficiente para girar 90 graus
    int passosGiro = (int)((1000.0 / TIME_STEP) * 2.0);  // ~1 segundo
    
    // Gira 90 graus no sentido horário (esquerda para frente, direita para trás)
    wb_motor_set_velocity(MotorEsquerdo, 6.28);
    wb_motor_set_velocity(MotorDireito, -6.28);

    
    for (int j = 0; j < passosGiro; j++) {
      wb_robot_step(TIME_STEP);
    }

    // Para após o giro
    wb_motor_set_velocity(MotorEsquerdo, 0);
    wb_motor_set_velocity(MotorDireito, 0);
    wb_robot_step(TIME_STEP);
    
    // Anda para frente por um curto tempo (~0.5 segundos)
    int passosAndar = (int)((1000.0 / TIME_STEP) * 2.0);  // meio segundo
    
    wb_motor_set_velocity(MotorEsquerdo, 6.28);
    wb_motor_set_velocity(MotorDireito, 6.28);
    
    for (int j = 0; j < passosAndar; j++) {
      wb_robot_step(TIME_STEP);
    }
    
    // Para o robô antes do segundo giro
    wb_motor_set_velocity(MotorEsquerdo, 0);
    wb_motor_set_velocity(MotorDireito, 0);
    wb_robot_step(TIME_STEP);
    
    // Gira mais 90 graus no sentido anti-horário (esquerda)
    wb_motor_set_velocity(MotorEsquerdo, -6.28);
    wb_motor_set_velocity(MotorDireito, 6.28);
    for (int j = 0; j < passosGiro; j++) {
      wb_robot_step(TIME_STEP);
    }
    
    // Para após o segundo giro
    wb_motor_set_velocity(MotorEsquerdo, 0);
    wb_motor_set_velocity(MotorDireito, 0);
    wb_robot_step(TIME_STEP);
    
    girou = 1;  // Marca que já girou, pra não repetir o giro
      
    }
    
  }
  
  else if (LeituraSensorProx[0] <= 1400 && LeituraSensorProx[7] <= 1400) {
    // Continua andando
    wb_motor_set_velocity(MotorEsquerdo, 6.28);
    wb_motor_set_velocity(MotorDireito, 6.28);
    girou = 0;  // Reseta a flag para permitir nova rotação
  }
  
  if((LeituraSensorProx[0] > 100 || LeituraSensorProx[7] > 100) && (LeituraSensorProx[0] <= 400 || LeituraSensorProx[7] <= 400)) {
      printf("CAIXA!!\n");
      wb_motor_set_velocity(MotorEsquerdo, 0);
      wb_motor_set_velocity(MotorDireito, 0);
      wb_robot_step(TIME_STEP);  // Espera um tempo
      
      // Espera tempo suficiente para girar 90 graus
    int passosGiro = (int)((1000.0 / TIME_STEP) * 2.0);  // ~1 segundo
    
    // Gira 90 graus no sentido horário (esquerda para frente, direita para trás)
    wb_motor_set_velocity(MotorEsquerdo, 6.28);
    wb_motor_set_velocity(MotorDireito, -6.28);

    
    for (int j = 0; j < passosGiro; j++) {
      wb_robot_step(TIME_STEP);
    }

    // Para após o giro
    wb_motor_set_velocity(MotorEsquerdo, 0);
    wb_motor_set_velocity(MotorDireito, 0);
    wb_robot_step(TIME_STEP);
    
    // Anda para frente por um curto tempo (~0.5 segundos)
    int passosAndar = (int)((1000.0 / TIME_STEP) * 1.4);  // meio segundo
    
    wb_motor_set_velocity(MotorEsquerdo, 6.28);
    wb_motor_set_velocity(MotorDireito, 6.28);
    
    for (int j = 0; j < passosAndar; j++) {
      wb_robot_step(TIME_STEP);
    }
    
    // Para o robô antes do segundo giro
    wb_motor_set_velocity(MotorEsquerdo, 0);
    wb_motor_set_velocity(MotorDireito, 0);
    wb_robot_step(TIME_STEP);
    
    // Gira mais 90 graus no sentido anti-horário (esquerda)
    wb_motor_set_velocity(MotorEsquerdo, -6.28);
    wb_motor_set_velocity(MotorDireito, 6.28);
    
    for (int j = 0; j < passosGiro; j++) {
      wb_robot_step(TIME_STEP);
    }
    
    // Para após o segundo giro
    wb_motor_set_velocity(MotorEsquerdo, 0);
    wb_motor_set_velocity(MotorDireito, 0);
    wb_robot_step(TIME_STEP);
    
     // Anda para frente por um curto tempo (~0.5 segundos)
    int passosAndar2 = (int)((1000.0 / TIME_STEP) * 2.1);  // meio segundo
    
    wb_motor_set_velocity(MotorEsquerdo, 6.28);
    wb_motor_set_velocity(MotorDireito, 6.28);
    
    for (int j = 0; j < passosAndar2; j++) {
      wb_robot_step(TIME_STEP);
    }
    
    // Para o robô antes do segundo giro
    wb_motor_set_velocity(MotorEsquerdo, 0);
    wb_motor_set_velocity(MotorDireito, 0);
    wb_robot_step(TIME_STEP);
    
    // Gira mais 90 graus no sentido anti-horário (esquerda)
    wb_motor_set_velocity(MotorEsquerdo, -6.28);
    wb_motor_set_velocity(MotorDireito, 6.28);
    for (int j = 0; j < passosGiro; j++) {
      wb_robot_step(TIME_STEP);
    }
    
    // Para após o segundo giro
    wb_motor_set_velocity(MotorEsquerdo, 0);
    wb_motor_set_velocity(MotorDireito, 0);
    wb_robot_step(TIME_STEP);
    
    
    
    
    
    girou = 1;  // Marca que já girou, pra não repetir o giro
  }
  
  }
}