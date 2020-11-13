

#ifndef stm32f407_H__
#define stm32f407_H__

#include <stdint.h>
#include <stddef.h>

#define _vo                     volatile

/*Memory Addresses*/
#define FLASH_BASE_ADDR			0x08000000U
#define SRAM1_BASE_ADDR			0x20000000U
#define SRAM 					SRAM1_BASE_ADDR
#define SRAM2_BASE_ADDR			0x2001C000U

/*GPIO BASE ADDRESSES*/
#define AHB1_BASE_ADDRESS		0x40020000U
#define GPIOA_BASE_ADDRESS		(AHB1_BASE_ADDRESS + 0x0000)

#define RCC_BASE_ADDRESS		(AHB1_BASE_ADDRESS + 0x3800)	



/*
GPIO Peripheral Register Definition structure
*/
typedef struct{
    _vo uint32_t MODER;
    _vo uint32_t OTYPE;
    _vo uint32_t OSPPEDR;
    _vo uint32_t PUPDR;
    _vo uint32_t IDR;
    _vo uint32_t ODR;
    _vo uint32_t BSRR;
    _vo uint32_t LCKR;
    _vo uint32_t AFRL;
    _vo uint32_t AFRH;
    
}GPIO_RegDef_t;

#define GPIOA                   ((GPIO_RegDef_t*)GPIOA_BASE_ADDRESS)


#endif