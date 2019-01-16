#ifndef _TIMER__H
#define _TIMER__H

#include "hardware.h"

struct timer;

/* callback function on interrupt */
typedef void (*timer_cb)(struct timer *t);

struct timer
{
   u32 base;
   timer_cb callback;
};

void timer_init(struct timer *t, u32 base, timer_cb cb);
void timer_start(struct timer *t, u32 micros, int oneshot);
void timer_pulse(struct timer *t);
void timer_clear(struct timer *t);

#endif // _TIMER__H
