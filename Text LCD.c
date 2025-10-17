#include "Text_LCD.h"

void LCD_pulse_enable(void)
{
	PORT_CONTROL |= (1 << E_PIN);
	_delay_ms(1);
	
	PORT_CONTROL &= ~(1 << E_PIN);
	_delay_ms(1);
}

void LCD_write_data(uint8_t data)
{
	PORT_CONTROL |= (1 << RS_PIN);
	
	if(MODE == 8){
		PORT_DATA = data;
		LCD_pulse_enable();
	}
	
	else{
		PORT_DATA = data & 0xF0;
		LCD_pulse_enable();
		
		PORT_DATA = (data << 4) & 0xF0;
		LCD_pulse_enable();
	}
	_delay_ms(2);
}

void LCD_write_command(uint8_t command)
{
	PORT_CONTROL &= ~(1 << RS_PIN);
	
	if(MODE == 8){
		PORT_DATA = command;
		LCD_pulse_enable();
	}
	
	else{
		PORT_DATA  = command & 0xF0;
		LCD_pulse_enable();
		PORT_DATA = (command << 4) & 0xF0;
		LCD_pulse_enable();
	}
	_delay_ms(2);
}

void LCD_write_string(char *string)
{
	uint8_t i;
	
	for(i=0 ; string[i] ; i++){
		LCD_write_data(string[i]);
	}
}

void LCD_goto_XY(uint8_t row, uint8_t col)
{
	col %= 16;
	row %= 2;
	
	uint8_t address = (0x40 * row) + col;
	uint8_t command = 0x80 + address;
	
	LCD_write_command(command);
}

void LCD_clear(void)
{
	LCD_write_command(COMMAND_CLEAR_DISPLAY);
	_delay_ms(20);
}

void LCD_init(void)
{
	_delay_ms(50);
	
	DDR_DATA = 0xFF;
	PORT_DATA = 0x00;
	
	DDR_CONTROL |= (1 << RS_PIN) | (1 << RW_PIN) | (1 << E_PIN);
	
	PORT_CONTROL &= ~(1 << RW_PIN);
	
	if(MODE == 8){
		LCD_write_command(COMMAND_8_BIT_MODE);
	}
	
	else{
		LCD_write_command(0x02);
		LCD_write_command(COMMAND_4_BIT_MODE);
	}
	
	uint8_t command = 0x08 | (1 << COMMAND_DISPLAY_ON_OFF_BIT);
	LCD_write_command(command);
	
	LCD_clear();
	LCD_write_command(0x06);
}
