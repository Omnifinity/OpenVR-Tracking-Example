//
// HTC Vive Lighthouse Tracking Example
// By Peter Thor 2016
//

#include "stdafx.h"
#include "Windows.h"

#include "LighthouseTracking.h"

// windows keyboard input
#include <conio.h>

int _tmain(int argc, _TCHAR* argv[])
{
	// If false we'll parse tracking data continously, if true we parse when an openvr event fires
	bool bAcquireTrackingDataByWaitingForVREvents = false;

	// Create a new LighthouseTracking instance and parse as needed
	LighthouseTracking *lighthouseTracking = new LighthouseTracking();
	if (lighthouseTracking) {
		char buf[1024];
		sprintf_s(buf, sizeof(buf), "Press 'q' to quit. Starting capture of tracking data...\n");
		printf_s(buf);
		Sleep(2000);

		while (lighthouseTracking->RunProcedure(bAcquireTrackingDataByWaitingForVREvents)) {

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

			// a delay to not overheat your computer... :)
			Sleep(1);
		}

		delete lighthouseTracking;
	}
	return EXIT_SUCCESS;
}

