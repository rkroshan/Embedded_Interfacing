
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

//User Simple Timer structure
typedef struct{
    TIMsx_RegDef_t* pTimx;
    Timer_Config_t Timer_Config_t;
}Timer_s_Handler_t;


typedef struct{
    uint8_t Capture_Direction; /*possible values @Capture_Direction*/
    uint8_t Capture_Edge; /*possible values @Capture_Edge*/
    uint8_t Capture_Prescaler; /*possible values @Capture_Prescaler*/
    uint8_t Capture_Filter;  /*possible values @Capture_Filter*/
}Timer_IC_Config_t;

typedef struct{
    Timer_IC_Config_t Timer_IC_Config;
    Timer_Config_t Timer_Config;
    TIMx_RegDef_t* pTimx;
}Timer_IC_Handler_t;

typedef struct{
    uint32_t OC_Period;      //user defined CCR value
    uint8_t OCMode; /*possible values @OCMode*/
    uint8_t OCPreloadEnable; /*possible values @OCPreloadEnable*/
    uint8_t OCPolarity;  /*possible values @OCPolarity*/   
}Timer_OC_Config_t;

typedef struct{
    Timer_OC_Config_t Timer_OC_Config;
    Timer_Config_t Timer_Config;
    TIMx_RegDef_t* pTimx;
}Timer_OC_Handler_t;

/*possible values @CHANNEL NUMBER*/
#define TIM_CHANNEL_1   1
#define TIM_CHANNEL_2   2
#define TIM_CHANNEL_3   3
#define TIM_CHANNEL_4   4

/*possible values @OCPolarity*/
#define OCPolarity_HIGH     0
#define OCPolarity_LOW      1

/*possible values @OCPreloadEnable*/
#define OCPreloadEnable_ENABLE      1
#define OCPreloadEnable_DISABLE     0

/*possible values @OCMode*/
#define OCMode_FROZEN           0
#define OCMode_HIGH_ON_MATCH    1
#define OCMode_LOW_ON_MATCH     2
#define OCMode_TOGGLE_ON_MATCH  3
#define OCMode_FORCED_LOW       4
#define OCMode_FORCED_HIGH      5
#define OCMode_PWM_MODE_1       6   //active if CNT < CCR1 else low
#define OCMode_PWM_MODE_2       7   //active if CNT > CCR1 else low

/*possible values @Capture_Prescaler*/
#define CAPTURE_PSC_NONE                0
#define CAPTURE_PSC_EVERY_TWO_EVENTS    1
#define CAPTURE_PSC_EVERY_FOUR_EVENTS   2
#define CAPTURE_PSC_EVERY_EIGHT_EVENTS  3

/*possible values @Capture_Filter*/
#define CAPTURE_FILTER_NO_FILTER    0
//for other possible values refer to documentation

/*possible values @Capture_Direction*/
#define CAPTURE_DIR_DIRECT_CAPTURE     1
#define CAPTURE_DIR_INDIRECT_CAPTURE   2
#define CAPTURE_DIR_TRC_CAPTURE        3

/*possible values @Capture_Edge*/
#define CAPTURE_EDGE_RISING_EDGE        0
#define CAPTURE_EDGE_FALLING_EDGE       1
#define CAPTURE_EDGE_BOTH_EDGE          3   

//Timer Handling function
void Timer_init(Timer_Handler_t* handler, Timer_s_Handler_t* shandler);
void Timer_reset(TIMx_RegDef_t* pTimx, TIMsx_RegDef_t* pTimsx);

void Timer_enable(TIMx_RegDef_t* pTimx, TIMsx_RegDef_t* pTimsx);
void Timer_disable(TIMx_RegDef_t* pTimx, TIMsx_RegDef_t* pTimsx);

void Timer_IT_state(TIMx_RegDef_t* pTimx, TIMsx_RegDef_t* pTimsx, uint8_t State);

void wait_till_Timer_event(TIMx_RegDef_t* pTimx, TIMsx_RegDef_t* pTimsx);

void Timer_PeriCLK(TIMx_RegDef_t* pTimx, TIMsx_RegDef_t* pTimsx, uint8_t state);

void Timer_interruptConfig(uint8_t IRQ_NUMBER, uint8_t State);
void Timer_priorityConfig(uint8_t IRQ_NUMBER, uint8_t priority);

void Timer_interrupt_callback_func(Timer_Handler_t* handler, Timer_s_Handler_t* shandler);

void Timer_clear_UIF_flag(TIMx_RegDef_t* pTimx, TIMsx_RegDef_t* pTimxs);

//Input Capture Timer Handling Function
void Timer_IC_init(Timer_IC_Handler_t* handler, uint8_t Channel);
void Timer_IC_enable(TIMx_RegDef_t* pTimx, uint8_t Channel);
void Timer_IC_disable(TIMx_RegDef_t* pTimx, uint8_t Channel);
void Timer_IC_IT_enable(TIMx_RegDef_t* pTimx, uint8_t Channel);
void Timer_IC_IT_disable(TIMx_RegDef_t* pTimx, uint8_t Channel);

//Output Compare Timer Handling Function
void Timer_OC_init(Timer_OC_Handler_t* handler, uint8_t Channel);
void Timer_OC_enable(TIMx_RegDef_t* pTimx, uint8_t Channel);
void Timer_OC_disable(TIMx_RegDef_t* pTimx, uint8_t Channel);
void Timer_OC_IT_enable(TIMx_RegDef_t* pTimx, uint8_t Channel);
void Timer_OC_IT_disable(TIMx_RegDef_t* pTimx, uint8_t Channel);

#endif