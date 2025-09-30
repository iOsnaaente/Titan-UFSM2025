// /*  
//  *  Description: Definições para controle de encoder magnético SPI AS5048A para o projeto Titan Ver:2.1.2
//  *  Author: Bruno G. F. Sampaio
//  *  Date: 15/03/2024
//  *  License: MIT
//  *
// */

// #include "freertos/FreeRTOS.h"
// #include "freertos/semphr.h"
// #include "freertos/task.h"

// #include "pinout.h"

// // Variáveis de contagem - Não deve ser manipulada
// volatile uint32_t _r_count = 0;
// volatile uint32_t _l_count = 0;

// /* 
//  * Função de interrupção para contagem dos pulsos. Para a contagem de pulsos, há dois encoders com
//  *     defasagem para detecção do sentido de rotação.
//  *
//  * O sinal de leitura será do tipo: 
//  * 
//  * Sinal A  __|¯¯¯¯|____|¯¯¯¯|____|¯¯¯¯|____|¯
//  * Sinal B  ____|¯¯¯¯|____|¯¯¯¯|____|¯¯¯¯|____
//  * LeituraA __|_________|_________|_________|_ 
//  * LeituraB ____|_________|_________|_________ 
//  * Leitura  __|_|_______|_|_______|_|_______|_
//  * 
//  * Para saber o sentido de rotação, basta ler um dos dois sinais A ou B:
//  * Lendo A: 
//  *      Sentido horário     -> (A=1,B=0)
//  *      Sentido antihorário -> (A=1,B=1) 
//  * Lendo B: 
//  *      Sentido horário     -> (A=0,B=1)
//  *      Sentido antihorário -> (A=1,B=1)
//  *
//  */
// void IRAM_ATTR left_count_pulses(void* arg) {
//   uint8_t sensor = gpio_get_level(ENC_MOTOR_LEFT_A) | gpio_get_level(ENC_MOTOR_LEFT_B);
//   // Entra na região de memória compartilhada
//   if (xSemaphoreTake(muxEncoder, portMAX_DELAY) == pdTRUE) {
//     if (sensor == 2) {  // 2 = 0b0010
//       _l_count++;
//     } else if (sensor == 3) {  // 3 = 0b0011
//       _l_count--;
//     }
//     // Sai da seção crítica
//     xSemaphoreGive(muxEncoder);
//   }
// }

// void IRAM_ATTR right_count_pulses(void* arg) {
//   uint8_t sensor = gpio_get_level(ENC_MOTOR_RIGHT_A) | gpio_get_level(ENC_MOTOR_RIGHT_B);
//   // Entra na região de memória compartilhada
//   if (xSemaphoreTake(muxEncoder, portMAX_DELAY) == pdTRUE) {
//     if (sensor == 2) {  // 2 = 0b0010
//       _r_count++;
//     } else if (sensor == 3) {  // 3 = 0b0011
//       _r_count--;
//     }
//     // Sai da seção crítica
//     xSemaphoreGive(muxEncoder);
//   }
// }


// /* 
//  * Task para contagem periódica de pulsos. O valor da velocidade será salvo dentro da
//  *     da variável global ```r_vel``` e ```l_vel```. 
//  * O periodo de contagem é dado por ```PULSES_COUNT_PERIOD_MS``` e deve ser ajustado de 
//  *     acordo com a resolução do sensor.
//  */
// void pulses_count_task(void* pvParameters) {

//   // Configura os estados dos pinos
//   gpio_config_t io_conf = {
//     .pin_bit_mask = ((1ULL << ENC_MOTOR_LEFT_A) | (1ULL << ENC_MOTOR_LEFT_B))
//                     | ((1ULL << ENC_MOTOR_RIGHT_A) | (1ULL << ENC_MOTOR_RIGHT_B)),
//     .mode = GPIO_MODE_INPUT,
//     .pull_up_en = GPIO_PULLUP_ENABLE,
//     .pull_down_en = GPIO_PULLDOWN_DISABLE,
//     .intr_type = GPIO_INTR_POSEDGE,
//   };
//   gpio_config(&io_conf);
    
//   // Instala a rotina de tratamento de interrupção
//   gpio_install_isr_service(0);
//   gpio_isr_handler_add(ENC_MOTOR_LEFT_A, left_count_pulses, NULL);
//   gpio_isr_handler_add(ENC_MOTOR_RIGHT_A, right_count_pulses, NULL);

//   // Inicializa a contagem de tempo para manter a periodicidade das medições
//   const TickType_t xFrequency = pdMS_TO_TICKS(PULSES_COUNT_PERIOD_MS);
//   TickType_t xLastWakeTime;
//   xLastWakeTime = xTaskGetTickCount();

//   while (true) {
//     // Espera até o próximo período de contagem
//     vTaskDelayUntil(&xLastWakeTime, xFrequency);

//     // Entra na região de memória compartilhada
//     if (xSemaphoreTake(muxEncoder, portMAX_DELAY) == pdTRUE) {
//       r_count = _r_count;
//       l_count = _l_count;
//       xSemaphoreGive(muxEncoder);
//     }

//     // Debug
//     #ifdef DEBUG
//       printf("Pulsos[R]: %d\nPulsos[L]: %d\n", r_count, l_count);
//     #endif 
//   }
// }
