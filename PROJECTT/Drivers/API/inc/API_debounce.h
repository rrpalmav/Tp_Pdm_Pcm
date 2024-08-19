/*
 * API_debounce.h
 *
 *  Created on: Jul 12, 2024
 *      Author: rpalma
 */

#ifndef API_INC_API_DEBOUNCE_H_
#define API_INC_API_DEBOUNCE_H_


#include <stdio.h>
#include "stm32f4xx_hal.h"  		/* <- HAL include */
#include "stm32f4xx_nucleo_144.h" 	/* <- BSP include */
#include <stdbool.h>
#include "API_delay.h"

// Definición de una tipo para definir y manejar estado
/* Exported types ------------------------------------------------------------*/
#define DELAY_TIME 40

typedef enum{
  BUTTON_UP,
  BUTTON_FALLING,
  BUTTON_DOWN,
  BUTTON_RAISING,
} debounceState_t;


/* Exported functions ------------------------------------------------------- */

// Funcion que seran implementadas para la libreria
void debounceFSM_init();		// debe cargar el estado inicial
void debounceFSM_update();		// debe leer las entradas, resolver la lÃ³gica de
								// transiciÃ³n de estados y actualizar las salidas
void buttonPressed();			// debe invertir el estado del LED1
void buttonReleased();			// debe invertir el estado del LED3

bool_t readKey(void);

#endif /* API_INC_API_DEBOUNCE_H_ */
