/*

  Advanced Sound Library (ASlib)
  ------------------------------

  file        : main.c 
  author      : Lasorsa Yohan (Noda)
  description : ARM7 main program

  history : 

    28/11/2007 - v1.0
      = Original release
      
    21/12/2007 - v1.1
      = corrected arm7/amr9 initialization (fix M3S/R4 problems)
      = fixed stereo detection problem (thanks to ThomasS)
      
*/

#include <nds.h>
#include <stdlib.h>

#include "as_lib7.h"


int vcount;
touchPosition first, tempPos;

// read stylus position (code from libnds template)
void VcountHandler() 
{
    static int lastbut = -1;
    
    uint16 but=0, x=0, y=0, xpx=0, ypx=0, z1=0, z2=0;

    but = REG_KEYXY;

    if (!( (but ^ lastbut) & (1 << 6))) {
 
        tempPos = touchReadXY();

        if ( tempPos.x == 0 || tempPos.y == 0 ) {
            but |= (1 << 6);
            lastbut = but;
        } else {
            x = tempPos.x;
            y = tempPos.y;
            xpx = tempPos.px;
            ypx = tempPos.py;
            z1 = tempPos.z1;
            z2 = tempPos.z2;
        }
        
    } else {
        lastbut = but;
        but |= (1 << 6);
    }

    if ( vcount == 80 ) {
        first = tempPos;
    } else {
        if ( abs( xpx - first.px) > 10 || abs( ypx - first.py) > 10 || (but & (1 << 6)) ) {
            but |= (1 << 6);
            lastbut = but;
        } else {     
            IPC->mailBusy = 1;
            IPC->touchX   = x;
            IPC->touchY   = y;
            IPC->touchXpx = xpx;
            IPC->touchYpx = ypx;
            IPC->touchZ1  = z1;
            IPC->touchZ2  = z2;
            IPC->mailBusy = 0;
        }
    }
    IPC->buttons = but;
    vcount ^= (80 ^ 130);
    SetYtrigger(vcount);
}

int main(int argc, char **argv) {

    // reset the clock if needed
    rtcReset();

    // enable sound
    powerON(POWER_SOUND);
    SOUND_CR = SOUND_ENABLE | SOUND_VOL(127);

    // init LED status to always on, as some flashcards make it blink
    writePowerManagement(PM_CONTROL_REG, ~PM_LED_BLINK & readPowerManagement(PM_CONTROL_REG));

    // initialize ASlib
    AS_Init();

    // set interrupts
    irqInit();
    SetYtrigger(80);
    vcount = 80;
    irqSet(IRQ_VCOUNT, VcountHandler);
    irqSet(IRQ_VBLANK, AS_SoundVBL);    // the sound engine
    irqEnable(IRQ_VBLANK | IRQ_VCOUNT);

    // main loop
    while (1) {
        AS_MP3Engine();     // the mp3 engine
        // don't wait for for VBlank if your plan to use high bitrates
        // or real-time pitching with faster rates than original.
        // note that it causes the arm9 to wait more because of ram locking,
        // ending up to ~5-7% arm9 cpu usage increase.
        // uncomment it to use nearly no arm9 cpu usage, but this may cause
        // freeze/glitches with high birates/faster pitched mp3s.
//        swiWaitForVBlank();
    }
    
    return 0;
}

