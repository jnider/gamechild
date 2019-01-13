#include "hardware.h"
#include "usart.h"

typedef struct USART_regs
{
   vu32 SR;       /* Status register */
#define SR_CTS    (1<<9) /* clear to send */
#define SR_LBD    (1<<8) /* LIN break detection */
#define SR_TXE    (1<<7) /* Transmit data register empty */
#define SR_TC     (1<<6) /* Transmission complete */
#define SR_RXNE   (1<<5) /* Read data register not empty */
#define SR_IDLE   (1<<4) /* Idle line detected */
#define SR_ORE    (1<<3) /* Overrun error */
#define SR_NE     (1<<2) /* Noise error */
#define SR_FE     (1<<1) /* Framing error */
#define SR_PE     (1<<0) /* Parity error */

   vu32 DR;       /* Data register */
   vu32 BRR;      /* Baud rate register */
#define BRR_DIV_MAN(_x) ((_x & 0x7FF) << 4) /* mantissa of USARTDIV */
#define BRR_DIV_FRC(_x) ((_x & 0xF) << 0)   /* fraction of USARTDIV */

   vu32 CR1;      /* Control register 1 */
#define CR1_UE    (1<<13)  /* USART enable */
#define CR1_M     (1<<12)  /* word length 0=8bit, 1=9bit*/
#define CR1_WAKE  (1<<11)  /* wakeup method */
#define CR1_PCE   (1<<10)  /* parity control enable */
#define CR1_PS    (1<<9)   /* parity selection */
#define CR1_PEIE  (1<<8)   /* PE interrupt enable */
#define CR1_TXEIE (1<<7)   /* TXE interrupt enable */
#define CR1_TCIE  (1<<6)   /* Transmission complete interrupt enable */
#define CR1_RXNEIE (1<<5)  /* Read data register not empty interrupt enable */
#define CR1_IDLEIE (1<<4)  /* Idle interrupt enable */
#define CR1_TE    (1<<3)   /* Transmitter enable */
#define CR1_RE    (1<<2)   /* Receiver enable */
#define CR1_RWU   (1<<1)   /* Receiver wakeup */
#define CR1_SBK   (1<<0)   /* Send break */

   vu32 CR2;      /* Control Register 2 */
#define CR2_LINEN (1<<14)  /* LIN mode enable */
#define CR2_STOP(_n)  ((_n & 0x3)<<12)  /* Stop bits */
#define CR2_CLKEN (1<<11)  /* Clock enable */
#define CR2_CPOL  (1<<10)  /* Clock polarity */
#define CR2_CPHA  (1<<9)   /* Clock phase */
#define CR2_LBCL  (1<<8)   /* last bit clock pulse */
#define CR2_LBDIE (1<<6)   /* LIN break detection interrupt enable */
#define CR2_LBDL  (1<<5)   /* LIN break detection length */
#define CR2_ADD   (0xF<<0) /* Address of USART node */

   vu32 CR3;      /* Control Register 3 */
#define CR3_CTSIE (1<<10)  /* CTS interrupt enable */
#define CR3_CTSE  (1<<9)   /* CTS enable */
#define CR3_RTSE  (1<<8)   /* RTS enable */
#define CR3_DMAT  (1<<7)   /* DMA enable transmitter */
#define CR3_DMAR  (1<<6)   /* DMA enable receiver */
#define CR3_SCEN  (1<<5)   /* smartcard mode enable */
#define CR3_NACK  (1<<4)   /* smartcard NACK enable */
#define CR3_HDSEL (1<<3)   /* half-duplex selection */
#define CR3_IRLP  (1<<2)   /* IrDA low-power */
#define CR3_IREN  (1<<1)   /* IrDA mode enable */
#define CR3_EIE   (1<<0)   /* Error interrupt enable */

   vu32 GTPR;     /* Guard time and prescaler register */
#define GTPR_GT   (0xFF<<8)   /* Guard time value */
#define GTPR_PSC  (0xFF<<0)   /* Prescaler value */
} USART_regs;

#define pRegs ((struct USART_regs*)u->base)

/*----------------------------------------------------------------------------
 Define  Baudrate setting (BRR) for USART1
 *----------------------------------------------------------------------------*/
#define __DIV(__PCLK, __BAUD)       ((__PCLK*25)/(4*__BAUD))
#define __DIVMANT(__PCLK, __BAUD)   (__DIV(__PCLK, __BAUD)/100)
#define __DIVFRAQ(__PCLK, __BAUD)   (((__DIV(__PCLK, __BAUD) - (__DIVMANT(__PCLK, __BAUD) * 100)) * 16 + 50) / 100)
#define __USART_BRR(__PCLK, __BAUD) ((__DIVMANT(__PCLK, __BAUD) << 4)|(__DIVFRAQ(__PCLK, __BAUD) & 0x0F))


void usart_init(struct usart *u, u32 base)
{
   u->base = base;

   // clear CTS and RTS bits
   pRegs->CR3 &= ~(CR3_CTSE | CR3_RTSE);
 
   // we want 115200-8N1 in case it's not clear
   usart_set_stop_bits(u, 1);
   usart_set_baud_rate(u, 115200);

   // enable tx and rx channels, and interrupts
   pRegs->CR1 |= CR1_TE | CR1_RE | CR1_RXNEIE; //CR1_PEIE | CR1_TXEIE | CR1_TCIE;

   // everything is ready - enable the USART
   pRegs->CR1 |= CR1_UE;
}

void usart_set_stop_bits(struct usart *u, enum usart_stop_bits sb)
{
   u32 temp = pRegs->CR2 & ~CR2_STOP(3);
   pRegs->CR2 = temp | CR2_STOP(sb);
}

void usart_set_baud_rate(struct usart *u, u32 bitrate)
{
   // assume the fCLK is 72MHz
   //u32 fclk = (72000000 >> 4);

   // to make the calculation harder, ST decided to use some
   // kind of screwy fixed point to code the number 
   // rather than just store the integer in bits

   // baud = fCLK / (bitrate x 16)
   // 115200 = 72M / (16 * USARTDIV)
   // (16 * USARTDIV) * bitrate = fCLK
   // USARTDIV = (fCLK/16) / bitrate
   // USARTDIV = 4500000 / bitrate
   // USARTDIV = 4500000 / 115200
   // USARTDIV = 39.0625
   // Mantissa = 39, fraction=0.0625 * 16 = 1
   u32 mantissa = 39;
   u32 fraction = 1;

   //if (u->base != USART1_BASE)
   //   fclk = (36000000 >> 4);

   // mantissa = __DIVMANT(fclk, bitrate);
   // fraction = __DIVFRAQ(fclk, bitrate);

   pRegs->BRR = BRR_DIV_MAN(mantissa) | BRR_DIV_FRC(fraction);
}

int usart_tx(struct usart *u, const u8 data)
{
   u32 status;

   // load the data register
   pRegs->DR = (u16)(data & 0xFF);

   // wait until the data has been sent
   while ((status = (pRegs->SR & SR_TXE)) == 0);

   if (status & SR_PE)
      return -10;
   if (status & SR_NE)
      return -11;
   if (status & SR_ORE)
      return -12;

   return 0;
}

int usart_rx_data(struct usart *u)
{
   // make sure there is data to read
   if (!(pRegs->SR & SR_RXNE))
      return -1;

   // reading the data will automatically clear the RXNE
   return (pRegs->DR & 0xFF);
}

void usart_tx_stop(struct usart *u)
{
   u32 status;

   // wait for the transmission to complete
   while ((status = (pRegs->SR & SR_TC)) == 0);

   // disable tx
   pRegs->CR1 &= ~CR1_TE;
}

