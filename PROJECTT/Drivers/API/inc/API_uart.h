/*
 * API_uart.h
 *
 *  Created on: Jul 25, 2024
 *      Author: rpalma
 */

#ifndef API_INC_API_UART_H_
#define API_INC_API_UART_H_

#include <stdio.h>
#include "stm32f4xx_hal.h"  		/* <- HAL include */
#include "stm32f4xx_nucleo_144.h" 	/* <- BSP include */
#include <stdbool.h>

bool uartInit();
void uartSendString(uint8_t * pstring);
void uartSendStringSize(uint8_t * pstring, uint16_t size);
void uartReceiveStringSize(uint8_t * pstring, uint16_t size);


#endif /* API_INC_API_UART_H_ */
