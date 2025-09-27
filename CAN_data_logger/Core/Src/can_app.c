/*
 * can_app.c
 *
 *  Created on: Sep 26, 2025
 *      Author: taison
 */

#include "can_app.h"
#include "main.h"
#include "FreeRTOS.h"
#include "task.h"

extern CAN_HandleTypeDef hcan1;
uint8_t RxData[CAN_DATA_BUFFER_SIZE];
CAN_RxHeaderTypeDef RxHeader;

/**
 * @brief  Configures and starts the CAN peripheral.
 *
 * This function enables the CAN hardware, activates CAN interrupt
 * notifications, and prepares the controller to receive messages via FIFO0.
 * Should be called after hardware and HAL initialization.
 *
 * @retval None
 */
void CAN_Config(void)
{
	// CAN peripheral already initialized by CubeMX-generated code
	HAL_CAN_Start(&hcan1);
	HAL_CAN_ActivateNotification(&hcan1, CAN_IT_RX_FIFO0_MSG_PENDING);
}

/**
 * @brief  Transmits a CAN data frame over the CAN bus.
 *
 * This function formats and sends a CAN message with the specified payload and length.
 *
 * @param data Pointer to the array of data bytes to send.
 * @param len  Number of data bytes to send (should be 0 to 8).
 * @retval None
 */
void CAN_Send(uint8_t *data, uint8_t len)
{
	CAN_TxHeaderTypeDef TxHeader;
	uint32_t TxMailbox;
	TxHeader.IDE = CAN_ID_STD;
	TxHeader.StdId = CAN_LED_TOGGLE_ID;
	TxHeader.RTR = CAN_RTR_DATA;
	TxHeader.DLC = len;
	HAL_CAN_AddTxMessage(&hcan1, &TxHeader, data, &TxMailbox);
}

/**
 * @brief  Rx FIFO 0 message pending interrupt callback.
 *
 * This function is called by the HAL CAN driver when a new CAN message is pending in Rx FIFO 0.
 * It is typically used to retrieve and process the received CAN frame.
 *
 * @param  hcan: Pointer to a CAN_HandleTypeDef structure that contains
 *         the configuration information for the specified CAN peripheral.
 * @retval None
 */
void HAL_CAN_RxFifo0MsgPendingCallback(CAN_HandleTypeDef *hcan)
{
    HAL_CAN_GetRxMessage(hcan, CAN_RX_FIFO0, &RxHeader, RxData);
    if (RxHeader.StdId == CAN_LED_TOGGLE_ID && RxData[0] > CAN_LED_THRESHOLD) {
    	HAL_GPIO_WritePin(CAN_LED_GPIO_PORT, CAN_LED_GPIO_PIN, GPIO_PIN_SET);
    } else {
    	HAL_GPIO_WritePin(CAN_LED_GPIO_PORT, CAN_LED_GPIO_PIN, GPIO_PIN_RESET);
    }
}

/**
 * @brief FreeRTOS task to periodically send a CAN message.
 *
 * This task repeatedly transmits a predefined CAN data frame every 3 seconds
 * using the CAN_Send() function. Intended to be created and managed by the
 * FreeRTOS scheduler. The task runs in an infinite loop and should not return.
 *
 * @param arg FreeRTOS task argument pointer (unused).
 */
void StartCANTxTask(void *arg)
{
    uint8_t sensor_data[] = {33, 25, 100, 89, 5, 6, 0};
    for(;;) {
        CAN_Send(sensor_data, CAN_DATA_LENGTH);
        vTaskDelay(pdMS_TO_TICKS(3000));
    }
}

