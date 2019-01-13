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

   vu32 CIR;         /* Clock interrupt register 7.3.3 */
#define RCC_CIR_CSSC          (1<<23) /* 'clear' bits */
#define RCC_CIR_PLLRDYC       (1<<20)
#define RCC_CIR_HSERDYC       (1<<19)
#define RCC_CIR_HSIRDYC       (1<<18)
#define RCC_CIR_LSERDYC       (1<<17)
#define RCC_CIR_LSIRDYC       (1<<16)
#define RCC_CIR_PLLRDYIE      (1<<12) /* 'interrupt enable' bits */
#define RCC_CIR_HSERDYIE      (1<<11)
#define RCC_CIR_HSIRDYIE      (1<<10)
#define RCC_CIR_LSERDYIE      (1<<9)
#define RCC_CIR_LSIRDYIE      (1<<8)
#define RCC_CIR_CSSF          (1<<7) /* 'flag' bits */
#define RCC_CIR_PLLRDYF       (1<<4)
#define RCC_CIR_HSERDYF       (1<<3)
#define RCC_CIR_HSIRDYF       (1<<2)
#define RCC_CIR_LSERDYF       (1<<1)
#define RCC_CIR_LSIRDYF       (1<<0)

   vu32 APB2RSTR;
#define RCC_APB2RSTR_TIM11RST   (1<<21)
#define RCC_APB2RSTR_TIM10RST   (1<<20)
#define RCC_APB2RSTR_TIM9RST    (1<<19)
#define RCC_APB2RSTR_ADC3RST    (1<<15)
#define RCC_APB2RSTR_USART1RST  (1<<14)
#define RCC_APB2RSTR_TIM8RST    (1<<13)
#define RCC_APB2RSTR_SPI1RST    (1<<12)
#define RCC_APB2RSTR_TIM1RST    (1<<11)
#define RCC_APB2RSTR_ADC2RST    (1<<10)
#define RCC_APB2RSTR_ADC1RST    (1<<9)
#define RCC_APB2RSTR_IOPGRST    (1<<8)
#define RCC_APB2RSTR_IOPFRST    (1<<7)
#define RCC_APB2RSTR_IOPERST    (1<<6)
#define RCC_APB2RSTR_IOPDRST    (1<<5)
#define RCC_APB2RSTR_IOPCRST    (1<<4)
#define RCC_APB2RSTR_IOPBRST    (1<<3)
#define RCC_APB2RSTR_IOPARST    (1<<2)
#define RCC_APB2RSTR_AFIORST    (1<<0)

   vu32 APB1RSTR;
#define RCC_APB1RSTR_DACRST     (1<<29)
#define RCC_APB1RSTR_PWRRST     (1<<28)
#define RCC_APB1RSTR_BKPRST     (1<<27)
#define RCC_APB1RSTR_CANRST     (1<<25)
#define RCC_APB1RSTR_USBRST     (1<<23)
#define RCC_APB1RSTR_I2C2RST    (1<<22)
#define RCC_APB1RSTR_I2C1RST    (1<<21)
#define RCC_APB1RSTR_UART5RST   (1<<20)
#define RCC_APB1RSTR_UART4RST   (1<<19)
#define RCC_APB1RSTR_UART3RST   (1<<18)
#define RCC_APB1RSTR_UART2RST   (1<<17)
#define RCC_APB1RSTR_SPI3RST    (1<<15)
#define RCC_APB1RSTR_SPI2RST    (1<<14)
#define RCC_APB1RSTR_WWDGRST    (1<<11)
#define RCC_APB1RSTR_TIM14RST   (1<<8)
#define RCC_APB1RSTR_TIM13RST   (1<<7)
#define RCC_APB1RSTR_TIM12RST   (1<<6)
#define RCC_APB1RSTR_TIM7RST    (1<<5)
#define RCC_APB1RSTR_TIM6RST    (1<<4)
#define RCC_APB1RSTR_TIM5RST    (1<<3)
#define RCC_APB1RSTR_TIM4RST    (1<<2)
#define RCC_APB1RSTR_TIM3RST    (1<<1)
#define RCC_APB1RSTR_TIM2RST    (1<<0)

   vu32 AHBENR;
#define RCC_AHBENR_SDIOEN        (1<<10)
#define RCC_AHBENR_FSMCEN        (1<<8)
#define RCC_AHBENR_CRCEN         (1<<6)
#define RCC_AHBENR_FLITFEN       (1<<4)
#define RCC_AHBENR_SRAMEN        (1<<2)
#define RCC_AHBENR_DMA2EN        (1<<1)
#define RCC_AHBENR_DMA1EN        (1<<0)

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
#define RCC_APB1ENR_DACEN     (1<<29)
#define RCC_APB1RSTR_PWREN    (1<<28)
#define RCC_APB1RSTR_BKPEN    (1<<27)
#define RCC_APB1RSTR_CANEN    (1<<25)
#define RCC_APB1RSTR_USBEN    (1<<23)
#define RCC_APB1RSTR_I2C2EN   (1<<22)
#define RCC_APB1RSTR_I2C1EN   (1<<21)
#define RCC_APB1RSTR_UART5EN  (1<<20)
#define RCC_APB1RSTR_UART4EN  (1<<19)
#define RCC_APB1RSTR_UART3EN  (1<<18)
#define RCC_APB1RSTR_UART2EN  (1<<17)
#define RCC_APB1RSTR_SPI3EN   (1<<15)
#define RCC_APB1RSTR_SPI2EN   (1<<14)
#define RCC_APB1RSTR_WWDGEN   (1<<11)
#define RCC_APB1RSTR_TIM14EN  (1<<8)
#define RCC_APB1RSTR_TIM13EN  (1<<7)
#define RCC_APB1RSTR_TIM12EN  (1<<6)
#define RCC_APB1RSTR_TIM7EN   (1<<5)
#define RCC_APB1RSTR_TIM6EN   (1<<4)
#define RCC_APB1RSTR_TIM5EN   (1<<3)
#define RCC_APB1RSTR_TIM4EN   (1<<2)
#define RCC_APB1RSTR_TIM3EN   (1<<1)
#define RCC_APB1RSTR_TIM2EN   (1<<0)

   vu32 BDCR;
   vu32 CSR;
#define RCC_CSR_LSIRDY        (1<<1)
#define RCC_CSR_LSION         (1<<0)
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
   pRCC->CIR &= ~(RCC_CIR_PLLRDYIE | RCC_CIR_HSERDYIE | RCC_CIR_HSIRDYIE | RCC_CIR_LSERDYIE | RCC_CIR_LSIRDYIE);

   // enable USART1 & GPIO (A & C) clocks
   pRCC->APB2ENR |= RCC_APB2ENR_USART1EN | RCC_APB2ENR_IOPAEN | RCC_APB2ENR_IOPCEN;

   // enable USB clock
   //pRCC->APB1ENR |= RCC_APB1ENR_USB_CLK;
}

