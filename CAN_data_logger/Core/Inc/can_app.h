/*
 * can_app.h
 *
 *  Created on: Sep 26, 2025
 *      Author: taison
 */

#ifndef INC_CAN_APP_H_
#define INC_CAN_APP_H_

#include "stm32f4xx_hal.h"

#define CAN_DATA_BUFFER_SIZE       8

//0x446:  acts as a specific marker so the code only reacts to messages with this identifier,
//allowing to filter for frames you control or generate.
//0x446 can be any value—replace it with one relevant to your application or test scenario.
#define CAN_LED_TOGGLE_ID		   0x446

//The threshold (50) on RxData is just a simple example for logic—such as turning on an LED
//only when sensor data or control commands reach a certain value.
#define CAN_LED_THRESHOLD          50

#define CAN_LED_GPIO_PORT         GPIOC
#define CAN_LED_GPIO_PIN		  GPIO_PIN_8

void CAN_Config(void);
void CAN_Send(uint8_t *data, uint8_t len);

#endif /* INC_CAN_APP_H_ */
