//
// HTC Lighthouse Tracking Example.cpp
// 
// By Peter Thor 2016
//
//

#include "stdafx.h"

#include "LighthouseTracking.h"

// windows keyboard input
#include <conio.h>

int _tmain(int argc, _TCHAR* argv[])
{

	LighthouseTracking *lighthouseTracking = new LighthouseTracking();
	if (lighthouseTracking) {


		char buf[1024];
		sprintf_s(buf, sizeof(buf), "Press 'q' to quit\n");
		printf_s(buf);

		while (lighthouseTracking->RunProcedure()) {

			// Windows quit routine - adapt as you need
			if (_kbhit()) {
				char ch = _getch();
				if ('q' == ch) {

					char buf[1024];
					sprintf_s(buf, sizeof(buf), "User pressed 'q' - exiting...");
					printf_s(buf);

					break;
				}
			}
		}

		delete lighthouseTracking;
	}
	return 0;
}

