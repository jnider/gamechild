#include "hardware.h"
#include "timer.h"
#include "stdio.h"

/* ST planned the timer register layout very nicely. Therefore, all timers use a subset of the following struct */
struct TIM_regs
{
   vu16 CR1;
   u16 reserved1;
#define TIM_CR1_CKD  (3<<8) /* clock division */
#define TIM_CR1_ARPE (1<<7) /* auto reload preload enable */
#define TIM_CR1_CMS  (1<<6) /* center-aligned mode selection */
#define TIM_CR1_DIR  (1<<4)
#define TIM_CR1_OPM  (1<<3) /* one pulse mode */
#define TIM_CR1_URS  (1<<2) /* update request source */
#define TIM_CR1_UDIS (1<<1) /* update disable */
#define TIM_CR1_CEN  (1<<0) /* counter enable */

   vu16 CR2;
   u16 reserved2;
   vu16 SMCR; /* slave mode control register */
   u16 reserved3;
   vu16 DIER; /* DMA/Interrupt enable register */
   u16 reserved4;
#define TIM_DIER_TDE (1<<14) /* DMA request enable */
#define TIM_DIER_CC4DE (1<<12)
#define TIM_DIER_CC3DE (1<<11)
#define TIM_DIER_CC2DE (1<<10)
#define TIM_DIER_CC1DE (1<<9)
#define TIM_DIER_UDE    (1<<8)
#define TIM_DIER_TIE    (1<<6) /* trigger interrupt disable */
#define TIM_DIER_CC4IE  (1<<4)
#define TIM_DIER_CC3IE  (1<<3)
#define TIM_DIER_CC2IE  (1<<2)
#define TIM_DIER_CC1IE  (1<<1)
#define TIM_DIER_UIE    (1<<0)

   vu16 SR;
   u16 reserved5;
#define TIM_SR_UIF     (1<<0)

   vu16 EGR;
   u16 reserved6;
#define TIM_EGR_UG     (1<<0)

   u32 reserved7[3];
   vu16 CNT; /* counter */
   u16 reserved8;
   vu16 PSC; /* prescaler */
   u16 reserved9;
   vu32 ARR; /* auto-reload */
};

#define pRegs(_t) ((struct TIM_regs*)_t->base)

#define __TIM1_PERIOD             0xF4240                 //  4
#define __PSC(__TIMCLK, __PERIOD)  (((__VAL(__TIMCLK, __PERIOD)+49999UL)/50000UL) - 1)
#define __ARR(__TIMCLK, __PERIOD) ((__VAL(__TIMCLK, __PERIOD)/(__PSC(__TIMCLK, __PERIOD)+1)) - 1)

/* Timer map
A = advanced
B = basic
G = general
Timer  1   2   3   4   5   6   7   8   9  10  11  12  13  14
APB1       G   G   G   G   B   B                   G   G   G
APB2   A                           A   G   G   G
*/

/* how many hardware timers can be used */
#define MAX_TIMERS 4

/* index into the 'timers' array for each hardware timer */
#define TIMER_INDEX_TIM2 0
#define TIMER_INDEX_TIM3 1
#define TIMER_INDEX_TIM4 2
#define TIMER_INDEX_TIM5 3

static struct timer *timers[MAX_TIMERS];

/* The prescaler (RCC_CFGR_PPRE1) is set to 1/2 so the clock source rate is (PCLK1 x 2)=36MHz */

void timer_init(struct timer *t, u32 base, timer_cb cb)
{
   switch(base)
   {
   case TIM2_BASE:
      timers[TIMER_INDEX_TIM2] = t;
      break;
   case TIM3_BASE:
      timers[TIMER_INDEX_TIM3] = t;
      break;
   case TIM4_BASE:
      timers[TIMER_INDEX_TIM4] = t;
      break;
   case TIM5_BASE:
      timers[TIMER_INDEX_TIM5] = t;
      break;
   }
   
   t->base = base;
   t->callback = cb;
   //pRegs(t)->CR1 = TIM_CR1_ARPE;
   pRegs(t)->DIER = TIM_DIER_UIE;// | TIM_DIER_TIE | TIM_DIER_UDE;
}

void timer_start(struct timer *t, u32 micros, int oneshot)
{
   //0.5s timer = 500ms = 500000us
   // 36MHz / 500000 = 72
   pRegs(t)->ARR = 0xFFFF;//36000000;///micros;
   pRegs(t)->PSC = 128;
   pRegs(t)->CR1 |= TIM_CR1_CEN;
}

void timer_pulse(struct timer *t)
{
   pRegs(t)->CR1 |= TIM_CR1_OPM;
}

void timer_clear(struct timer *t)
{
   pRegs(t)->SR |= TIM_SR_UIF;
   pRegs(t)->EGR |= TIM_EGR_UG;
}

/////// Interrupt Handlers ////////
void __attribute__((interrupt("IRQ"))) TIM2_IRQHandler()
{
   struct timer *t = timers[TIMER_INDEX_TIM2];
   if (t && t->callback)
   {
      //timer_clear(t);
     // NVIC_clearPending(INT_VEC_TIM2);
      t->callback(t);
   }
   else
   {
      printf("unexpected TIM2 INT\r\n");
   }
}

