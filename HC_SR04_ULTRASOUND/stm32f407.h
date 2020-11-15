

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
#define GPIOD_BASE_ADDRESS		(AHB1_BASE_ADDRESS + 0x0C00)

#define RCC_BASE_ADDRESS		(AHB1_BASE_ADDRESS + 0x3800)	



/*
GPIO Peripheral Register Definition structure
*/
typedef struct{
    _vo uint32_t MODER;
    _vo uint32_t OTYPE;
    _vo uint32_t OSPEEDR;
    _vo uint32_t PUPDR;
    _vo uint32_t IDR;
    _vo uint32_t ODR;
    _vo uint32_t BSRR;
    _vo uint32_t LCKR;
    _vo uint32_t AFRL;
    _vo uint32_t AFRH;   
}GPIO_RegDef_t;

typedef struct{
   _vo uint32_t RCC_CR; 
   _vo uint32_t RCC_PLLCFGR; 
   _vo uint32_t RCC_CFGR; 
   _vo uint32_t RCC_CIR; 
   _vo uint32_t RCC_AHB1RSTR; 
   _vo uint32_t RCC_AHB2RSTR; 
   _vo uint32_t RCC_AHB3RSTR; 
       uint32_t RESERVED_D0; 
   _vo uint32_t RCC_APB1RSTR; 
   _vo uint32_t RCC_APB2RSTR; 
       uint32_t RESERVED_D1[2]; 
   _vo uint32_t RCC_AHB1ENR; 
   _vo uint32_t RCC_AHB2ENR; 
   _vo uint32_t RCC_AHB3ENR; 
       uint32_t RESERVED_D2; 
   _vo uint32_t RCC_APB1ENR; 
   _vo uint32_t RCC_APB2ENR;
       uint32_t RESERVED_D3[2];  
   _vo uint32_t RCC_AHB1LPENR; 
   _vo uint32_t RCC_AHB2LPENR; 
   _vo uint32_t RCC_AHB3LPENR; 
       uint32_t RESERVED_D4;
   _vo uint32_t RCC_APB1LPENR; 
   _vo uint32_t RCC_APB2LPENR;
       uint32_t RESERVED_D5[2]; 
   _vo uint32_t RCC_BDCR; 
   _vo uint32_t RCC_CSR; 
       uint32_t RESERVED_D6[2]; 
   _vo uint32_t RCC_SSCGR; 
   _vo uint32_t RCC_PLLI2SCFGR; 
}RCC_RegDef_t;

#define GPIOA                   ((GPIO_RegDef_t*)GPIOA_BASE_ADDRESS)
#define GPIOD                   ((GPIO_RegDef_t*)GPIOD_BASE_ADDRESS)
#define RCC                     ((RCC_RegDef_t*)RCC_BASE_ADDRESS)

#define GPIOA_PCLK_EN()         (RCC->RCC_AHB1ENR |= (1 << 0))
#define GPIOA_PCLK_DI()         (RCC->RCC_AHB1ENR &= ~(1 << 0))

#define GPIOA_REG_RESET()       do{ (RCC->RCC_AHB1RSTR |= (1 << 0)) ; (RCC->RCC_AHB1RSTR &= ~(1 << 0)); }while(0)

#define GPIOD_PCLK_EN()         (RCC->RCC_AHB1ENR |= (1 << 3))
#define GPIOD_PCLK_DI()         (RCC->RCC_AHB1ENR &= ~(1 << 3))

#define GPIOD_REG_RESET()       do{ (RCC->RCC_AHB1RSTR |= (1 << 3)) ; (RCC->RCC_AHB1RSTR &= ~(1 << 3)); }while(0)


#endif