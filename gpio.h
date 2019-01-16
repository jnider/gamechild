#ifndef _GPIO__H
#define _GPIO__H

void gpio_bit(u32 bank, u8 pin, bool on);
void gpio_config_pin(u8 bank, u8 pin, bool opendrain, bool alt, bool out);

#endif // _GPIO__H
