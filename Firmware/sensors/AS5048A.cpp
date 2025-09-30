#include "AS5048A.h"

AS5048A::AS5048A( gpio_num_t chip_select ) : cs(chip_select), error_flag(false) {
	
    // Configura o pino do CS como saída
	gpio_set_direction( cs, GPIO_MODE_OUTPUT );

	// Barramento do SPI 
	this->device = { 0 };
	this->device.clock_speed_hz = 3000000;      // 3MHz
	this->device.mode = SPI_MODE0;              // CPOL=0, CPHA=1 (Dado pelo Datasheet)
	this->device.spics_io_num = chip_select;    // Pino do CS
	this->device.queue_size = 7;
	this->device.flags = 0;
	this->device.pre_cb = NULL;
    
    // Adiciona o dispositivo ao barramento SPI
    esp_err_t ret = spi_bus_add_device( HSPI_HOST, &this->device, &this->spi );
	if ( ret != ESP_OK ){
		DEBUG_SERIAL( "SPI DEV INIT", "spi_bus_add_device Failed!");
	} else {
		DEBUG_SERIAL( "SPI DEV INIT", "spi_bus_add_device successfully!");
	}

    // Alocação dos valores de posição e velocidade
    this->measurement_time = 0.0;
    this->position_raw = 0;
    this->speed_raw = 0.0;
    this->get_position();

}


uint8_t AS5048A::parity_bit(int16_t value) {
    int16_t parity = 0;
    while (value) {
        parity ^= value & 1;
        value >>= 1;
    }
    return parity;
}

int16_t AS5048A::send_command( int16_t data, uint8_t mode_rw ) {
    // Monta o comando: Read / Write no bit 14
    uint16_t command = ((mode_rw == MODE_READ) ? 0x4000 : 0x0000) | (data & 0x3FFF);
    // Calcula paridade EVEN no bit 15
    command |= (parity_bit(command) << 15);
    // Prepara dados para envio
    uint8_t tx_data[2] = {
        static_cast<uint8_t>((command >> 8) & 0xFF),
        static_cast<uint8_t>(command & 0xFF)
    };
    uint8_t rx_data[2] = {0};
    spi_transaction_t t = {};

    t.length = 16;
    t.tx_buffer = tx_data;
    t.rx_buffer = rx_data;
    esp_err_t ret = spi_device_transmit(spi, &t);
    if (ret != ESP_OK) {
        DEBUG_SERIAL("SPI CMD", String(esp_err_to_name(ret)));
        this->error_flag = true;
        return -1;
    }

    // Para leitura: extrai dados
    if (mode_rw == MODE_READ ) {
        this->error_flag = (rx_data[0] & 0x40) != 0;
        uint16_t result = ((rx_data[0] << 8) | rx_data[1]) & 0x3FFF;
        return result;
    }
    // Para escrita, não retornamos dado lido
    return (int16_t)ret;
}


int16_t AS5048A::read(int16_t register_address) {
    // Envia comando de leitura
    int16_t read = send_command( register_address, MODE_READ );
    // Envia NOP para coletar resposta
    send_command( AS5048A_NOP, MODE_READ );
    return read;
}


int16_t AS5048A::write( int16_t register_address, int16_t data ) {
    // Envia comando de escrita e em seguida envia os dados
    send_command( register_address, MODE_WRITE );
    return send_command(data, MODE_WRITE );
}


int32_t AS5048A::get_position(uint8_t unit) {
    int16_t position = ( read( AS5048A_ANGLE ) ) - AS5048A_MAX_VALUE;
    uint64_t now = esp_timer_get_time();
    this->speed_raw = (this->position_raw - position ) / ( (now - this->measurement_time / 1e6) );
    this->measurement_time = now;
    this->position_raw = position;
    switch ( unit ) {
        case UNIT_DEGREE:
            // 0-360
            return (int32_t)(( this->position_raw * 360.0 ) / AS5048A_MAX_VALUE);   
        case UNIT_RADIAN:
            // 0-2pi
            return (int32_t)(( this->position_raw * AS5048A_2PI ) / AS5048A_MAX_VALUE );  
        case UNIT_RAW:
            // 0-AS5048A_MAX_VALUE
            return this->position_raw; 
    }
}

double AS5048A::get_speed(uint8_t unit) {
    this->read( AS5048A_ANGLE );
    switch (unit) {
        case UNIT_DEGREE_SECOND:
            // Convertendo para graus por segundo
            return this->speed_raw = (this->speed_raw * 360.0) / AS5048A_MAX_VALUE; 
        case UNIT_RADIAN_SECOND:
            // Convertendo para radianos por segundo
            return this->speed_raw = (this->speed_raw * AS5048A_2PI) / AS5048A_MAX_VALUE;
        case UNIT_RAW_SECOND:
        default:    
            // Retorna a velocidade bruta em unidades do sensor
            return this->speed_raw;
    }
    this->speed_raw;
}

int16_t AS5048A::get_errors() {
    return read(AS5048A_CLEAR_ERROR_FLAG);
}

int16_t AS5048A::get_state() {
    return read(AS5048A_DIAG_AGC);
}

uint8_t AS5048A::get_gain() {
    int16_t state = get_state();
    return (uint8_t)(state & 0xFF);
}

bool AS5048A::has_error() {
    return error_flag;
}
