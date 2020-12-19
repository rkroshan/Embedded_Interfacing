
#include <stdio.h>
#include <stdbool.h>
#include "main.h"
#include <string.h>
// #include "HC_SR04.h"
#include "gpio_driver.h"
#include "timer_driver.h"
#include "usart_driver.h"

#if !defined(__SOFT_FP__) && defined(__ARM_FP)
  #warning "FPU is not initialized, but the project is compiling for an FPU. Please initialize the FPU before use."
#endif


#ifdef SEMIHOSTING
extern void initialise_monitor_handles(void);
#endif

void enable_processor_faults(void);

#define GPIO_DELAY 500000
GPIO_handler_t handler, AFhandler;
void gpio_init_test(uint8_t PinNumber);
void gpio_delay(uint32_t time);
void gpio_pin_toggle(GPIO_RegDef_t* pGpiox, uint8_t PinNumber);
void gpio_toggle(GPIO_RegDef_t* pGpiox, uint8_t PinNumber);
void tim_delay(TIMx_RegDef_t* pTimx, uint32_t delay);

void testing_general_timer(void);
void testing_basic_timer_with_interrupt(void);
void testing_input_capture_with_interrupt(void);
void testing_output_capture_with_interrupt(void); //it will generate pwm square wave which will toggle led
void testing_hcsr04(void);
void testing_hcsr04_it(void);
void testing_uart(void);
bool basic_timer_interrupt = false;
bool input_capture_with_interrupt = false;
bool output_capture_with_interrupt = false;
bool test_hcsr04 = false;

bool is_capture1 = false;
bool is_capture2 = false;
uint32_t capture[2];

Timer_s_Handler_t TSHandler; //need for testing_basic_timer_with_interrupt testing

Timer_IC_Handler_t TICHandler; //need for testing_input_capture_with_interrupt testing

Timer_OC_Handler_t TOCHandler; //need for testing_output_capture_with_interrupt

const float speedOfSound = 0.0343/2; // (cm/us)/2

uint8_t icFlag = 0;
uint8_t captureIdx=0;
uint32_t edge1Time=0, edge2Time=0;
float distance;

//This flag indicates reception completion
uint8_t rxCmplt = RESET;

int main(void){

	enable_processor_faults();

#ifdef SEMIHOSTING
	initialise_monitor_handles();
#endif


	// testing_general_timer();
	// testing_basic_timer_with_interrupt();
	// testing_input_capture_with_interrupt();
	// testing_output_capture_with_interrupt();
	// testing_hcsr04();
	testing_hcsr04_it();
	// testing_uart();
	while(1){
		//just for safety to prevent memory fault
	}	
}

void gpio_toggle(GPIO_RegDef_t* pGpiox, uint8_t PinNumber){
	pGpiox->ODR ^= (1 << PinNumber);
}

void gpio_init_test(uint8_t PinNumber){
	handler.pGpiox = GPIOD;
	handler.Gpio_PinConfig.PinNumber = PinNumber;
	handler.Gpio_PinConfig.PinMode = GPIO_MODE_OUTPUT;
	handler.Gpio_PinConfig.PinOtype = GPIO_OTYPE_PUSHPULL;
	handler.Gpio_PinConfig.PinOspeed = GPIO_OSPEED_HIGH;
	handler.Gpio_PinConfig.PinPuPdControl = GPIO_PUPD_PUP;
	
	Gpio_reset(handler.pGpiox);
	Gpio_init(&handler);
}

void gpio_pin_toggle(GPIO_RegDef_t* pGpiox, uint8_t PinNumber){
	GPIO_writeOutputInPin(pGpiox,PinNumber, GPIO_BIT_RESET);
	#ifdef SEMIHOSTING
		//printf("It's working 1\n");
	#endif
		gpio_delay(GPIO_DELAY);
	#ifdef SEMIHOSTING
		//printf("It's working 2\n");
	#endif
	GPIO_writeOutputInPin(pGpiox,PinNumber,GPIO_BIT_SET);
	gpio_delay(GPIO_DELAY);
}

void gpio_delay(uint32_t time){
	for(uint32_t i=0; i<time; i++){
	}
}

void tim_delay(TIMx_RegDef_t* pTimx, uint32_t delay){
	pTimx->TIMx_ARR = delay -1;
	pTimx->TIMx_EGR |= (1 << 0);
	pTimx->TIMx_SR &= ~(1 << 0);
	pTimx->TIMx_CR1 |= (1 << 0);
	while(!(pTimx->TIMx_SR & 0x0001));
	pTimx->TIMx_SR &= ~(1 << 0);
}

void testing_uart(void)
{
	char msg[1024] = "UART Tx testing...\n\r";

	GPIO_handler_t usart_gpios;

	usart_gpios.pGpiox = GPIOA;
	usart_gpios.Gpio_PinConfig.PinMode = GPIO_MODE_ALTFUNC;
	usart_gpios.Gpio_PinConfig.PinOtype = GPIO_OTYPE_PUSHPULL;
	usart_gpios.Gpio_PinConfig.PinPuPdControl = GPIO_PUPD_PUP;
	usart_gpios.Gpio_PinConfig.PinOspeed = GPIO_OSPEED_HIGH;
	usart_gpios.Gpio_PinConfig.PinAltFuncMode = GPIO_AF8;

	//USART4 TX PA0
	usart_gpios.Gpio_PinConfig.PinNumber = GPIO_PIN_0;
	Gpio_reset(usart_gpios.pGpiox);
	Gpio_init(&usart_gpios);

	//USART4 RX PA1
	usart_gpios.Gpio_PinConfig.PinNumber = GPIO_PIN_1;
	Gpio_init(&usart_gpios);

	GPIO_handler_t GpioBtn;

	GpioBtn.pGpiox = GPIOA;
	GpioBtn.Gpio_PinConfig.PinNumber = GPIO_PIN_0;
	GpioBtn.Gpio_PinConfig.PinMode = GPIO_MODE_INPUT;
	GpioBtn.Gpio_PinConfig.PinPuPdControl = GPIO_PUPD_NOPP;
	GpioBtn.Gpio_PinConfig.PinOspeed = GPIO_OSPEED_HIGH;

	Gpio_init(&GpioBtn);

	USART_Handler_t usart4_handle;
	usart4_handle.pUSARTx = UART4;
	usart4_handle.Pin_Config.USART_Baud = USART_STD_BAUD_115200;
	usart4_handle.Pin_Config.USART_HXFlowControl = USART_HW_FLOW_CTRL_NONE;
	usart4_handle.Pin_Config.USART_MODE = USART_MODE_TX_ONLY;
	usart4_handle.Pin_Config.USART_NoOfStopBits = USART_STOPBITS_1;
	usart4_handle.Pin_Config.USART_WordLength = USART_WORDLEN_8BITS;
	usart4_handle.Pin_Config.USART_ParityControl = USART_PARITY_DISABLE;
	USART_reset(usart4_handle.pUSARTx);
	USART_Init(&usart4_handle);




	USART_PeripheralControl(UART4, ENABLE);

	while (1) {
		//wait till button is pressed
		while (!GPIO_readInputFromPin(GPIOA, GPIO_PIN_0));

		//to avoid button de-bouncing related issues 200ms of delay
		gpio_delay(GPIO_DELAY/2);
		#ifdef SEMIHOSTING
		printf("Sent Data\n");
		#endif

		USART_SendData(&usart4_handle, (uint8_t*) msg, strlen(msg));

	}

}

void testing_hcsr04_it(void){
	test_hcsr04 = true;
	#ifdef SEMIHOSTING
	printf("test_hcsr04 : 1\n");
	#endif
	//init timer for pulse trigger
	Timer_Handler_t thandle;
	thandle.pTimx = TIM4;
	thandle.Timer_Config.PRESCALER = 15; //for 1us considering signal at 16mhz
	thandle.Timer_Config.AUTO_RELOAD_VALUE = 0;
	Timer_reset(thandle.pTimx,NULL);
	Timer_init(&thandle,NULL);

	//init timer for echo receiving
	Timer_IC_Handler_t ICHandle;
	ICHandle.pTimx = TIM2;
	ICHandle.Timer_Config.PRESCALER = 15;
	ICHandle.Timer_Config.AUTO_RELOAD_VALUE = 1000000 - 1;
	ICHandle.Timer_IC_Config.Capture_Direction = CAPTURE_DIR_DIRECT_CAPTURE;
	ICHandle.Timer_IC_Config.Capture_Edge = CAPTURE_EDGE_BOTH_EDGE;
	ICHandle.Timer_IC_Config.Capture_Filter = CAPTURE_FILTER_NO_FILTER;
	ICHandle.Timer_IC_Config.Capture_Prescaler = CAPTURE_PSC_NONE;
	Timer_reset(ICHandle.pTimx,NULL);
	Timer_IC_init(&ICHandle,TIM_CHANNEL_1);
	Timer_IC_enable(ICHandle.pTimx,TIM_CHANNEL_1);
	Timer_priorityConfig(IRQ_NUM_TIM2,15);
	Timer_interruptConfig(IRQ_NUM_TIM2,TIMER_ENABLE);

	Timer_IC_IT_enable(ICHandle.pTimx,TIM_CHANNEL_1);
	Timer_enable(ICHandle.pTimx,NULL);
	

	//init gpio for alt func mode
	GPIO_handler_t ghandle;
	ghandle.pGpiox = GPIOA;
	ghandle.Gpio_PinConfig.PinMode = GPIO_MODE_ALTFUNC;
	ghandle.Gpio_PinConfig.PinNumber = GPIO_PIN_0;
	ghandle.Gpio_PinConfig.PinAltFuncMode = GPIO_AF1;
	ghandle.Gpio_PinConfig.PinPuPdControl = GPIO_PUPD_NOPP;
	Gpio_reset(ghandle.pGpiox);
	Gpio_init(&ghandle);

	//init gpio for trig pin PA1
	ghandle.Gpio_PinConfig.PinNumber = GPIO_PIN_1;
	ghandle.Gpio_PinConfig.PinMode = GPIO_MODE_OUTPUT;
	ghandle.Gpio_PinConfig.PinOspeed = GPIO_OSPEED_LOW;
	ghandle.Gpio_PinConfig.PinOtype = GPIO_OTYPE_PUSHPULL;
	ghandle.Gpio_PinConfig.PinPuPdControl = GPIO_PUPD_NOPP;
	Gpio_init(&ghandle);

	
	//now measurement will happen
	while(1){
			GPIO_writeOutputInPin(GPIOA,GPIO_PIN_1,0);
	  		tim_delay(TIM4, 3);

	  		//*** START Ultrasonic measure routine ***//
	  		
			  GPIO_writeOutputInPin(GPIOA,GPIO_PIN_1,1);
	  		  tim_delay(TIM4,10);
			  GPIO_writeOutputInPin(GPIOA,GPIO_PIN_1,0);

	  		//2. ECHO signal pulse width
	  		uint32_t startTick = 0;
			uint32_t tick=0;
	  		do
	  		{
	  			if(icFlag) break;
				else tick++;
	  		}while((tick - startTick) < 5000000/2);  //500ms
	  		icFlag = 0;

			  Timer_IC_IT_disable(ICHandle.pTimx,TIM_CHANNEL_1);

	  		//Calculate distance in cm
	  		if(edge2Time > edge1Time)
	  		{	  
	  			distance = ((edge2Time - edge1Time) + 0.0f)*speedOfSound;
	  		}
	  		else if(edge2Time < edge1Time)
	  		{
	  			distance = (ICHandle.Timer_Config.AUTO_RELOAD_VALUE - edge2Time + edge1Time + 0.0f)*speedOfSound;
	  		}else{
				  distance = 0.0f;
			  }

			#ifdef SEMIHOSTING
	  			printf("D = %f\n",distance);
			#endif

	  		tim_delay(TIM4,50000);
			Timer_IC_IT_enable(ICHandle.pTimx,TIM_CHANNEL_1);
	}

}

void testing_hcsr04(void){

	Timer_Handler_t thandle;
	thandle.pTimx = TIM4;
	thandle.Timer_Config.PRESCALER = 15;
	thandle.Timer_Config.AUTO_RELOAD_VALUE = 0;
	Timer_init(&thandle,NULL);

	GPIO_handler_t ghandle;
	ghandle.pGpiox = GPIOE;
	ghandle.Gpio_PinConfig.PinMode = GPIO_MODE_INPUT;
	ghandle.Gpio_PinConfig.PinNumber = GPIO_PIN_5;
	ghandle.Gpio_PinConfig.PinPuPdControl = GPIO_PUPD_NOPP;
	Gpio_reset(ghandle.pGpiox);
	Gpio_init(&ghandle);

	ghandle.Gpio_PinConfig.PinNumber = GPIO_PIN_4;
	ghandle.Gpio_PinConfig.PinMode = GPIO_MODE_OUTPUT;
	ghandle.Gpio_PinConfig.PinOspeed = GPIO_OSPEED_LOW;
	ghandle.Gpio_PinConfig.PinOtype = GPIO_OTYPE_PUSHPULL;
	ghandle.Gpio_PinConfig.PinPuPdControl = GPIO_PUPD_NOPP;
	Gpio_init(&ghandle);

	uint32_t numTicks;
	float distance;
	while(1){

		/* USER CODE END WHILE */
	//   HAL_GPIO_WritePin(TRIG_GPIO_Port, TRIG_Pin, GPIO_PIN_RESET);
	  GPIO_writeOutputInPin(GPIOE,GPIO_PIN_4,0);
	//   usDelay(3);
	  tim_delay(TIM4,3);

	  //*** START Ultrasonic measure routine ***//
	  		//1. Output 10 usec TRIG
	  		// HAL_GPIO_WritePin(TRIG_GPIO_Port, TRIG_Pin, GPIO_PIN_SET);
			GPIO_writeOutputInPin(GPIOE,GPIO_PIN_4,1);
	  		// usDelay(10);
			tim_delay(TIM4,10);
	  		// HAL_GPIO_WritePin(TRIG_GPIO_Port, TRIG_Pin, GPIO_PIN_RESET);
			GPIO_writeOutputInPin(GPIOE,GPIO_PIN_4,0);

	  		//2. Wait for ECHO pin rising edge
	  		// while(HAL_GPIO_ReadPin(ECHO_GPIO_Port, ECHO_Pin) == GPIO_PIN_RESET);
			while(GPIO_readInputFromPin(GPIOE,GPIO_PIN_5) == 0);

	  		//3. Start measuring ECHO pulse width in usec

	  		numTicks = 0;
	  		// while(HAL_GPIO_ReadPin(ECHO_GPIO_Port, ECHO_Pin) == GPIO_PIN_SET)
			while(GPIO_readInputFromPin(GPIOE,GPIO_PIN_5) == 1)  
	  		{
	  			numTicks++;
	  			tim_delay(TIM4,2); //2.8usec
	  		};

	  		//4. Estimate distance in cm
	  		distance = (numTicks + 0.0f)*2.8*speedOfSound;
			#ifdef SEMIHOSTING
		  		printf("%f \n",distance);
			#endif
	  		tim_delay(TIM4,50000);

	}



}

void testing_output_capture_with_interrupt(void){
	output_capture_with_interrupt = true;
	TOCHandler.pTimx = TIM2;
	TOCHandler.Timer_Config.PRESCALER = 15999;
	TOCHandler.Timer_Config.AUTO_RELOAD_VALUE = 2000;
	TOCHandler.Timer_OC_Config.OCMode = OCMode_PWM_MODE_1;
	TOCHandler.Timer_OC_Config.OCPolarity = OCPolarity_HIGH;
	TOCHandler.Timer_OC_Config.OCPreloadEnable = OCPreloadEnable_DISABLE;
	TOCHandler.Timer_OC_Config.OC_Period = (TOCHandler.Timer_Config.AUTO_RELOAD_VALUE/2); //50% duty cycle

	Timer_OC_init(&TOCHandler,TIM_CHANNEL_1);
	Timer_OC_enable(TOCHandler.pTimx,TIM_CHANNEL_1);
	Timer_priorityConfig(IRQ_NUM_TIM2,15);
	Timer_interruptConfig(IRQ_NUM_TIM2,TIMER_ENABLE);
	Timer_OC_IT_enable(TOCHandler.pTimx,TIM_CHANNEL_1);

	//output pin config for TIM2 channel 1
	AFhandler.pGpiox = GPIOA;
	AFhandler.Gpio_PinConfig.PinNumber = GPIO_PIN_5;
	AFhandler.Gpio_PinConfig.PinMode = GPIO_MODE_ALTFUNC;
	AFhandler.Gpio_PinConfig.PinAltFuncMode = GPIO_AF1;

	Gpio_init(&AFhandler);

	//enable the Timer
	Timer_enable(TOCHandler.pTimx,NULL);

	while(1){
		//just for looping forever
	}
}

void testing_general_timer(void){
	Timer_Handler_t THandler;
	THandler.pTimx = TIM2;
	THandler.Timer_Config.PRESCALER = 10;
	THandler.Timer_Config.AUTO_RELOAD_VALUE = 2000000;
	Timer_reset(THandler.pTimx,NULL);
	Timer_init(&THandler,NULL);
	Timer_enable(THandler.pTimx,NULL);

	while(1){
		#ifdef SEMIHOSTING
			printf("CNT =%ld\n", THandler.pTimx->TIMx_CNT); //test it with arm semihosting "make run_itm"
		#endif
	}
}


void testing_basic_timer_with_interrupt(void){	
	basic_timer_interrupt = true;

	TSHandler.pTimx = TIM6;
	TSHandler.Timer_Config_t.PRESCALER = 10;
	TSHandler.Timer_Config_t.AUTO_RELOAD_VALUE = 10000;
	Timer_init(NULL,&TSHandler);

	// enable TIM6 interrupt
	Timer_interruptConfig(IRQ_NUM_TIM6_DAC,TIMER_ENABLE);
	Timer_priorityConfig(IRQ_NUM_TIM6_DAC,15);
	Timer_IT_state(NULL,TSHandler.pTimx,TIMER_ENABLE);

	Timer_enable(NULL,TSHandler.pTimx);

	gpio_init_test(GPIO_PIN_14);

	while(1){
		//forever loop to do nothing
	}

}

void testing_input_capture_with_interrupt(void){
	input_capture_with_interrupt = true;
	TICHandler.pTimx = TIM2;
	TICHandler.Timer_Config.PRESCALER = 10;
	TICHandler.Timer_Config.AUTO_RELOAD_VALUE = 0xFFFFFFFF;
	TICHandler.Timer_IC_Config.Capture_Direction = CAPTURE_DIR_DIRECT_CAPTURE;
	TICHandler.Timer_IC_Config.Capture_Edge = CAPTURE_EDGE_BOTH_EDGE;
	TICHandler.Timer_IC_Config.Capture_Filter = CAPTURE_FILTER_NO_FILTER;
	TICHandler.Timer_IC_Config.Capture_Prescaler = CAPTURE_PSC_NONE;
	Timer_reset(TICHandler.pTimx,NULL);
	Timer_IC_init(&TICHandler,TIM_CHANNEL_2);
	Timer_IC_enable(TICHandler.pTimx,TIM_CHANNEL_2);
	Timer_priorityConfig(IRQ_NUM_TIM2,15);
	Timer_interruptConfig(IRQ_NUM_TIM2,TIMER_ENABLE);

	Timer_IC_IT_enable(TICHandler.pTimx,TIM_CHANNEL_2);
	Timer_enable(TICHandler.pTimx,NULL);

	gpio_init_test(GPIO_PIN_14);
	AFhandler.pGpiox = GPIOA;
	AFhandler.Gpio_PinConfig.PinNumber = GPIO_PIN_1;
	AFhandler.Gpio_PinConfig.PinMode = GPIO_MODE_ALTFUNC;
	AFhandler.Gpio_PinConfig.PinAltFuncMode = GPIO_AF1;

	Gpio_init(&AFhandler);

	while(1){
		gpio_pin_toggle(GPIOD, GPIO_PIN_14);
	}
}


void TIM6_DAC_Handler(void){
	if(basic_timer_interrupt){
		gpio_toggle(GPIOD, GPIO_PIN_14);		//toggle the pin
		TSHandler.pTimx->TIMx_SR &= ~(1 << 0);	//clear the UIF Flag
	}else{
		//something is wrong
	}
}

void TIM2_Handler(void)
{
	if(input_capture_with_interrupt){
		//check for overcapture flag is set
		if(TIM2->TIMx_SR & (1 << 10)){
			#ifdef SEMIHOSTING
			printf("ONP = %ld\n",TIM2->TIMx_CCR2);
			#endif
		//clear the CC1OF bit
		TIM2->TIMx_SR &= ~(1 << 10);	
		}

		if(TIM2->TIMx_SR & (1 << 2)){
			#ifdef SEMIHOSTING
			printf("CNP = %ld\n",TIM2->TIMx_CCR2);
			#endif
			//clear the CC1IF flag		//no need as CC1IF flag is zero when we read the CCR1 register still if not used semihosting then their is a need
			TIM2->TIMx_SR &= ~(1 << 2);
		}
	}else if(output_capture_with_interrupt){

		//check for overcapture flag is set
		if(TIM2->TIMx_SR & (1 << 9)){
			#ifdef SEMIHOSTING
			printf("ONP = %ld\n",TIM2->TIMx_CCR1);
			#endif
		//clear the CC1OF bit
		TIM2->TIMx_SR &= ~(1 << 9);	
		}

		if(TIM2->TIMx_SR & (1 << 1)){
			#ifdef SEMIHOSTING
			printf("CNP = %ld\n",TIM2->TIMx_CCR1);
			#endif
			//clear the CC1IF flag		//no need as CC1IF flag is zero when we read the CCR1 register still if not used semihosting then their is a need
			TIM2->TIMx_SR &= ~(1 << 1);
		}

	}else if(test_hcsr04){
		if(captureIdx == 0) //Fisrt edge
		{
			edge1Time = TIM2->TIMx_CCR1; //

			captureIdx = 1;
		}
		else if(captureIdx == 1) //Second edge
		{
			edge2Time = TIM2->TIMx_CCR1;
			captureIdx = 0;
			icFlag = 1;
		}
	}	

}

void USART_ApplicationCallBack(USART_Handler_t *pUSART_Handler, uint8_t EVENT)
{
   if(EVENT == USART_EVENT_RX_CMPLT)
   {
			rxCmplt = SET;
			#ifdef SEMIHOSTING
				printf("USART_EVENT_RX_CMPLT");
			#endif

   }else if (EVENT == USART_EVENT_TX_CMPLT)
   {
	   #ifdef SEMIHOSTING
			printf("USART_EVENT_TX_CMPLT");
	   #endif
   }
}










void enable_processor_faults(void)
{
	uint32_t *pSHCSR = (uint32_t*)0xE000ED24;

	*pSHCSR |= ( 1 << 16); //mem manage
	*pSHCSR |= ( 1 << 17); //bus fault
	*pSHCSR |= ( 1 << 18); //usage fault
}

void HardFault_Handler(void)
{

#ifdef SEMIHOSTING
	printf("Exception : Hardfault\n");
#endif

	while(1);
}


void MemManage_Handler(void)
{
	#ifdef SEMIHOSTING
	printf("Exception : MemManage\n");
	#endif
	while(1);
}

void BusFault_Handler(void)
{
	#ifdef SEMIHOSTING
	printf("Exception : BusFault\n");
	#endif
	while(1);
}

