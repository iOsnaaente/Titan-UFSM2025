// /*  
//  *  Description: Controle do motor DC para o projeto Titan Ver:2.1.2  
//  *  Author: Bruno G. F. Sampaio
//  *  Date: 15/03/2024
//  *  License: MIT
//  *
// */

// #include "driver/gpio.h"
// #include "driver/ledc.h"
// #include "esp_timer.h"

// #include "pinout.h" 

// /* Constantes para configuração do LEDC */
// #define MOTORS_LEDC_MODE                LEDC_HIGH_SPEED_MODE            //    
// #define MOTORS_LEDC_RESOLUTION          LEDC_TIMER_13_BIT               // 
// #define MOTORS_LEDC_TIMER               LEDC_TIMER_0                    //          

// #define MOTOR_ENB_RIGHT_LEDC_CHANNEL    LEDC_CHANNEL_0                  //   
// #define MOTOR_ENB_MIDDLE_LEDC_CHANNEL   LEDC_CHANNEL_1                  //          
// #define MOTOR_ENB_LEFT_LEDC_CHANNEL     LEDC_CHANNEL_2                  //          

// #define MOTORS_LEDC_FREQ                ((uint32_t)1000)                //

// /* Constantes para definir valores mínimos e máximo de potência */
// #define BDC_TURN_OFF                    ((double)(0))                   //        
// #define BDC_MIN_POWER                   ((double)(0))                   //          
// #define BDC_MAX_POWER                   ((double)(8192))                //            

// /* Constantes para estados on-off */
// #define BDC_ENABLE                      ((bool)(true))                  // 
// #define BDC_DISABLE                     ((bool)(false))                 // 

// /* Constantes para definir o estado de mocimento do motor */
// #define BDC_STOPED                      ((uint8_t)(0))                  //   
// #define BDC_BREAK                       ((uint8_t)(1))                  //   
// #define BDC_FORWARD                     ((uint8_t)(2))                  //   
// #define BDC_BACKWARD                    ((uint8_t)(3))                  //   

// /* Identificação dos motores */
// #define BDC_MOTOR_RIGTH                 ((uint8_t)(0))                  //   
// #define BDC_MOTOR_MIDDLE                ((uint8_t)(1))                  //   
// #define BDC_MOTOR_LEFT                  ((uint8_t)(2))                  //   


// /* 13 bits de resolução = 8192 de duty máximo */
// void bdc_motor_set_speed( uint8_t motor_id, double speed) {
//   // Limita os valores entre MIN e MAX 
//   if (speed < BDC_MIN_POWER) {
//     speed = BDC_MIN_POWER;
//   } else if ( speed > BDC_MAX_POWER ) {
//     speed = BDC_MAX_POWER;
//   }
//   // Calcula a normalização linear
//   uint32_t duty = (uint32_t)(((speed - BDC_MIN_POWER) / (BDC_MAX_POWER - BDC_MIN_POWER))*BDC_MAX_POWER);
//   // Right motor 
//   if ( motor_id == BDC_MOTOR_RIGTH ){
//     ledc_set_duty( MOTORS_LEDC_MODE, MOTOR_ENB_RIGHT_LEDC_CHANNEL, duty);
//     ledc_update_duty( MOTORS_LEDC_MODE, MOTOR_ENB_RIGHT_LEDC_CHANNEL);  
//   // Middle motor 
//   } else if ( motor_id == BDC_MOTOR_MIDDLE ){
//     ledc_set_duty( MOTORS_LEDC_MODE, MOTOR_ENB_MIDDLE_LEDC_CHANNEL, duty);
//     ledc_update_duty( MOTORS_LEDC_MODE, MOTOR_ENB_MIDDLE_LEDC_CHANNEL);
//   // Left motor 
//   } else if ( motor_id == BDC_MOTOR_LEFT ){
//     ledc_set_duty( MOTORS_LEDC_MODE, MOTOR_ENB_LEFT_LEDC_CHANNEL, duty);
//     ledc_update_duty( MOTORS_LEDC_MODE, MOTOR_ENB_LEFT_LEDC_CHANNEL);
//   }
// }

// uint8_t bdc_motor_forward( uint8_t motor_id ) {
//   bdc_motor_enable( motor_id );
//   // Right Motor 
//   if ( motor_id == BDC_MOTOR_RIGTH ){
//     gpio_set_level( MOTOR_RIGHT_INA_GPIO, true);
//     gpio_set_level( MOTOR_RIGHT_INB_GPIO, false);
//   // Middle Motor 
//   } else if ( motor_id == BDC_MOTOR_LEFT ) {
//     gpio_set_level( MOTOR_MIDDLE_INA_GPIO, true);
//     gpio_set_level( MOTOR_MIDDLE_INB_GPIO, false);
//   // Left Motor 
//   } else if ( motor_id == BDC_MOTOR_LEFT ){
//     gpio_set_level( MOTOR_LEFT_INA_GPIO, true);
//     gpio_set_level( MOTOR_LEFT_INB_GPIO, false);
//   }
//   return BDC_FORWARD;
// }

// uint8_t bdc_motor_backward( uint8_t motor_id ) {
//   bdc_motor_enable( motor_id );
//   // Right Motor 
//   if ( motor_id == BDC_MOTOR_RIGTH ){
//     gpio_set_level( MOTOR_RIGHT_INA_GPIO, false);
//     gpio_set_level( MOTOR_RIGHT_INB_GPIO, true);
//   // Middle motor 
//   } else if ( motor_id == BDC_MOTOR_MIDDLE ) {
//     gpio_set_level( MOTOR_MIDDLE_INA_GPIO, false);
//     gpio_set_level( MOTOR_MIDDLE_INB_GPIO, true);
//   // Left motor
//   } else if ( motor_id == BDC_MOTOR_LEFT ){
//     gpio_set_level( MOTOR_LEFT_INA_GPIO, false);
//     gpio_set_level( MOTOR_LEFT_INB_GPIO, true);
//   }
//   return BDC_BACKWARD;
// }

// uint8_t bdc_motor_stop( uint8_t motor_id ) {
//   bdc_motor_disable( motor_id );

//   // Right Motor 
//   if ( motor_id == BDC_MOTOR_RIGTH ){
//     gpio_set_level( MOTOR_RIGHT_INA_GPIO, false);
//     gpio_set_level( MOTOR_RIGHT_INB_GPIO, false);
//   // Middle Motor 
//   } else if ( motor_id == BDC_MOTOR_MIDDLE ) {
//     gpio_set_level( MOTOR_MIDDLE_INA_GPIO, false);
//     gpio_set_level( MOTOR_MIDDLE_INB_GPIO, false);
//   // Left Motor 
//   } else if ( motor_id == BDC_MOTOR_LEFT ){
//     gpio_set_level( MOTOR_LEFT_INA_GPIO, false);
//     gpio_set_level( MOTOR_LEFT_INB_GPIO, false);
//   }
//   return BDC_STOPED;
// }

// uint8_t bdc_motor_brake( uint8_t motor_id ) {
//   bdc_motor_disable( motor_id );
//   // Right Motor 
//   if ( motor_id == BDC_MOTOR_RIGTH ){
//     gpio_set_level( MOTOR_RIGHT_INA_GPIO, true);
//     gpio_set_level( MOTOR_RIGHT_INB_GPIO, true);
//   // Middle Motor 
//   } else if ( motor_id == BDC_MOTOR_MIDDLE ) {
//     gpio_set_level( MOTOR_MIDDLE_INA_GPIO, true);
//     gpio_set_level( MOTOR_MIDDLE_INB_GPIO, true);
//   // Left Motor 
//   } else if ( motor_id == BDC_MOTOR_LEFT ){
//     gpio_set_level( MOTOR_LEFT_INA_GPIO, true);
//     gpio_set_level( MOTOR_LEFT_INB_GPIO, true);
//   }
//   return BDC_BREAK;
// }

// static inline void bdc_motor_enable( uint8_t motor_id ) {
//     // Right Motor 
//   if ( motor_id == BDC_MOTOR_RIGTH ){
//     gpio_set_level( MOTOR_RIGHT_ENB_GPIO, BDC_ENABLE );
//   // Middle Motor 
//   } else if ( motor_id == BDC_MOTOR_MIDDLE ) {
//     gpio_set_level( MOTOR_MIDDLE_ENB_GPIO, BDC_ENABLE );
//   // Left Motor 
//   } else if ( motor_id == BDC_MOTOR_LEFT ){
//     gpio_set_level( MOTOR_LEFT_ENB_GPIO, BDC_ENABLE );
//   }
// }

// static inline void bdc_motor_disable( uint8_t motor_id ) {
//   // Right Motor 
//   if ( motor_id == BDC_MOTOR_RIGTH ){
//     gpio_set_level( MOTOR_RIGHT_ENB_GPIO, BDC_DISABLE );
//   // Middle Motor 
//   } else if ( motor_id == BDC_MOTOR_MIDDLE ) {
//     gpio_set_level( MOTOR_MIDDLE_ENB_GPIO, BDC_DISABLE);
//   // Left Motor 
//   } else if ( motor_id == BDC_MOTOR_LEFT ){
//     gpio_set_level( MOTOR_LEFT_ENB_GPIO, BDC_DISABLE );
//   }
// }
