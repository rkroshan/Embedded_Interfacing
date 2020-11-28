
#include "HC_SR04.h"
#include "timer_driver.h"
#include "gpio_driver.h"

uint8_t send_trigger = 0;
uint8_t is_capture_1 = 0;
uint8_t is_capture_2 = 0;
uint32_t capture_data[2] = {0};

//blocking functions
void hc_sr04_send_trigger(int time_in_us)
{
	//set the boolean
	send_trigger = 1;
	//init the timer 1
	Timer_OC_Handler_t handler;
	handler.pTimx = TIM2; 					//using 16Mhz internal clock
	handler.Timer_Config.PRESCALER = 15; 	//so timer resolution per count is 1us
	handler.Timer_Config.AUTO_RELOAD_VALUE = 10*time_in_us ;
	handler.Timer_OC_Config.OCMode = OCMode_PWM_MODE_1;
	handler.Timer_OC_Config.OCPolarity = OCPolarity_HIGH;
	handler.Timer_OC_Config.OCPreloadEnable = OCPreloadEnable_DISABLE;
	handler.Timer_OC_Config.OC_Period = time_in_us; //19/2 50% duty cycle we need 10us high pulse


	Timer_reset(handler.pTimx,NULL);
	Timer_OC_init(&handler,TIM_CHANNEL_1);
	Timer_OC_enable(handler.pTimx,TIM_CHANNEL_1);
	Timer_priorityConfig(IRQ_NUM_TIM2,15);
	Timer_interruptConfig(IRQ_NUM_TIM2,TIMER_ENABLE);
	Timer_OC_IT_enable(handler.pTimx,TIM_CHANNEL_1);


	//output pin config for TIM2 channel 1
	GPIO_handler_t AFhandler;
	AFhandler.pGpiox = GPIOA;
	AFhandler.Gpio_PinConfig.PinNumber = GPIO_PIN_5;
	AFhandler.Gpio_PinConfig.PinMode = GPIO_MODE_ALTFUNC;
	AFhandler.Gpio_PinConfig.PinAltFuncMode = GPIO_AF1;

	Gpio_init(&AFhandler);

	// //enable the Timer
	// Timer_enable(handler.pTimx,NULL); //this will start generating pulse of specified time_in_us at pin A5

	//setup the channel 2 to receive echo in input capture mode
	handler.pTimx->TIMx_CCMR1 |= (CAPTURE_DIR_DIRECT_CAPTURE << 8);

	//set the filter duration for channel 2
    handler.pTimx->TIMx_CCMR1 |= (CAPTURE_FILTER_NO_FILTER << 12);

    //set the edge of activation for channel 2
    handler.pTimx->TIMx_CCER |= ((1 << 5) | (1 << 7));

    //set the input capture prescaler for channel 2
    handler.pTimx->TIMx_CCMR1 |= (CAPTURE_PSC_NONE << 10);

	//enable the channel 2 input capture 
	handler.pTimx->TIMx_CCER |= (1 << 4);

	//enable the interrupt for tim2 channel 2
	Timer_priorityConfig(IRQ_NUM_TIM2,15);
	Timer_interruptConfig(IRQ_NUM_TIM2,TIMER_ENABLE);
	//enable channel 2 interrupt
	handler.pTimx->TIMx_DIER |= (1 << 2);

	//setup the gpio to handle the TIM2 channel 2 as input
	AFhandler.pGpiox = GPIOA;
	AFhandler.Gpio_PinConfig.PinNumber = GPIO_PIN_1;
	AFhandler.Gpio_PinConfig.PinMode = GPIO_MODE_ALTFUNC;
	AFhandler.Gpio_PinConfig.PinAltFuncMode = GPIO_AF1;

	Gpio_init(&AFhandler);
	
	//enable the Timer
	Timer_enable(handler.pTimx,NULL); 		//this will start generating pulse of specified time_in_us at pin A5

	//rest depends on TIM2 interrupt
}

float hc_sr04_get_time_lapse()
{
	while(!(is_capture_2)); //wait till we get the data

	//find the difference in time
	float time_diff;
	if(capture_data[1] > capture_data[0]){
		time_diff = ((float)(capture_data[1] - capture_data[0]))*(1/(1000000)); //because 1 us resolution
	}else{
		time_diff = ((float)(TIM2->TIMx_ARR - capture_data[0] + capture_data[1]))*(1/(1000000));
	}

	return time_diff;
}


float hc_sr04_get_distance_in_cm()
{
	return (hc_sr04_get_time_lapse()*SOUND_SPEED)/2;
}

float hc_sr04_get_distance_in_inch()
{
	return hc_sr04_get_distance_in_cm()*0.393701; //0.393701 is the conversion factor
}
