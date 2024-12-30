#include<lpc21xx.h>
#define LED0 1<<4
#define LED1 1<<5
#define LED2 1<<6
#define LED3 1<<7
#define RELAY1 1<<8

#define LCD_D 0xf<<14
#define RS 1<<12
#define E 1<<13


void UART0_CONFIG(void);
void UART0_TX(unsigned char);
char UART0_RX-(void);
void UART0_STR(unsigned char *);
void controldevice(char);

void LCD_INIT(void);
void LCD_COMMAND(unsigned char);
void LCD_DATA(unsigned char);
void LCD_STR(unsigned char *);
void delay_milliseconds(int);


int main()
{
char controlcommand;
LCD_INIT();
UART0_CONFIG();
IODIR0|= LED0 | LED1 | LED2 | LED3 | RELAY1;
while(1)
{
controlcommand = UART0_RX();
controldevice(controlcommand);
}
}

void UART0_CONFIG(void)
{
PINSEL0|=0x05;
U0LCR=0x83;
U0DLL=97;
U0DLM=0;
U0LCR=0x03;
}

void UART0_TX(unsigned char ch)
{
U0THR = ch;
while((U0LSR>>5&1)==0);
}

char UART0_RX(void)
{
while((U0LSR&1)==0);
return U0RBR;
}

void UART0_STR(unsigned char *s)
{
while(*s)
UART0_TX(*s++);
}

void controldevice(char cmd)
{

if(cmd=='A')
{
IOCLR0 = LED0;
LCD_STR("LED0 ON");
UART0_STR("LED0 ON");
//delay_milliseconds(1000);
}
else if(cmd=='B')
{
IOSET0 = LED0;
LCD_STR("LED0 OFF");
//delay_milliseconds();
UART0_STR("LED0 OFF");
}
else if(cmd=='C')
{
IOCLR0 = LED1;
LCD_STR("LED1 ON");
UART0_STR("LED1 ON");
//delay_milliseconds(2);
}
else if(cmd=='D')
{
IOSET0 = LED1;
//delay_milliseconds(2);
LCD_STR("LED1 OFF");
UART0_STR("LED1 OFF");
}
else if(cmd=='E')
{
IOCLR0 = LED2;
LCD_STR("LED2 ON");
UART0_STR("LED2 ON");
}
else if(cmd=='F')
{
IOSET0 = LED2;
LCD_STR("LED2 OFF");
UART0_STR("LED2 OFF");
}
else if(cmd=='G')
{
IOCLR0 = LED3;
LCD_STR("LED3 ON");
UART0_STR("LED3 ON");
}
else if(cmd=='H')
{
IOSET0 = LED3;
LCD_STR("LED3 OFF");
UART0_STR("LED3 OFF");
}
else if(cmd=='I')
{
IOCLR0 = RELAY1;
LCD_STR("RELAY1 ON");
UART0_STR("RELAY1 ON");
}
else if(cmd=='J')
{
IOSET0 = RELAY1;
LCD_STR("RELAY1 OFF");
UART0_STR("RELAY1 OFF");
}
}

void LCD_INIT(void)
{
	IODIR0|=LCD_D|RS|E;
	LCD_COMMAND(0x01);
	LCD_COMMAND(0x02);
	LCD_COMMAND(0x0C);
	LCD_COMMAND(0x28);
	LCD_COMMAND(0x80);
}

void LCD_COMMAND(unsigned char cmd)
{
IOCLR0=LCD_D;
IOSET0=(cmd & 0xf0)<<10;
IOCLR0=RS;
IOSET0=E;
delay_milliseconds(2);
IOCLR0=E;

IOCLR0=LCD_D;
IOSET0=(cmd & 0x0f)<<14;
IOCLR0=RS;
IOSET0=E;
delay_milliseconds(2);
IOCLR0=E;
}

void LCD_DATA(unsigned char d)
{
IOCLR0=LCD_D;
IOSET0=(d & 0xf0)<<10;
IOSET0=RS;
IOSET0=E;
delay_milliseconds(2);
IOCLR0=E;

IOCLR0=LCD_D;
IOSET0=(d & 0x0f)<<14;
IOSET0=RS;
IOSET0=E;
delay_milliseconds(2);
IOCLR0=E;
}

void LCD_STR(unsigned char *s)
{
while(*s)
LCD_DATA(*s++);
}


void delay_milliseconds(int n)
{
T0PR=15000-1;
T0TCR=0x01;
while(T0TC<n);
T0TCR=0x03;
T0TCR=0x00;
}

