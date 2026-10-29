#include<lpc21xx.h>
#include "header.h"

extern u8 flag;
void exintr0_handler() __irq
{
flag=!flag;
EXTINT=1;
VICVectAddr=0;

}

void config_vic_for_exint0(void)
{
PINSEL1|=1;
EXTMODE=1;
EXTPOLAR=0;
VICIntSelect=0;
VICVectCntl0=14|(1<<5);
VICVectAddr0=(u32) exintr0_handler;
VICIntEnable=1<<14;

}
