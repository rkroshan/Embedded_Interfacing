
#include <stdio.h>
#include "main.h"
#include "HC_SR04.h"
#include "gpio_driver.h"
#include "timer_driver.h"

#if !defined(__SOFT_FP__) && defined(__ARM_FP)
  #warning "FPU is not initialized, but the project is compiling for an FPU. Please initialize the FPU before use."
#endif


#ifdef SEMIHOSTING
extern void initialise_monitor_handles(void);
#endif

void enable_processor_faults(void);

#define GPIO_DELAY 500000
GPIO_handler_t handler, AFhandler;
void gpio_testing(uint8_t PinNumber);
void gpio_delay(uint32_t time);
void gpio_pin_toggle(GPIO_RegDef_t* pGpiox, uint8_t PinNumber);
void gpio_toggle(GPIO_RegDef_t* pGpiox, uint8_t PinNumber);

Timer_s_Handler_t TSHandler;

Timer_IC_Handler_t TICHandler;

int main(void){

	enable_processor_faults();

#ifdef SEMIHOSTING
	initialise_monitor_handles();
#endif

	// Timer_Handler_t THandler;
	// THandler.pTimx = TIM2;
	// THandler.Timer_Config.PRESCALER = 10;
	// THandler.Timer_Config.AUTO_RELOAD_VALUE = 2000000;
	// Timer_init(&THandler);
	// Timer_enable(THandler.pTimx);

	
	// TSHandler.pTimx = TIM6;
	// TSHandler.Timer_Config_t.PRESCALER = 10;
	// TSHandler.Timer_Config_t.AUTO_RELOAD_VALUE = 10000;
	// Timer_init(NULL,&TSHandler);

	//enable TIM6 interrupt
	// Timer_interruptConfig(IRQ_NUM_TIM6_DAC,TIMER_ENABLE);
	// Timer_priorityConfig(IRQ_NUM_TIM6_DAC,15);
	// Timer_IT_state(NULL,TSHandler.pTimx,TIMER_ENABLE);

	// Timer_enable(NULL,TSHandler.pTimx);

	TICHandler.pTimx = TIM2;
	TICHandler.Timer_Config.PRESCALER = 10;
	TICHandler.Timer_Config.AUTO_RELOAD_VALUE = 0xFFFFFFFF;
	TICHandler.Timer_IC_Config.Capture_Direction = CAPTURE_DIR_DIRECT_CAPTURE;
	TICHandler.Timer_IC_Config.Capture_Edge = CAPTURE_EDGE_BOTH_EDGE;
	TICHandler.Timer_IC_Config.Capture_Filter = CAPTURE_FILTER_NO_FILTER;
	TICHandler.Timer_IC_Config.Capture_Prescaler = CAPTURE_PSC_NONE;

	Timer_IC_init(&TICHandler);
	Timer_IC_enable(TICHandler.pTimx);
	Timer_priorityConfig(IRQ_NUM_TIM2,15);
	Timer_interruptConfig(IRQ_NUM_TIM2,TIMER_ENABLE);

	Timer_IC_IT_enable(TICHandler.pTimx);
	Timer_enable(TICHandler.pTimx,NULL);

	gpio_testing(GPIO_PIN_14);

	AFhandler.pGpiox = GPIOA;
	AFhandler.Gpio_PinConfig.PinNumber = GPIO_PIN_5;
	AFhandler.Gpio_PinConfig.PinMode = GPIO_MODE_ALTFUNC;
	AFhandler.Gpio_PinConfig.PinAltFuncMode = GPIO_AF1;

	Gpio_init(&AFhandler);


while(1){
#ifdef SEMIHOSTING
	// printf("CNT =%ld\n", TSHandler.pTimx->TIMx_CNT);
	// if(TSHandler.pTimx->TIMx_SR & (1 << 0)){
	// printf("SR =%ld\n", TSHandler.pTimx->TIMx_SR);
	// }
#endif
	
	// while(!(TSHandler.pTimx->TIMx_SR & (1 << 0)));
	// TSHandler.pTimx->TIMx_SR &= ~(1 << 0);
	// gpio_toggle(GPIOD, GPIO_PIN_14);
	gpio_pin_toggle(GPIOD, GPIO_PIN_14);	
}
	
}

void gpio_toggle(GPIO_RegDef_t* pGpiox, uint8_t PinNumber)
{
	pGpiox->ODR ^= (1 << PinNumber);
}

void gpio_testing(uint8_t PinNumber){
	handler.pGpiox = GPIOD;
	handler.Gpio_PinConfig.PinNumber = PinNumber;
	handler.Gpio_PinConfig.PinMode = GPIO_MODE_OUTPUT;
	handler.Gpio_PinConfig.PinOtype = GPIO_OTYPE_PUSHPULL;
	handler.Gpio_PinConfig.PinOspeed = GPIO_OSPEED_HIGH;
	handler.Gpio_PinConfig.PinPuPdControl = GPIO_PUPD_PUP;
	
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


// void TIM6_DAC_Handler(void){
// 	gpio_toggle(GPIOD, GPIO_PIN_14);
// 	TSHandler.pTimx->TIMx_SR &= ~(1 << 0);	
// }



void TIM2_Handler(void)
{
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
		//clear the CC1IF flag
		//TIM2->TIMx_SR &= ~(1 << 1);
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

