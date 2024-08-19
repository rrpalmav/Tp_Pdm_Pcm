/*
 * API_uart.c
 *
 *  Created on: Jul 25, 2024
 *      Author: rpalma
 */

#include "API_uart.h"
#include <stdio.h>
#include "stm32f4xx_hal.h"  		/* <- HAL include */
#include "stm32f4xx_nucleo_144.h" 	/* <- BSP include */
#include <stdbool.h>
#include "main.h"

#define TIMEOUT 100


UART_HandleTypeDef uart_master;
uint8_t WELCOME_MESSAGE[]= "INICIALIZACION DE UART";

bool uartInit(){

	  uart_master.Instance        = USART3;

	  uart_master.Init.BaudRate   = 115200;
	  uart_master.Init.WordLength = UART_WORDLENGTH_8B;
	  uart_master.Init.StopBits   = UART_STOPBITS_1;
	  uart_master.Init.Parity 	  = UART_PARITY_NONE;
	  uart_master.Init.HwFlowCtl  = UART_HWCONTROL_NONE;
	  uart_master.Init.Mode       = UART_MODE_TX_RX;
	  uart_master.Init.OverSampling = UART_OVERSAMPLING_16;
	  if (HAL_UART_Init(&uart_master) != HAL_OK) return false;

	  uartSendString(WELCOME_MESSAGE);

	  return true;
}


void uartSendString(uint8_t * pstring){
	while(*pstring) uartSendStringSize(pstring++, 1);
}


void uartSendStringSize(uint8_t * pstring, uint16_t size){
	HAL_UART_Transmit(&uart_master, pstring, size, TIMEOUT);
}


void uartReceiveStringSize(uint8_t * pstring, uint16_t size){
	HAL_UART_Receive(&uart_master, pstring, size, TIMEOUT);
}


