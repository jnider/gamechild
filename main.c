#include "hardware.h"
#include "clock.h"
#include "timer.h"
#include "gpio.h"
#include "system.h"
#include "nvic.h"
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

void board_init(void)
{

   setupHSE();
   setupPLL();
   setupClocks();

   // enable GPIOC pin 13 as output (for LED)
   gpio_config_pin(2, 13, 0, 0, 1);

   // GPIOA pin 9 is USART1 Tx
   gpio_config_pin(0, 9, 1, 1, 1);

   // enable IRQ for USART Rx/Tx
   NVIC_enableInt(INT_VEC_USART1, 0);
}

void __attribute__((interrupt("IRQ"))) USART1_IRQHandler()
{
   gpio_bit(2, 13, 1);
   int c = stdio_getchar();
   printf("Got %i from USART\r\n", c);
   gpio_bit(2, 13, 0);
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
   printf("CPUID: impl=%i var=%i part=%i rev=%i\r\n", impl, var, part, rev);

   // wait for an interrupt
   while(1);

   stdio_shutdown();

   return 0;
}
