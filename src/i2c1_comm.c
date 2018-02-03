/*
 * i2c1_comm.c
 *
 *  Created on: Jan 28, 2018
 *      Author: abel
 */

#include "i2c1_comm.h"
#include "timer2_output.h"
#include <stm32f4xx_gpio.h>

void init_i2c_bus_config(void)
{

	I2C_InitTypeDef I2C1_InitStruct;

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_I2C1, ENABLE);
	I2C1_InitStruct.I2C_ClockSpeed = 120000;
	I2C1_InitStruct.I2C_Ack = I2C_Ack_Enable;
	I2C1_InitStruct.I2C_AcknowledgedAddress = I2C_AcknowledgedAddress_7bit;
	I2C1_InitStruct.I2C_Mode = I2C_Mode_I2C;
	I2C1_InitStruct.I2C_DutyCycle = I2C_DutyCycle_2;
	I2C1_InitStruct.I2C_OwnAddress1 = 0;

	I2C_Init(I2C1, &I2C1_InitStruct);

	I2C_Cmd(I2C1, ENABLE);

}

void init_i2c_gpio(void)
{
	GPIO_InitTypeDef GPIO_I2C_InitStruct;

	/*PB6- SCL, PB9- SDA*/
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);
	GPIO_I2C_InitStruct.GPIO_Pin = GPIO_Pin_6 | GPIO_Pin_9;
	GPIO_I2C_InitStruct.GPIO_OType = GPIO_OType_OD;
	GPIO_I2C_InitStruct.GPIO_Mode = GPIO_Mode_AF;
	GPIO_I2C_InitStruct.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_I2C_InitStruct.GPIO_Speed = GPIO_Low_Speed;

	GPIO_Init(GPIOB, &GPIO_I2C_InitStruct);

	GPIO_PinAFConfig(GPIOB, GPIO_PinSource6, GPIO_AF_I2C1);
	GPIO_PinAFConfig(GPIOB, GPIO_PinSource9, GPIO_AF_I2C1);
	GPIO_ResetBits(GPIOB, GPIO_Pin_6);
	GPIO_ResetBits(GPIOB, GPIO_Pin_9);

}

void I2C_Transmit(uint8_t data, uint8_t hw_address)
{
	init_led();

	while ((I2C_GetFlagStatus(I2C1, I2C_FLAG_BUSY)))
		;

	/* Step 1: Send Start Condition*/
	I2C_GenerateSTART(I2C1, ENABLE);

	/* Step 2: Read the status Register for the event EV5, i.e Master mode  */
	while ((I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_MODE_SELECT)) != SUCCESS)
		;

	/* Step 3: Send the HW address  */
	I2C_Send7bitAddress(I2C1,hw_address<<1,I2C_Direction_Transmitter);


	/* Step 4: Wait for EV6: ADDR=1 cleared by reading SR1  */
	while ((I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED)) != SUCCESS)
		;


	/* Step 5: Put byte into Data Register  */
	I2C_SendData(I2C1,data);

	/* Step 6: Wait for TRA, BUSY, MSL, TXE and BTF flags in SR  */
	while ((I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_BYTE_TRANSMITTED)) != SUCCESS)
			;


	/* Step 7: SendStop byte to terminate communication  */
	I2C_GenerateSTOP(I2C1, ENABLE);


	//GPIO_SetBits(GPIOD, GPIO_Pin_15);

}

