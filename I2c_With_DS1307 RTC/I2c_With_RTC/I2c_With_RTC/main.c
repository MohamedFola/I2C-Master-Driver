/*
--> I2c_With_RTC.c
--> Author : Mohamed Hassan Hassan
 */ 

#include "I2C_Driver_Master.h"
#include <util/delay.h>


#define Seconds_Port          PORTB
#define Seconds_Direction     DDRB

#define Minutes_Port          PORTA
#define Minutes_Direction     DDRA

#define Hours_Port            PORTD
#define Hours_Direction       DDRD

#define Slave_Address_Write   0xD0
#define Slave_Address_Read    0xD1

int main(void)
{
	Seconds_Direction = 0xFF;
	Minutes_Direction = 0xFF;
	Hours_Direction   = 0xFF;
	
	uint8_t status;
	uint8_t seconds = 0;
	uint8_t minutes = 0;
	uint8_t hours = 0;
	
	TWI_INIT(Bus_Speed_100KHZ);
	status = TWI_Start(Slave_Address_Write);
	if(status != AddressWrite_Transmitted_ACK_Received) return 0;
	status = TWI_Write(0x07);
	if(status != Data_Transmitted_ACK_Received) return 0;
	status = TWI_Write(1<<4);
	if(status != Data_Transmitted_ACK_Received) return 0;
	TWI_End();

    while (1) 
    {
		status = TWI_Start(Slave_Address_Write);
		if(status != AddressWrite_Transmitted_ACK_Received) return 0;
		status = TWI_Write(0x00);
		if(status != Data_Transmitted_ACK_Received) return 0;
		TWI_End();
		
		status = TWI_Start(Slave_Address_Read);
		if(status != AddressRead_Transmitted_ACK_Received) return 0;
		
		// Seconds
		status = TWI_Receive_Data(&seconds, Condition_ACK);
		if(status != Data_Recevied_ACK_Transmitted) return 0;
		
		// Minutes
		status = TWI_Receive_Data(&minutes, Condition_ACK);
		if(status != Data_Recevied_ACK_Transmitted) return 0;
		
		// Hours
		status = TWI_Receive_Data(&hours, Condition_NACK);
		if(status != Data_Recevied_NACK_Transmitted) return 0;
		TWI_End();
		
		Seconds_Port = seconds;
		Minutes_Port = minutes;
		Hours_Port   = hours;
		
		_delay_ms(1000);
    }
	
	return 0;
}

