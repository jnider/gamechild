#include "hardware.h"
#include "system.h"

struct SCB_regs
{
   vu32 CPUID;
#define CPUID_IMPL_R(_x) ((_x >> 24) & 0xFF)
#define CPUID_VAR_R(_x)  ((_x >> 20) & 0xF)
#define CPUID_PART_R(_x) ((_x >> 4) & 0xFFF)
#define CPUID_REV_R(_x)  ((_x) & 0xF)

};

#define pSCB ((struct SCB_regs*)SCB_BASE)

void SCB_ReadCPUID(u8 *impl, u8 *var, u16 *part, u8 *rev)
{
   u32 cpuid = pSCB->CPUID;

   if (impl)
      *impl = CPUID_IMPL_R(cpuid);

   if (var)
      *var = CPUID_VAR_R(cpuid);

   if (part)
      *part = CPUID_PART_R(cpuid);

   if (rev)
      *rev = CPUID_REV_R(cpuid);
}
