#include "hardware.h"
#include "clock.h"
#include "timer.h"
#include "gpio.h"
#include "system.h"
#include "stdio.h"

#define BLINK_RATE 0x200000

const char message[] = "Hello, Joel!\r\n";

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

/*
__inline static void stm32_NvicSetup (void) {

  if (__NVIC_USED & (1 << 0)) {                              // Vector Table Offset Register
    SCB->VTOR = (__NVIC_VTOR_VAL & (u32)0x3FFFFF80);         // set register
  }
} 
*/

void board_init(void)
{

   setupHSE();
   setupPLL();
   setupClocks();

   // enable GPIOC pin 13 as output (for LED)
   gpio_config_pin(2, 13, 0, 0, 1);

   // GPIOA pin 9 is USART1 Tx
   gpio_config_pin(0, 9, 1, 1, 1);
}

int main(void)
{
   u16 part;
   u8 impl, var, rev;

   board_init();
   stdio_init();
   //timer_init();

	blink(2, 13, 5, BLINK_RATE, BLINK_RATE);

   gpio_bit(2, 13, 0);

   printf("STF103 initialized\r\n");

   SCB_ReadCPUID(&impl, &var, &part, &rev);
   printf("CPUID: impl=%i var=%i part=%i rev=%i\n",
      (int)impl, (int)var, (int)part, (int)rev);

   stdio_shutdown();

   return 0;
}
