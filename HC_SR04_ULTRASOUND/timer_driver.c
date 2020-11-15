

#include "timer_driver.h"

//Timer Handling function
void Timer_init(Timer_Handler_t* handler)
{
    //Enable the Timer clk
    Timer_PeriCLK(handler->pTimx,TIMER_ENABLE);

    //reset the Timer register
    Timer_reset(handler->pTimx);

    //clear the update event flag
    handler->pTimx->TIMx_SR &= ~(1 << 0);

    //set the prescaler to divide the main clk formula clk/(psc +1)
    handler->pTimx->TIMx_PSC = handler->Timer_Config.PRESCALER;

    //set the auto reload register to setup no of counts in counter
    handler->pTimx->TIMx_ARR = handler->Timer_Config.AUTO_RELOAD_VALUE;
}

void Timer_enable(TIMx_RegDef_t* pTimx)
{
    pTimx->TIMx_CCR1 |= (1 << 0);
}

void Timer_disable(TIMx_RegDef_t* pTimx)
{
    pTimx->TIMx_CCR1 &= ~(1 << 0);
}

void wait_till_Timer_event(TIMx_RegDef_t* pTimx)
{
    while(!(pTimx->TIMx_SR & 0)){} //wait till update flag is not set to 1
    //clear the UIF flag
    pTimx->TIMx_SR &= ~(1 << 0);
}

void Timer_reset(TIMx_RegDef_t* pTimx)
{
    if(pTimx == TIM2){
        TIM2_REG_RESET();
    }
}

void Timer_PeriCLK(TIMx_RegDef_t* pTimx, uint8_t state)
{
    if(state == TIMER_ENABLE){
        if(pTimx == TIM2){
            TIM2_PCLK_EN();
        }
    }
    else{
        if(pTimx == TIM2){
            TIM2_PCLK_DI();
        }
    }
}