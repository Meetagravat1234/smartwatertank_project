 typedef unsigned int u32;
 typedef unsigned char u8;
 typedef signed char s8;
 typedef signed int s32;
extern void delay_ms(unsigned int ms);
extern void delay_sec(unsigned int sec);
extern void delay_uc(unsigned int uc);
extern void lcd_data(unsigned char data);
extern void lcd_cmd(unsigned int cmd);
extern void lcd_integer(int num);
extern void lcd_float(float f);
extern void lcd_string(char *ptr);
void lcd_init(void);
extern void uart0_init(unsigned int baud);
extern void uart0_tx(unsigned char data);
extern unsigned char uart0_rx(void);
extern void uart0_tx_string(char *ptr);
extern void uart0_binary(int num);
extern void uart0_integer(int num);
extern void uart0_float(float f);
extern void adc_init(void);
extern unsigned int adc_read(unsigned char );

extern void ultrasonic_init(void);
extern u32 ultrasonic_read(void);

extern void relay_init(void);
extern void relay_on(void);

extern void relay_off(void);

extern void servo_init(void);
extern void servo_0deg(void);
extern void servo_90deg(void);
extern void servo_180deg(void);



extern void wifi_connect(void);
extern void esp_read(void);

extern void i2c_init(void);
extern void i2c_write(u8 , u8 , u8 );
extern u8 i2c_read(u8 ,u8 );

extern void eeprom(u8 ,u8,u8 );

extern void config_vic_for_exint0(void);

typedef struct CAN1_MSG{
        u32 id;
        u32 byteA;
        u32 byteB;
        u8 rtr;
        u8 dlc;
        u8 ff;

}CAN1;
extern void can1_tx(CAN1 v);

extern void can1_init(void);

extern void can1_send_parameter(u8 t,u8 w,u8 d);




extern void uart0_hex(s32 );
extern void can1_rx(CAN1 *ptr);


