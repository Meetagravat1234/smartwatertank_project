#include<lpc21xx.h>
#include<stdio.h>
#include "header.h"
u8 flag;

int main()
{
int temp=55;
u8 h,h1,m,s,d,mo,y;
char buf[100];
float vout,water_level=0;

unsigned int adcout,distance=62,adc_value;
IODIR0=1<<14;
//VPBDIV=1;

config_vic_for_exint0();
uart0_init(9600);
adc_init();
lcd_init();
relay_init();
ultrasonic_init();
servo_init();
i2c_init();
//can1_init();




//wifi_connect();
//esp_read();
i2c_write(0xd0,0x02,0x72);        //hr
i2c_write(0xd0,0x01,0x58);        //min
i2c_write(0xd0,0x00,0x55);        //sec

i2c_write(0xd0,0x04,0x12);
i2c_write(0xd0,0x05,0x08);
i2c_write(0xd0,0x06,0x26);




while(1)
{
uart0_tx_string("======================================\r\n");
uart0_tx_string("SMART WATER TANK MANAGEMENT\r\n");
uart0_tx_string("======================================\r\n");
uart0_tx_string("Controller  :LPC2129 ARM7\r\n");
uart0_tx_string("STATUS  :RUNNING\r\n");

if(flag==1)
{
 uart0_tx_string("MANUAL MODE\r\n");
}
else
{
uart0_tx_string("AUTO MODE\r\n");
}



adc_value=adc_read(0);
water_level=(adc_value*100)/1023;
uart0_tx_string("Water level: ");
uart0_float(water_level);
uart0_tx_string("%\r\n");

distance=ultrasonic_read();
uart0_tx_string("Distance= ");
uart0_integer(distance);
uart0_tx_string("cm\r\n");




adcout=adc_read(2);
vout=(adcout*3.3)/1023;
temp=vout*100;
uart0_tx_string("Temp= ");
uart0_float(temp);
uart0_tx_string(" c\r\n");
lcd_cmd(0x80);
lcd_float(temp);


switch(flag)
{
case 0:if(water_level<30 && distance<2)
                {
                uart0_tx_string("Pump Status:ON\r\n");
                uart0_tx_string("Relay Status:ON\r\n");
                uart0_tx_string("valve Status:ON\r\n");
                servo_90deg();
                relay_on();
                IOCLR0=1<<14;
                }
                else
                {
                uart0_tx_string("Pump Status:OFF\r\n");
                uart0_tx_string("Relay Status:OFF\r\n");
                uart0_tx_string("valve Status:OFF\r\n");
                servo_0deg();
                IOSET0=1<<14;
                relay_off();
                }
                break;

case 1:if(water_level>45 && distance<3)
                {
                uart0_tx_string("Pump Status:OFF\r\n");
                uart0_tx_string("Relay Status:OFF\r\n");
                uart0_tx_string("valve Status:OFF\r\n");
                servo_0deg();
                relay_off();
                IOSET0=1<<14;
                }
                else {
                uart0_tx_string("Pump Status:ON\r\n");
                uart0_tx_string("Relay Status:ON\r\n");
                uart0_tx_string("valve Status:ON\r\n");
                servo_90deg();
                relay_on();
                IOCLR0=1<<14;
                }
                break;

}
uart0_tx_string("RTC date :");
d=i2c_read(0xd1,0x04);
mo=i2c_read(0xd1,0x05);
y=i2c_read(0xd1,0x06);


uart0_tx(d/16+48);
uart0_tx(d%16+48);
uart0_tx(':');
uart0_tx(mo/16+48);
uart0_tx(mo%16+48);
uart0_tx(':');
uart0_tx_string("20");
uart0_tx(y/16+48);
uart0_tx(y%16+48);
uart0_tx_string("\r\n");

uart0_tx_string("RTC Time :");

h=i2c_read(0xd1,0x02);
m=i2c_read(0xd1,0x01);
s=i2c_read(0xd1,0x00);
h1=h&0x0f;
uart0_tx(h1/16+48);
uart0_tx(h1%16+48);
uart0_tx(':');
uart0_tx(m/16+48);
uart0_tx(m%16+48);
uart0_tx(':');
uart0_tx(s/16+48);
uart0_tx(s%16+48);


if(h>>5&1)
{
uart0_tx_string(" PM");

}
else
{
uart0_tx_string(" AM");
}
uart0_tx_string("\r\nEEPROM :");
eeprom(temp);


uart0_tx_string("GET /update?api_key=P3NMKR3M8JKHDZFJ&field1=Water_level&field2=distance&field3=Temp&field4=1 HTTP/1.1\r\nHost:api.thingspeak.com\r\nConnection:close\r\r\r\n") ;
//uart0_tx_string(buf);
//uart0_tx_string("\r\n");

delay_sec(10);




//can1_send_parameter(temp,water_level,distance);
//delay_sec(1);
}




}

