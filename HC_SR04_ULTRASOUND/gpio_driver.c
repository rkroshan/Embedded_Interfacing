#include "gpio_driver.h"

/*
 * GPIO init function
 */

/********************************************************
 * @fn			- Gpio_init
 *
 * @brief		- For GPIO init function
 *
 * @param[in]	- pointer of GPIO hanlder
 * 
 * @return		- none
 *
 * @Note		- none
 */
void Gpio_init(GPIO_handler_t* handler)
{
    //enable the gpio peripheral
    GPIO_PeriCLKcontrol(handler->pGpiox,GPIO_ENABLE);

    // //reset the gpio port
    // Gpio_reset(handler->pGpiox);

    //set pin mode
    handler->pGpiox->MODER |= ((handler->Gpio_PinConfig.PinMode) << 2*(handler->Gpio_PinConfig.PinNumber));

    //set pin output type
    handler->pGpiox->OTYPE |= ((handler->Gpio_PinConfig.PinOtype) << handler->Gpio_PinConfig.PinOtype);

    //set pin output speed
    handler->pGpiox->OSPEEDR |= ((handler->Gpio_PinConfig.PinOspeed) << handler->Gpio_PinConfig.PinOspeed) ;

    //set pupd control
    handler->pGpiox->PUPDR |= ((handler->Gpio_PinConfig.PinPuPdControl) << handler->Gpio_PinConfig.PinPuPdControl) ;

    //set alternate function mode
    if(handler->Gpio_PinConfig.PinMode == GPIO_MODE_ALTFUNC){
        if(handler->Gpio_PinConfig.PinNumber < 8){
            handler->pGpiox->AFRL |= ((handler->Gpio_PinConfig.PinAltFuncMode) << 4*(handler->Gpio_PinConfig.PinNumber)) ;
        }else{
            handler->pGpiox->AFRH |= ((handler->Gpio_PinConfig.PinAltFuncMode) << 4*(handler->Gpio_PinConfig.PinNumber)) ;
        }
    }
}

/*
 * GPIO reset function
 */

/********************************************************
 * @fn			- Gpio_deinit
 *
 * @brief		- For GPIO reset function
 *
 * @param[in]	- base address of the GPIO peripheral
 * 
 * @return		- none
 *
 * @Note		- none
 */
void Gpio_reset(GPIO_RegDef_t* pGpiox)
{
    if(pGpiox == GPIOA){
        GPIOA_REG_RESET();
    }else if(pGpiox == GPIOD){
        GPIOD_REG_RESET();
    }else if(pGpiox == GPIOE){
        GPIOE_REG_RESET();
    }
}

/*
 * GPIO PeripheralClock Control
 */

/********************************************************
 * @fn			- GPIO_PeriCLKcontrol
 *
 * @brief		- For GPIO Clock Peripheral Control enable or disables clock for given GPIO port
 *
 * @param[in]	- base address of the GPIO peripheral
 * @param[in]	- ENABLE or DISABLE macros
 *
 * @return		- none
 *
 * @Note		- none
 */
void GPIO_PeriCLKcontrol(GPIO_RegDef_t* pGpiox, uint8_t state)
{
    if(state == GPIO_ENABLE){
        if(pGpiox == GPIOA){
            GPIOA_PCLK_EN();
        }
        else if(pGpiox == GPIOD){
            GPIOD_PCLK_EN();
        }else if(pGpiox == GPIOE){
            GPIOE_PCLK_EN();
        }
    }else{
        if(pGpiox == GPIOA){
            GPIOA_PCLK_DI();
        }
        else if(pGpiox == GPIOD){
            GPIOD_PCLK_DI();
        }else if(pGpiox == GPIOE){
            GPIOE_PCLK_DI();
        }
    }
}

/*
 * GPIO read input pin function
 */

/********************************************************
 * @fn			- GPIO_readInputFromPin
 *
 * @brief		- GPIO read input function
 *
 * @param[in]	- base address of the GPIO peripheral
 * @param[in]	- Pin Number
 * 
 * @return		- uint8
 *
 * @Note		- none
 */
uint8_t  GPIO_readInputFromPin(GPIO_RegDef_t* pGpiox, uint8_t PinNumber)
{
    uint8_t data = (uint8_t)((pGpiox->IDR >> PinNumber) & 0x1) ;
    return data;
}

/*
 * GPIO read input port function
 */

/********************************************************
 * @fn			- GPIO_readInputFromPort
 *
 * @brief		- GPIO read whole input port function
 *
 * @param[in]	- base address of the GPIO peripheral
 * 
 * @return		- uint16
 *
 * @Note		- none
 */
uint16_t GPIO_readInputFromPort(GPIO_RegDef_t* pGpiox)
{
    return (uint16_t)pGpiox->IDR;
}

/*
 * GPIO write pin function
 */

/********************************************************
 * @fn			- GPIO_writeOutputInPin
 *
 * @brief		- GPIO write pin function
 *
 * @param[in]	- base address of the GPIO peripheral
 * @param[in]	- Pin Number
 * @param[in]	- value
 * 
 * @return		- none
 *
 * @Note		- none
 */
void GPIO_writeOutputInPin(GPIO_RegDef_t* pGpiox, uint8_t PinNumber, uint8_t Value)
{
    if(Value == GPIO_BIT_SET){
        pGpiox->BSRR |= (1 << PinNumber);
    }else{
        pGpiox->BSRR |= (1 << (PinNumber + 16));
    }
}

/*
 * GPIO write to port function
 */

/********************************************************
 * @fn			- GPIO_writeOutputInPort
 *
 * @brief		- GPIO write to port function
 *
 * @param[in]	- base address of the GPIO peripheral
 * @param[in]	- value
 * 
 * @return		- none
 *
 * @Note		- none
 */
void GPIO_writeOutputInPort(GPIO_RegDef_t* pGpiox, uint16_t Value)
{
    pGpiox->ODR = Value;
}