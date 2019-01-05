#ifndef _USART__H
#define _USART__H

#include "stm32f10x_type.h"

/* this encoding directly correlates to the hardware setting */
typedef enum usart_stop_bits
{
   USART_STOP_BITS_1,   /* 1 stop bit - default */
   USART_STOP_BITS_0_5, /* 0.5 stop bits */
   USART_STOP_BITS_2,   /* 2 stop bits */
   USART_STOP_BITS_1_5, /* 1.5 stop bits */
} usart_stop_bits;

struct usart
{
   u32 base;
};

#define __LL_USART_DIV_SAMPLING8_100(__PERIPHCLK__, __BAUDRATE__)      (((__PERIPHCLK__)*25)/(2*(__BAUDRATE__)))
#define __LL_USART_DIVMANT_SAMPLING8(__PERIPHCLK__, __BAUDRATE__)      (__LL_USART_DIV_SAMPLING8_100((__PERIPHCLK__), (__BAUDRATE__))/100)
#define __LL_USART_DIVFRAQ_SAMPLING8(__PERIPHCLK__, __BAUDRATE__)      (((__LL_USART_DIV_SAMPLING8_100((__PERIPHCLK__), (__BAUDRATE__)) - (__LL_USART_DIVMANT_SAMPLING8((__PERIPHCLK__), (__BAUDRATE__)) * 100)) * 8 + 50) / 100)
/* UART BRR = mantissa + overflow + fraction
            = (UART DIVMANT << 4) + ((UART DIVFRAQ & 0xF8) << 1) + (UART DIVFRAQ & 0x07) */
#define __LL_USART_DIV_SAMPLING8(__PERIPHCLK__, __BAUDRATE__)             (((__LL_USART_DIVMANT_SAMPLING8((__PERIPHCLK__), (__BAUDRATE__)) << 4) + \
                                                                           ((__LL_USART_DIVFRAQ_SAMPLING8((__PERIPHCLK__), (__BAUDRATE__)) & 0xF8) << 1)) + \
                                                                           (__LL_USART_DIVFRAQ_SAMPLING8((__PERIPHCLK__), (__BAUDRATE__)) & 0x07))


void usart_init(struct usart *u, u32 base);
void usart_set_stop_bits(struct usart *u, enum usart_stop_bits sb);
void usart_set_baud_rate(struct usart *u, u32 bitrate);
int usart_tx(struct usart *u, u8 data);
void usart_tx_stop(struct usart *u);

#endif // _USART__H
