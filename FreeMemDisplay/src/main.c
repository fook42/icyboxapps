#define __NOLIBBASE__

#include <proto/exec.h>
#include <proto/dos.h>
#include <proto/i2c.h>

#include "i2cdisplay.h"

int __nocommandline=1;
int __initlibraries=0;

STATIC CONST ULONG StuffChar[] = { 0x16c04e75 };

struct ExecBase     *SysBase  = NULL;
struct DosLibrary   *DOSBase  = NULL;
struct Library      *I2C_Base = NULL;

int mymain(void)
{
    int     rc = 0;
    ULONG   freemem = 0;
    char    text[20];
    ULONG   array[2];
    //--------------------------------------------------
    SysBase  = *((struct ExecBase **)4);
    DOSBase  = (struct DosLibrary *) OpenLibrary("dos.library",0);
    I2C_Base = (struct Library *)    OpenLibrary("i2c.library",40);

    if (I2C_Base)
    {
        i2cdisplay_setup_interface ( I2C_1602DISPLAY_ADDRESS, 2,1,0,3,4,5,6,7);
        i2cdisplay_init();

        while (!(SetSignal(0L, SIGBREAKF_CTRL_C) & SIGBREAKF_CTRL_C))
        {
            freemem = AvailMem(MEMF_CHIP);
            array[0] = (ULONG) freemem;
            /* create output-string */
            (void) RawDoFmt("Chip: %10ld", &array, (void (*)(void))&StuffChar, text );
            i2cdisplay_gotoxy( 0,0 );
            i2cdisplay_print(text);

            freemem = AvailMem(MEMF_FAST);
            array[0] = (ULONG) freemem;
            /* create output-string */
            (void) RawDoFmt("Fast: %10ld", &array, (void (*)(void))&StuffChar, text );
            i2cdisplay_gotoxy( 0,1 );
            i2cdisplay_print(text);

            Delay(50*2);
        }

        i2cdisplay_off();

    } else {
        Printf("failed to open libraries!!!\n");
        rc = RETURN_FAIL;
    }

    if (I2C_Base) { CloseLibrary(I2C_Base); }
    if (DOSBase)  { CloseLibrary((struct Library *) DOSBase); }

    return rc;
}
