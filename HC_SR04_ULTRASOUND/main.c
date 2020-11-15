
#include <stdio.h>
#include "main.h"
#include "HC_SR04.h"
#include "gpio_driver.h"


#if !defined(__SOFT_FP__) && defined(__ARM_FP)
  #warning "FPU is not initialized, but the project is compiling for an FPU. Please initialize the FPU before use."
#endif


#ifdef SEMIHOSTING
extern void initialise_monitor_handles(void);
#endif

void enable_processor_faults(void);

#define GPIO_DELAY 500000
GPIO_handler_t handler;
void gpio_testing(uint8_t PinNumber);
void gpio_delay(uint32_t time);
void gpio_pin_toggle(GPIO_RegDef_t* pGpiox, uint8_t PinNumber);

int main(void){

	enable_processor_faults();

#ifdef SEMIHOSTING
	initialise_monitor_handles();
#endif

while(1){

	gpio_testing(GPIO_PIN_14);
	gpio_pin_toggle(GPIOD, GPIO_PIN_14);

}
	
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
	printf("It's working 1\n");
#endif
	gpio_delay(GPIO_DELAY);
#ifdef SEMIHOSTING
	printf("It's working 2\n");
#endif
	GPIO_writeOutputInPin(pGpiox,PinNumber,GPIO_BIT_SET);
	gpio_delay(GPIO_DELAY);
}

void gpio_delay(uint32_t time){
	for(uint32_t i=0; i<time; i++){
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

