

#ifndef GPIO_DRIVER_H__
#define GPIO_DRIVER_H__

#include "stm32f407.h"

#define GPIO_ENABLE     1
#define GPIO_DISABLE    0

#define GPIO_BIT_SET    1
#define GPIO_BIT_RESET  0


/*
    User GPIO Pin Definition structure  
*/
typedef struct{
    uint8_t PinNumber;              //possible values from @PIN_NUMBER
    uint8_t PinMode;                //possible values from @PIN_MODE
    uint8_t PinOtype;               //possible values from @PIN_OTYPE
    uint8_t PinOspeed;              //possible values from @PIN_OSPEED
    uint8_t PinPuPdControl;         //possible values from @PIN_PUPDCONTROL
    uint8_t PinAltFuncMode;         //possible values from @PIN_ALTFUNC
}GPIO_PinConfig_t;

/*
    User GPIO Handler Defintion structure
*/
typedef struct{
    GPIO_RegDef_t *pGpiox;
    GPIO_PinConfig_t  Gpio_PinConfig;
}GPIO_handler_t;

/*
    @PIN_NUMBER
*/
#define GPIO_PIN_0      0
#define GPIO_PIN_1      1
#define GPIO_PIN_2      2
#define GPIO_PIN_3      3
#define GPIO_PIN_4      4
#define GPIO_PIN_5      5
#define GPIO_PIN_6      6
#define GPIO_PIN_7      7
#define GPIO_PIN_8      8
#define GPIO_PIN_9      9
#define GPIO_PIN_10     10
#define GPIO_PIN_11     11
#define GPIO_PIN_12     12
#define GPIO_PIN_13     13
#define GPIO_PIN_14     14
#define GPIO_PIN_15     15

/*
    @PIN_MODE  
*/
#define GPIO_MODE_INPUT     0
#define GPIO_MODE_OUTPUT    1
#define GPIO_MODE_ALTFUNC   2
#define GPIO_MODE_ANALOG    3

/*
   @PIN_OTYPE 
*/
#define GPIO_OTYPE_PUSHPULL       0
#define GPIO_OTYPE_OPENDRAIN      1

/*
    @PIN_OSPEED
*/
#define GPIO_OSPEED_LOW         0
#define GPIO_OSPEED_MED_HIGH    1
#define GPIO_OSPEED_HIGH        2
#define GPIO_OSPEDD_VERY_HIGH   3

/*
    @PIN_PUPDCONTROL
*/
#define GPIO_PUPD_NOPP     0
#define GPIO_PUPD_PUP      1
#define GPIO_PUPD_PDN      2

/*
    @PIN_ALTFUNC
*/

#define GPIO_AF0        0
#define GPIO_AF1        1
#define GPIO_AF2        2
#define GPIO_AF3        3
#define GPIO_AF4        4
#define GPIO_AF5        5
#define GPIO_AF6        6
#define GPIO_AF7        7
#define GPIO_AF8        8
#define GPIO_AF9        9
#define GPIO_AF10       10
#define GPIO_AF11       11
#define GPIO_AF12       12
#define GPIO_AF13       13
#define GPIO_AF14       14
#define GPIO_AF15       15

//GPIO Handling functions
void Gpio_init(GPIO_handler_t* handler);
void Gpio_reset(GPIO_RegDef_t* pGpiox);

void GPIO_PeriCLKcontrol(GPIO_RegDef_t* pGpiox, uint8_t state);

uint8_t  GPIO_readInputFromPin(GPIO_RegDef_t* pGpiox, uint8_t PinNumber);
uint16_t GPIO_readInputFromPort(GPIO_RegDef_t* pGpiox);
void GPIO_writeOutputInPin(GPIO_RegDef_t* pGpiox, uint8_t PinNumber, uint8_t Value);
void GPIO_writeOutputInPort(GPIO_RegDef_t* pGpiox, uint16_t Value);

#endif