/*
 * API_debounce.c
 *
 *  Created on: Aug 15, 2024
 *      Author: rpalma
 */


#include "API_numpad_4x4.h"


void numpad_4x4_init(){

	GPIO_InitTypeDef GPIO_InitStruct = {0};
	/* USER CODE BEGIN MX_GPIO_Init_1 */
	/* USER CODE END MX_GPIO_Init_1 */

	  /* GPIO Ports Clock Enable */
	  __HAL_RCC_GPIOC_CLK_ENABLE();
	  __HAL_RCC_GPIOF_CLK_ENABLE();
	  __HAL_RCC_GPIOH_CLK_ENABLE();
	  __HAL_RCC_GPIOA_CLK_ENABLE();
	  __HAL_RCC_GPIOB_CLK_ENABLE();
	  __HAL_RCC_GPIOE_CLK_ENABLE();
	  __HAL_RCC_GPIOD_CLK_ENABLE();
	  __HAL_RCC_GPIOG_CLK_ENABLE();


	  /*Configure GPIO pins : D7_Pin D4_Pin */
	  GPIO_InitStruct.Pin = D7_Pin|D4_Pin;
	  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	  GPIO_InitStruct.Pull = GPIO_NOPULL;
	  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
	  HAL_GPIO_Init(GPIOF, &GPIO_InitStruct);

	  /*Configure GPIO pin : D2_Pin */
	  GPIO_InitStruct.Pin = D2_Pin;
	  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
	  GPIO_InitStruct.Pull = GPIO_PULLUP;
	  HAL_GPIO_Init(D2_GPIO_Port, &GPIO_InitStruct);

	  /*Configure GPIO pins : D6_Pin D5_Pin */
	  GPIO_InitStruct.Pin = D6_Pin|D5_Pin;
	  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	  GPIO_InitStruct.Pull = GPIO_NOPULL;
	  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
	  HAL_GPIO_Init(GPIOE, &GPIO_InitStruct);

	  /*Configure GPIO pin : D3_Pin */
	  GPIO_InitStruct.Pin = D3_Pin;
	  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
	  GPIO_InitStruct.Pull = GPIO_PULLUP;
	  HAL_GPIO_Init(D3_GPIO_Port, &GPIO_InitStruct);

	  /*Configure GPIO pins : D0_Pin D1_Pin */
	  GPIO_InitStruct.Pin = D0_Pin|D1_Pin;
	  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
	  GPIO_InitStruct.Pull = GPIO_PULLUP;
	  HAL_GPIO_Init(GPIOG, &GPIO_InitStruct);


}

char detectNumPad(void){

	HAL_GPIO_WritePin(D7_GPIO_Port, D7_Pin, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(D6_GPIO_Port, D6_Pin, GPIO_PIN_SET);
	HAL_GPIO_WritePin(D5_GPIO_Port, D5_Pin, GPIO_PIN_SET);
	HAL_GPIO_WritePin(D4_GPIO_Port, D4_Pin, GPIO_PIN_SET);

	if(!HAL_GPIO_ReadPin(D3_GPIO_Port, D3_Pin) ){

		while(!HAL_GPIO_ReadPin(D3_GPIO_Port, D3_Pin));
		return '1';
	}
	if(!HAL_GPIO_ReadPin(D2_GPIO_Port, D2_Pin)){

		while(!HAL_GPIO_ReadPin(D2_GPIO_Port, D2_Pin));
		return '2';
	}
	if(!HAL_GPIO_ReadPin(D1_GPIO_Port, D1_Pin)){

		while(!HAL_GPIO_ReadPin(D1_GPIO_Port, D1_Pin));
		return '3';
	}
	if(!HAL_GPIO_ReadPin(D0_GPIO_Port, D0_Pin)){

		while(!HAL_GPIO_ReadPin(D0_GPIO_Port, D0_Pin));
		return 'A';
	}

	HAL_GPIO_WritePin(D7_GPIO_Port, D7_Pin, GPIO_PIN_SET);
	HAL_GPIO_WritePin(D6_GPIO_Port, D6_Pin, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(D5_GPIO_Port, D5_Pin, GPIO_PIN_SET);
	HAL_GPIO_WritePin(D4_GPIO_Port, D4_Pin, GPIO_PIN_SET);

	if(!HAL_GPIO_ReadPin(D3_GPIO_Port, D3_Pin)){

		while(!HAL_GPIO_ReadPin(D3_GPIO_Port, D3_Pin));
		return '4';
	}
	if(!HAL_GPIO_ReadPin(D2_GPIO_Port, D2_Pin)){

		while(!HAL_GPIO_ReadPin(D2_GPIO_Port, D2_Pin));
		return '5';
	}
	if(!HAL_GPIO_ReadPin(D1_GPIO_Port, D1_Pin)){

		while(!HAL_GPIO_ReadPin(D1_GPIO_Port, D1_Pin));
		return '6';
	}
	if(!HAL_GPIO_ReadPin(D0_GPIO_Port, D0_Pin)){

		while(!HAL_GPIO_ReadPin(D0_GPIO_Port, D0_Pin));
		return 'B';
	}

	HAL_GPIO_WritePin(D7_GPIO_Port, D7_Pin, GPIO_PIN_SET);
	HAL_GPIO_WritePin(D6_GPIO_Port, D6_Pin, GPIO_PIN_SET);
	HAL_GPIO_WritePin(D5_GPIO_Port, D5_Pin, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(D4_GPIO_Port, D4_Pin, GPIO_PIN_SET);

	if(!HAL_GPIO_ReadPin(D3_GPIO_Port, D3_Pin)){

		while(!HAL_GPIO_ReadPin(D3_GPIO_Port, D3_Pin));
		return '7';
	}
	if(!HAL_GPIO_ReadPin(D2_GPIO_Port, D2_Pin)){

		while(!HAL_GPIO_ReadPin(D2_GPIO_Port, D2_Pin));
		return '8';
	}
	if(!HAL_GPIO_ReadPin(D1_GPIO_Port, D1_Pin)){

		while(!HAL_GPIO_ReadPin(D1_GPIO_Port, D1_Pin));
		return '9';
	}
	if(!HAL_GPIO_ReadPin(D0_GPIO_Port, D0_Pin)){

		while(!HAL_GPIO_ReadPin(D0_GPIO_Port, D0_Pin));
		return 'C';
	}

	HAL_GPIO_WritePin(D7_GPIO_Port, D7_Pin, GPIO_PIN_SET);
	HAL_GPIO_WritePin(D6_GPIO_Port, D6_Pin, GPIO_PIN_SET);
	HAL_GPIO_WritePin(D5_GPIO_Port, D5_Pin, GPIO_PIN_SET);
	HAL_GPIO_WritePin(D4_GPIO_Port, D4_Pin, GPIO_PIN_RESET);

	if(!HAL_GPIO_ReadPin(D3_GPIO_Port, D3_Pin)){

		while(!HAL_GPIO_ReadPin(D3_GPIO_Port, D3_Pin));
		return '*';
	}
	if(!HAL_GPIO_ReadPin(D2_GPIO_Port, D2_Pin)){

		while(!HAL_GPIO_ReadPin(D2_GPIO_Port, D2_Pin));
		return '0';
	}
	if(!HAL_GPIO_ReadPin(D1_GPIO_Port, D1_Pin)){

		while(!HAL_GPIO_ReadPin(D1_GPIO_Port, D1_Pin));
		return '#';
	}
	if(!HAL_GPIO_ReadPin(D0_GPIO_Port, D0_Pin)){

		while(!HAL_GPIO_ReadPin(D0_GPIO_Port, D0_Pin));
		return 'D';
	}

	HAL_GPIO_WritePin(D7_GPIO_Port, D7_Pin, GPIO_PIN_SET);
	HAL_GPIO_WritePin(D6_GPIO_Port, D6_Pin, GPIO_PIN_SET);
	HAL_GPIO_WritePin(D5_GPIO_Port, D5_Pin, GPIO_PIN_SET);
	HAL_GPIO_WritePin(D4_GPIO_Port, D4_Pin, GPIO_PIN_SET);

}



