#include "hardware.h"
#include "usart.h"

static struct usart serial1;

void stdio_init(void)
{
   usart_init(&serial1, USART1_BASE);
}

void stdio_shutdown(void)
{
   usart_tx_stop(&serial1);
}

int printf(const char* msg)
{
   int err=0;
   while (*msg && !err)
      err = usart_tx(&serial1, *msg++);

   return err;
}
