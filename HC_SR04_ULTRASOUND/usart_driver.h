

#ifndef __USART_DRIVER_H__

#define __USART_DRIVER_H__

#include <stdint.h>
#include "stm32f407.h"

typedef struct{
	uint8_t USART_MODE;
	uint32_t USART_Baud;
	uint8_t USART_NoOfStopBits;
	uint8_t	USART_ParityControl;
	uint8_t USART_WordLength;
	uint8_t USART_HXFlowControl;
}USART_PinConfig;

typedef struct{
	USARTx_RegDef_t *pUSARTx;
	USART_PinConfig Pin_Config;
	uint8_t *pTxBuffer;
	uint8_t *pRxBuffer;
	uint32_t TxLen;
	uint32_t RxLen;
	uint8_t TxBusyState;
	uint8_t RxBusyState;
}USART_Handler_t;

/*
 * @USART_MODE
 */
#define USART_MODE_RX_ONLY	0
#define USART_MODE_TX_ONLY	1
#define USART_MODE_TXRX		2

/*
 * @USART_Baud
 */
#define USART_STD_BAUD_1200			1200
#define USART_STD_BAUD_2400			2400
#define USART_STD_BAUD_4800			4800
#define USART_STD_BAUD_9600			9600
#define USART_STD_BAUD_19200		19200
#define USART_STD_BAUD_38400		38400
#define USART_STD_BAUD_57600		57600
#define USART_STD_BAUD_115200		115200

/*
 *@USART_NoOfStopBits
 */
#define USART_PARITY_EN_ODD			2
#define USART_PARITY_EN_EVEN		1
#define USART_PARITY_DISABLE		0

/*
 *@USART_WordLength
 */
#define USART_WORDLEN_8BITS		0
#define USART_WORDLEN_9BITS		1

/*
 * @USART_NoOfStopBits
 */
#define USART_STOPBITS_1		0
#define USART_STOPBITS_0_5		1
#define USART_STOPBITS_2		2
#define USART_STOPBITS_1_5		3

/*
 * @USART_HWcONTROLFLOW
 */
#define USART_HW_FLOW_CTRL_NONE		0
#define USART_HW_FLOW_CTRL_CTS		1
#define USART_HW_FLOW_CTRL_RTS		2
#define USART_HW_FLOW_CTRL_CTS_RTS	3

/*
 * pin macros
 */

#define USART_SR_PE		0
#define USART_SR_FE		1
#define USART_SR_NF		2
#define USART_SR_ORE	3
#define USART_SR_IDLE	4
#define USART_SR_RXNE	5
#define USART_SR_TC		6
#define USART_SR_TXE	7
#define USART_SR_LBD	8
#define USART_SR_CTS	9

#define USART_BRR_DIVFR	 	0
#define USART_BRR_DIVMANT	4

#define USART_CR1_SBK		0
#define USART_CR1_RWU		1
#define USART_CR1_RE		2
#define USART_CR1_TE		3
#define USART_CR1_IDLEIE	4
#define USART_CR1_RXNEIE	5
#define USART_CR1_TCIE		6
#define USART_CR1_TXEIE		7
#define USART_CR1_PEIE		8
#define USART_CR1_PS		9
#define USART_CR1_PCE		10
#define USART_CR1_WAKE		11
#define USART_CR1_M			12
#define USART_CR1_UE		13
#define USART_CR1_OVER8		15

#define USART_CR2_ADD		0
#define USART_CR2_LBDL		5
#define USART_CR2_LBDIE		6
#define USART_CR2_LBCL		8
#define USART_CR2_CPHA		9
#define USART_CR2_CPOL		10
#define USART_CR2_CLKEN		11
#define USART_CR2_STOP		12
#define USART_CR2_LINEN		14

#define USART_CR3_EIE		0
#define USART_CR3_IREN		1
#define USART_CR3_IRLP		2
#define USART_CR3_HDSEL		3
#define USART_CR3_NACK		4
#define USART_CR3_SCEN		5
#define USART_CR3_DMAR		6
#define USART_CR3_DMAT		7
#define USART_CR3_RTSE		8
#define USART_CR3_CTSE		9
#define USART_CR3_CTSIE		10
#define USART_CR3_ONEBIT	11

#define USART_GTPR_PSC		0
#define USART_GTPR_GT		8


/*
 * macros
 */
#define USART_READY			0
#define USART_BUSY_IN_TX 	1
#define USART_BUSY_IN_RX 	2

#define USART_EVENT_TX_CMPLT	0
#define USART_EVENT_RX_CMPLT	1
#define USART_EVENT_CTS			2
#define USART_EVENT_IDLE		3
#define USART_ERR_ORE			4
#define USART_ERR_FE			5
#define USART_ERR_NE			6

/*
 * API SUPPORTED BY THIS DRIVER
 */
void USART_PeriClockControl(USARTx_RegDef_t *pUSARTx, uint8_t State);

void USART_PeripheralControl(USARTx_RegDef_t *pUSARTx, uint8_t State);

uint8_t USART_GetFlagStatus(USARTx_RegDef_t *pUSARTx, uint8_t Flagname);

void USART_ClearFlag(USARTx_RegDef_t *pUSARTx, uint16_t FlagName);


void USART_IRQInterruptConfig(uint8_t IRQNumber, uint8_t State);
void USART_IRQPriorityConfig(uint8_t IRQNumber, uint32_t IRQPriority);

void USART_Init(USART_Handler_t *pUSART_Handler);
void USART_reset(USARTx_RegDef_t *pUSARTx);

void USART_SendData(USART_Handler_t *pUSART_Handle, uint8_t *TxBuffer, uint32_t Len);
void USART_ReceiveData(USART_Handler_t *pUSART_Handle, uint8_t *RxBuffer, uint32_t Len);

uint8_t USART_SendDataIT(USART_Handler_t *pUSART_Handler, uint8_t *TxBuffer, uint32_t Len);
uint8_t USART_ReceiveDataIT(USART_Handler_t *pUSART_Handler, uint8_t *RxBuffer, uint32_t Len);

void USART_IRQHandling(USART_Handler_t *pUSART_Handler);

void USART_ApplicationCallBack(USART_Handler_t *pUSART_Handler, uint8_t EVENT);


#endif