#ifndef _HARDWARE__H
#define _HARDWARE__H

#include "stm32f10x_type.h"

/*
See http://www.st.com/web/en/resource/technical/document/reference_manual/CD00171190.pdf
*/

#define USER_CODE_FLASH0X8002000    ((u32)0x08002000)
#define RAM_END                     ((u32)0x20005000)

/* Core Peripheral Base Addresses - CD00228163.pdf */
#define SYSTIMER_BASE   (0xE000E010)
#define NVIC_BASE       (0xE000E100)
#define SCB_BASE        (0xE000ED00)
#define MPU_BASE        (0xE000ED90)
#define NVIC2_BASE      (0xE000EF00)

/* Peripheral Base Addresses */
#define TIM2_BASE    (0x40000000)
#define TIM3_BASE    (0x40000400)
#define TIM4_BASE    (0x40000800)
#define TIM5_BASE    (0x40000C00)
#define USB_BASE     (0x40005C00)
#define USB_SRAM_BASE (0x40006000)
#define AFIO_BASE    (0x40010000)
#define GPIO_BASE    (0x40010800)
#define TIM1_BASE    (0x40012C00)
#define SPI1_BASE    (0x40013000)
#define TIM8_BASE    (0x40013400)
#define USART1_BASE  (0x40013800) /* USART low speed serial */
#define TIM9_BASE    (0x40014C00)
#define TIM10_BASE   (0x40015000)
#define RCC_BASE     (0x40021000) /* Reset and clock control */
#define FLASH_BASE   (0x40022000) /* Flash memory interface */
#define SCS_BASE     (0xE000E000)

#define AFIO_MAPR    (AFIO_BASE + 0x04)

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

#define STK_BASE   (SCS_BASE+0x10)
#define SCB_VTOR   (SCB_BASE+0x08)
#define STK_CTRL   (STK_BASE+0x00)

#define TIM1_UP_IRQ_Channel ((u8)0x19)

#define USB_HP_IRQ  ((u8)0x13)
#define USB_LP_IRQ  ((u8)0x14)
#define TIM2_IRQ    ((u8)0x1C)


/* AIRCR  */
#define AIRCR_RESET         0x05FA0000
#define AIRCR_RESET_REQ     (AIRCR_RESET | (u32)0x04);

// SWD and JTAG DEBUGGING
#define AFIO_MAPR_SWJ_CFG                      (0x7 << 24)
#define AFIO_MAPR_SWJ_CFG_FULL_SWJ             (0x0 << 24)
#define AFIO_MAPR_SWJ_CFG_FULL_SWJ_NO_NJRST    (0x1 << 24)
#define AFIO_MAPR_SWJ_CFG_NO_JTAG_SW           (0x2 << 24)
#define AFIO_MAPR_SWJ_CFG_NO_JTAG_NO_SW        (0x4 << 24)

#endif // _HARDWARE__H
