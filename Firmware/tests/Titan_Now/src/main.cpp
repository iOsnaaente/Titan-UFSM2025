#include "serial/serialController.h"
#include "sensors/AS5048A.h"
#include "motors/L298N.h"

L298N *motor_yaw_leg;
L298N *motor_pitch_leg;
L298N *motor_pitch_knee;

AS5048A *sensor_yaw_leg;
AS5048A *sensor_pitch_leg;
AS5048A *sensor_pitch_knee;


void setup() {
  serial_begin(); 

  // Inicializa o barramento do SPI0 
	spi_bus_config_t spi_bus = { 0 };
	spi_bus.mosi_io_num = MOSI_GPIO;
	spi_bus.miso_io_num = MISO_GPIO;
	spi_bus.sclk_io_num = CLK_GPIO;
	spi_bus.quadwp_io_num = -1;
	spi_bus.quadhd_io_num = -1;
  spi_bus.max_transfer_sz = 0;
  
  /** Inicializa o barramento SPI */ 
  esp_err_t ret = spi_bus_initialize( HSPI_HOST, &spi_bus, SPI_DMA_DISABLED );
	if ( ret != ESP_OK ){
		DEBUG_SERIAL( "SPI BUS INIT", "SPI_bus_initialize Failed! [" + String(esp_err_to_name(ret)) + "]" );
	} else {
		DEBUG_SERIAL( "SPI BUS INIT", "SPI_bus_initialize successfully!");
	}
  /** Adiciona os sesnores no barramento SPI criado */
  sensor_yaw_leg     = new AS5048A( CS1_GPIO );
  // sensor_pitch_leg   = new AS5048A( CS2_GPIO );
  // sensor_pitch_knee  = new AS5048A( CS3_GPIO );

  /** Motores */
  // motor_yaw_leg      = new L298N( "motor yaw leg"   , MOTOR_RIGHT_INA_GPIO , MOTOR_RIGHT_INB_GPIO , MOTOR_RIGHT_ENB_GPIO  );
  // motor_pitch_leg    = new L298N( "motor pitch leg" , MOTOR_MIDDLE_INA_GPIO, MOTOR_MIDDLE_INB_GPIO, MOTOR_MIDDLE_ENB_GPIO );
  // motor_pitch_knee   = new L298N( "motor pitch knee", MOTOR_LEFT_INA_GPIO  , MOTOR_LEFT_INB_GPIO  , MOTOR_LEFT_ENB_GPIO   );


  gpio_set_direction( BOARD_LED, GPIO_MODE_OUTPUT );
}

bool state = false;
void loop() {
  gpio_set_level( BOARD_LED, state = !state );

  DEBUG_SERIAL( "SENSOR YAW - RAW", sensor_yaw_leg->get_position( UNIT_RAW ) );
  DEBUG_SERIAL( "SENSOR YAW - DEG", sensor_yaw_leg->get_position( UNIT_DEGREE ) );
  // DEBUG_SERIAL( "SENSOR PITCH", sensor_pitch_leg->getRotation() );
  // DEBUG_SERIAL( "SENSOR KNEE", sensor_pitch_knee->getRotation() );
  
  vTaskDelay(pdTICKS_TO_MS(100)); // Delay de 1 segundo
}