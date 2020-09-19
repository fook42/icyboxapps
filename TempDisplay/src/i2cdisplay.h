// *******************************
// * i2cdisplay.h
// * - headerfiles for i2cdisplay.c
// *
// *******************************

#ifndef _I2CDISPLAY_H_
#define _I2CDISPLAY_H_
#include <exec/types.h>

#define I2C_1602DISPLAY_ADDRESS (0x27)

/* HD44780 commands */
#define CMD_CLEARDISPLAY        0x01
#define CMD_RETURNHOME          0x02
#define CMD_ENTRYMODE           0x04
#define CMD_DISPLAYCTL          0x08
#define CMD_DISPLAYSHIFT        0x10
#define CMD_FUNCTIONSET         0x20
#define CMD_SETCGRADDR          0x40
#define CMD_SETDDRADDR          0x80

#define LCD_8BITMODE            0x10
#define LCD_4BITMODE            0x00
#define LCD_2LINE               0x08
#define LCD_1LINE               0x00
#define LCD_5x10DOTS            0x04
#define LCD_5x8DOTS             0x00

#define LCD_ENTRYRIGHT          0x00
#define LCD_ENTRYLEFT           0x02
#define LCD_ENTRYSHIFTINCREMENT 0x01
#define LCD_ENTRYSHIFTDECREMENT 0x00

#define LCD_DISPLAYON           0x04
#define LCD_DISPLAYOFF          0x00
#define LCD_CURSORON            0x02
#define LCD_CURSOROFF           0x00
#define LCD_BLINKON             0x01
#define LCD_BLINKOFF            0x00

#define LCD_DISPLAYMOVE         0x08
#define LCD_CURSORMOVE          0x00
#define LCD_MOVERIGHT           0x04
#define LCD_MOVELEFT            0x00

#define PCF8574_4BitMODE        0x03
#define PCF8574_4BitInterface   0x02


#define MODE_CMD            0x00
#define MODE_DATA           0x01
#define MODE_4BIT           0x02

void i2cdisplay_setup_interface(UBYTE displayI2CAdr, UBYTE En, UBYTE Rw, UBYTE Rs,
                            UBYTE d4, UBYTE d5, UBYTE d6, UBYTE d7, UBYTE Bl );

void i2cdisplay_send_2_chip(UBYTE senddata, UBYTE mode);

void i2cdisplay_send(UBYTE senddata, UBYTE mode);

void i2cdisplay_print(char* chardata);

void i2cdisplay_gotoxy(UBYTE xpos, UBYTE ypos);

void i2cdisplay_clear();

void i2cdisplay_off();

void i2cdisplay_init();


#endif
