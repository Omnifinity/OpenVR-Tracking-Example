//
// HTC Vive Lighthouse Tracking Example
// By Peter Thor 2016, 2017, 2018, 2019
//

#include "stdafx.h"
#include "Windows.h"
#include <string>
#include <conio.h>
#include <iomanip>		// for std::setprecision
#include <iostream>

#include "LighthouseTracking.h"

// windows keyboard input
#include <conio.h>
#include <vector>

using std::vector;
using std::string;

#define SOFTWARE_NAME "vive_debugger"
#define SOFTWARE_VERSION "1.4"
#define SOFTWARE_CURRENT_YEAR "2019"
#define VER_130 "Using openvr 1.3.22"
#define VER_120 "Using openvr 1.2.10"
#define VER_110 "Using openvr 1.1.3"
#define VER_CURRENT_STRING VER_130

void printSoftwareVersion() {
	char buf[1024];
	sprintf_s(buf, sizeof(buf), "\n%s v%s by Omnifinity in %s\n%s\n\n", SOFTWARE_NAME, SOFTWARE_VERSION, SOFTWARE_CURRENT_YEAR, VER_CURRENT_STRING);
	printf_s(buf);
}

int _tmain(int argc, _TCHAR* argv[])
{
	bool parseTrackingData = true;
	int shouldListDevicesAndQuit = 0;
	int shouldPrintVersionAndQuit = 0;
	int showOnlyDevice = -1;
	int calcControllerDeltaPos = 0;

	char buf[1024];

	// very basic command line parser, from:
	// http://stackoverflow.com/questions/17144037/change-a-command-line-argument-argv
	vector<string> validArgs;
	validArgs.reserve(argc); //Avoids reallocation; it's one or two (if --item is given) too much, but safe and not pedentatic while handling rare cases where argc can be zero

	// parse all user input
	for (int i = 1; i<argc; ++i) {
		const std::string myArg(argv[i]);

		if (myArg == std::string("--listdevices")) shouldListDevicesAndQuit = atoi(argv[i + 1]);
		if (myArg == std::string("--showonlydeviceid")) showOnlyDevice = atoi(argv[i + 1]);
		if (myArg == std::string("--calccontrollerdeltapos")) calcControllerDeltaPos = atoi(argv[i + 1]);
		if (myArg == std::string("--v")) shouldPrintVersionAndQuit = 1;

		validArgs.push_back(myArg);
	}

	if (shouldPrintVersionAndQuit) {
		printSoftwareVersion();
		return EXIT_SUCCESS;
	}
	printSoftwareVersion();

	if (showOnlyDevice == -1)
		sprintf_s(buf, sizeof(buf), "Showing data for all devices\n");
	else
		sprintf_s(buf, sizeof(buf), "Showing data for chosen device: %d\n", showOnlyDevice);
	printf_s(buf);

	// If false we'll parse tracking data continously, if true we parse when an openvr event fires
	bool bAcquireTrackingDataByWaitingForVREvents = false;

	// Create a new LighthouseTracking instance and parse as needed
	LighthouseTracking *lighthouseTracking = new LighthouseTracking();
	if (lighthouseTracking) {
		if (shouldListDevicesAndQuit) {
			parseTrackingData = false;
		}

		lighthouseTracking->PrintDevices();

		if (parseTrackingData) {
			sprintf_s(buf, sizeof(buf), "Press 'q' to quit. Starting capture of tracking data...\n");
			printf_s(buf);
			Sleep(2000);

			while (lighthouseTracking->RunProcedure(bAcquireTrackingDataByWaitingForVREvents, showOnlyDevice)) {

				// Windows quit routine - adapt as you need
				if (_kbhit()) {
					char ch = _getch();
					if ('q' == ch) {

						char buf[1024];
						sprintf_s(buf, sizeof(buf), "User pressed 'q' - exiting...");
						printf_s(buf);

						break;
					}
					else if ('l' == ch) {
						lighthouseTracking->PrintDevices();
					}
				}

				// calculate the difference
				if (calcControllerDeltaPos) {
					vr::HmdVector3_t vecDiff = lighthouseTracking->GetControllerPositionDelta();
					vr::HmdVector3_t vecLeft = lighthouseTracking->GetLeftControllerPosition();
					vr::HmdVector3_t vecRight = lighthouseTracking->GetRightControllerPosition();
					sprintf_s(buf, sizeof(buf), "(lx, ly, lz) = (%.3f, %.3f, %.3f) | (rx, ry, rz) = (%.3f, %.3f, %.3f) | (dX, dY, dZ) = (%.3f, %.3f, %.3f)\n", vecLeft.v[0], vecLeft.v[1], vecLeft.v[2], vecRight.v[0], vecRight.v[1], vecRight.v[2], vecDiff.v[0], vecDiff.v[1], vecDiff.v[2]);
					//sprintf_s(buf, sizeof(buf), "(lx, ly, lz) = (%.4f, %.4f, %.4f) | (rx, ry, rz) = (%.4f, %.4f, %.4f) | (dX, dY, dZ) = (%.4f, %.4f, %.4f)\n", vecLeft.v[0], vecLeft.v[1], vecLeft.v[2], vecRight.v[0], vecRight.v[1], vecRight.v[2], vecDiff.v[0], vecDiff.v[1], vecDiff.v[2]);
					printf_s(buf);
				}

				// a delay to not overheat your computer... :)
				Sleep(2);
			}
		}

		delete lighthouseTracking;
	}
	return EXIT_SUCCESS;
}

