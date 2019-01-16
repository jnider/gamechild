#include "hardware.h"
#include "clock.h"
#include "timer.h"
#include "gpio.h"
#include "system.h"
#include "nvic.h"
#include "stdio.h"

#define BLINK_RATE 0x200000

static int led=0;
const char message[] = "Hello, Joel!\r\n";
static struct timer timer;

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

   // TIM2
   NVIC_enableInt(INT_VEC_TIM2, 0);
}

void __attribute__((interrupt("IRQ"))) USART1_IRQHandler()
{
   char temp[2];
   temp[1] =0;

   gpio_bit(2, 13, 1);
   int c = stdio_getchar();
   temp[0] = c;
   printf("Got %s from USART\r\n", temp);
   gpio_bit(2, 13, 0);
}

void tim2_onTimer(struct timer *t)
{
   static int count;
   count++;
   if (count >= 0x20000)
   {
      count = 0;
      led=!led;
      //printf ("led=%i\r\n", led);

      gpio_bit(2, 13, led);
   }
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

   printf("STM32F103 initialized\r\n");

   SCB_ReadCPUID(&impl, &var, &part, &rev);
   printf("CPUID: impl=%i var=%i part=%i rev=%i\r\n", impl, var, part, rev);

   timer_init(&timer, TIM2_BASE, tim2_onTimer);
   timer_start(&timer, 500000, 0);

   // wait for an interrupt
   while(1);

   stdio_shutdown();

   return 0;
}
