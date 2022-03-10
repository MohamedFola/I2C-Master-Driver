/*
--> Atmega16/Atmega32 I2C Driver for "MASTER" device
--> Author: Mohamed Hassan Fola

	   -----
1	 -|		|-		 40
2	 -|		|-		 39
3	 -|		|-		 38
4	 -|		|-		 37
5:   -|		|-		 36
6:   -|		|-		 35
7: 	 -|		|-		 34
8:   -|		|-		 33
9	 -|		|-		 32
10	 -|		|-		 31
11	 -|		|-		 30
12	 -|		|-		 29
13	 -|		|-		 28
14	 -|		|-		 27
15	 -|		|-		 26
16	 -|		|-		 25
17	 -|		|-		 24
18	 -|		|-	SDA :23
19	 -|		|-	SCL :22
20	 -|		|-       21
	   -----	       
*/

#ifndef I2C_DRIVER_MASTER_H_
#define I2C_DRIVER_MASTER_H_

#include "_Bit_Manipulation.h"  
#include <stddef.h>
#include <stdint.h>
#include <util/twi.h>
#define F_CPU 8000000UL


#define NULL_POINTER        0xFF
#define Bus_Speed_100KHZ    100
#define Bus_Speed_400KHZ    400

// Register definitions
#define TWI_Status_Register     TWSR
#define TWI_Control_Register    TWCR
#define TWI_Bit_Rate_Register   TWBR
#define TWI_Data_Register       TWDR
#define TWI_Port_Name           C

// TWI pins
#define SDA    1
#define SCL    0 

// TWCR register pins
#define TWINT  7
#define TWEA   6
#define TWSTA  5
#define TWSTO  4
#define TWWC   3
#define TWEN   2
#define TWIE   0

// TWSR register pins
#define TWPS0  0
#define TWPS1  1


// the available pre_scaler values
//                               TWPS1   TWPSO
#define Pre_Scaler_1     1   //    0       0
#define Pre_Scaler_4     4   //    0       1 
#define Pre_Scaler_16    16  //    1       0 
#define Pre_Scaler_64    64  //    1       1

// the receive conditions
#define Condition_ACK     1
#define Condition_NACK    0

// values of TWI status registers for all cases of master mode
#define Start_Transmitted                            0x08
#define Repeated_Start_Transmitted                   0x10

#define AddressWrite_Transmitted_ACK_Received        0x18
#define AddressWrite_Transmitted_NACK_Received       0x20

#define AddressRead_Transmitted_ACK_Received         0x40
#define AddressRead_Transmitted_NACK_Received        0x48

#define Data_Transmitted_ACK_Received                0x28
#define Data_Transmitted_NACK_Received               0x30

#define Data_Recevied_ACK_Transmitted                0x50
#define Data_Recevied_NACK_Transmitted               0x58



void TWI_INIT(uint8_t Bus_Speed)
{
	SET_BIT_INPUT(TWI_Port_Name, SDA);
	SET_BIT_INPUT(TWI_Port_Name, SCL);
	SET_BIT_HIGH(TWI_Port_Name, SDA);
	SET_BIT_HIGH(TWI_Port_Name, SCL);
	TWI_Bit_Rate_Register = ((F_CPU / Bus_Speed) - 16) / 2;
}

uint8_t TWI_Start(uint8_t Slave_Address)
{
	TWI_Control_Register |= MASK(TWINT) | MASK(TWSTA) | MASK(TWEN);
	CLEAR_BIT(TWI_Control_Register, TWSTA);
	while(READ_BIT(TWI_Control_Register, TWINT) == 0);
	
	TWI_Data_Register = Slave_Address;
	TWI_Control_Register |= MASK(TWINT) | MASK(TWEN);
	while(READ_BIT(TWI_Control_Register, TWINT) == 0);
	
	return (TWI_Status_Register & 0xF8);
}


uint8_t TWI_Write(uint8_t Data)
{	
	TWI_Data_Register = Data;
	TWI_Control_Register |= MASK(TWINT) | MASK(TWEN);
	while(READ_BIT(TWI_Control_Register, TWINT) == 0);
	
	return (TWI_Status_Register & 0xF8);
}


uint8_t TWI_Receive_Data(uint8_t *Data, uint8_t Condition)
{
	TWI_Control_Register |= MASK(TWINT) | MASK(TWEN) | (READ_BIT(Condition, 0)<<TWEA);
	while(READ_BIT(TWI_Control_Register, TWINT) == 0);
	CLEAR_BIT(TWI_Control_Register, TWEA);
	
	if(Data == NULL) return NULL_POINTER;
	*Data = TWI_Data_Register;
	return (TWI_Status_Register & 0xF8);
}

void TWI_End()
{
	TWI_Control_Register |= MASK(TWINT) | MASK(TWSTO) | MASK(TWEN);
	while(READ_BIT(TWI_Control_Register, TWSTO) == 1);
}

#endif /* I2C_DRIVER_MASTER_H_ */