#include "hardware.h"

struct NVIC_regs
{
   vu32 ISER[2];       /* set enable   : 0x00 */
   u32  reserved0[30];
   vu32 ICER[2];       /* clear enable : 0x80 */
   u32  reserved1[30];
   vu32 ISPR[2];       /* set pending  : 0x100 */
   u32  reserved2[30];
   vu32 ICPR[2];       /* clear pending: 0x180 */
   u32  reserved3[30];
   vu32 IABR[2];       /* active bits  : 0x200 */
   u32  reserved4[62];
   vu8  IPR[67];       /* priority     : 0x300 */
};

#define pNVIC ((struct NVIC_regs*)NVIC_BASE)

#define NVIC_STIR (NVIC2_BASE) /* software trigger */

typedef struct {
    u8 NVIC_IRQChannel;
    u8 NVIC_IRQChannelPreemptionPriority;
    u8 NVIC_IRQChannelSubPriority;
    bool NVIC_IRQChannelCmd; /* TRUE for enable */
} NVIC_InitTypeDef;

void NVIC_enableInt(int vecnum, int priority)
{
    //pNVIC->IPR[(channel >> 0x02)] = 0;

   pNVIC->ISER[(vecnum >> 0x05)] = (u32)0x01 << (vecnum & (u8)0x1F);
   //pNVIC->ISER[0] = 0xFFFFFFFF;
   //pNVIC->ISER[1] = 0xFFFFFFFF;
}

void NVIC_DisableInterrupts()
{
    pNVIC->ICER[0] = 0xFFFFFFFF;
    pNVIC->ICER[1] = 0xFFFFFFFF;
    pNVIC->ICPR[0] = 0xFFFFFFFF;
    pNVIC->ICPR[1] = 0xFFFFFFFF;

    //*STK_CTRL = 0x04; /* disable the systick, which operates separately from nvic */
}

void NVIC_setPending(int vecnum)
{
   pNVIC->ISPR[(vecnum >> 0x05)] = (u32)0x01 << (vecnum & (u8)0x1F);
}

void NVIC_clearPending(int vecnum)
{
   pNVIC->ICPR[(vecnum >> 0x05)] = (u32)0x01 << (vecnum & (u8)0x1F);
}

