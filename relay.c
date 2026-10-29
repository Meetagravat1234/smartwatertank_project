 #include<lpc21xx.h>
   #include "header.h"
   #define RELAY (1<<17)
   void relay_init(void)
   {
   IODIR0|=RELAY;

   }
void relay_on(void)
{
IOCLR0=RELAY;
}
void relay_off(void)
{
IOSET0=RELAY;
}
