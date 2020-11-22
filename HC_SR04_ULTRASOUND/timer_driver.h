
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

//Input Capture Timer Handling Function
void Timer_IC_init(Timer_IC_Handler_t* handler);
void Timer_IC_enable(TIMx_RegDef_t* pTimx);
void Timer_IC_IT_enable(TIMx_RegDef_t* pTimx);

#endif