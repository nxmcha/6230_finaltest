/* USER CODE BEGIN Header */
/**
 ******************************************************************************
 * @file           : main.c
 * @brief          : Main program body
 ******************************************************************************
 * @attention
 *
 * <h2><center>&copy; Copyright (c) 2021 STMicroelectronics.
 * All rights reserved.</center></h2>
 *
 * This software component is licensed by ST under BSD 3-Clause license,
 * the "License"; You may not use this file except in compliance with the
 * License. You may obtain a copy of the License at:
 *                        opensource.org/licenses/BSD-3-Clause
 *
 ******************************************************************************
 */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "string.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

#ifndef HSEM_ID_0
#define HSEM_ID_0 (0U) /* HW semaphore 0*/
#endif
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
#if defined ( __ICCARM__ ) /*!< IAR Compiler */

#pragma location=0x30040000
ETH_DMADescTypeDef  DMARxDscrTab[ETH_RX_DESC_CNT]; /* Ethernet Rx DMA Descriptors */
#pragma location=0x30040060
ETH_DMADescTypeDef  DMATxDscrTab[ETH_TX_DESC_CNT]; /* Ethernet Tx DMA Descriptors */
#pragma location=0x30040200
uint8_t Rx_Buff[ETH_RX_DESC_CNT][ETH_MAX_PACKET_SIZE]; /* Ethernet Receive Buffers */

#elif defined ( __CC_ARM )  /* MDK ARM Compiler */

__attribute__((at(0x30040000))) ETH_DMADescTypeDef  DMARxDscrTab[ETH_RX_DESC_CNT]; /* Ethernet Rx DMA Descriptors */
__attribute__((at(0x30040060))) ETH_DMADescTypeDef  DMATxDscrTab[ETH_TX_DESC_CNT]; /* Ethernet Tx DMA Descriptors */
__attribute__((at(0x30040200))) uint8_t Rx_Buff[ETH_RX_DESC_CNT][ETH_MAX_PACKET_SIZE]; /* Ethernet Receive Buffer */

#elif defined ( __GNUC__ ) /* GNU Compiler */

ETH_DMADescTypeDef DMARxDscrTab[ETH_RX_DESC_CNT] __attribute__((section(".RxDecripSection"))); /* Ethernet Rx DMA Descriptors */
ETH_DMADescTypeDef DMATxDscrTab[ETH_TX_DESC_CNT] __attribute__((section(".TxDecripSection"))); /* Ethernet Tx DMA Descriptors */
uint8_t Rx_Buff[ETH_RX_DESC_CNT][ETH_MAX_PACKET_SIZE] __attribute__((section(".RxArraySection"))); /* Ethernet Receive Buffers */

#endif

ETH_TxPacketConfig TxConfig;

ETH_HandleTypeDef heth;

UART_HandleTypeDef huart3;

/* USER CODE BEGIN PV */

typedef struct { //ประกาศตัวแปร structure ของ led
	uint8_t led1;
	uint8_t led2;
	uint8_t led3;
	uint8_t led4;
	uint8_t led5;
	uint8_t led6;
	uint8_t led7;
	uint8_t led8;
	float someVar;
	int someVar2;
} SharedType;
SharedType *sharedMemory = (SharedType*) (0x38000000);
uint32_t ButtonTimeStamp = 0; //ตัวแปรเก็บค่า timestamp

GPIO_PinState SwitchState[2]; //Now,Last switch state 1-4
GPIO_PinState SwitchState2[2];
GPIO_PinState SwitchState3[2];
GPIO_PinState SwitchState4[2];
int charge1 = 0;// charge player 1 value
int charge2 = 0;// charge player 2 value

int heart1 = 0;// win player 1 value
int heart2 = 0;// win player 2 value
int c1 = 0;// charge player 1 flag
int c2 = 0;// charge player 2 flag
int d1 = 0;// defense player 1 flag
int d2 = 0;// defense player 2 flag
int a1 = 0;// attack player 1 flag
int a2 = 0;	//attack player 2 flag
uint64_t time = 0;// time constant
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
static void MX_GPIO_Init(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

/**
 * @brief  The application entry point.
 * @retval int
 */
int main(void) {
	/* USER CODE BEGIN 1 */

	/* USER CODE END 1 */

	/* USER CODE BEGIN Boot_Mode_Sequence_1 */
	/*HW semaphore Clock enable*/
	__HAL_RCC_HSEM_CLK_ENABLE();
	/* Activate HSEM notification for Cortex-M4*/
	HAL_HSEM_ActivateNotification(__HAL_HSEM_SEMID_TO_MASK(HSEM_ID_0));
	/*
	 Domain D2 goes to STOP mode (Cortex-M4 in deep-sleep) waiting for Cortex-M7 to
	 perform system initialization (system clock config, external memory configuration.. )
	 */
	HAL_PWREx_ClearPendingEvent();
	HAL_PWREx_EnterSTOPMode(PWR_MAINREGULATOR_ON, PWR_STOPENTRY_WFE,
	PWR_D2_DOMAIN);
	/* Clear HSEM flag */
	__HAL_HSEM_CLEAR_FLAG(__HAL_HSEM_SEMID_TO_MASK(HSEM_ID_0));

	/* USER CODE END Boot_Mode_Sequence_1 */
	/* MCU Configuration--------------------------------------------------------*/

	/* Reset of all peripherals, Initializes the Flash interface and the Systick. */
	HAL_Init();

	/* USER CODE BEGIN Init */

	/* USER CODE END Init */

	/* USER CODE BEGIN SysInit */

	/* USER CODE END SysInit */

	/* Initialize all configured peripherals */
	MX_GPIO_Init();
	/* USER CODE BEGIN 2 */

	/* USER CODE END 2 */

	/* Infinite loop */
	/* USER CODE BEGIN WHILE */
	while (1) {
		time = HAL_GetTick(); //get time value
		if (time - ButtonTimeStamp >= 200) {
			ButtonTimeStamp = time; //หน่าวเวลาของระบบ
			SwitchState[0] = HAL_GPIO_ReadPin(S1_GPIO_Port, S1_Pin);// read pin s1
			SwitchState2[0] = HAL_GPIO_ReadPin(S2_GPIO_Port, S2_Pin);// read pin s2
			SwitchState3[0] = HAL_GPIO_ReadPin(S3_GPIO_Port, S3_Pin);// read pin s3
			SwitchState4[0] = HAL_GPIO_ReadPin(S4_GPIO_Port, S4_Pin);// read pin s4
			if (SwitchState[1] == GPIO_PIN_SET
					&& SwitchState[0] == GPIO_PIN_RESET
					&& SwitchState2[1] == GPIO_PIN_SET
					&& SwitchState2[0] == GPIO_PIN_RESET){ //toggle s1 & s2 together
				a1 = 1;//set attack player 1 flag = 1

			} else if (SwitchState3[1] == GPIO_PIN_SET
					&& SwitchState3[0] == GPIO_PIN_RESET
					&& SwitchState4[1] == GPIO_PIN_SET
					&& SwitchState4[0] == GPIO_PIN_RESET) {//toggle s3 & s4 together
				a2 = 1;//set attack player 2 flag = 1

			} else if (SwitchState[1] == GPIO_PIN_SET
					&& SwitchState[0] == GPIO_PIN_RESET) {//toggle s1
				c1 = 1;//set charge player 1 flag = 1

			} else if (SwitchState3[1] == GPIO_PIN_SET
					&& SwitchState3[0] == GPIO_PIN_RESET) {//toggle s3
				c2 = 1;//set charge player 1 flag = 1
			} else if (SwitchState2[1] == GPIO_PIN_SET
					&& SwitchState2[0] == GPIO_PIN_RESET) {//toggle s2
				d1 = 1;//set defense player 1 flag = 1

			} else if (SwitchState4[1] == GPIO_PIN_SET
					&& SwitchState4[0] == GPIO_PIN_RESET) {//toggle s4
				d2 = 1;//set defense player 2 flag = 1

			}
			SwitchState[1] = SwitchState[0];
			SwitchState2[1] = SwitchState2[0];
			SwitchState3[1] = SwitchState3[0];
			SwitchState4[1] = SwitchState4[0];//reset switch switch state
			if (c1 == 1 && c2 == 1) { //ถ้าทั้งสองฝั่งออก charge and change
				charge1 += 1;
				charge2 += 1;//plus charge value
				c1 = 0;
				c2 = 0;// reset charge flag
			}
			if (c1 == 1 && d2 == 1) {//ถ้าทั้งสองฝั่งออก charge(p1) and defense(p2)
				charge1 += 1;//plus charge player 1 value

				c1 = 0;// reset charge player1 flag
				d2 = 0;// reset defense player2 flag
			}
			if (c2 == 1 && d1 == 1) {//ถ้าทั้งสองฝั่งออก charge(p2) and defense(p1)
				charge2 += 1;//plus charge player 2 value
				c2 = 0;// reset charge player2 flag
				d1 = 0;// reset defense player1 flag
			}
			if (d2 == 1 && d1 == 1) {//ถ้าทั้งสองฝั่งออก defense(p2) and defense(p1)
				d2 = 0;// reset defense player2 flag
				d1 = 0;// reset defense player1 flag
			}
			if (a2 == 1 && d1 == 1 && charge2 > 0) {//ถ้าทั้งสองฝั่งออก attack(p2) and defense(p1)
				charge2 -= 1; //ลด charge player 2 ที่เก็บไว้

				a2 = 0;// reset attack player2 flag
				d1 = 0;// reset defense player1 flag
			}
			if (a1 == 1 && d2 == 1 && charge1 > 0) {//ถ้าทั้งสองฝั่งออก attack(p1) and defense(p2)
				charge1 -= 1;//ลด charge player 1 ที่เก็บไว้

				a1 = 0; // reset attack player1 flag
				d2 = 0; // reset defense player2 flag
			}
			if (a1 == 1 && a2 == 1 && charge1 > 0 && charge2 > 0) {//ถ้าทั้งสองฝั่งออก attack(p1) and attack(p2)
				charge1 -= 1; //ลด charge player 1 ที่เก็บไว้
				charge2 -= 1;//ลด charge player 2 ที่เก็บไว้
				a1 = 0;// reset attack player1 flag
				a2 = 0;// reset attack player2 flag
			}
			if (a2 == 1 && c1 == 1 && charge1 > 0) {//ถ้าทั้งสองฝั่งออก attack(p2) and charge(p1)
				heart2 += 1;//คะแนน player 2 บวก 1
				charge1 = 0;//reset charge p1
				charge2 = 0;//reset charge p2
				a2 = 0;//reset attack player2 flag
				c1 = 0;//reset charge player1 flag
			}
			if (a1 == 1 && c2 == 1 && charge2 > 0) {//ถ้าทั้งสองฝั่งออก attack(p1) and charge(p2)
				heart1 += 1;//คะแนน player 1 บวก 1
				charge1 = 0;//reset charge p1
				charge2 = 0;//reset charge p2
				a1 = 0;//reset attack player1 flag
				c2 = 0;//reset charge player2 flag
			}
		}

		if (HAL_HSEM_FastTake(1) == HAL_OK) { //Can lock HSEM1
			if (heart1 == 0 && heart2 == 0) {
				sharedMemory->led1 = GPIO_PIN_SET;//initial condition ให้ led ดับ
				sharedMemory->led2 = GPIO_PIN_SET;
				sharedMemory->led3 = GPIO_PIN_SET;
				sharedMemory->led4 = GPIO_PIN_SET;
				sharedMemory->led5 = GPIO_PIN_SET;
				sharedMemory->led6 = GPIO_PIN_SET;
				sharedMemory->led7 = GPIO_PIN_SET;
				sharedMemory->led8 = GPIO_PIN_SET;
			}
			if (heart1 == 1) {
				sharedMemory->led1 = GPIO_PIN_RESET; //ถ้า heart player 1 = 1 แสดง led 1 ให้ติด
			}
			if (heart1 == 2) {
				sharedMemory->led2 = GPIO_PIN_RESET;//ถ้า heart player 1 = 2 แสดง led 2 ให้ติด
			}
			if (heart1 == 3) {
				sharedMemory->led3 = GPIO_PIN_RESET;//ถ้า heart player 1 = 3 แสดง led 3 ให้ติด
			}
			if (heart1 == 4) {
				sharedMemory->led4 = GPIO_PIN_RESET;//ถ้า heart player 1 = 4 แสดง led 4 ให้ติด
			}
			if (heart2 == 1) {
				sharedMemory->led5 = GPIO_PIN_RESET;//ถ้า heart player 2 = 1 แสดง led 5 ให้ติด
			}
			if (heart2 == 2) {
				sharedMemory->led6 = GPIO_PIN_RESET;//ถ้า heart player 2 = 1 แสดง led 6 ให้ติด
			}
			if (heart2 == 3) {
				sharedMemory->led7 = GPIO_PIN_RESET;//ถ้า heart player 2 = 1 แสดง led 7 ให้ติด
			}
			if (heart2 == 4) {
				sharedMemory->led8 = GPIO_PIN_RESET;//ถ้า heart player 2 = 1 แสดง led 8 ให้ติด
			}

			HAL_HSEM_Release(1, 0);//unlock hsem

		}
		/* USER CODE END WHILE */

		/* USER CODE BEGIN 3 */
	}
	/* USER CODE END 3 */
}

/**
 * @brief ETH Initialization Function
 * @param None
 * @retval None
 */
void MX_ETH_Init(void) {

	/* USER CODE BEGIN ETH_Init 0 */

	/* USER CODE END ETH_Init 0 */

	/* USER CODE BEGIN ETH_Init 1 */

	/* USER CODE END ETH_Init 1 */
	heth.Instance = ETH;
	heth.Init.MACAddr[0] = 0x00;
	heth.Init.MACAddr[1] = 0x80;
	heth.Init.MACAddr[2] = 0xE1;
	heth.Init.MACAddr[3] = 0x00;
	heth.Init.MACAddr[4] = 0x00;
	heth.Init.MACAddr[5] = 0x00;
	heth.Init.MediaInterface = HAL_ETH_RMII_MODE;
	heth.Init.TxDesc = DMATxDscrTab;
	heth.Init.RxDesc = DMARxDscrTab;
	heth.Init.RxBuffLen = 1524;

	/* USER CODE BEGIN MACADDRESS */

	/* USER CODE END MACADDRESS */

	if (HAL_ETH_Init(&heth) != HAL_OK) {
		Error_Handler();
	}

	memset(&TxConfig, 0, sizeof(ETH_TxPacketConfig));
	TxConfig.Attributes = ETH_TX_PACKETS_FEATURES_CSUM
			| ETH_TX_PACKETS_FEATURES_CRCPAD;
	TxConfig.ChecksumCtrl = ETH_CHECKSUM_IPHDR_PAYLOAD_INSERT_PHDR_CALC;
	TxConfig.CRCPadCtrl = ETH_CRC_PAD_INSERT;
	/* USER CODE BEGIN ETH_Init 2 */

	/* USER CODE END ETH_Init 2 */

}

/**
 * @brief USART3 Initialization Function
 * @param None
 * @retval None
 */
void MX_USART3_UART_Init(void) {

	/* USER CODE BEGIN USART3_Init 0 */

	/* USER CODE END USART3_Init 0 */

	/* USER CODE BEGIN USART3_Init 1 */

	/* USER CODE END USART3_Init 1 */
	huart3.Instance = USART3;
	huart3.Init.BaudRate = 115200;
	huart3.Init.WordLength = UART_WORDLENGTH_8B;
	huart3.Init.StopBits = UART_STOPBITS_1;
	huart3.Init.Parity = UART_PARITY_NONE;
	huart3.Init.Mode = UART_MODE_TX_RX;
	huart3.Init.HwFlowCtl = UART_HWCONTROL_NONE;
	huart3.Init.OverSampling = UART_OVERSAMPLING_16;
	huart3.Init.OneBitSampling = UART_ONE_BIT_SAMPLE_DISABLE;
	huart3.Init.ClockPrescaler = UART_PRESCALER_DIV1;
	huart3.AdvancedInit.AdvFeatureInit = UART_ADVFEATURE_NO_INIT;
	if (HAL_UART_Init(&huart3) != HAL_OK) {
		Error_Handler();
	}
	if (HAL_UARTEx_SetTxFifoThreshold(&huart3,
	UART_TXFIFO_THRESHOLD_1_8) != HAL_OK) {
		Error_Handler();
	}
	if (HAL_UARTEx_SetRxFifoThreshold(&huart3,
	UART_RXFIFO_THRESHOLD_1_8) != HAL_OK) {
		Error_Handler();
	}
	if (HAL_UARTEx_DisableFifoMode(&huart3) != HAL_OK) {
		Error_Handler();
	}
	/* USER CODE BEGIN USART3_Init 2 */

	/* USER CODE END USART3_Init 2 */

}

/**
 * @brief GPIO Initialization Function
 * @param None
 * @retval None
 */
static void MX_GPIO_Init(void) {
	GPIO_InitTypeDef GPIO_InitStruct = { 0 };

	/* GPIO Ports Clock Enable */
	__HAL_RCC_GPIOC_CLK_ENABLE();
	__HAL_RCC_GPIOE_CLK_ENABLE();
	__HAL_RCC_GPIOG_CLK_ENABLE();

	/*Configure GPIO pin : B1_Pin */
	GPIO_InitStruct.Pin = B1_Pin;
	GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	HAL_GPIO_Init(B1_GPIO_Port, &GPIO_InitStruct);

	/*Configure GPIO pins : S4_Pin S2_Pin S3_Pin */
	GPIO_InitStruct.Pin = S4_Pin | S2_Pin | S3_Pin;
	GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	HAL_GPIO_Init(GPIOE, &GPIO_InitStruct);

	/*Configure GPIO pin : S1_Pin */
	GPIO_InitStruct.Pin = S1_Pin;
	GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	HAL_GPIO_Init(S1_GPIO_Port, &GPIO_InitStruct);

}

/* USER CODE BEGIN 4 */

/* USER CODE END 4 */

/**
 * @brief  This function is executed in case of error occurrence.
 * @retval None
 */
void Error_Handler(void) {
	/* USER CODE BEGIN Error_Handler_Debug */
	/* User can add his own implementation to report the HAL error return state */
	__disable_irq();
	while (1) {
	}
	/* USER CODE END Error_Handler_Debug */
}

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
