// LIGHTHOUSETRACKING.h
#ifndef _LIGHTHOUSETRACKING_H_
#define _LIGHTHOUSETRACKING_H_

// OpenVR
#include <openvr.h>
#include "samples\shared\Matrices.h"

class LighthouseTracking {
private:

	// Basic stuff
	vr::IVRSystem *m_pHMD = NULL;
	vr::TrackedDevicePose_t m_rTrackedDevicePose[vr::k_unMaxTrackedDeviceCount];
	Matrix4 m_rmat4DevicePose[vr::k_unMaxTrackedDeviceCount];

	// Position and rotation of pose
	vr::HmdVector3_t LighthouseTracking::GetPosition(vr::HmdMatrix34_t matrix);
	vr::HmdQuaternion_t LighthouseTracking::GetRotation(vr::HmdMatrix34_t matrix);

	// If false the program will parse tracking data continously and not wait for openvr events
	bool bWaitForEventsBeforeParsing = false;

public:
	~LighthouseTracking();
	LighthouseTracking();

	// Main loop that listens for openvr events and calls process and parse routines, if false the service has quit
	bool RunProcedure(bool bWaitForEvents, int filterIndex);

	// Process a VR event and print some general info of what happens
	bool ProcessVREvent(const vr::VREvent_t & event, int filterIndex);

	// Parse a tracking frame and print its position / rotation / events.
	// Supply a filterIndex different than -1 to only show data for one specific device.
	void ParseTrackingFrame(int filterIndex);

	// prints information of devices
	void PrintDevices();
};

#endif _LIGHTHOUSETRACKING_H_
