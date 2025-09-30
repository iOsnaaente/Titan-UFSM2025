// /*  
//  *  Description: Tasks de controle dos motores DC para o projeto Titan Ver:2.1.2 
//  *  Author: Bruno G. F. Sampaio
//  *  Date: 07/03/2024
//  *  License: MIT
//  *
//  */

// #include "driver/gpio.h"
// #include "driver/ledc.h"
// #include "esp_timer.h"


// /*         Variáveis de controle     */
// /*              RIGHT  LEFT          */
// double KP[2] = { 1.00, 1.00 };
// double KD[2] = { 0.10, 0.10 };
// double KI[2] = { 0.00, 0.00 };

// /* Ganhos dos sensores */
// double SENSOR_GAIN[2] = { 1, 1 };


// void bdc_motor_right(void* pvParameters) {
//   /* Identificação do motor direito */
//   const int motor_id = 0;
//   /* Variaveis de controle */
//   double motor_out = 0;
//   double prev_error = 0;
//   double error = 0;
//   double acum = 0;
//   double input = 0;
//   double ref = 0;
//   /* Inicia com velocidade nula */
//   bdc_motor_set_speed(motor_id, 0);

//   // Inicializa a contagem de tempo para manter a periodicidade das atualizações do motor
//   const TickType_t xLoopFrequency = pdMS_TO_TICKS(MOTORS_PERIOD_MS);
//   TickType_t xLastWakeTime;
//   xLastWakeTime = xTaskGetTickCount();
  
//   // Inicia o Loop 
//   while (true) {
//     // Pega o valor dos pulsos 
//     if (xSemaphoreTake(muxEncoder, portMAX_DELAY) == pdTRUE ){
//       input = r_count;
//       xSemaphoreGive(muxEncoder);
//     } 
//     // Pega o valor da referencia
//     if ( xSemaphoreTake(muxComm, portMAX_DELAY) == pdTRUE) {
//       ref = _r_ref;
//       xSemaphoreGive(muxEncoder);
//     }
//     // Computa o erro 
//     error = (double)(ref - (input * SENSOR_GAIN[motor_id]));
//     // Soma no acumulador do Integrativo
//     acum += error;
//     // Calcula o derivativo
//     double derivative = error - prev_error;
//     prev_error = error;
//     // Calcula a saída da velocidade das rodas com o PID
//     motor_out += (double)((KP[motor_id] * error) + (KD[motor_id] * derivative) + (KI[motor_id] * acum));
//     if (motor_out > 0) {
//       bdc_motor_set_speed(motor_id, (uint8_t)motor_out);
//       bdc_motor_forward(motor_id);
//     } else if (motor_out < 0) {
//       bdc_motor_set_speed(motor_id, (uint8_t)(motor_out * (-1)));
//       bdc_motor_backward(motor_id);
//     } else {
//       bdc_motor_stop(motor_id);
//     }
//     // Espera até o próximo período de contagem
//     vTaskDelayUntil(&xLastWakeTime, xLoopFrequency);
//   }
// }


// void bdc_motor_left(void* pvParameters) {
//   /* Identificação do motor esquerdo */
//   const int motor_id = 1;
//   /* Variaveis de controle */
//   double motor_out = 0;
//   double prev_error = 0;
//   double error = 0;
//   double acum = 0;
//   double input = 0;
//   double ref = 0;

//   /* Inicia com velocidade nula */
//   bdc_motor_set_speed(motor_id, 0);

//   // Inicializa a contagem de tempo para manter a periodicidade das atualizações do motor
//   const TickType_t xLoopFrequency = pdMS_TO_TICKS(MOTORS_PERIOD_MS);
//   TickType_t xLastWakeTime;
//   xLastWakeTime = xTaskGetTickCount();
  
//   // Inicia o loop 
//   while (true) {    
//     // Pega o valor dos pulsos 
//     if (xSemaphoreTake(muxEncoder, portMAX_DELAY) == pdTRUE ){
//       input = l_count;
//       xSemaphoreGive(muxEncoder);
//     } 
//     // Pega o valor da referencia
//     if ( xSemaphoreTake(muxComm, portMAX_DELAY) == pdTRUE) {
//       ref = _l_ref;
//       xSemaphoreGive(muxEncoder);
//     }
//     // Computa o erro 
//     error = (double)(ref - (input * SENSOR_GAIN[motor_id]));
//     // Soma no acumulador do Integrativo
//     acum += error;
//     // Calcula o derivativo
//     double derivative = error - prev_error;
//     prev_error = error;
//     // Calcula a saída da velocidade das rodas com o PID
//     motor_out += (double)((KP[motor_id] * error) + (KD[motor_id] * derivative) + (KI[motor_id] * acum));
//     if (motor_out > 0) {
//       bdc_motor_set_speed(motor_id, (uint8_t)motor_out);
//       bdc_motor_forward(motor_id);
//     } else if (motor_out < 0) {
//       bdc_motor_set_speed(motor_id, (uint8_t)(motor_out * (-1)));
//       bdc_motor_backward(motor_id);
//     } else {
//       bdc_motor_stop(motor_id);
//     }
//     // Espera até o próximo período de contagem
//     vTaskDelayUntil(&xLastWakeTime, xLoopFrequency);
//   }
// }


// /*  
//  *  FUNÇÕES PARA MOTOR CONTROL 
//  */
// void bdc_motor_task(void* pvParameters) {
//   // Define os pinos como direção de saída
//   gpio_config_t io_conf = {
//     .pin_bit_mask = (1ULL << MOTOR_RIGHT_INA_GPIO) | (1ULL << MOTOR_RIGHT_INB_GPIO) | (1ULL << MOTOR_RIGHT_ENB_GPIO)
//                     | (1ULL << MOTOR_LEFT_INA_GPIO) | (1ULL << MOTOR_LEFT_INB_GPIO) | (1ULL << MOTOR_LEFT_ENB_GPIO), /*!< GPIO pin: set with bit mask, each bit maps to a GPIO */
//     .mode = GPIO_MODE_OUTPUT,                                                                                        /*!< GPIO mode: set input/output mode                     */
//     .pull_up_en = GPIO_PULLUP_DISABLE,                                                                               /*!< GPIO pull-up                                         */
//     .pull_down_en = GPIO_PULLDOWN_DISABLE,                                                                           /*!< GPIO pull-down                                       */
//     .intr_type = GPIO_INTR_DISABLE,                                                                                  /*!< GPIO interrupt type                                  */
//   };
//   gpio_config(&io_conf);

//   // Configura o TIMMER para PWM (Timmer compartilhado)
//   ledc_timer_config_t ledc_timer = (ledc_timer_config_t){
//     .speed_mode = MOTORS_LEDC_MODE,             // timer mode
//     .duty_resolution = MOTORS_LEDC_RESOLUTION,  // resolution of PWM duty
//     .timer_num = MOTORS_LEDC_TIMER,             // timer index
//     .freq_hz = MOTORS_LEDC_FREQ,                // frequency of PWM signal
//     .clk_cfg = LEDC_AUTO_CLK,                   // Auto select the source clock
//   };
//   ledc_timer_config(&ledc_timer);

//   // Configura o CHANNEL usado pelo motore ENB1
//   ledc_channel_config_t motor_enb_1_config = (ledc_channel_config_t){
//     .gpio_num = MOTOR_RIGHT_ENB_GPIO,        /*!< the LEDC output gpio_num, if you want to use gpio16, gpio_num = 16 */
//     .speed_mode = MOTORS_LEDC_MODE,          /*!< LEDC speed speed_mode, high-speed mode [0] or low-speed mode [] */
//     .channel = MOTOR_ENB_RIGHT_LEDC_CHANNEL, /*!< LEDC channel (0 - 7) */
//     .intr_type = LEDC_INTR_DISABLE,          /*!< configure interrupt, Fade interrupt enable  or Fade interrupt disable */
//     .timer_sel = MOTORS_LEDC_TIMER,          /*!< Select the timer source of channel (0 - 3) */
//     .duty = 0,                               /*!< LEDC channel duty, the range of duty setting is [0, (2**duty_resolution)] */
//   };
//   ledc_channel_config(&motor_enb_1_config);

//   // Configura o CHANNEL usado pelo motore ENB2
//   ledc_channel_config_t motor_enb_2_config = (ledc_channel_config_t){
//     .gpio_num = MOTOR_LEFT_ENB_GPIO,        /*!< the LEDC output gpio_num, if you want to use gpio16, gpio_num = 16 */
//     .speed_mode = MOTORS_LEDC_MODE,         /*!< LEDC speed speed_mode, high-speed mode [0] or low-speed mode [] */
//     .channel = MOTOR_ENB_LEFT_LEDC_CHANNEL, /*!< LEDC channel (0 - 7) */
//     .intr_type = LEDC_INTR_DISABLE,         /*!< configure interrupt, Fade interrupt enable  or Fade interrupt disable */
//     .timer_sel = MOTORS_LEDC_TIMER,         /*!< Select the timer source of channel (0 - 3) */
//     .duty = 0,                              /*!< LEDC channel duty, the range of duty setting is [0, (2**duty_resolution)] */
//   };
//   ledc_channel_config(&motor_enb_2_config);

//   /* Inicia as tasks dos motores e configurações dos parametros de controle individuais */
//   xTaskCreate(&bdc_motor_right, "motor_R", 4096, NULL, 5, NULL);
//   xTaskCreate(&bdc_motor_left, "motor_L", 4096, NULL, 5, NULL);
// }
