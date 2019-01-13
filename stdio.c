#include "hardware.h"
#include <stdarg.h>
#include "usart.h"

static struct usart serial1;
static const char hex[]="0123456789ABCDEF";

int stdio_getchar(void)
{
   return usart_rx_data(&serial1);
}

void stdio_init(void)
{
   usart_init(&serial1, USART1_BASE);
}

void stdio_shutdown(void)
{
   usart_tx_stop(&serial1);
}

char* itoa(int i)
{
   u8 count=0;
   static char tmp[12];
   tmp[11] = 0;
   while (i)
   {
      count++;
      tmp[11 - count] = hex[(i & 0xF)];
      i >>= 4;
   }
   return &tmp[11 - count];
}

int printf(const char* format, ...)
{
   va_list arg;
   int err=0;
   char *num, *str;

   va_start(arg, format);
   while (*format && !err)
   {
      if (*format == '%')
      {
         format++;
         switch(*format++)
         {
         case 'i':
            num = itoa(va_arg(arg,int));
            while (*num)
               err = usart_tx(&serial1, *num++);
            break;

         case 's':
            str = va_arg(arg, char*);
            while (*str)
               err = usart_tx(&serial1, *str++);
            break;
         }
      }
      err = usart_tx(&serial1, *format++);
   }

   va_end(arg);

   return err;
}
