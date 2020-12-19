#include "usart_driver.h"
#include <stdio.h>

uint16_t AHB_PreScaler[8] = {2,4,8,16,64,128,256,512};
uint8_t APB1_PreScaler[4] = { 2, 4 , 8, 16};


uint32_t RCC_GetPCLK1Value(void)
{
	uint32_t pclk1,SystemClk;

	uint8_t clksrc,temp,ahbp,apb1p;

	clksrc = ((RCC->RCC_CFGR >> 2) & 0x3);

	if(clksrc == 0 )
	{
		SystemClk = 16000000;
	}else if(clksrc == 1)
	{
		SystemClk = 8000000;
	}

	//for ahb
	temp = ((RCC->RCC_CFGR >> 4 ) & 0xF);

	if(temp < 8)
	{
		ahbp = 1;
	}else
	{
		ahbp = AHB_PreScaler[temp-8];
	}



	//apb1
	temp = ((RCC->RCC_CFGR >> 10 ) & 0x7);

	if(temp < 4)
	{
		apb1p = 1;
	}else
	{
		apb1p = APB1_PreScaler[temp-4];
	}

	pclk1 =  (SystemClk / ahbp) /apb1p;

	return pclk1;
}

void USART_SetBaudRate(USARTx_RegDef_t *pUSARTx, uint32_t BaudRate)
{
	//Variable to hold the APB clock
	uint32_t PCLKx;

	uint32_t usartdiv;

	//variables to hold Mantissa and Fraction values
	uint32_t M_part, F_part;

	uint32_t tempreg = 0;

	//Get the value of APB bus clock in to the variable PCLKx
	// if (pUSARTx == USART1 || pUSARTx == USART6) {
	// 	//USART1 and USART6 are hanging on APB2 bus
	// 	PCLKx = RCC_GetPCLK2Value();
	// } else {
		PCLKx = RCC_GetPCLK1Value();
	// }

	//Check for OVER8 configuration bit
	if (pUSARTx->CR1 & (1 << USART_CR1_OVER8)) {
		//OVER8 = 1 , over sampling by 8
		usartdiv = ((25 * PCLKx) / (2 * BaudRate));
	} else {
		//over sampling by 16
		usartdiv = ((25 * PCLKx) / (4 * BaudRate));
	}

	//Calculate the Mantissa part
	M_part = usartdiv / 100;

	//Place the Mantissa part in appropriate bit position . refer USART_BRR
	tempreg |= M_part << 4;

	//Extract the fraction part
	F_part = (usartdiv - (M_part * 100));

	//Calculate the final fractional
	if (pUSARTx->CR1 & (1 << USART_CR1_OVER8)) {
		//OVER8 = 1 , over sampling by 8
		F_part = (((F_part * 8) + 50) / 100) & ((uint8_t) 0x07);

	} else {
		//over sampling by 16
		F_part = (((F_part * 16) + 50) / 100) & ((uint8_t) 0x0F);

	}

	//Place the fractional part in appropriate bit position . refer USART_BRR
	tempreg |= F_part;

	//copy the value of tempreg in to BRR register
	pUSARTx->BRR = tempreg;

}



void USART_PeriClockControl(USARTx_RegDef_t *pUSARTx, uint8_t State)
{
    if (State == ENABLE) {
		if (pUSARTx == UART4) {
			UART4_PCLK_EN();
		} 

	} else {
        if (pUSARTx == UART4) {
			UART4_PCLK_DI();
		} 
    }
		
}

void USART_PeripheralControl(USARTx_RegDef_t *pUSARTx, uint8_t State)
{
    if (State == ENABLE) {
		pUSARTx->CR1 |= (1 << USART_CR1_UE);
	} else {
		pUSARTx->CR1 &= ~(1 << USART_CR1_UE);
	}
}

uint8_t USART_GetFlagStatus(USARTx_RegDef_t *pUSARTx, uint8_t Flagname)
{
    if (pUSARTx->SR & (1 << Flagname)) {
		return SET;
	}
	return RESET;
}

void USART_ClearFlag(USARTx_RegDef_t *pUSARTx, uint16_t FlagName)
{
    pUSARTx->SR &= ~(1 << FlagName);
}


void USART_IRQInterruptConfig(uint8_t IRQNumber, uint8_t State)
{
    if (State == ENABLE) {
		if (IRQNumber <= 31) {
			//program ISER0 register
			*NVIC_ISER0 |= (1 << IRQNumber);
		} else if (IRQNumber > 31 && IRQNumber < 64) {
			//program ISER1 register
			*NVIC_ISER1 |= (1 << (IRQNumber - 32));
		} else if (IRQNumber >= 64 && IRQNumber < 96) {
			//program ISER2 register
			*NVIC_ISER2 |= (1 << (IRQNumber - 64));
		}
	} else {
		if (IRQNumber <= 31) {
			//program ICER0 register
			*NVIC_ICER0 |= (1 << IRQNumber);
		} else if (IRQNumber > 31 && IRQNumber < 64) {
			//program ICER1 register
			*NVIC_ICER1 |= (1 << (IRQNumber - 32));
		} else if (IRQNumber > 63 && IRQNumber < 96) {
			//program ICER2 register
			*NVIC_ICER2 |= (1 << (IRQNumber - 64));
		}

	}
}
void USART_IRQPriorityConfig(uint8_t IRQNumber, uint32_t IRQPriority)
{
    uint8_t ipr_x = IRQNumber / 4;
	uint8_t iprx_sec = IRQNumber % 4;

	*(NVIC_IPR_BASE_ADDR + ipr_x) |= (IRQPriority << (8 * iprx_sec + (4)));
}

void USART_Init(USART_Handler_t *pUSART_Handler)
{
    //enable the clock peripheral
	USART_PeriClockControl(pUSART_Handler->pUSARTx, ENABLE);


	//enable the USART MODE
	if(pUSART_Handler->Pin_Config.USART_MODE == USART_MODE_TX_ONLY){
		pUSART_Handler->pUSARTx->CR1 |= (1 << USART_CR1_TE);
	}else if(pUSART_Handler->Pin_Config.USART_MODE == USART_MODE_RX_ONLY){
		pUSART_Handler->pUSARTx->CR1 |= (1 << USART_CR1_RE);
	}else if (pUSART_Handler->Pin_Config.USART_MODE == USART_MODE_TXRX){
		pUSART_Handler->pUSARTx->CR1 |= ((1 << USART_CR1_TE)|(1 << USART_CR1_RE));
	}

	//configure the word length
	if(pUSART_Handler->Pin_Config.USART_WordLength == USART_WORDLEN_8BITS){
		pUSART_Handler->pUSARTx->CR1 &= ~(1 << USART_CR1_M);
	}else {
		pUSART_Handler->pUSARTx->CR1 |= (1 << USART_CR1_M);
	}
	//configure Parity mode
	if(pUSART_Handler->Pin_Config.USART_ParityControl ==USART_PARITY_EN_ODD ){
		pUSART_Handler->pUSARTx->CR1 |= ((1 << USART_CR1_PCE)|(1 << USART_CR1_PS));
	}else if (pUSART_Handler->Pin_Config.USART_ParityControl ==USART_PARITY_EN_EVEN ){
		pUSART_Handler->pUSARTx->CR1 |= (1 << USART_CR1_PCE);
		pUSART_Handler->pUSARTx->CR1 &= ~(1 << USART_CR1_PS);
	}else{
		pUSART_Handler->pUSARTx->CR1 &= ~(1 << USART_CR1_PCE);
	}


	//configure No of stop bits
	pUSART_Handler->pUSARTx->CR2 |= ( pUSART_Handler->Pin_Config.USART_NoOfStopBits << USART_CR2_STOP);


	//configure baud rate
	USART_SetBaudRate(pUSART_Handler->pUSARTx, pUSART_Handler->Pin_Config.USART_Baud);


	////configure hardware control flow

	if(pUSART_Handler->Pin_Config.USART_HXFlowControl == USART_HW_FLOW_CTRL_CTS){
		pUSART_Handler->pUSARTx->CR3 &= ~(1 << USART_CR3_CTSE);
		pUSART_Handler->pUSARTx->CR3 |= (1 << USART_CR3_CTSE);
	}else if (pUSART_Handler->Pin_Config.USART_HXFlowControl == USART_HW_FLOW_CTRL_RTS){
		pUSART_Handler->pUSARTx->CR3 &= ~(1 << USART_CR3_RTSE);
		pUSART_Handler->pUSARTx->CR3 |= (1 << USART_CR3_RTSE);
	}else if (pUSART_Handler->Pin_Config.USART_HXFlowControl == USART_HW_FLOW_CTRL_CTS_RTS){
		pUSART_Handler->pUSARTx->CR3 &= ~(1 << USART_CR3_CTSE);
		pUSART_Handler->pUSARTx->CR3 |= (1 << USART_CR3_CTSE);
		pUSART_Handler->pUSARTx->CR3 &= ~(1 << USART_CR3_RTSE);
		pUSART_Handler->pUSARTx->CR3 |= (1 << USART_CR3_RTSE);
	}else {
		pUSART_Handler->pUSARTx->CR3 &= ~(1 << USART_CR3_CTSE);
		pUSART_Handler->pUSARTx->CR3 &= ~(1 << USART_CR3_RTSE);
	}

}

void USART_reset(USARTx_RegDef_t *pUSARTx)
{
    if (pUSARTx == UART4) {
		UART4_REG_RESET();
	} 
}

void USART_SendData(USART_Handler_t *pUSART_Handle, uint8_t *TxBuffer, uint32_t Len)
{
    uint16_t *pdata;
	//Loop over until "Len" number of bytes are transferred
	for (uint32_t i = 0; i < Len; i++) {
		printf("Here1\n");
		//Implement the code to wait until TXE flag is set in the SR
		while (!USART_GetFlagStatus(pUSART_Handle->pUSARTx, USART_SR_TXE));
		printf("Here2\n");

		//Check the USART_WordLength item for 9BIT or 8BIT in a frame
		if (pUSART_Handle->Pin_Config.USART_WordLength == USART_WORDLEN_9BITS) {
			//if 9BIT load the DR with 2bytes masking  the bits other than first 9 bits
			pdata = (uint16_t*) TxBuffer;
			pUSART_Handle->pUSARTx->DR = (*pdata & (uint16_t) 0x01FF);

			//check for USART_ParityControl
			if (pUSART_Handle->Pin_Config.USART_ParityControl == USART_PARITY_DISABLE) {
				//No parity is used in this transfer , so 9bits of user data will be sent
				//Implement the code to increment pTxBuffer twice
				TxBuffer++;
				TxBuffer++;
			} else {
				//Parity bit is used in this transfer . so 8bits of user data will be sent
				//The 9th bit will be replaced by parity bit by the hardware
				TxBuffer++;
			}
		} else {
			//This is 8bit data transfer
			pUSART_Handle->pUSARTx->DR = (*TxBuffer & (uint8_t) 0xFF);

			//Implement the code to increment the buffer address
			TxBuffer++;
		}
	}

	printf("Here3\n");

	//Implement the code to wait till TC flag is set in the SR
	while (!USART_GetFlagStatus(pUSART_Handle->pUSARTx, USART_SR_TC));

	printf("Here4\n");
}
void USART_ReceiveData(USART_Handler_t *pUSART_Handle, uint8_t *RxBuffer, uint32_t Len)
{
    //Loop over until "Len" number of bytes are transferred
	for (uint32_t i = 0; i < Len; i++) {
		//Implement the code to wait until RXNE flag is set in the SR
		while (!USART_GetFlagStatus(pUSART_Handle->pUSARTx, USART_SR_RXNE))
			;

		//Check the USART_WordLength to decide whether we are going to receive 9bit of data in a frame or 8 bit
		if (pUSART_Handle->Pin_Config.USART_WordLength == USART_WORDLEN_9BITS) {
			//We are going to receive 9bit data in a frame

			//Now, check are we using USART_ParityControl control or not
			if (pUSART_Handle->Pin_Config.USART_ParityControl == USART_PARITY_DISABLE) {
				//No parity is used , so all 9bits will be of user data

				//read only first 9 bits so mask the DR with 0x01FF
				*((uint16_t*) RxBuffer) = (pUSART_Handle->pUSARTx->DR & (uint16_t) 0x01FF);

				//Now increment the pRxBuffer two times
				RxBuffer++;
				RxBuffer++;
			} else {
				//Parity is used, so 8bits will be of user data and 1 bit is parity
				*RxBuffer = (pUSART_Handle->pUSARTx->DR & (uint8_t) 0xFF);
				RxBuffer++;
			}
		} else {
			//We are going to receive 8bit data in a frame

			//Now, check are we using USART_ParityControl control or not
			if (pUSART_Handle->Pin_Config.USART_ParityControl == USART_PARITY_DISABLE) {
				//No parity is used , so all 8bits will be of user data

				//read 8 bits from DR
				*RxBuffer = (uint8_t) (pUSART_Handle->pUSARTx->DR & (uint8_t) 0xFF);
			}

			else {
				//Parity is used, so , 7 bits will be of user data and 1 bit is parity

				//read only 7 bits , hence mask the DR with 0X7F
				*RxBuffer = (uint8_t) (pUSART_Handle->pUSARTx->DR & (uint8_t) 0x7F);
			}

			//Now , increment the pRxBuffer
			RxBuffer++;
		}
	}
}

uint8_t USART_SendDataIT(USART_Handler_t *pUSART_Handler, uint8_t *TxBuffer, uint32_t Len)
{
    uint8_t txstate = pUSART_Handler->TxBusyState;

	if (txstate != USART_BUSY_IN_TX) {
		pUSART_Handler->TxLen = Len;
		pUSART_Handler->pTxBuffer = TxBuffer;
		pUSART_Handler->TxBusyState = USART_BUSY_IN_TX;

		//Implement the code to enable interrupt for TXE
		pUSART_Handler->pUSARTx->CR1 |= (1 << USART_CR1_TXEIE);

		//Implement the code to enable interrupt for TC
		pUSART_Handler->pUSARTx->CR1 |= (1 << USART_CR1_TCIE);

	}

	return txstate;
}
uint8_t USART_ReceiveDataIT(USART_Handler_t *pUSART_Handler, uint8_t *RxBuffer, uint32_t Len)
{
    uint8_t rxstate = pUSART_Handler->RxBusyState;

	if (rxstate != USART_BUSY_IN_RX) {
		pUSART_Handler->RxLen = Len;
		pUSART_Handler->pRxBuffer = RxBuffer;
		pUSART_Handler->RxBusyState = USART_BUSY_IN_RX;

	//	(void) pUSART_Handler->pUSARTx->DR;

		//Implement the code to enable interrupt for RXNE
		pUSART_Handler->pUSARTx->CR1 |= (1 << USART_CR1_RXNEIE);

	}

	return rxstate;
}

void USART_IRQHandling(USART_Handler_t *pUSART_Handler)
{
    uint32_t temp1 , temp2;

		uint16_t *pdata;

	/*************************Check for TC flag ********************************************/

	    //Implement the code to check the state of TC bit in the SR
		temp1 = pUSART_Handler->pUSARTx->SR & ( 1 << USART_SR_TC);

		 //Implement the code to check the state of TCEIE bit
		temp2 = pUSART_Handler->pUSARTx->CR1 & ( 1 << USART_CR1_TCIE);

		if(temp1 && temp2 )
		{
			//this interrupt is because of TC

			//close transmission and call application callback if TxLen is zero
			if ( pUSART_Handler->TxBusyState == USART_BUSY_IN_TX)
			{
				//Check the TxLen . If it is zero then close the data transmission
				if(! pUSART_Handler->TxLen )
				{
					//Implement the code to clear the TC flag
					pUSART_Handler->pUSARTx->SR &= ~( 1 << USART_SR_TC);

					//Implement the code to clear the TCIE control bit

					//Reset the application state
					pUSART_Handler->TxBusyState = USART_READY;

					//Reset Buffer address to NULL
					pUSART_Handler->pTxBuffer = NULL;

					//Reset the length to zero
					pUSART_Handler->TxLen = 0;

					//Call the application call back with event USART_EVENT_TX_CMPLT
					USART_ApplicationCallBack(pUSART_Handler,USART_EVENT_TX_CMPLT);
				}
			}
		}

    /*************************Check for TXE flag ********************************************/

		//Implement the code to check the state of TXE bit in the SR
		temp1 = pUSART_Handler->pUSARTx->SR & ( 1 << USART_SR_TXE);

		//Implement the code to check the state of TXEIE bit in CR1
		temp2 = pUSART_Handler->pUSARTx->CR1 & ( 1 << USART_CR1_TXEIE);


		if(temp1 && temp2 )
		{
			//this interrupt is because of TXE

			if(pUSART_Handler->TxBusyState == USART_BUSY_IN_TX)
			{
				//Keep sending data until Txlen reaches to zero
				if(pUSART_Handler->TxLen > 0)
				{
					//Check the USART_WordLength item for 9BIT or 8BIT in a frame
					if(pUSART_Handler->Pin_Config.USART_WordLength == USART_WORDLEN_9BITS)
					{
						//if 9BIT load the DR with 2bytes masking  the bits other than first 9 bits
						pdata = (uint16_t*) pUSART_Handler->pTxBuffer;
						pUSART_Handler->pUSARTx->DR = (*pdata & (uint16_t)0x01FF);

						//check for USART_ParityControl
						if(pUSART_Handler->Pin_Config.USART_ParityControl == USART_PARITY_DISABLE)
						{
							//No parity is used in this transfer , so 9bits of user data will be sent
							//Implement the code to increment pTxBuffer twice
							pUSART_Handler->pTxBuffer++;
							pUSART_Handler->pTxBuffer++;
							pUSART_Handler->TxLen-=2;
						}
						else
						{
							//Parity bit is used in this transfer . so 8bits of user data will be sent
							//The 9th bit will be replaced by parity bit by the hardware
							pUSART_Handler->pTxBuffer++;
							pUSART_Handler->TxLen-=1;
						}
					}
					else
					{
						//This is 8bit data transfer
						pUSART_Handler->pUSARTx->DR = (*pUSART_Handler->pTxBuffer  & (uint8_t)0xFF);

						//Implement the code to increment the buffer address
						pUSART_Handler->pTxBuffer++;
						pUSART_Handler->TxLen-=1;
					}

				}
				if (pUSART_Handler->TxLen == 0 )
				{
					//TxLen is zero
					//Implement the code to clear the TXEIE bit (disable interrupt for TXE flag )
					pUSART_Handler->pUSARTx->CR1 &= ~( 1 << USART_CR1_TXEIE);
				}
			}
		}

	/*************************Check for RXNE flag ********************************************/

		temp1 = pUSART_Handler->pUSARTx->SR & ( 1 << USART_SR_RXNE);
		temp2 = pUSART_Handler->pUSARTx->CR1 & ( 1 << USART_CR1_RXNEIE);


		if(temp1 && temp2 )
		{
			//this interrupt is because of rxne
			if(pUSART_Handler->RxBusyState == USART_BUSY_IN_RX)
			{
				if(pUSART_Handler->RxLen > 0)
				{
					//Check the USART_WordLength to decide whether we are going to receive 9bit of data in a frame or 8 bit
					if(pUSART_Handler->Pin_Config.USART_WordLength == USART_WORDLEN_9BITS)
					{
						//We are going to receive 9bit data in a frame

						//Now, check are we using USART_ParityControl control or not
						if(pUSART_Handler->Pin_Config.USART_ParityControl == USART_PARITY_DISABLE)
						{
							//No parity is used , so all 9bits will be of user data

							//read only first 9 bits so mask the DR with 0x01FF
							*((uint16_t*) pUSART_Handler->pRxBuffer) = (pUSART_Handler->pUSARTx->DR  & (uint16_t)0x01FF);

							//Now increment the pRxBuffer two times
							pUSART_Handler->pRxBuffer++;
							pUSART_Handler->pRxBuffer++;
							pUSART_Handler->RxLen-=2;
						}
						else
						{
							//Parity is used, so 8bits will be of user data and 1 bit is parity
							 *pUSART_Handler->pRxBuffer = (pUSART_Handler->pUSARTx->DR  & (uint8_t)0xFF);
							 pUSART_Handler->pRxBuffer++;
							 pUSART_Handler->RxLen-=1;
						}
					}
					else
					{
						//We are going to receive 8bit data in a frame

						//Now, check are we using USART_ParityControl control or not
						if(pUSART_Handler->Pin_Config.USART_ParityControl == USART_PARITY_DISABLE)
						{
							//No parity is used , so all 8bits will be of user data

							//read 8 bits from DR
							 *pUSART_Handler->pRxBuffer = (uint8_t) (pUSART_Handler->pUSARTx->DR  & (uint8_t)0xFF);

						}

						else
						{
							//Parity is used, so , 7 bits will be of user data and 1 bit is parity

							//read only 7 bits , hence mask the DR with 0X7F
							 *pUSART_Handler->pRxBuffer = (uint8_t) (pUSART_Handler->pUSARTx->DR  & (uint8_t)0x7F);

						}

						//Now , increment the pRxBuffer
						pUSART_Handler->pRxBuffer++;
						pUSART_Handler->RxLen-=1;
					}


				}//if of >0

				if(! pUSART_Handler->RxLen)
				{
					//disable the rxne
					pUSART_Handler->pUSARTx->CR1 &= ~( 1 << USART_CR1_RXNEIE );
					pUSART_Handler->RxBusyState = USART_READY;
					USART_ApplicationCallBack(pUSART_Handler,USART_EVENT_RX_CMPLT);
				}
			}
		}

}

__weak void USART_ApplicationCallBack(USART_Handler_t *pUSART_Handler, uint8_t EVENT)
{

}