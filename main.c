#include "hardware.h"
#include "stdio.h"

#define BLINK_RATE 0x200000

const char message[] = "Hello, Joel!\r\n";

static void gpio_bit(u32 bank, u8 pin, bool on)
{
   pGPIO(bank)->BSRR |= (1 << pin) << (on << 4);
}

static void blink(u32 bank, u8 pin, u32 count, u32 on, u32 off)
{
    u32 c;

    gpio_bit(bank, pin, 1);
    while (count-- > 0)
    {
        for (c = on; c > 0; c--)
            asm volatile("nop");

        gpio_bit(bank, pin, 0);

        for (c = off; c > 0; c--)
            asm volatile("nop");

        gpio_bit(bank, pin, 1);
    }
}

static void gpio_config_pin_out(u8 bank, u8 pin, bool opendrain, bool alt, bool out)
{
   // treat CNF + MODE as one field
   if (pin > 7)
   {
      pin -= 8;
      pGPIO(bank)->CRH |= (((out?1:0) | (alt?8:0) | (opendrain?4:0)) << (pin << 2));
   }
   else
   {
      pGPIO(bank)->CRL |= (((out?1:0) | (alt?8:0) | (opendrain?4:0)) << (pin << 2));
   }
}

static void setupHSE(void)
{
   // turn on HSE + HSI and wait for the HSE to be ready
   pRCC->CR |= RCC_CR_HSEON | RCC_CR_HSION;
   while ((pRCC->CR & RCC_CR_HSERDY) == 0);
}

static void setupPLL(void)
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
   // enable USART1 & GPIO clocks
   pRCC->APB2ENR |= RCC_APB2ENR_USART1EN | RCC_APB2ENR_IOPAEN | RCC_APB2ENR_IOPCEN;

   // enable USB clock
   pRCC->APB1ENR |= RCC_APB1ENR_USB_CLK;
}

/*
__inline static void stm32_NvicSetup (void) {

  if (__NVIC_USED & (1 << 0)) {                              // Vector Table Offset Register
    SCB->VTOR = (__NVIC_VTOR_VAL & (u32)0x3FFFFF80);         // set register
  }
} 
*/

void board_init(void)
{
   // disable all RCC interrupts 
   pRCC->CIR = 0;

   setupHSE();
   setupPLL();
   setupClocks();

   // enable GPIOC pin 13 as output (for LED)
   gpio_config_pin_out(2, 13, 0, 0, 1);

   // GPIOA pin 9 is USART1 Tx
   gpio_config_pin_out(0, 9, 1, 1, 1);
}

int main(void)
{
   int i;

   board_init();
   stdio_init();

	blink(2, 13, 5, BLINK_RATE, BLINK_RATE);

   gpio_bit(2, 13, 0);

   for (i=0; i < 10; i++)
   {
      if (printf(message))
      {
         gpio_bit(2, 13, 1);
         break;
      }
   }

   stdio_shutdown();

   return 0;
}
