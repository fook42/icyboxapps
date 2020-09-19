// *******************************
// * i2cdisplay.c
// * - routines to access HD44780 display via I2C
// *
// *******************************

#include "i2cdisplay.h"
#include <exec/types.h>
#include <proto/dos.h>
#include <proto/i2c.h>

/***************************************************************/
/* variables to store pin-assignments:
 * - default-values according to "LCM1602 IIC V1" - PCB
*/
UBYTE _DisplayAdr = (I2C_1602DISPLAY_ADDRESS < 1);     // I2C Address of the IO expander (PCF8574)
UBYTE _Bl         = 0x08;     // LCD expander word for backlight pin
UBYTE _En         = 0x04;     // LCD expander word for enable pin
UBYTE _Rw         = 0x02;     // LCD expander word for R/W pin
UBYTE _Rs         = 0x01;     // LCD expander word for Register Select pin
UBYTE _data[4]    = {0x10,0x20,0x40,0x80};    // LCD data lines

//-------
UBYTE _Backlight  = 0;   // Backlight is turned off by default

/***************************
 * i2cdisplay_setup_interface
 * - to assign all used display-pins to output of PCF8574-chip
 */
void i2cdisplay_setup_interface(UBYTE displayI2CAdr, UBYTE En, UBYTE Rw, UBYTE Rs, UBYTE Bl,
                                UBYTE d4, UBYTE d5, UBYTE d6, UBYTE d7 )
{
    // store I2C-address shifted by 1
    _DisplayAdr = displayI2CAdr << 1;
    _En         = ( 1 << En );
    _Rw         = ( 1 << Rw );
    _Rs         = ( 1 << Rs );
    _Bl         = ( 1 << Bl );
    _data[0]    = ( 1 << d4 );
    _data[1]    = ( 1 << d5 );
    _data[2]    = ( 1 << d6 );
    _data[3]    = ( 1 << d7 );
    _Backlight  = 0;
}

/***************************
 * i2cdisplay_send_2_chip
 * - send the data via I2C to the PCF8574 to be passed
 *   to the display and use the right pin-assignments
 */
void i2cdisplay_send_2_chip(UBYTE senddata, UBYTE mode)
{
    UBYTE pinMapValue = 0;
    UBYTE i;

    for ( i = 0; i < 4; ++i )
    {
        if ( 0 != ( senddata & 0x01 ) )
        {
            pinMapValue |= _data[i];
        }
        senddata = ( senddata >> 1 );
    }

    // Is it a command or data
    if ( MODE_DATA == mode )
    {
        pinMapValue |= _Rs;
    }
    pinMapValue |= _Backlight;
    pinMapValue |= _En ;
    (void) SendI2C( _DisplayAdr, 1, &pinMapValue);
    pinMapValue &= (~_En) ;
    (void) SendI2C( _DisplayAdr, 1, &pinMapValue);
}

/***************************
 * i2cdisplay_send
 * - this will send commands directly and split data in 2*4bit transmissions
 */
void i2cdisplay_send(UBYTE senddata, UBYTE mode)
{
    if ( MODE_4BIT == mode )
    {
        i2cdisplay_send_2_chip((senddata & 0x0F), MODE_CMD );
    } else {
        i2cdisplay_send_2_chip(((senddata >> 4) & 0x0F), mode);
        i2cdisplay_send_2_chip( (senddata       & 0x0F), mode);
    }
}

/***************************
 * i2cdisplay_print
 * - rudimentary routine to send a char-array bytewise to the display
 */
void i2cdisplay_print(char* chardata)
{
    while (0 != (*chardata))
    {
        i2cdisplay_send(*chardata, MODE_DATA);
        ++chardata;
    }
}

/***************************
 * i2cdisplay_gotoxy
 * - change the display-address pointer to new position
 */
void i2cdisplay_gotoxy(UBYTE xpos, UBYTE ypos)
{
    UBYTE yposarray[] = { 0, 0x40, 0x10, 0x50 }; // 1604 - displays
    i2cdisplay_send ( CMD_SETDDRADDR | xpos | yposarray[ypos], MODE_CMD );
}

/***************************
 * i2cdisplay_clear
 * - clear the screen and reset the current cursor position
 */
void i2cdisplay_clear()
{
    i2cdisplay_send ( CMD_CLEARDISPLAY , MODE_CMD );
    i2cdisplay_send ( CMD_RETURNHOME , MODE_CMD );
}

/***************************
 * i2cdisplay_off
 * - turn off the backlight
 * - clear the screen
 * - power off the display-controller
 */
void i2cdisplay_off()
{
    _Backlight=0;
    i2cdisplay_send ( CMD_CLEARDISPLAY , MODE_CMD );
    i2cdisplay_send ( CMD_DISPLAYCTL | LCD_DISPLAYOFF , MODE_CMD );
}

/***************************
 * i2cdisplay_init
 * - initialize a display behind the PCF8574
 * - use "2LINE"-LCDs and write from left to right
 * - clear the screen and turn on the backlight
 */
void i2cdisplay_init()
{
    i2cdisplay_send ( PCF8574_4BitMODE, MODE_CMD );
    Delay(1);
    i2cdisplay_send ( PCF8574_4BitMODE, MODE_CMD );
    Delay(1);
    i2cdisplay_send ( PCF8574_4BitMODE, MODE_CMD );
    Delay(1);
    i2cdisplay_send ( PCF8574_4BitInterface, MODE_CMD );
    Delay(1);

    i2cdisplay_send ( CMD_FUNCTIONSET | LCD_4BITMODE | LCD_2LINE | LCD_5x8DOTS, MODE_CMD );
    Delay(1);
    i2cdisplay_send ( CMD_FUNCTIONSET | LCD_4BITMODE | LCD_2LINE | LCD_5x8DOTS, MODE_CMD );
    Delay(1);
    i2cdisplay_send ( CMD_FUNCTIONSET | LCD_4BITMODE | LCD_2LINE | LCD_5x8DOTS, MODE_CMD );
    Delay(1);

    i2cdisplay_send ( CMD_DISPLAYCTL | LCD_DISPLAYON , MODE_CMD );
    i2cdisplay_send ( CMD_CLEARDISPLAY , MODE_CMD );
    i2cdisplay_send ( CMD_ENTRYMODE | LCD_ENTRYLEFT , MODE_CMD );

    _Backlight = _Bl;
    i2cdisplay_send ( CMD_RETURNHOME , MODE_CMD );
    Delay(1);

    i2cdisplay_gotoxy( 0, 0 );
}
