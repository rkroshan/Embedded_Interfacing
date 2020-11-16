
#ifndef TIMER_DRIVER_H__
#define TIMER_DRIVER_H__

#include "stm32f407.h"

#define TIMER_ENABLE    1
#define TIMER_DISABLE   0

//16bit timer config 
typedef struct{

    uint16_t PRESCALER;
    uint32_t AUTO_RELOAD_VALUE;

}Timer_Config_t;

//User Timer Handler structure
typedef struct{
    TIMx_RegDef_t* pTimx;
    Timer_Config_t Timer_Config;
}Timer_Handler_t;


//Timer Handling function
void Timer_init(Timer_Handler_t* handler);
void Timer_reset(TIMx_RegDef_t* pTimx);

void Timer_enable(TIMx_RegDef_t* pTimx);
void Timer_disable(TIMx_RegDef_t* pTimx);

void wait_till_Timer_event(TIMx_RegDef_t* pTimx);

void Timer_PeriCLK(TIMx_RegDef_t* pTimx, uint8_t state);

#endif