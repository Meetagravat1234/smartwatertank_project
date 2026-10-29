#include<lpc21xx.h>
#include "header.h"

u32 add;

void eeprom(u8 t,u8 w,u8 d)
{
 u8 temp,f1=0,f2=0,f3=0;

//u8 f1=0;
i2c_write(0xa0,add,t);
delay_ms(30);
temp=i2c_read(0xa1,add++);
if(temp==t)
{
f1=1;

}
i2c_write(0xa0,add,w);
delay_ms(30);
temp=i2c_read(0xa1,add++);
if(temp==t)
{
f2=1;
}
i2c_write(0xa0,add,d);
delay_ms(30);
temp=i2c_read(0xa1,add++);
if(temp==t)
{
f3=1;
}

if(f1&&f2&&f3)
{
uart0_tx_string("Data saved\r\n");
}
else
{
uart0_tx_string("Data not saved\r\n");
}
}
