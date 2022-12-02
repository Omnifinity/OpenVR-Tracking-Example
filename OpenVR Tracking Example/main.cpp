/*
OpenVR-Tracking-Example Copyright 2016-2022 Omnifinity
Example on how to access tracking data from OpenVR

OpenVR Copyright (c) 2015, Valve Corporation

All rights reserved.

Redistribution and use in source and binary forms, with or without modification,
are permitted provided that the following conditions are met :

1. Redistributions of source code must retain the above copyright notice, this
list of conditions and the following disclaimer.

2. Redistributions in binary form must reproduce the above copyright notice,
this list of conditions and the following disclaimer in the documentation and / or
other materials provided with the distribution.

3. Neither the name of the copyright holder nor the names of its contributors
may be used to endorse or promote products derived from this software without
specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
DISCLAIMED.IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR
ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
(INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON
ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
(INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/

#include "pch.h" // use stdafx.h in Visual Studio 2017 and earlier
#include "Windows.h"
#include "tchar.h"
#include <string>
#include <conio.h>
#include <iomanip>		// for std::setprecision
#include <iostream>

#include "OpenVRTrackingExample.h"

// windows keyboard input
#include <conio.h>
#include <vector>

using std::vector;
using std::string;

#define SOFTWARE_NAME "openvr_tracking_example"
#define SOFTWARE_VERSION "1.8"
#define SOFTWARE_CURRENT_YEAR "2022"
#define VER_180 "Built for MSVC2022. Updates to project settings. Using openvr 1.23.7"
#define VER_170 "Built for MSVC2019. Renamed Executable. Renamed class."
#define VER_160 "Using openvr 1.14.15"
#define VER_150 "Using openvr 1.9.16"
#define VER_140 "Internal use"
#define VER_130 "Using openvr 1.3.22"
#define VER_120 "Using openvr 1.2.10"
#define VER_110 "Using openvr 1.1.3"
#define VER_CURRENT_STRING VER_160

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
	int showEventsOnly = 0;

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
		if (myArg == std::string("--showeventsonly")) showEventsOnly = atoi(argv[i + 1]);
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
	bool bAcquireTrackingDataByWaitingForVREvents;
	if (showEventsOnly == 1)
		bAcquireTrackingDataByWaitingForVREvents = true;
	else
		bAcquireTrackingDataByWaitingForVREvents = false;

	// Create a new LighthouseTracking instance and parse as needed
	OpenVRTrackingExample *trackingHandler = new OpenVRTrackingExample();
	if (trackingHandler) {
		if (shouldListDevicesAndQuit) {
			parseTrackingData = false;
		}

		trackingHandler->PrintDevices();

		if (parseTrackingData) {
			sprintf_s(buf, sizeof(buf), "Press 'q' to quit. Starting capture of tracking data...\n");
			printf_s(buf);
			Sleep(2000);

			while (trackingHandler->RunProcedure(bAcquireTrackingDataByWaitingForVREvents, showOnlyDevice)) {

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
						trackingHandler->PrintDevices();
					}
				}

				// calculate the difference
				if (calcControllerDeltaPos) {
					vr::HmdVector3_t vecDiff = trackingHandler->GetControllerPositionDelta();
					vr::HmdVector3_t vecLeft = trackingHandler->GetLeftControllerPosition();
					vr::HmdVector3_t vecRight = trackingHandler->GetRightControllerPosition();
					sprintf_s(buf, sizeof(buf), "(lx, ly, lz) = (%.3f, %.3f, %.3f) | (rx, ry, rz) = (%.3f, %.3f, %.3f) | (dX, dY, dZ) = (%.3f, %.3f, %.3f)\n", vecLeft.v[0], vecLeft.v[1], vecLeft.v[2], vecRight.v[0], vecRight.v[1], vecRight.v[2], vecDiff.v[0], vecDiff.v[1], vecDiff.v[2]);
					//sprintf_s(buf, sizeof(buf), "(lx, ly, lz) = (%.4f, %.4f, %.4f) | (rx, ry, rz) = (%.4f, %.4f, %.4f) | (dX, dY, dZ) = (%.4f, %.4f, %.4f)\n", vecLeft.v[0], vecLeft.v[1], vecLeft.v[2], vecRight.v[0], vecRight.v[1], vecRight.v[2], vecDiff.v[0], vecDiff.v[1], vecDiff.v[2]);
					printf_s(buf);
				}

				// a delay to not overheat your computer... :)
				Sleep(2);
			}
		}

		delete trackingHandler;
	}
	return EXIT_SUCCESS;
}

