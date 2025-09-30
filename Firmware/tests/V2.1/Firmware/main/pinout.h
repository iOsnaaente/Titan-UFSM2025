/*  
 *  Description: Constantes dos pinos usados no projeto de PCB do Titan Ver:2.1.2
 *  Author: Bruno G. F. Sampaio
 *  Date: 15/03/2024
 *  License: MIT
 *
 */

#ifndef PINOUT_H_
#define PINOUT_H_ 

#include "driver/gpio.h"

/* LED da placa para debug visual  */
#define LED_BUILTIN                     GPIO_NUM_2        // Define o pino do LED embutido na placa

// Sensors SPI
#define MISO_GPIO                       GPIO_NUM_12       // Master In Slave Out     
#define MOSI_GPIO                       GPIO_NUM_13       // Master Out Slave In 
#define CLK_GPIO                        GPIO_NUM_14       // SPI Clock
#define CS1_GPIO                        GPIO_NUM_15       // Chip Select 1
#define CS2_GPIO                        GPIO_NUM_26       // Chip Select 2
#define CS3_GPIO                        GPIO_NUM_27       // Chip Select 3

/* GPIO para ponte H - L298N */               
#define MOTOR_RIGHT_INA_GPIO            GPIO_NUM_32       // Pino de controle IN_A do motor direito
#define MOTOR_RIGHT_INB_GPIO            GPIO_NUM_33       // Pino de controle IN_B do motor direito
#define MOTOR_RIGHT_ENB_GPIO            GPIO_NUM_25       // Pino de controle ENB do motor direito

#define MOTOR_MIDDLE_INA_GPIO           GPIO_NUM_16       // Pino de controle IN_A do motor direito
#define MOTOR_MIDDLE_INB_GPIO           GPIO_NUM_4        // Pino de controle IN_B do motor direito
#define MOTOR_MIDDLE_ENB_GPIO           GPIO_NUM_17       // Pino de controle ENB do motor direito

#define MOTOR_LEFT_INA_GPIO             GPIO_NUM_19       // Pino de controle IN_A do motor esquerdo
#define MOTOR_LEFT_INB_GPIO             GPIO_NUM_23       // Pino de controle IN_B do motor esquerdo
#define MOTOR_LEFT_ENB_GPIO             GPIO_NUM_18       // Pino de controle ENB do motor esquerdo

/* Pinos para UART */
#define UART_TXD0_GPIO                  GPIO_NUM_1        // Pino de transmissão UART para debug ou Rádio transmissor/receptor
#define UART_RXD0_GPIO                  GPIO_NUM_3        // Pino de recepção UART para debug ou Rádio transmissor/receptor
#define UART_FLUX_GPIO                  GPIO_NUM_23       // Pino de recepção UART para debug ou Rádio transmissor/receptor

/* Interface I2C com soquete externo */
#define SDA_GPIO                        GPIO_NUM_21       // Pino de Dados do barramento I2C
#define SCL_GPIO                        GPIO_NUM_22       // Pino de Clock do barramento I2C

// Analog inputs
#define ADC1_GPIO                       GPIO_NUM_36       // Sensor analógico
#define ADC2_GPIO                       GPIO_NUM_39       // Sensor analógico
#define ADC3_GPIO                       GPIO_NUM_34       // Sensor analógico
#define ADC4_GPIO                       GPIO_NUM_35       // Sensor analógico

#endif