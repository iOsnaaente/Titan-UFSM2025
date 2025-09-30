

#ifndef AS5048A_h
#define AS5048A_h

#include "../pinout.h"

#include "serial/serialController.h"

#include "driver/spi_master.h"

#define AS5048A_NOP                        	0x0000
#define AS5048A_CLEAR_ERROR_FLAG	 		0x0001
#define AS5048A_PROGRAMMING_CONTROL	 		0x0003
#define AS5048A_OTP_REGISTER_ZERO_POS_HIGH	0x0016
#define AS5048A_OTP_REGISTER_ZERO_POS_LOW	0x0017
#define AS5048A_DIAG_AGC	 				0x3FFD
#define AS5048A_MAGNITUDE	 				0x3FFE
#define AS5048A_ANGLE	 					0x3FFF

#define AS5048A_AGC_FLAG					0xFF
#define AS5048A_ERROR_PARITY_FLAG			0x04
#define AS5048A_ERROR_COMMAND_INVALID_FLAG	0x02
#define AS5048A_ERROR_FRAMING_FLAG			0x01

#define AS5048A_DIAG_COMP_HIGH	 			0x2000
#define AS5048A_DIAG_COMP_LOW	 			0x1000
#define AS5048A_DIAG_COF	 				0x0800
#define AS5048A_DIAG_OCF	 				0x0400


// Valores de apoio para conversões de unidade do AS5048A
#define AS5048A_MAX_VALUE 					((double)(8191.0))
#define AS5048A_2PI 						((double)(6.28318530718))

// Unidades de medida para o AS5048A
#define UNIT_DEGREE 0
#define UNIT_RADIAN 1
#define UNIT_RAW 	2

#define UNIT_DEGREE_SECOND  0
#define UNIT_RADIAN_SECOND  1
#define UNIT_RAW_SECOND 	2

#define MODE_READ  0x00
#define MODE_WRITE 0x01


class AS5048A {
private:
	// Calculo da paridade 
	uint8_t parity_bit( int16_t );

	// Envia um comando para o SPI 
	int16_t send_command( int16_t data, uint8_t mode_rw = MODE_READ );

protected: 
	// Parametros do SPI e handle do canal 
	spi_device_interface_config_t device;
	spi_device_handle_t spi;

	// Chip Select (CS) pin
	gpio_num_t cs;
	
	// Debug flag
	bool error_flag;

	// Ultimo tempo de medição 
	uint64_t measurement_time;
	// Ultima posição detectada pelo sensor 
	int32_t position_raw;
	// Ultima velocidade detectada pelo sensor
	double speed_raw;
	
	// Escreve em um registrador 
	int16_t write( int16_t register_address, int16_t data );
	// Lê um registrador 
	int16_t read( int16_t register_address);

public:	
	// Construtor 
	AS5048A( gpio_num_t chip_select_pin );

	int32_t get_position( uint8_t unit = UNIT_DEGREE );
	double  get_speed( uint8_t unit = UNIT_DEGREE_SECOND ); 

	int16_t get_errors();
	int16_t get_state();
	uint8_t get_gain();

	bool has_error();

	// Destrutor 
	~AS5048A();
};

#endif