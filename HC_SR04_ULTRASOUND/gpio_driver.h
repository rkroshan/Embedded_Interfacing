

#ifndef DRIVER_H__
#define DRIVER_H__

#include "stm32f407.h"

typedef struct{
    uint8_t PinNumber;
    uint8_t PinMode;
    uint8_t PinOtype;
    uint8_t PinOspeed;
    uint8_t PinPuPdControl;
    uint8_t PinAltFuncMode;
}GPIO_PinConfig_t;

typedef struct{
    GPIO_RegDef_t *pGpiox;
    GPIO_PinConfig_t  Gpio_PinConfig;
}GPIO_handler_t;

//GPIO Handling functions
void Gpio_init(GPIO_handler_t* handler);
void Gpio_deinit(GPIO_RegDef_t* pGpiox);

void GPIO_PeriCLKcontrol(GPIO_RegDef_t* pGpiox, uint8_t state);

uint8_t  GPIO_readInputFromPin(GPIO_RegDef_t* pGpiox, uint8_t PinNumber);
uint16_t GPIO_readInputFromPort(GPIO_RegDef_t* pGpiox);
void GPIO_writeOutputInPin(GPIO_RegDef_t* pGpiox, uint8_t PinNumber, uint8_t Value);
void GPIO_writeOutputInPort(GPIO_RegDef_t* pGpiox, uint16_t Value);

#endif