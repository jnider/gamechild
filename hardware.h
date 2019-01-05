#ifndef _CLOCKS__H
#define _CLOCKS__H

#include "stm32f10x_type.h"

#define USER_CODE_FLASH0X8002000    ((u32)0x08002000)
#define RAM_END                     ((u32)0x20005000)

/*
    These defineds are use to setup the hardware of the GPIO.
    See http://www.st.com/web/en/resource/technical/document/reference_manual/CD00171190.pdf
*/

// Use Boot1 PB2 as the button, as hardly anyone uses this pin as GPIO
// Need to set the button input mode to just CR_INPUT and not CR_INPUT_PU_PD because the external pullup on the jumplink is very weak
	#define BUTTON_INPUT_MODE 	CR_INPUT
    #define BUTTON_BANK GPIOB
    #define BUTTON_PIN 2
    #define BUTTON_PRESSED_STATE 1

/* Peripheral Base Addresses */
#define AFIO_BASE    (0x40010000)
#define GPIO_BASE    (0x40010800)
#define TIM1_BASE    (0x40012C00)
#define USART1_BASE  (0x40013800) /* USART low speed serial */
#define RCC_BASE     (0x40021000) /* Reset and clock control */
#define FLASH_BASE   (0x40022000) /* Flash memory interface */
#define SCS_BASE     (0xE000E000)

#define AFIO_MAPR    (AFIO_BASE + 0x04)


/* Reset and clock control: Section 7.3.11 on page 121 */
struct RCC_regs
{
   vu32 CR;          /* Clock control register: Section 8.3.1 */
#define RCC_CR_PLL3RDY        (1<<29) /* PLL3 clock ready flag */
#define RCC_CR_PLL3ON         (1<<28) /* PLL3 enable */
#define RCC_CR_PLL2RDY        (1<<27) /* PLL2 clock ready flag */
#define RCC_CR_PLL2ON         (1<<26) /* PLL2 enable */
#define RCC_CR_PLLRDY         (1<<25) /* PLL clock ready flag */
#define RCC_CR_PLLON          (1<<24) /* PLL enable */
#define RCC_CR_CSSON          (1<<19) /* Clock security system enable */
#define RCC_CR_HSEBYP         (1<<18) /* External high-speed clock bypass */
#define RCC_CR_HSERDY         (1<<17) /* External high-speed clock ready */
#define RCC_CR_HSEON          (1<<16) /* HSE clock enable */
#define RCC_CR_HSICAL         (0xFF<<8) /* Internal high-speed clock calibration */
#define RCC_CR_HSITRIM        (0x1F<<3) /* Internal high-speed clock trimming */
#define RCC_CR_HSIRDY         (1<<1)  /* Internal high-speed clock ready */
#define RCC_CR_HSION          (1<<0)  /* Internal high-speed clock enable */

   vu32 CFGR;        /* Clock configuration register: 8.3.2 */
#define RCC_CFGR_MCO          (0xF<<24) /* Microcontroller clock output */
#define RCC_CFGR_USBPRE       (1<<22) /* USB OTG FS prescaler */
#define RCC_CFGR_PLLMUL(_x)   ((_x-2)<<18) /* PLL multiplier */
#define RCC_CFGR_PLLXTPRE     (1<<17) /* LSB of division factor PREDIV1 */
#define RCC_CFGR_PLLSRC       (1<<16) /* PLL entry clock source */
#define RCC_CFGR_ADCPRE_MASK  (3<<14) /* ADC prescaler */
#define RCC_CFGR_PPRE2_MASK   (0x7<<11) /* APB2 high-speed prescaler */
#define RCC_CFGR_PPRE2_X2     (0x4<<11)
#define RCC_CFGR_PPRE2_X4     (0x5<<11)
#define RCC_CFGR_PPRE2_X8     (0x6<<11)
#define RCC_CFGR_PPRE2_X16    (0x7<<11)
#define RCC_CFGR_PPRE1_MASK   (0x7<<8) /* APB1 high-speed prescaler */
#define RCC_CFGR_PPRE1_X2     (0x4<<8)
#define RCC_CFGR_PPRE1_X4     (0x5<<8)
#define RCC_CFGR_PPRE1_X8     (0x6<<8)
#define RCC_CFGR_PPRE1_X16    (0x7<<8)
#define RCC_CFGR_HPRE_MASK    (0xF<<4) /* AHB prescaler */
#define RCC_CFGR_HPRE_X2      (0x8<<4)
#define RCC_CFGR_HPRE_X4      (0x9<<4)
#define RCC_CFGR_HPRE_X8      (0xA<<4)
#define RCC_CFGR_HPRE_X16     (0xB<<4)
#define RCC_CFGR_HPRE_X64     (0xC<<4)
#define RCC_CFGR_HPRE_X128    (0xD<<4)
#define RCC_CFGR_HPRE_X256    (0xE<<4)
#define RCC_CFGR_HPRE_X512    (0xF<<4)
#define RCC_CFGR_SWS_HSI      (0<<2) /* System clock switch status */
#define RCC_CFGR_SWS_HSE      (1<<2)
#define RCC_CFGR_SWS_PLL      (2<<2)
#define RCC_CFGR_SW_HSE       (1<<0) /* System clock switch - HSE */
#define RCC_CFGR_SW_PLL       (2<<0) /* System clock switch - PLL */

   vu32 CIR;         /* Clock interrupt register 8.3.3 */
   vu32 APB2RSTR;
#define RCC_APB2RSTR_USART1RST  (1<<14)
#define RCC_APB2RSTR_SPI1RST    (1<<12)

   vu32 APB1RSTR;
   vu32 AHBENR;
   vu32 APB2ENR;
#define RCC_APB2ENR_TIM11EN   (1<<21)  /* TIM11 timer enable */
#define RCC_APB2ENR_TIM10EN   (1<<20)  /* TIM10 timer enable */
#define RCC_APB2ENR_TIM9EN    (1<<19)  /* TIM9 timer enable */
#define RCC_APB2ENR_ADC3EN    (1<<15)
#define RCC_APB2ENR_USART1EN  (1<<14)
#define RCC_APB2ENR_TIM8EN    (1<<13)
#define RCC_APB2ENR_SPI1EN    (1<<12)
#define RCC_APB2ENR_TIM1EN    (1<<11)
#define RCC_APB2ENR_ADC2EN    (1<<10)
#define RCC_APB2ENR_ADC1EN    (1<<9)
#define RCC_APB2ENR_IOPGEN    (1<<8)
#define RCC_APB2ENR_IOPFEN    (1<<7)
#define RCC_APB2ENR_IOPEEN    (1<<6)
#define RCC_APB2ENR_IOPDEN    (1<<5)
#define RCC_APB2ENR_IOPCEN    (1<<4)
#define RCC_APB2ENR_IOPBEN    (1<<3)
#define RCC_APB2ENR_IOPAEN    (1<<2)
#define RCC_APB2ENR_AFIOEN    (1<<0)

   vu32 APB1ENR;
#define RCC_APB1ENR_USB_CLK   0x00800000 // The USB clock is the same for all boards
#define RCC_APB1ENR_PWR_CLK   0x10000000 // Clocks for the backup domain registers
#define RCC_APB1ENR_BKP_CLK   0x08000000

   vu32 BDCR;
   vu32 CSR;
};
#define pRCC ((struct RCC_regs*) RCC_BASE)


struct FLASH_regs
{
   vu32 ACR;
#define FLASH_ACR_PRFTBS      (1<<5)
#define FLASH_ACR_PRFTBE      (1<<4)
#define FLASH_ACR_HLFCYA      (1<<3)
#define FLASH_ACR_WAITSTATES(_x) (_x<<0) /* number of wait states 0-2 */
   vu32 KEYR;
   vu32 OPTKEYR;
   vu32 SR;
   vu32 CR;
#define FLASH_CR_PER   0x02
#define FLASH_CR_PG    0x01
#define FLASH_CR_START 0x40
   vu32 AR;
   vu32 blank18;
   vu32 OBR;
   vu32 WRPR;
};
#define pFLASH ((struct FLASH_regs*)FLASH_BASE)

#define FLASH_KEY1     0x45670123
#define FLASH_KEY2     0xCDEF89AB
#define FLASH_RDPRT    0x00A5
#define FLASH_SR_BSY   0x01

struct GPIO_regs
{
   vu32 CRL;            /* Port configuration register (low) */
   vu32 CRH;            /* Port configuration register (high) */
#define GPIO_CR_CNF(_x)   (_x)  /* Mode configuration */
#define GPIO_CR_MODE(_x)  (_x)  /* Mode */
   vu32 IDR;            /* Port input data register */
   vu32 ODR;            /* Port output data register */
   vu32 BSRR;           /* Port bit set/reset register */
   vu32 BRR;            /* Port bit reset register */
   vu32 LCKR;           /* Port configuration lock register */
};
#define pGPIO(_port) ((struct GPIO_regs*)(GPIO_BASE + (_port * 0x400)))

#define GPIO_CR(port,pin) (port + (0x04*(pin>7)))

#define CR_OUTPUT_OD        0x05
#define CR_OUTPUT_PP        0x01
#define CR_INPUT            0x04
#define CR_INPUT_PU_PD      0x08



#define NVIC_BASE  (SCS_BASE + 0x0100)
#define SCB_BASE   (SCS_BASE + 0x0D00)
#define STK        (SCS_BASE+0x10)
#define SCB_VTOR   (SCB_BASE+0x08)
#define STK_CTRL   (STK_BASE+0x00)

#define TIM1_APB2_ENB ((u32)0x00000800)
#define TIM1_PSC      (TIM1+0x28)
#define TIM1_ARR      (TIM1+0x2C)
#define TIM1_RCR      (TIM1+0x30)
#define TIM1_CR1      (TIM1+0x00)
#define TIM1_CR2      (TIM1+0x04)
#define TIM1_DIER     (TIM1+0x0C)
#define TIM1_UP_IRQ_Channel ((u8)0x19)

#define USB_HP_IRQ  ((u8)0x13)
#define USB_LP_IRQ  ((u8)0x14)
#define TIM2_IRQ    ((u8)0x1C)


/* AIRCR  */
#define AIRCR_RESET         0x05FA0000
#define AIRCR_RESET_REQ     (AIRCR_RESET | (u32)0x04);

/* temporary copyage of example from kiel */
#define __VAL(__TIMCLK, __PERIOD) ((__TIMCLK/1000000UL)*__PERIOD)
#define __PSC(__TIMCLK, __PERIOD)  (((__VAL(__TIMCLK, __PERIOD)+49999UL)/50000UL) - 1)
#define __ARR(__TIMCLK, __PERIOD) ((__VAL(__TIMCLK, __PERIOD)/(__PSC(__TIMCLK, __PERIOD)+1)) - 1)

// SWD and JTAG DEBUGGING
#define AFIO_MAPR_SWJ_CFG                      (0x7 << 24)
#define AFIO_MAPR_SWJ_CFG_FULL_SWJ             (0x0 << 24)
#define AFIO_MAPR_SWJ_CFG_FULL_SWJ_NO_NJRST    (0x1 << 24)
#define AFIO_MAPR_SWJ_CFG_NO_JTAG_SW           (0x2 << 24)
#define AFIO_MAPR_SWJ_CFG_NO_JTAG_NO_SW        (0x4 << 24)

void gpio_write_bit(u32 bank, u8 pin, u8 val);

#endif