#define __NOLIBBASE__

#include <proto/exec.h>
#include <proto/dos.h>
#include <proto/i2c.h>
#include <proto/mathieeesingbas.h>

#include "i2cdisplay.h"

int __nocommandline=1;
int __initlibraries=0;

#define I2C_LTC_ADDRESS (0x98)

static const ULONG StuffChar = 0x16c04e75;

struct ExecBase     *SysBase = NULL;
struct DosLibrary   *DOSBase = NULL;
struct Library      *I2C_Base = NULL;
struct MathIEEEBase *MathIeeeSingBasBase = NULL;

int mymain(void)
{
    int     rc = 0;
    UBYTE   i;
    char    text[20];

    ULONG Voltage;
    UBYTE LTC_Mode[]    = { 0x01, 0x58 };
    UBYTE LTC_Trigger[] = { 0x02, 0x01 };
    UBYTE LTC_Data[17];//  = { "\0" };
    ULONG array[5];
    //--------------------------------------------------
    SysBase               = (*((struct ExecBase **)4));
    DOSBase               = (struct DosLibrary *)   OpenLibrary("dos.library",0);
    I2C_Base              = (struct Library *)      OpenLibrary("i2c.library",40);
    MathIeeeSingBasBase   = (struct MathIEEEBase *) OpenLibrary("mathieeesingbas.library", 0);

    if ((DOSBase) && (I2C_Base) && (MathIeeeSingBasBase))
    {
        (void) SendI2C   (I2C_LTC_ADDRESS,  2, LTC_Mode);
        (void) ReceiveI2C(I2C_LTC_ADDRESS, 16, LTC_Data);
        (void) SendI2C   (I2C_LTC_ADDRESS,  2, LTC_Trigger);

        i2cdisplay_setup_interface (I2C_1602DISPLAY_ADDRESS, 2,1,0,3,4,5,6,7);
        i2cdisplay_init();

        for (i=0; i<16; ++i)
        {
            LTC_Data[i]=0;
        }
        for (i=0; i<4; ++i)
        {
            array[i]=0;
        }

        while (!(SetSignal(0L, SIGBREAKF_CTRL_C) & SIGBREAKF_CTRL_C))
        {
            /* read LTC-Temp-Data */
            (void) SendI2C   (I2C_LTC_ADDRESS,  2, LTC_Mode);
            (void) ReceiveI2C(I2C_LTC_ADDRESS, 16, LTC_Data);
            (void) SendI2C   (I2C_LTC_ADDRESS,  2, LTC_Trigger);

            array[0] = (ULONG) ((LTC_Data[8] & 0x7F) << 4 ) | ((LTC_Data[9] & 0xF0) >> 4);
            array[1] = (ULONG) ((LTC_Data[9] & 0x0F) * 100) >> 4;
            array[2] = (ULONG) "\xDF";
            array[3] = 0;
            /* create output-string */
            (void) RawDoFmt("Zorro :  %2ld.%02ld%sC", &array, (void (*)(void))&StuffChar, text );
            i2cdisplay_gotoxy( 0,0 );
            i2cdisplay_print(text);

            //V = D[14:0] • 305.18μV
            //V1 = 1/2 * real  -> V1 = D[14:0] *  610µV = x * 0,61 mV
            // -> x *  0.61 / 1000
            //V2 = 1/4 * real  -> V2 = D[14:0] * 1220µV = x * 1,22 mV
            // -> x *  1.22 / 1000

            Voltage = (ULONG) (((LTC_Data[4] & 0x7F) << 8) + LTC_Data[5]);
            Voltage = (ULONG) ( ((float) Voltage) * 0.061f);
            array[0] = (ULONG) (Voltage / 100);
            array[1] = (ULONG) (Voltage % 100);

            Voltage = (ULONG) (((LTC_Data[6] & 0x7F) << 8) + LTC_Data[7]);
            Voltage = (ULONG) ( ((float) Voltage) * 0.122f);
            array[2] = (ULONG) (Voltage / 100);
            array[3] = (ULONG) (Voltage % 100);
            array[4] = 0;
            (void) RawDoFmt("%2ld.%02ld V  %2ld.%02ld V", &array, (void (*)(void))&StuffChar, text );
            i2cdisplay_gotoxy( 0,1 );
            i2cdisplay_print(text);

            // wait / sleep for 5 seconds */
            Delay(50*2);
        }

        i2cdisplay_off();

    } else {
        if (DOSBase)
        {
            Printf("failed to open libraries!!!\n");
        }
        rc = RETURN_FAIL;
    }

    if (MathIeeeSingBasBase)   { CloseLibrary((struct Library *) MathIeeeSingBasBase); }
    if (I2C_Base)              { CloseLibrary(I2C_Base); }
    if (DOSBase)               { CloseLibrary((struct Library *) DOSBase); }

    return rc;
}
