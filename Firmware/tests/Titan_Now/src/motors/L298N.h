/**
 *  Description: 
 *  Author: Bruno G. F. Sampaio
 *  Date: 28/08/2024
 *  License: MIT
 *
*/

#ifndef L298N_H
#define L298N_H

#include "../pinout.h"

/* Constantes para configuração do LEDC */ 
#define LEDC_MODE           LEDC_HIGH_SPEED_MODE   //      
#define LEDC_CHANNEL        LEDC_CHANNEL_0         //            
#define LEDC_TIMER          LEDC_TIMER_0           //          
#define LEDC_RESOLUTION     LEDC_TIMER_13_BIT      // 
#define LEDC_FREQ           ((uint32_t)1000)       //  
              
/* Constantes para definir valores mínimos e máximo de potência */
#define BDC_TURN_OFF        ((double)(0))          //
#define BDC_MIN_POWER       ((double)(2000))       //
#define BDC_MAX_POWER       ((double)(8192))       //

#define BDC_ERROR           ((uint8_t)(0))         //
#define BDC_SUCCESS         ((uint8_t)(1))         //

/* Constantes para estados on-off */
#define BDC_ENABLE          ((bool)(true))         //
#define BDC_DISABLE         ((bool)(false))        //

/* Constantes para definir o estado de mocimento do motor */
#define BDC_STOPED          ((uint8_t)(0))         //
#define BDC_BREAK           ((uint8_t)(1))         //
#define BDC_FORWARD         ((uint8_t)(2))         //
#define BDC_BACKWARD        ((uint8_t)(3))         //


class L298N  {
public:
    gpio_num_t IN1;
    gpio_num_t IN2;
    gpio_num_t ENB;

    /* Construtor */
    L298N(const char* description,  gpio_num_t IN1, gpio_num_t IN2, gpio_num_t ENB );
    
    /* Métodos */
    uint8_t set_torque( bool torque);
    uint8_t set_direction( uint8_t direction) ;
    uint8_t set_speed( double speed) ;
    uint8_t stop(void) ;
    uint8_t brake(void) ;
};

#endif // L298N_H 