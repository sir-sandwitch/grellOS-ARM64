#ifndef io_h
#include "io.h"
#define io_h
#endif
#ifndef fb_h
#include "fb.h"
#define fb_h
#endif
#ifndef uspios_h
#include "uspi/include/uspios.h"
#define uspios_h
#endif
#ifndef uspi_h
#include "uspi/include/uspi.h"
#define uspi_h
#endif
#ifndef osfunc_h
#include "osfunc.h"
#define osfunc_h
#endif
#ifndef kernel_h
#include "kernel.h"
#define kernel_h
#endif

const unsigned int timer1_int = CLOCKHZ;
const unsigned int timer3_int = CLOCKHZ / 4;
unsigned int timer1_val = 0;
unsigned int timer3_val = 0;

// void main()
// {
//     uart_init();
//     fb_init();

//     // drawRect(150,150,400,400,0x03,0);
//     // drawRect(300,300,350,350,0x2e,1);

//     // drawCircle(960,540,250,0x0e,0);
//     // drawCircle(960,540,50,0x13,1);

//     // drawPixel(250,250,0x0e);

//     // drawChar('O',500,500,0x05, 3);
//     // drawString(100,100,"Hello world!",0x0f, 4);

//     // drawLine(100,500,350,700,0x0c);



//     while (1);
// }

static const char FromSample[] = "sample";

static void KeyPressedHandler (const char *pString)
{
	//ScreenDeviceWrite (USPiEnvGetScreen (), pString, strlen (pString));
    drawString(100,100,pString,0x0f, 4);
}

//count on timer 1 (1MHz)
void handle_timer_1(){
    timer1_val += timer1_int;
    REGS_TIMER->compare[1] = timer1_val;
    REGS_TIMER->control_status |= SYS_TIMER_IRQ_1;
}

//count on timer 3 (250KHz)
void handle_timer_3(){
    timer3_val += timer3_int;
    REGS_TIMER->compare[3] = timer3_val;
    REGS_TIMER->control_status |= SYS_TIMER_IRQ_3;
}

int main (void)
{
    MemoryPoolInit(67108864, 4);

    //timer_init();

	// if (!USPiEnvInitialize ())
	// {
	// 	return 1;
	// }
	
	if (!USPiInitialize ())
	{
		LogWrite (FromSample, LOG_ERROR, "Cannot initialize USPi");

		//USPiEnvClose ();

		return 1;
	}
	
	if (!USPiKeyboardAvailable ())
	{
		LogWrite (FromSample, LOG_ERROR, "Keyboard not found");

		//USPiEnvClose ();

		return 1;
	}

	USPiKeyboardRegisterKeyPressedHandler (KeyPressedHandler);

	LogWrite (FromSample, LOG_NOTICE, "Just type something!");

	// just wait and turn the rotor
	for (unsigned nCount = 0; 1; nCount++)
	{
		USPiKeyboardUpdateLEDs ();

		//ScreenDeviceRotor (USPiEnvGetScreen (), 0, nCount);
        drawCircle(960,540,250,0x0e,0);
	}

	return 1;
}