

#include "timer_driver.h"

//Timer Handling function
void Timer_init(Timer_Handler_t* handler, Timer_s_Handler_t* shandler)
{
    if(handler !=NULL){
    //Enable the Timer clk
    Timer_PeriCLK(handler->pTimx,NULL,TIMER_ENABLE);

    //reset the Timer register
    Timer_reset(handler->pTimx,NULL);

    //clear the update event flag
    handler->pTimx->TIMx_SR &= ~(1 << 0);

    //set the prescaler to divide the main clk formula clk/(psc +1)
    handler->pTimx->TIMx_PSC = handler->Timer_Config.PRESCALER;

    //set the auto reload register to setup no of counts in counter
    handler->pTimx->TIMx_ARR = handler->Timer_Config.AUTO_RELOAD_VALUE;
    }
    else if(shandler != NULL){
    //Enable the Timer clk
    Timer_PeriCLK(NULL,shandler->pTimx,TIMER_ENABLE);

    //reset the Timer register
    Timer_reset(NULL,shandler->pTimx);

    //clear the update event flag
    shandler->pTimx->TIMx_SR &= ~(1 << 0);

    //set the prescaler to divide the main clk formula clk/(psc +1)
    shandler->pTimx->TIMx_PSC = handler->Timer_Config.PRESCALER;

    //set the auto reload register to setup no of counts in counter
    shandler->pTimx->TIMx_ARR = handler->Timer_Config.AUTO_RELOAD_VALUE;
    }

}

void Timer_IC_init(Timer_IC_Handler_t* handler)
{
    //Enable the peripheral clock
    Timer_PeriCLK(handler->pTimx,NULL,TIMER_ENABLE);

    //Reset the register
    Timer_reset(handler->pTimx,NULL);

    //clear the capture flag
    handler->pTimx->TIMx_SR &= ~(1 << 0);

    //set the prescaler
    handler->pTimx->TIMx_PSC = handler->Timer_Config.PRESCALER;

    //set the auto reload register
    handler->pTimx->TIMx_ARR = handler->Timer_Config.AUTO_RELOAD_VALUE;

    //activate input capture mode to channel 1
    handler->pTimx->TIMx_CCMR1 |= (handler->Timer_IC_Config.Capture_Direction << 0);

    //set the filter duration
    handler->pTimx->TIMx_CCMR1 |= (handler->Timer_IC_Config.Capture_Filter << 4);

    //set the edge of activation
    if(handler->Timer_IC_Config.Capture_Edge == CAPTURE_EDGE_BOTH_EDGE){
        handler->pTimx->TIMx_CCER |= ((1 << 1) | (1 << 3));
    }
    //TODO: the rest active edge detection

    //set the input capture prescaler
    handler->pTimx->TIMx_CCMR1 |= (handler->Timer_IC_Config.Capture_Prescaler << 2);

}

void Timer_IC_IT_enable(TIMx_RegDef_t* pTimx)
{
    pTimx->TIMx_DIER |= (1 << 1); 
}

void Timer_IC_enable(TIMx_RegDef_t* pTimx)
{
    pTimx->TIMx_CCER |= (1 << 0);
}

void Timer_enable(TIMx_RegDef_t* pTimx, TIMsx_RegDef_t* pTimsx)
{
    if(pTimx != NULL){
        pTimx->TIMx_CR1 |= (1 << 0);
    }else{
        pTimsx->TIMx_CR1 |= (1 << 0);
    }
}

void Timer_disable(TIMx_RegDef_t* pTimx, TIMsx_RegDef_t* pTimsx)
{
    if(pTimx != NULL){
        pTimx->TIMx_CR1 &= ~(1 << 0);
    }else{
        pTimsx->TIMx_CR1 &= ~(1 << 0);
    }
}

void wait_till_Timer_event(TIMx_RegDef_t* pTimx, TIMsx_RegDef_t* pTimsx)
{
    if(pTimx != NULL){
        while(!(pTimx->TIMx_SR & (1 << 0))){} //wait till update flag is not set to 1
        //clear the UIF flag
        pTimx->TIMx_SR &= ~(1 << 0);
    }else{
        while(!(pTimsx->TIMx_SR & (1 << 0))){} //wait till update flag is not set to 1
        //clear the UIF flag
        pTimsx->TIMx_SR &= ~(1 << 0);
    }
    
}

void Timer_reset(TIMx_RegDef_t* pTimx, TIMsx_RegDef_t* pTimsx)
{
    if(pTimx != NULL){
        if(pTimx == TIM2){
        TIM2_REG_RESET();
        }
    }else{
        if(pTimsx == TIM6){
            TIM6_REG_RESET();
        }
    }
    
}

void Timer_PeriCLK(TIMx_RegDef_t* pTimx, TIMsx_RegDef_t* pTimsx, uint8_t state)
{
    if(state == TIMER_ENABLE){
        if(pTimx == TIM2){
            TIM2_PCLK_EN();
        }
        else if(pTimsx == TIM6){
            TIM6_PCLK_EN();
        }
    }
    else{
        if(pTimx == TIM2){
            TIM2_PCLK_DI();
        }
        else if(pTimsx == TIM6){
            TIM6_PCLK_DI();
        }
    }
}

void Timer_IT_state(TIMx_RegDef_t* pTimx, TIMsx_RegDef_t* pTimsx, uint8_t State)
{
    if(pTimx != NULL){
        pTimx->TIMx_DIER |= (State << 0);
    }else{
        pTimsx->TIMx_DIER |= (State << 0);
    }
}



void Timer_interruptConfig(uint8_t IRQ_NUMBER, uint8_t State)
{
    if(State == TIMER_ENABLE){
        if(IRQ_NUMBER < 32){
            *NVIC_ISER0 |= (1 << IRQ_NUMBER);
        }
        else if(IRQ_NUMBER < 64){
            *NVIC_ISER1 |= (1 << (IRQ_NUMBER % 32));
        }
        else if(IRQ_NUMBER < 96){
            *NVIC_ISER2 |= (1 << (IRQ_NUMBER % 64));
        }

    }
    else{
        if(IRQ_NUMBER < 32){
            *NVIC_ICER0 &= ~(1 << IRQ_NUMBER);
        }
        else if(IRQ_NUMBER < 64){
            *NVIC_ICER1 &= ~(1 << (IRQ_NUMBER % 32));
        }
        else if(IRQ_NUMBER < 96){
            *NVIC_ICER2 &= ~(1 << (IRQ_NUMBER % 64));
        }
    }
}
void Timer_priorityConfig(uint8_t IRQ_NUMBER, uint8_t priority)
{
    uint8_t ipr = IRQ_NUMBER/4;
    uint8_t iprx = IRQ_NUMBER%4;

    *(NVIC_IPR_BASE_ADDR + ipr) |= (priority << (8*iprx + 4) );
}

void Timer_interrupt_callback_func(Timer_Handler_t* handler, Timer_s_Handler_t* shandler)
{

}
