#ifndef _GPIO__H
#define _GPIO__H

#include "hardware.h"

struct GPIO_regs
{
   vu32 CRL;            /* Port configuration register (low) */
   vu32 CRH;            /* Port configuration register (high) */
#define GPIO_CR_CNF(_x)   (_x)  /* Mode configuration */
#define GPIO_CR_MODE(_x)  (_x)  /* Mode */
   vu32 IDR;            /* Port input data register */
   vu32 ODR;            /* Port output data register */
   vu32 BSRR;           /* Port bit set/reset register */
   vu32 BRR;            /* Port bit reset register */
   vu32 LCKR;           /* Port configuration lock register */
};
#define pGPIO(_port) ((struct GPIO_regs*)(GPIO_BASE + (_port * 0x400)))

void gpio_bit(u32 bank, u8 pin, bool on);
void gpio_config_pin(u8 bank, u8 pin, bool opendrain, bool alt, bool out);

#endif // _GPIO__H
