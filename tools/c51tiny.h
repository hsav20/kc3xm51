
#ifdef  ROOT
    #define EXTR 
#else        
    #define EXTR        extern 
#endif                  

typedef unsigned char uint8_t;
typedef unsigned short uint16_t;
typedef unsigned long uint32_t;

typedef bit BOOL;
typedef unsigned char BYTE;
typedef unsigned short WORD;
typedef unsigned long DWORD;
typedef unsigned char u8;
typedef unsigned short u16;
typedef unsigned long u32;

typedef unsigned char code CONST_CHAR;
typedef unsigned short code CONST_WORD;
    
sfr IOP1                        = 0x90;
sfr IOP3                        = 0xb0;
sfr IOP2                        = 0xa0;
sfr IOP0                        = 0x80;
sfr IOPE                        = 0xe8;

sfr PSW                         = 0xd0;
sfr ACC                         = 0xe0;
sfr WDT                         = 0xe1;
sfr ISP                         = 0xe7;
sfr B                           = 0xf0;
sfr SP                          = 0x81;
sfr DPL                         = 0x82;
sfr DPH                         = 0x83;
sfr PCON                        = 0x87;
sfr TCON                        = 0x88;
sfr TMOD                        = 0x89;

sfr TL0    						= 0x8a;    					// Timer 0, low byte
sfr TL1    						= 0x8b;    					// Timer 1, low byte
sfr TH0    						= 0x8c;   	 				// Timer 0, high byte
sfr TH1    						= 0x8d;    					// Timer 1, high byte
sfr AUXR0                       = 0x8e;
sfr SCON                        = 0x98;
sfr SBUF                        = 0x99;
sfr AUXR1                       = 0xa2;
sfr IE                          = 0xa8;
sfr IP                          = 0xb8;
#define T0FREQ					TH0


sfr GRAM2                       = 0xa9;
sfr GRAM3                       = 0xb9;
sfr GRAM4                       = 0xca;
sfr GRAM5                       = 0xcb;
sfr GRAM6                       = 0xcc;
sfr GRAM7                       = 0xcd;

sfr T2CON                       = 0xC8;
sfr T2MOD                       = 0xC9;
sfr RCAP2L                      = 0xCA;
sfr RCAP2H                      = 0xCB;
sfr TL2                         = 0xCC;
sfr TH2                         = 0xCD;

sfr ISP_DATA                    = 0xe2;
sfr ISP_ADDRH                   = 0xe3;
sfr ISP_ADDRL                   = 0xe4;
sfr ISP_CMD                     = 0xe5;
sfr ISP_TRIG                    = 0xe6;
sfr ISP_CONTR                   = 0xe7;


// BIT Registers
// PSW
sbit CY                         = PSW^7;
sbit AC                         = PSW^6;
sbit F0                         = PSW^5;
sbit RS1                        = PSW^4;
sbit RS0                        = PSW^3;
sbit OV                         = PSW^2;
sbit P                          = PSW^0;

// TCON
sbit TF1                        = TCON^7;
sbit TR1                        = TCON^6;
sbit TF0                        = TCON^5;
sbit TR0                        = TCON^4;
sbit IE1                        = TCON^3;
sbit IT1                        = TCON^2;
sbit IE0                        = TCON^1;
sbit IT0                        = TCON^0;

// IE
sbit EA                         = IE^7;
sbit ET2                        = IE^5;
sbit ES                         = IE^4;
sbit ET1                        = IE^3;
sbit EX1                        = IE^2;
sbit ET0                        = IE^1;
sbit EX0                        = IE^0;

// IP
sbit PT2                        = IP^5;  
sbit PS                         = IP^4;  
sbit PT1                        = IP^3;  
sbit PX1                        = IP^2;  
sbit PT0                        = IP^1;  
sbit PX0                        = IP^0;  
                                         
// SCON                                  
sbit SM0                        = SCON^7;
sbit SM1                        = SCON^6;
sbit SM2                        = SCON^5;
sbit REN                        = SCON^4;
sbit TB8                        = SCON^3;
sbit RB8                        = SCON^2;
sbit TI                         = SCON^1;
sbit RI                         = SCON^0;

sbit P10    					= IOP1^0;
sbit P11    					= IOP1^1;
sbit P12    					= IOP1^2;
sbit P13    					= IOP1^3;
sbit P14    					= IOP1^4;
sbit P15    					= IOP1^5;
sbit P16    					= IOP1^6;
sbit P17    					= IOP1^7;

sbit P30    					= IOP3^0;
sbit P31    					= IOP3^1;
sbit P32    					= IOP3^2;
sbit P33    					= IOP3^3;
sbit P34    					= IOP3^4;
sbit P35    					= IOP3^5;
sbit P36    					= IOP3^6;
sbit P37    					= IOP3^7;

sbit P20    					= IOP2^0;
sbit P21    					= IOP2^1;
sbit P22    					= IOP2^2;
sbit P23    					= IOP2^3;
sbit P24    					= IOP2^4;
sbit P25    					= IOP2^5;
sbit P26    					= IOP2^6;
sbit P27    					= IOP2^7;

sbit P00    					= IOP0^0;
sbit P01    					= IOP0^1;
sbit P02    					= IOP0^2;
sbit P03    					= IOP0^3;
sbit P04    					= IOP0^4;
sbit P05    					= IOP0^5;
sbit P06    					= IOP0^6;
sbit P07    					= IOP0^7;

sbit PE0    					= IOPE^0;
sbit PE1    					= IOPE^1;
sbit PE2    					= IOPE^2;
sbit PE3    					= IOPE^3;

/*? T2CON? */
sbit TF2                        = T2CON^7;
sbit EXF2                       = T2CON^6;
sbit RCLK                       = T2CON^5;
sbit TCLK                       = T2CON^4;
sbit EXEN2                      = T2CON^3;
sbit TR2                        = T2CON^2;
sbit C_T2                       = T2CON^1;
sbit CP_RL2                     = T2CON^0;


#define OP10(bit)               if (bit) {P10=1;} else {P10=0;}
#define OP11(bit)               if (bit) {P11=1;} else {P11=0;}
#define OP12(bit)               if (bit) {P12=1;} else {P12=0;}
#define OP13(bit)               if (bit) {P13=1;} else {P13=0;}
#define OP14(bit)               if (bit) {P14=1;} else {P14=0;}
#define OP15(bit)               if (bit) {P15=1;} else {P15=0;}
#define OP16(bit)               if (bit) {P16=1;} else {P16=0;}
#define OP17(bit)               if (bit) {P17=1;} else {P17=0;}

#define OP30(bit)               if (bit) {P30=1;} else {P30=0;}
#define OP31(bit)               if (bit) {P31=1;} else {P31=0;}
#define OP32(bit)               if (bit) {P32=1;} else {P32=0;}
#define OP33(bit)               if (bit) {P33=1;} else {P33=0;}
#define OP34(bit)               if (bit) {P34=1;} else {P34=0;}
#define OP35(bit)               if (bit) {P35=1;} else {P35=0;}
#define OP36(bit)               if (bit) {P36=1;} else {P36=0;}
#define OP37(bit)               if (bit) {P37=1;} else {P37=0;}

#define OP20(bit)               if (bit) {P20=1;} else {P20=0;}
#define OP21(bit)               if (bit) {P21=1;} else {P21=0;}
#define OP22(bit)               if (bit) {P22=1;} else {P22=0;}
#define OP23(bit)               if (bit) {P23=1;} else {P23=0;}
#define OP24(bit)               if (bit) {P24=1;} else {P24=0;}
#define OP25(bit)               if (bit) {P25=1;} else {P25=0;}
#define OP26(bit)               if (bit) {P26=1;} else {P26=0;}
#define OP27(bit)               if (bit) {P27=1;} else {P27=0;}

#define OP00(bit)               if (bit) {P00=1;} else {P00=0;}
#define OP01(bit)               if (bit) {P01=1;} else {P01=0;}
#define OP02(bit)               if (bit) {P02=1;} else {P02=0;}
#define OP03(bit)               if (bit) {P03=1;} else {P03=0;}
#define OP04(bit)               if (bit) {P04=1;} else {P04=0;}
#define OP05(bit)               if (bit) {P05=1;} else {P05=0;}
#define OP06(bit)               if (bit) {P06=1;} else {P06=0;}
#define OP07(bit)               if (bit) {P07=1;} else {P07=0;}

#define OPE0(bit)               if (bit) {PE0=1;} else {PE0=0;}
#define OPE1(bit)               if (bit) {PE1=1;} else {PE1=0;}
#define OPE2(bit)               if (bit) {PE2=1;} else {PE2=0;}
#define OPE3(bit)               if (bit) {PE3=1;} else {PE3=0;}

#define TMODE_16bitTimer0       0x01
#define TMODE_8bitAutoTimer0    0x02
#define TMODE_16bitTimer1       0x10
#define TMODE_8bitAutoTimer1    0x20

#define TCON_IT0                0x01   /* 1 = Edge-sensitive */
#define TCON_IE0                0x02   /* 1 = INT0 interrupt and auto clear */
#define TCON_IT1                0x04   /* 1 = Edge-sensitive */
#define TCON_IE1                0x08   /* 1 = INT0 interrupt and auto clear */
#define TCON_TR0                0x10   /* TIMER0 start */
#define TCON_TF0                0x20   /* 1 = TIMER0 interrupt and auto clear */
#define TCON_TR1                0x40   /* TIMER1 start */
#define TCON_TE1                0x80   /* 1 = TIMER0 interrupt and auto clear */

#define SCON_MODE0              0x00  /* fosc/12 shift */ 
#define SCON_MODE1              0x40  /* 8bit UART */ 
#define SCON_MODE2              0x80  /* fosc/64or32 9bit UART */ 
#define SCON_MODE3              0xc0  /* 9bit UART */ 
#define SCON_SM2                0x20  /* 1 = enable mult1processor */
#define SCON_REN                0x10  /* 1 = SERIAL start */

#define PCON_SMOD               0x80  /* 1 = SMOD set */
#define IEX0                    0x01
#define IEX1                    0x04


#define MSYS_INT(valA, valB)    {T0FREQ = 256 - ((valA / 240) * 4); IP |= valB; IE |= (0x80 | valB);}
extern void          _nop_     (void);
#define NOP          _nop_

void MLOG(char *fmt, ...);
