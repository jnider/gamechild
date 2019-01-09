#include "hardware.h"
#include "clock.h"

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

void setupHSE(void)
{
   // turn on HSE + HSI and wait for the HSE to be ready
   pRCC->CR |= RCC_CR_HSEON | RCC_CR_HSION;
   while ((pRCC->CR & RCC_CR_HSERDY) == 0);
}

void setupPLL(void)
{
    /* Since we are executing code directly from the flash, we must enable flash
       wait states to prevent loss of access when accelerating the system clock.
       For SYSCLK >= 48MHz we need 2 wait states. Also enable the prefetch buffer. */
   pFLASH->ACR = FLASH_ACR_PRFTBE | FLASH_ACR_WAITSTATES(2);

   // set HSE as the source for the PLL, and multiply x9
   pRCC->CFGR = RCC_CFGR_PLLSRC | RCC_CFGR_PLLMUL(9) | RCC_CFGR_PPRE1_X2;
   pRCC->CR |= RCC_CR_PLLON;
   while ((pRCC->CR & RCC_CR_PLLRDY) == 0);

   // set up peripheral bus clock scaling
   // [HPRE]   AHB(HCLK) <- SYSCLK
   // [PPRE1]  APB1(PCLK1) <- HCLK
   // [PPRE2]  APB2(PCLK2) <- HCLK
   // [ADCPRE] ADC <- PCLK2
   // [USBPRE] USB <- ?
   //pRCC->CFGR &= ~(RCC_CFGR_USBPRE | RCC_CFGR_PPRE1_MASK | RCC_CFGR_PPRE2_MASK | RCC_CFGR_HPRE_MASK | RCC_CFGR_ADCPRE_MASK);

   // select PLL as system clock source (SYSCLK)
   pRCC->CFGR |= RCC_CFGR_SW_PLL;
   while ((pRCC->CFGR & RCC_CFGR_SWS_PLL) == 0);
}

void setupClocks(void)
{
   // disable all RCC interrupts 
   pRCC->CIR = 0;

   // enable USART1 & GPIO clocks
   pRCC->APB2ENR |= RCC_APB2ENR_USART1EN | RCC_APB2ENR_IOPAEN | RCC_APB2ENR_IOPCEN;

   // enable USB clock
   pRCC->APB1ENR |= RCC_APB1ENR_USB_CLK;
}

