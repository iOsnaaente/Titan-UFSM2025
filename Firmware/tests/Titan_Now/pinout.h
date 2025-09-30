/**
 * @file pinout.h
 * @brief Header do configurações da placa Titan  
 * @author Bruno Gabriel Flores Sampaio,
 * @date 29 de agosto de 2024
 */

#ifndef TITAN_PINOUT_H_
#define TITAN_PINOUT_H_

// Includes padrão
#include "stdlib.h"
#include "stdio.h"
#include "string"

// Includes do freeRTOS 
#include "freertos/FreeRTOS.h"
#include "freertos/queue.h"
#include "freertos/semphr.h"
#include "freertos/task.h"

#include "WString.h"

#include "driver/gpio.h"
#include "driver/ledc.h"
#include "driver/uart.h"
#include "driver/i2c.h"

#include "Arduino.h"


/** 
 * @brief Mensagem de debug para serial. 
 * @details É usaod um Mutex para não haver conflitos de uso de memórias compartilhadas e deadlocks  D:
 * Para desativar o debug serial, deve ser definido a Macrodefinição DEBUG_SERIAL_DESLIGADO
 * 
 * @example `#define DEBUG_SERIAL_COMPLETO`  -> Printa o nome do arquivo e linha + mensagem de debug
 * @example `#define DEBUG_SERIAL_REDUZIDO`  -> Printa somente a mensagem de debug
 * @example `#define DEBUG_SERIAL_DESLIGADO` -> Desliga o print de mensagens de debug 
 */

// #define DEBUG_SERIAL_COMPLETO 
#define DEBUG_SERIAL_REDUZIDO 
// #define DEBUG_SERIAL_DESLIGADO 


// Clock da SPI para leitura dos sensores 
#define SPI_SENSOR_CLOCK 4000000

/* LED da placa para debug visual  */
#define BOARD_LED                       GPIO_NUM_2        // Define o pino do LED embutido na placa

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
#define UART_BAUDRATE                   1000000           // Baudrate utilizado pela Serial do USB 

/* Interface I2C com soquete externo */
#define SDA_GPIO                        GPIO_NUM_21       // Pino de Dados do barramento I2C
#define SCL_GPIO                        GPIO_NUM_22       // Pino de Clock do barramento I2C

// Analog inputs
#define ADC1_GPIO                       GPIO_NUM_36       // Sensor analógico
#define ADC2_GPIO                       GPIO_NUM_39       // Sensor analógico
#define ADC3_GPIO                       GPIO_NUM_34       // Sensor analógico
#define ADC4_GPIO                       GPIO_NUM_35       // Sensor analógico

#endif