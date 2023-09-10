#include "io.h"
#include "fb.h"
#include "uspi/include/uspios.h"
#include "uspi/include/uspi.h"

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
	ScreenDeviceWrite (USPiEnvGetScreen (), pString, strlen (pString));
}

int main (void)
{
	if (!USPiEnvInitialize ())
	{
		return 1;
	}
	
	if (!USPiInitialize ())
	{
		LogWrite (FromSample, LOG_ERROR, "Cannot initialize USPi");

		USPiEnvClose ();

		return 1;
	}
	
	if (!USPiKeyboardAvailable ())
	{
		LogWrite (FromSample, LOG_ERROR, "Keyboard not found");

		USPiEnvClose ();

		return 1;
	}

	USPiKeyboardRegisterKeyPressedHandler (KeyPressedHandler);

	LogWrite (FromSample, LOG_NOTICE, "Just type something!");

	// just wait and turn the rotor
	for (unsigned nCount = 0; 1; nCount++)
	{
		USPiKeyboardUpdateLEDs ();

		ScreenDeviceRotor (USPiEnvGetScreen (), 0, nCount);
	}

	return 1;
}