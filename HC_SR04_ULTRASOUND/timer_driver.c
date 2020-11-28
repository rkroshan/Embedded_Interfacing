

#include "timer_driver.h"


//internal functions
static void Timer_IC_set_Capture_Edge(Timer_IC_Handler_t* handler, uint8_t Channel);
static void t_init(Timer_Config_t config, TIMx_RegDef_t* pTimx);

//Timer Handling function
void Timer_init(Timer_Handler_t* handler, Timer_s_Handler_t* shandler)
{
    if(handler !=NULL){
    //Enable the Timer clk
    Timer_PeriCLK(handler->pTimx,NULL,TIMER_ENABLE);

    // Remember to reset the Timer if initializing first time

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

    // Remember to reset the Timer if initializing first time

    //clear the update event flag
    shandler->pTimx->TIMx_SR &= ~(1 << 0);

    //set the prescaler to divide the main clk formula clk/(psc +1)
    shandler->pTimx->TIMx_PSC = handler->Timer_Config.PRESCALER;

    //set the auto reload register to setup no of counts in counter
    shandler->pTimx->TIMx_ARR = handler->Timer_Config.AUTO_RELOAD_VALUE;
    }

}

static void t_init(Timer_Config_t config, TIMx_RegDef_t* pTimx){
    
    //Enable the Timer clk
    Timer_PeriCLK(pTimx,NULL,TIMER_ENABLE);

    // Remember to reset the Timer if initializing first time

    //clear the update event flag
    pTimx->TIMx_SR &= ~(1 << 0);

    //set the prescaler to divide the main clk formula clk/(psc +1)
    pTimx->TIMx_PSC = config.PRESCALER;

    //set the auto reload register to setup no of counts in counter
    pTimx->TIMx_ARR = config.AUTO_RELOAD_VALUE;

}

void Timer_OC_init(Timer_OC_Handler_t* handler, uint8_t Channel)
{
    t_init(handler->Timer_Config, handler->pTimx);

    //remember to reset the register if required

    switch(Channel){
        case TIM_CHANNEL_1:
            handler->pTimx->TIMx_CCMR1 &= ~(0x11 << 0);                                     //activate the output mode to channel
            handler->pTimx->TIMx_CCMR1 |= (handler->Timer_OC_Config.OCMode << 4);           //set output mode on output channel 
            handler->pTimx->TIMx_CCMR1 |= (handler->Timer_OC_Config.OCPreloadEnable << 3);  //set preloader register settings
            handler->pTimx->TIMx_CCER |= (handler->Timer_OC_Config.OCPolarity << 1);        //set the output polarity
            handler->pTimx->TIMx_CCR1 = handler->Timer_OC_Config.OC_Period;                 //put the CCR data in it's register    
            break;
        case TIM_CHANNEL_2:
            handler->pTimx->TIMx_CCMR1 &= ~(0x11 << 8);                                     //activate the output mode to channel
            handler->pTimx->TIMx_CCMR1 |= (handler->Timer_OC_Config.OCMode << 12);           //set output mode on output channel 
            handler->pTimx->TIMx_CCMR1 |= (handler->Timer_OC_Config.OCPreloadEnable << 11);  //set preloader register settings
            handler->pTimx->TIMx_CCER |= (handler->Timer_OC_Config.OCPolarity << 5);        //set the output polarity
            handler->pTimx->TIMx_CCR2 = handler->Timer_OC_Config.OC_Period;                 //put the CCR data in it's register
            break;
        case TIM_CHANNEL_3:
            handler->pTimx->TIMx_CCMR2 &= ~(0x11 << 0);                                     //activate the output mode to channel
            handler->pTimx->TIMx_CCMR2 |= (handler->Timer_OC_Config.OCMode << 4);           //set output mode on output channel 
            handler->pTimx->TIMx_CCMR2 |= (handler->Timer_OC_Config.OCPreloadEnable << 3);  //set preloader register settings
            handler->pTimx->TIMx_CCER |= (handler->Timer_OC_Config.OCPolarity << 9);        //set the output polarity
            handler->pTimx->TIMx_CCR3 = handler->Timer_OC_Config.OC_Period;                 //put the CCR data in it's register
            break;
        case TIM_CHANNEL_4:
            handler->pTimx->TIMx_CCMR2 &= ~(0x11 << 8);                                     //activate the output mode to channel
            handler->pTimx->TIMx_CCMR2 |= (handler->Timer_OC_Config.OCMode << 12);           //set output mode on output channel 
            handler->pTimx->TIMx_CCMR2 |= (handler->Timer_OC_Config.OCPreloadEnable << 11);  //set preloader register settings
            handler->pTimx->TIMx_CCER |= (handler->Timer_OC_Config.OCPolarity << 13);        //set the output polarity
            handler->pTimx->TIMx_CCR4 = handler->Timer_OC_Config.OC_Period;                 //put the CCR data in it's register
            break;
    }

    

}

void Timer_OC_enable(TIMx_RegDef_t* pTimx, uint8_t Channel)
{
    Timer_IC_enable(pTimx,Channel);    //enable output capture on that channel
}

void Timer_OC_disable(TIMx_RegDef_t* pTimx, uint8_t Channel)
{
    Timer_IC_disable(pTimx,Channel);    //disable output capture on that channel
}

void Timer_OC_IT_enable(TIMx_RegDef_t* pTimx, uint8_t Channel)
{
    Timer_IC_IT_enable(pTimx,Channel);  //enable IT output capture on that channel
}

void Timer_OC_IT_disable(TIMx_RegDef_t* pTimx, uint8_t Channel)
{
    Timer_IC_IT_disable(pTimx,Channel); //disable IT output capture on that channel
}

void Timer_IC_init(Timer_IC_Handler_t* handler, uint8_t Channel)
{
    t_init(handler->Timer_Config, handler->pTimx);

    //remember to reset the register if initializing

    switch(Channel){
        case TIM_CHANNEL_1:
            handler->pTimx->TIMx_CCMR1 |= (handler->Timer_IC_Config.Capture_Direction << 0);    //activate input capture mode to channel 1
            handler->pTimx->TIMx_CCMR1 |= (handler->Timer_IC_Config.Capture_Filter << 4);       //set the filter duration
            handler->pTimx->TIMx_CCMR1 |= (handler->Timer_IC_Config.Capture_Prescaler << 2);    //set the input capture prescaler
            Timer_IC_set_Capture_Edge(handler,Channel);
            break;
        case TIM_CHANNEL_2:
            handler->pTimx->TIMx_CCMR1 |= (handler->Timer_IC_Config.Capture_Direction << 8);    //activate input capture mode to channel 1
            handler->pTimx->TIMx_CCMR1 |= (handler->Timer_IC_Config.Capture_Filter << 12);       //set the filter duration
            handler->pTimx->TIMx_CCMR1 |= (handler->Timer_IC_Config.Capture_Prescaler << 10);    //set the input capture prescaler
            Timer_IC_set_Capture_Edge(handler,Channel);
            break;
        case TIM_CHANNEL_3:
            handler->pTimx->TIMx_CCMR2 |= (handler->Timer_IC_Config.Capture_Direction << 0);    //activate input capture mode to channel 1
            handler->pTimx->TIMx_CCMR2 |= (handler->Timer_IC_Config.Capture_Filter << 4);       //set the filter duration
            handler->pTimx->TIMx_CCMR2 |= (handler->Timer_IC_Config.Capture_Prescaler << 2);    //set the input capture prescaler
            Timer_IC_set_Capture_Edge(handler,Channel);
            break;
        case TIM_CHANNEL_4:
            handler->pTimx->TIMx_CCMR2 |= (handler->Timer_IC_Config.Capture_Direction << 8);    //activate input capture mode to channel 1
            handler->pTimx->TIMx_CCMR2 |= (handler->Timer_IC_Config.Capture_Filter << 12);       //set the filter duration
            handler->pTimx->TIMx_CCMR2 |= (handler->Timer_IC_Config.Capture_Prescaler << 10);    //set the input capture prescaler
            Timer_IC_set_Capture_Edge(handler,Channel);
            break;
        
    }

}

static void Timer_IC_set_Capture_Edge(Timer_IC_Handler_t* handler, uint8_t Channel){
    
    uint8_t Pol =0, NPol = 0;
    switch(handler->Timer_IC_Config.Capture_Edge){
        case CAPTURE_EDGE_RISING_EDGE:
            Pol = 0; NPol = 0;
            break;
        case CAPTURE_EDGE_FALLING_EDGE:
            Pol = 1; NPol = 0;
            break;
        case CAPTURE_EDGE_BOTH_EDGE:
            Pol = 1; NPol = 1;
            break;
    }

    switch(Channel){
        case TIM_CHANNEL_1:
            handler->pTimx->TIMx_CCER &= ~((1 << 1) | (1 << 3));
            handler->pTimx->TIMx_CCER |=  ((Pol << 1) | (NPol << 3));
            break;
        case TIM_CHANNEL_2:
            handler->pTimx->TIMx_CCER &= ~((1 << 5) | (1 << 7));
            handler->pTimx->TIMx_CCER |=  ((Pol << 5) | (NPol << 7));
            break;
        case TIM_CHANNEL_3:
            handler->pTimx->TIMx_CCER &= ~((1 << 9) | (1 << 11));
            handler->pTimx->TIMx_CCER |=  ((Pol << 9) | (NPol << 11));
            break;
        case TIM_CHANNEL_4:
            handler->pTimx->TIMx_CCER &= ~((1 << 13) | (1 << 15));
            handler->pTimx->TIMx_CCER |=  ((Pol << 13) | (NPol << 15));
            break;
                   
    }
}

void Timer_IC_IT_enable(TIMx_RegDef_t* pTimx, uint8_t Channel)
{
    pTimx->TIMx_DIER |= (1 << Channel); //enable interrupt for that channel
}

void Timer_IC_IT_disable(TIMx_RegDef_t* pTimx, uint8_t Channel)
{
    pTimx->TIMx_DIER &= ~(1 << Channel); //disable interrupt for that channels
}

void Timer_IC_enable(TIMx_RegDef_t* pTimx, uint8_t Channel)
{
    switch(Channel){
        case TIM_CHANNEL_1:
            pTimx->TIMx_CCER |= (1 << 0); //enable the output compare mode
            break;
        case TIM_CHANNEL_2:
            pTimx->TIMx_CCER |= (1 << 4); //enable the output compare mode
            break;
        case TIM_CHANNEL_3:
            pTimx->TIMx_CCER |= (1 << 8); //enable the output compare mode
            break;
        case TIM_CHANNEL_4:
            pTimx->TIMx_CCER |= (1 << 12); //enable the output compare mode
            break;
    }
}

void Timer_IC_disable(TIMx_RegDef_t* pTimx, uint8_t Channel)
{
    switch(Channel){
        case TIM_CHANNEL_1:
            pTimx->TIMx_CCER &= ~(1 << 0); //enable the output compare mode
            break;
        case TIM_CHANNEL_2:
            pTimx->TIMx_CCER &= ~(1 << 4); //enable the output compare mode
            break;
        case TIM_CHANNEL_3:
            pTimx->TIMx_CCER &= ~(1 << 8); //enable the output compare mode
            break;
        case TIM_CHANNEL_4:
            pTimx->TIMx_CCER &= ~(1 << 12); //enable the output compare mode
            break;
    }
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

void Timer_clear_UIF_flag(TIMx_RegDef_t* pTimx, TIMsx_RegDef_t* pTimxs)
{
    if(pTimx == NULL){
        pTimxs->TIMx_SR &= ~(1 << 0);   //clear the UIF FLAG
    }else{
        pTimx->TIMx_SR &= ~(1 << 0);    //clear the UIF FLAG    
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
        pTimx->TIMx_DIER &= ~(1 << 0);
        pTimx->TIMx_DIER |= (State << 0);
    }else{
        pTimsx->TIMx_DIER &= ~(1 << 0);
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
