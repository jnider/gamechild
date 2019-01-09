#include "gpio.h"

void gpio_bit(u32 bank, u8 pin, bool on)
{
   pGPIO(bank)->BSRR |= (1 << pin) << (on << 4);
}

void gpio_config_pin(u8 bank, u8 pin, bool opendrain, bool alt, bool out)
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

