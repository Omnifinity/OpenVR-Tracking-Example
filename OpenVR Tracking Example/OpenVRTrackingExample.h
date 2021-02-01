/*
OpenVR-Tracking-Example
Example on how to access tracking data from OpenVR
Copyright 2016, 2017, 2018, 2019, 2020, 2021, Omnifinity

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

#ifndef _OPENVRTRACKINGEXAMPLE_H_
#define _OPENVRTRACKINGEXAMPLE_H_

// OpenVR
#include <openvr.h>
#include "..\..\openvr\samples\shared\Matrices.h"
#include "..\..\openvr\samples\shared\pathtools.h"

//#include "pathtools_excerpt.h"

class OpenVRTrackingExample {
private:

	// Basic stuff
	vr::IVRSystem *m_pHMD = NULL;
	vr::TrackedDevicePose_t m_rTrackedDevicePose[vr::k_unMaxTrackedDeviceCount];
	Matrix4 m_rmat4DevicePose[vr::k_unMaxTrackedDeviceCount];

	// For getting the steam application key
	char applicationKey[vr::k_unMaxApplicationKeyLength];

	// Handles for the new IVRInput
	vr::VRActionSetHandle_t m_actionSetDemo = vr::k_ulInvalidActionSetHandle;
	const char *actionSetDemoPath = "/actions/demo";

	vr::VRActionHandle_t m_actionAnalogInput = vr::k_ulInvalidActionHandle;
	const char *actionDemoAnalogInputPath = "/actions/demo/in/AnalogInput";

	vr::VRActionHandle_t m_actionHideCubes = vr::k_ulInvalidActionHandle;
	const char *actionDemoHideCubesPath = "/actions/demo/in/HideCubes";

	vr::VRActionHandle_t m_actionClick = vr::k_ulInvalidActionHandle;
	const char *actionDemoClickPath = "/actions/demo/in/ClickAction";

	vr::VRActionHandle_t m_actionTouch = vr::k_ulInvalidActionHandle;
	const char *actionDemoTouchPath = "/actions/demo/in/TouchAction";

	vr::VRActionHandle_t m_actionDemoHandLeft = vr::k_ulInvalidActionHandle;
	const char *actionDemoHandLeftPath = "/actions/demo/in/Hand_Left";

	vr::VRActionHandle_t m_actionDemoHandRight = vr::k_ulInvalidActionHandle;
	const char *actionDemoHandRightPath = "/actions/demo/in/Hand_Right";

	vr::VRInputValueHandle_t m_inputHandLeftPath = vr::k_ulInvalidInputValueHandle;
	const char *inputHandLeftPath = "/user/hand/left";

	vr::VRInputValueHandle_t m_inputHandRightPath = vr::k_ulInvalidInputValueHandle;
	const char *inputHandRightPath = "/user/hand/right";

	struct ControllerInfo_t
	{
		vr::VRInputValueHandle_t m_source = vr::k_ulInvalidInputValueHandle;
		vr::VRActionHandle_t m_actionPose = vr::k_ulInvalidActionHandle;
		vr::VRActionHandle_t m_actionHaptic = vr::k_ulInvalidActionHandle;
		Matrix4 m_rmat4Pose;
		//CGLRenderModel *m_pRenderModel = nullptr;
		std::string m_sRenderModelName;
		bool m_bShowController;
	};

	enum EHand
	{
		Left = 0,
		Right = 1,
	};
	ControllerInfo_t m_Hand[2];

	vr::HmdVector3_t m_vecLeftController = {};
	vr::HmdVector3_t m_vecRightController = {};

	// Position and rotation of pose
	vr::HmdVector3_t GetPosition(vr::HmdMatrix34_t matrix);
	vr::HmdQuaternion_t GetRotation(vr::HmdMatrix34_t matrix);

	void StoreLeftControllerPosition(vr::HmdVector3_t vector);
	void StoreRightControllerPosition(vr::HmdVector3_t vector);

	// If false the program will parse tracking data continously and not wait for openvr events
	bool bWaitForEventsBeforeParsing = false;

	bool printHmdTrackingData = false;
	bool printRightControllerTrackingData = false;
	bool printLeftControllerTrackingData = false;
	bool printBasestationTrackingData = false;

public:
	OpenVRTrackingExample();
	~OpenVRTrackingExample();

	// Initialize the VR Compositor
	bool BInitCompositor();

	// Main loop that listens for openvr events and calls process and parse routines, if false the service has quit
	bool RunProcedure(bool bWaitForEvents, int filterIndex);

	// Process a VR event and print some general info of what happens
	bool ProcessVREvent(const vr::VREvent_t & event, int filterIndex);

	// Parse a tracking frame and print its position / rotation / events.
	// Supply a filterIndex different than -1 to only show data for one specific device.
	void ParseTrackingFrame(int filterIndex);

	// calc diff between positions
	vr::HmdVector3_t GetControllerPositionDelta();
	vr::HmdVector3_t GetLeftControllerPosition();
	vr::HmdVector3_t GetRightControllerPosition();

	// prints information of devices
	void PrintDevices();
};

#endif _OPENVRTRACKINGEXAMPLE_H_
