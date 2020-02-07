//
// HTC Vive Lighthouse Tracking Example
// By Peter Thor 2016, 2017, 2018, 2019
//
// Shows how to extract basic tracking data
//

#include "stdafx.h"
#include "LighthouseTracking.h"

// Destructor
LighthouseTracking::~LighthouseTracking() {
	if (m_pHMD != NULL)
	{
		vr::VR_Shutdown();
		m_pHMD = NULL;
	}
}

// Constructor
LighthouseTracking::LighthouseTracking() {
	vr::EVRInitError eError = vr::VRInitError_None;
	m_pHMD = vr::VR_Init(&eError, vr::VRApplication_Background);
	char buf[1024];

	if (eError != vr::VRInitError_None)
	{
		m_pHMD = NULL;
		sprintf_s(buf, sizeof(buf), "Unable to init VR runtime: %s", vr::VR_GetVRInitErrorAsEnglishDescription(eError));
		printf_s(buf);
		exit(EXIT_FAILURE);
	}

	// Ensure VR Compositor is avail, otherwise getting poses causes a crash (openvr v1.3.22)
	if (!BInitCompositor()) {
		//printf("%s - Failed to initialize VR Compositor!\n", __FUNCTION__);
		sprintf_s(buf, sizeof(buf), "%s - Failed to initialize VR Compositor!", __FUNCTION__);
		printf_s(buf);
		exit(EXIT_FAILURE);
	}
	else {
		sprintf_s(buf, sizeof(buf), "Successfully initialized VR Compositor\n");
		printf_s(buf);
	}

	// Prepare manifest file
	const char *manifestPath = "C:/Users/pt/Documents/Visual Studio 2013/Projects/HTC Lighthouse Tracking Example/Release/win32/vive_debugger_actions.json";
	vr::EVRInputError inputError = vr::VRInput()->SetActionManifestPath(manifestPath);
	if (inputError != vr::VRInputError_None) {
		sprintf_s(buf, sizeof(buf), "Error: Unable to set manifest path: %d\n", inputError);
		printf_s(buf);
	}
	else {
		sprintf_s(buf, sizeof(buf), "Successfully set manifest path: %s\n", manifestPath);
		printf_s(buf);
	}

	// Handles for the new IVRInput
	inputError = vr::VRInput()->GetActionSetHandle(actionSetDemoPath, &m_actionSetDemo);
	if (inputError != vr::VRInputError_None) {
		sprintf_s(buf, sizeof(buf), "Error: Unable to get action set handle: %d\n", inputError);
		printf_s(buf);
	}
	else {
		sprintf_s(buf, sizeof(buf), "Successfully got %s action set handle: %d\n", actionSetDemoPath, m_actionSetDemo);
		printf_s(buf);
	}

	// handle for left controller pose
	inputError = vr::VRInput()->GetActionHandle(actionDemoHandLeftPath, &m_actionDemoHandLeft);
	if (inputError != vr::VRInputError_None) {
		sprintf_s(buf, sizeof(buf), "Error: Unable to get action handle: %d\n", inputError);
		printf_s(buf);
	}
	else {
		sprintf_s(buf, sizeof(buf), "Successfully got %s handle: %d\n", actionDemoHandLeftPath, m_actionDemoHandLeft);
		printf_s(buf);
	}

	// handle for right controller pose
	inputError = vr::VRInput()->GetActionHandle(actionDemoHandRightPath, &m_actionDemoHandRight);
	if (inputError != vr::VRInputError_None) {
		sprintf_s(buf, sizeof(buf), "Error: Unable to get action handle: %d\n", inputError);
		printf_s(buf);
	}
	else {
		sprintf_s(buf, sizeof(buf), "Successfully got %s handle: %d\n", actionDemoHandRightPath, m_actionDemoHandRight);
		printf_s(buf);
	}

	// handle for analog trackpad action
	inputError = vr::VRInput()->GetActionHandle(actionDemoAnalogInputPath, &m_actionAnalogInput);
	if (inputError != vr::VRInputError_None) {
		sprintf_s(buf, sizeof(buf), "Error: Unable to get action handle: %d\n", inputError);
		printf_s(buf);
	}
	else {
		sprintf_s(buf, sizeof(buf), "Successfully got %s handle: %d\n", actionDemoAnalogInputPath, m_actionAnalogInput);
		printf_s(buf);
	}

	// handle for a hide cube action
	inputError = vr::VRInput()->GetActionHandle(actionDemoHideCubesPath, &m_actionHideCubes);
	if (inputError != vr::VRInputError_None) {
		sprintf_s(buf, sizeof(buf), "Error: Unable to get action handle: %d\n", inputError);
		printf_s(buf);
	}
	else {
		sprintf_s(buf, sizeof(buf), "Successfully got %s handle: %d\n", actionDemoHideCubesPath, m_actionHideCubes);
		printf_s(buf);
	}

	// handle for a touch action
	inputError = vr::VRInput()->GetActionHandle(actionDemoTouchPath, &m_actionTouch);
	if (inputError != vr::VRInputError_None) {
		sprintf_s(buf, sizeof(buf), "Error: Unable to get action handle: %d\n", inputError);
		printf_s(buf);
	}
	else {
		sprintf_s(buf, sizeof(buf), "Successfully got %s handle: %d\n", actionDemoTouchPath, m_actionTouch);
		printf_s(buf);
	}

	// handle for a click action
	inputError = vr::VRInput()->GetActionHandle(actionDemoClickPath, &m_actionClick);
	if (inputError != vr::VRInputError_None) {
		sprintf_s(buf, sizeof(buf), "Error: Unable to get action handle: %d\n", inputError);
		printf_s(buf);
	}
	else {
		sprintf_s(buf, sizeof(buf), "Successfully got %s handle: %d\n", actionDemoClickPath, m_actionClick);
		printf_s(buf);
	}

	// handle for controller pose source - not used atm
	inputError = vr::VRInput()->GetInputSourceHandle(inputHandLeftPath, &m_inputHandLeftPath);
	if (inputError != vr::VRInputError_None) {
		sprintf_s(buf, sizeof(buf), "Error: Unable to get input handle: %d\n", inputError);
		printf_s(buf);
	}
	else {
		sprintf_s(buf, sizeof(buf), "Successfully got %s input handle: %d\n", inputHandLeftPath, m_inputHandLeftPath);
		printf_s(buf);
	}

	inputError = vr::VRInput()->GetInputSourceHandle(inputHandRightPath, &m_inputHandRightPath);
	if (inputError != vr::VRInputError_None) {
		sprintf_s(buf, sizeof(buf), "Error: Unable to get input handle: %d\n", inputError);
		printf_s(buf);
	}
	else {
		sprintf_s(buf, sizeof(buf), "Successfully got %s input handle: %d\n", inputHandRightPath, m_inputHandRightPath);
		printf_s(buf);
	}
}

//-----------------------------------------------------------------------------
// Purpose: Initialize Compositor. Returns true if the compositor was
//          successfully initialized, false otherwise.
//-----------------------------------------------------------------------------
bool LighthouseTracking::BInitCompositor()
{
	vr::EVRInitError peError = vr::VRInitError_None;

	if (!vr::VRCompositor())
	{
		printf("Compositor initialization failed. See log file for details\n");
		return false;
	}

	return true;
}

/*
* Loop-listen for events then parses them (e.g. prints the to user)
* Supply a filterIndex other than -1 to show only info for that device in question. e.g. 0 is always the hmd.
* Returns true if success or false if openvr has quit
*/
bool LighthouseTracking::RunProcedure(bool bWaitForEvents, int filterIndex = -1) {


	// check if HMD is connected or not
	if (!m_pHMD->IsTrackedDeviceConnected(0)) {
		char buf[1024];
		sprintf_s(buf, sizeof(buf), "(OpenVR) HMD Not Connected\n");
		printf_s(buf);
		return false;
	}

	// Either A) wait for events, such as hand controller button press, before parsing...
	if (bWaitForEvents) {
		// Process VREvent
		vr::VREvent_t event;
		while (m_pHMD->PollNextEvent(&event, sizeof(event)))
		{
			// Process event
			if (!ProcessVREvent(event, filterIndex)) {
				char buf[1024];
				sprintf_s(buf, sizeof(buf), "\n\n(OpenVR) service quit?\n");
				printf_s(buf);
				//return false;
			}

			// parse a frame
			//ParseTrackingFrame(filterIndex);
		}
	}
	else {
		// ... or B) continous parsint of tracking data irrespective of events
		//std::cout << std::endl << "Parsing next frame...";

		ParseTrackingFrame(filterIndex);
	}

	return true;
}

//-----------------------------------------------------------------------------
// Purpose: Processes a single VR event
//-----------------------------------------------------------------------------

bool LighthouseTracking::ProcessVREvent(const vr::VREvent_t & event, int filterOutIndex = -1)
{
	// if user supplied a device filter index only show events for that device
	if (filterOutIndex != -1)
		if (event.trackedDeviceIndex == filterOutIndex)
			return true;

	// print stuff for various events (this is not a complete list). Add/remove upon your own desire...
	switch (event.eventType)
	{
		case vr::VREvent_TrackedDeviceActivated:
		{
			//SetupRenderModelForTrackedDevice(event.trackedDeviceIndex);
			char buf[1024];
			sprintf_s(buf, sizeof(buf), "(OpenVR) Device : %d attached\n", event.trackedDeviceIndex);
			printf_s(buf);
		}
		break;

		case vr::VREvent_TrackedDeviceDeactivated:
		{
			char buf[1024];
			sprintf_s(buf, sizeof(buf), "(OpenVR) Device : %d detached\n", event.trackedDeviceIndex);
			printf_s(buf);
		}
		break;

		case vr::VREvent_TrackedDeviceUpdated:
		{
			char buf[1024];
			sprintf_s(buf, sizeof(buf), "(OpenVR) Device : %d updated\n", event.trackedDeviceIndex);
			printf_s(buf);
		}
		break;

		case (vr::VREvent_DashboardActivated) :
		{
			char buf[1024];
			sprintf_s(buf, sizeof(buf), "(OpenVR) Dashboard activated\n");
			printf_s(buf);
		}
		break;

		case (vr::VREvent_DashboardDeactivated) :
		{
			char buf[1024];
			sprintf_s(buf, sizeof(buf), "(OpenVR) Dashboard deactivated\n");
			printf_s(buf);

		}
		break;

		case (vr::VREvent_ChaperoneDataHasChanged) :
		{
			char buf[1024];
			sprintf_s(buf, sizeof(buf), "(OpenVR) Chaperone data has changed\n");
			printf_s(buf);

		}
		break;

		case (vr::VREvent_ChaperoneSettingsHaveChanged) :
		{
			char buf[1024];
			sprintf_s(buf, sizeof(buf), "(OpenVR) Chaperone settings have changed\n");
			printf_s(buf);
		}
		break;

		case (vr::VREvent_ChaperoneUniverseHasChanged) :
		{
			char buf[1024];
			sprintf_s(buf, sizeof(buf), "(OpenVR) Chaperone universe has changed\n");
			printf_s(buf);

		}
		break;

		case (vr::VREvent_Quit) :
		{
			char buf[1024];
			sprintf_s(buf, sizeof(buf), "(OpenVR) VREvent_Quit\n");
			printf_s(buf);

			return false;
		}
		break;

		case (vr::VREvent_ProcessQuit) :
		{
			char buf[1024];
			sprintf_s(buf, sizeof(buf), "(OpenVR) VREvent_ProcessQuit\n");
			printf_s(buf);

			return false;
		}
		break;

		case (vr::VREvent_QuitAcknowledged) :
		{
			char buf[1024];
			sprintf_s(buf, sizeof(buf), "(OpenVR) QuitAcknowledged\n");
			printf_s(buf);

			return false;
		}
		break;

		case (vr::VREvent_TrackedDeviceRoleChanged) :
		{

			char buf[1024];
			sprintf_s(buf, sizeof(buf), "(OpenVR) TrackedDeviceRoleChanged: %d\n", event.trackedDeviceIndex);
			printf_s(buf);
		}
		break;

		case (vr::VREvent_TrackedDeviceUserInteractionStarted) :
		{
			char buf[1024];
			sprintf_s(buf, sizeof(buf), "(OpenVR) TrackedDeviceUserInteractionStarted: %d\n", event.trackedDeviceIndex);
			printf_s(buf);
		}
		break;

		// various events not handled/moved yet into the previous switch chunk.
		default: {
			char buf[1024];
			switch (event.eventType) {
			case vr::EVREventType::VREvent_ButtonTouch:
				sprintf_s(buf, sizeof(buf), "(OpenVR) Event: Touch Device: %d\n", event.trackedDeviceIndex);
				printf_s(buf);
				break;
			case vr::EVREventType::VREvent_ButtonUntouch:
				sprintf_s(buf, sizeof(buf), "(OpenVR) Event: Untouch Device: %d\n", event.trackedDeviceIndex);
				printf_s(buf);
				break;
			case vr::EVREventType::VREvent_ButtonPress: {
				sprintf_s(buf, sizeof(buf), "(OpenVR) Event: Press Device: %d\n", event.trackedDeviceIndex);
				printf_s(buf);

				vr::VREvent_Data_t vrevent_data = event.data;
				vr::VREvent_Controller_t vrevent_controller = vrevent_data.controller;
				sprintf_s(buf, sizeof(buf), "(OpenVR) controller button: %d\n", vrevent_controller.button);
				printf_s(buf);
			}

				break;
			case vr::EVREventType::VREvent_ButtonUnpress:
				sprintf_s(buf, sizeof(buf), "(OpenVR) Event: Release Device: %d\n", event.trackedDeviceIndex);
				printf_s(buf);
				break;
			case vr::EVREventType::VREvent_EnterStandbyMode:
				sprintf_s(buf, sizeof(buf), "(OpenVR) Event: Enter StandbyMode: %d\n", event.trackedDeviceIndex);
				printf_s(buf);
				break;
			case vr::EVREventType::VREvent_LeaveStandbyMode:
				sprintf_s(buf, sizeof(buf), "(OpenVR) Event: Leave StandbyMode: %d\n", event.trackedDeviceIndex);
				printf_s(buf);
				break;
			case vr::EVREventType::VREvent_PropertyChanged:
				sprintf_s(buf, sizeof(buf), "(OpenVR) Event: Property Changed Device: %d\n", event.trackedDeviceIndex);
				printf_s(buf);
				break;
			case vr::EVREventType::VREvent_SceneApplicationChanged:
				sprintf_s(buf, sizeof(buf), "(OpenVR) Event: Scene Application Changed\n");
				printf_s(buf);

				sprintf_s(buf, sizeof(buf), "(OpenVR) old pid: %d new pid: %d\n", event.data.process.oldPid, event.data.process.pid);
				printf_s(buf);

				break;
			case vr::EVREventType::VREvent_SceneFocusChanged:
				sprintf_s(buf, sizeof(buf), "(OpenVR) Event: Scene Focus Changed\n");
				printf_s(buf);

				sprintf_s(buf, sizeof(buf), "(OpenVR) old pid: %d new pid: %d\n", event.data.process.oldPid, event.data.process.pid);
				printf_s(buf);

				break;
			case vr::EVREventType::VREvent_TrackedDeviceUserInteractionStarted:
				sprintf_s(buf, sizeof(buf), "(OpenVR) Event: Tracked Device User Interaction Started Device: %d\n", event.trackedDeviceIndex);
				printf_s(buf);
				break;
			case vr::EVREventType::VREvent_TrackedDeviceUserInteractionEnded:
				sprintf_s(buf, sizeof(buf), "(OpenVR) Event: Tracked Device User Interaction Ended Device: %d\n", event.trackedDeviceIndex);
				printf_s(buf);
				break;
			case vr::EVREventType::VREvent_ProcessDisconnected:
				sprintf_s(buf, sizeof(buf), "(OpenVR) Event: A process was disconnected\n");
				printf_s(buf);
				break;
			case vr::EVREventType::VREvent_ProcessConnected:
				sprintf_s(buf, sizeof(buf), "(OpenVR) Event: A process was connected\n");
				printf_s(buf);
				break;

			case vr::VREvent_Compositor_ApplicationNotResponding:
				sprintf_s(buf, sizeof(buf), "(OpenVR) Compositor: Application Not Responding\n");
				printf_s(buf);
				break;
			case vr::VREvent_Compositor_ApplicationResumed:
				sprintf_s(buf, sizeof(buf), "(OpenVR) Compositor: Application Resumed\n");
				printf_s(buf);
				break;

			case vr::VRInitError_Compositor_FirmwareRequiresUpdate:
				sprintf_s(buf, sizeof(buf), "(OpenVR) Compositor: Firmware Requires Update\n");
				printf_s(buf);
				break;
			case vr::VRInitError_Compositor_SettingsInterfaceIsNull:
				sprintf_s(buf, sizeof(buf), "(OpenVR) Compositor: Settings Interface Is Null\n");
				printf_s(buf);
				break;
			case vr::VRInitError_Compositor_MessageOverlaySharedStateInitFailure:
				sprintf_s(buf, sizeof(buf), "(OpenVR) Compositor: Message Overlay Shared State Init Failure\n");
				printf_s(buf);
				break;

			case vr::VREvent_Input_BindingLoadFailed:
				sprintf_s(buf, sizeof(buf), "(OpenVR) Event: Input: Binding Load Failed\n");
				printf_s(buf);
				break;

			case vr::VREvent_Input_BindingLoadSuccessful:
				sprintf_s(buf, sizeof(buf), "(OpenVR) Event: Input: Binding Load Sucessful\n");
				printf_s(buf);
				break;

			case vr::VREvent_Input_ActionManifestReloaded:
				sprintf_s(buf, sizeof(buf), "(OpenVR) Event: Input: Action Manifest Reloaded\n");
				printf_s(buf);
				break;

			default:
				sprintf_s(buf, sizeof(buf), "(OpenVR) Unmanaged Event: %d Device: %d\n", event.eventType, event.trackedDeviceIndex);
				printf_s(buf);
				break;
			}
		}
		break;
	}

	return true;
}


// Get the quaternion representing the rotation
vr::HmdQuaternion_t LighthouseTracking::GetRotation(vr::HmdMatrix34_t matrix) {
	vr::HmdQuaternion_t q;

	q.w = sqrt(fmax(0, 1 + matrix.m[0][0] + matrix.m[1][1] + matrix.m[2][2])) / 2;
	q.x = sqrt(fmax(0, 1 + matrix.m[0][0] - matrix.m[1][1] - matrix.m[2][2])) / 2;
	q.y = sqrt(fmax(0, 1 - matrix.m[0][0] + matrix.m[1][1] - matrix.m[2][2])) / 2;
	q.z = sqrt(fmax(0, 1 - matrix.m[0][0] - matrix.m[1][1] + matrix.m[2][2])) / 2;
	q.x = copysign(q.x, matrix.m[2][1] - matrix.m[1][2]);
	q.y = copysign(q.y, matrix.m[0][2] - matrix.m[2][0]);
	q.z = copysign(q.z, matrix.m[1][0] - matrix.m[0][1]);
	return q;
}

// Get the vector representing the position
vr::HmdVector3_t LighthouseTracking::GetPosition(vr::HmdMatrix34_t matrix) {
	vr::HmdVector3_t vector;

	vector.v[0] = matrix.m[0][3];
	vector.v[1] = matrix.m[1][3];
	vector.v[2] = matrix.m[2][3];

	return vector;
}

void LighthouseTracking::StoreLeftControllerPosition(vr::HmdVector3_t vector) {
	m_vecLeftController.v[0] = vector.v[0];
	m_vecLeftController.v[1] = vector.v[1];
	m_vecLeftController.v[2] = vector.v[2];
}

void LighthouseTracking::StoreRightControllerPosition(vr::HmdVector3_t vector) {
	m_vecRightController.v[0] = vector.v[0];
	m_vecRightController.v[1] = vector.v[1];
	m_vecRightController.v[2] = vector.v[2];
}

vr::HmdVector3_t LighthouseTracking::GetLeftControllerPosition() {
	return m_vecLeftController;
}

vr::HmdVector3_t LighthouseTracking::GetRightControllerPosition() {
	return m_vecRightController;
}


vr::HmdVector3_t LighthouseTracking::GetControllerPositionDelta() {
	vr::HmdVector3_t vecDiff;
	vecDiff.v[0] = m_vecLeftController.v[0] - m_vecRightController.v[0];
	vecDiff.v[1] = m_vecLeftController.v[1] - m_vecRightController.v[1];
	vecDiff.v[2] = m_vecLeftController.v[2] - m_vecRightController.v[2];
	return vecDiff;
}

/*
* Parse a Frame with data from the tracking system
*
* Handy reference:
* https://github.com/TomorrowTodayLabs/NewtonVR/blob/master/Assets/SteamVR/Scripts/SteamVR_Utils.cs
*
* Also:
* Open VR Convention (same as OpenGL)
* right-handed system
* +y is up
* +x is to the right
* -z is going away from you
* http://www.3dgep.com/understanding-the-view-matrix/
*
*/
void LighthouseTracking::ParseTrackingFrame(int filterIndex) {

	char buf[1024];
	vr::EVRInputError inputError;

	sprintf_s(buf, sizeof(buf), "\n");
	printf_s(buf);

	// Process SteamVR action state
	// UpdateActionState is called each frame to update the state of the actions themselves. The application
	// controls which action sets are active with the provided array of VRActiveActionSet_t structs.
	vr::VRActiveActionSet_t actionSet = { 0 };
	actionSet.ulActionSet = m_actionSetDemo;
	inputError = vr::VRInput()->UpdateActionState(&actionSet, sizeof(actionSet), 1);
	if (inputError == vr::VRInputError_None) {
		sprintf_s(buf, sizeof(buf), "%s | UpdateActionState(): Ok\n", actionSetDemoPath);
		printf_s(buf);
	}
	else {
		sprintf_s(buf, sizeof(buf), "%s | UpdateActionState(): Error: %d\n", actionSetDemoPath, inputError);
		printf_s(buf);
	}

	// Get analog data
	vr::InputAnalogActionData_t analogData;
	inputError = vr::VRInput()->GetAnalogActionData(m_actionAnalogInput, &analogData, sizeof(analogData), vr::k_ulInvalidInputValueHandle);
	if (inputError == vr::VRInputError_None)
	{
		sprintf_s(buf, sizeof(buf), "%s | GetAnalogActionData() Ok\n", actionDemoAnalogInputPath);
		printf_s(buf);

		if (analogData.bActive) {
			float m_vAnalogValue0 = analogData.x;
			float m_vAnalogValue1 = analogData.y;
			sprintf_s(buf, sizeof(buf), "%s | x: %f  y:%f\n", actionDemoAnalogInputPath, m_vAnalogValue0, m_vAnalogValue1);
			printf_s(buf);
		}
		else {
			sprintf_s(buf, sizeof(buf), "%s | action not avail to be bound\n", actionDemoAnalogInputPath);
			printf_s(buf);
		}
	}
	else {
		sprintf_s(buf, sizeof(buf), "%s | GetAnalogActionData() Not Ok. Error: %d\n", actionDemoAnalogInputPath, inputError);
		printf_s(buf);
	}


	// Get digital data
	vr::InputDigitalActionData_t digitalData;
	inputError = vr::VRInput()->GetDigitalActionData(m_actionHideCubes, &digitalData, sizeof(digitalData), vr::k_ulInvalidInputValueHandle);
	if (inputError == vr::VRInputError_None)
	{
		sprintf_s(buf, sizeof(buf), "%s | GetDigitalActionData() Ok\n", actionDemoHideCubesPath);
		printf_s(buf);

		if (digitalData.bActive) {
			bool m_vDigitalValue0 = digitalData.bState;
			sprintf_s(buf, sizeof(buf), "%s | State: %d\n", actionDemoHideCubesPath, m_vDigitalValue0);
			printf_s(buf);

			// check from which device the action came
			vr::InputOriginInfo_t originInfo;
			if (vr::VRInputError_None == vr::VRInput()->GetOriginTrackedDeviceInfo(digitalData.activeOrigin, &originInfo, sizeof(originInfo)))
			{
				if (originInfo.devicePath == m_inputHandLeftPath) {
					sprintf_s(buf, sizeof(buf), "Action comes from left hand\n");
					printf_s(buf);
				} else if (originInfo.devicePath == m_inputHandRightPath) {
					sprintf_s(buf, sizeof(buf), "Action comes from right hand\n");
					printf_s(buf);
				}
			}

		}
		else {
			sprintf_s(buf, sizeof(buf), "%s | action not avail to be bound\n", actionDemoHideCubesPath);
			printf_s(buf);
		}
	}
	else {
		sprintf_s(buf, sizeof(buf), "%s | GetDigitalActionData() Not Ok. Error: %d\n", actionDemoHideCubesPath, inputError);
		printf_s(buf);
	}

	// Get digital data of a "Touch Action"
	vr::InputDigitalActionData_t digitalDataTouch;
	inputError = vr::VRInput()->GetDigitalActionData(m_actionTouch, &digitalDataTouch, sizeof(digitalDataTouch), vr::k_ulInvalidInputValueHandle);
	if (inputError == vr::VRInputError_None)
	{
		sprintf_s(buf, sizeof(buf), "%s | GetDigitalActionData() Ok\n", actionDemoTouchPath);
		printf_s(buf);

		if (digitalDataTouch.bActive) {
			bool m_vDigitalValue0 = digitalDataTouch.bState;
			sprintf_s(buf, sizeof(buf), "%s | State: %d\n", actionDemoTouchPath, m_vDigitalValue0);
			printf_s(buf);

			// check from which device the action came
			vr::InputOriginInfo_t originInfo;
			if (vr::VRInputError_None == vr::VRInput()->GetOriginTrackedDeviceInfo(digitalDataTouch.activeOrigin, &originInfo, sizeof(originInfo)))
			{
				if (originInfo.devicePath == m_inputHandLeftPath) {
					sprintf_s(buf, sizeof(buf), "Action comes from left hand\n");
					printf_s(buf);
				}
				else if (originInfo.devicePath == m_inputHandRightPath) {
					sprintf_s(buf, sizeof(buf), "Action comes from right hand\n");
					printf_s(buf);
				}
			}

		}
		else {
			sprintf_s(buf, sizeof(buf), "%s | action not avail to be bound\n", actionDemoTouchPath);
			printf_s(buf);
		}
	}
	else {
		sprintf_s(buf, sizeof(buf), "%s | GetDigitalActionData() Not Ok. Error: %d\n", actionDemoTouchPath, inputError);
		printf_s(buf);
	}

	// Get digital data of a "Click Action"
	vr::InputDigitalActionData_t digitalDataClick;
	inputError = vr::VRInput()->GetDigitalActionData(m_actionClick, &digitalDataClick, sizeof(digitalDataClick), vr::k_ulInvalidInputValueHandle);
	if (inputError == vr::VRInputError_None)
	{
		sprintf_s(buf, sizeof(buf), "%s | GetDigitalActionData() Ok\n", actionDemoClickPath);
		printf_s(buf);

		if (digitalDataClick.bActive) {
			bool m_vDigitalValue0 = digitalDataClick.bState;
			sprintf_s(buf, sizeof(buf), "%s | State: %d\n", actionDemoClickPath, m_vDigitalValue0);
			printf_s(buf);

			// check from which device the action came
			vr::InputOriginInfo_t originInfo;
			if (vr::VRInputError_None == vr::VRInput()->GetOriginTrackedDeviceInfo(digitalDataClick.activeOrigin, &originInfo, sizeof(originInfo)))
			{
				if (originInfo.devicePath == m_inputHandLeftPath) {
					sprintf_s(buf, sizeof(buf), "Action comes from left hand\n");
					printf_s(buf);
				}
				else if (originInfo.devicePath == m_inputHandRightPath) {
					sprintf_s(buf, sizeof(buf), "Action comes from right hand\n");
					printf_s(buf);
				}
			}

		}
		else {
			sprintf_s(buf, sizeof(buf), "%s | action not avail to be bound\n", actionDemoClickPath);
			printf_s(buf);
		}
	}
	else {
		sprintf_s(buf, sizeof(buf), "%s | GetDigitalActionData() Not Ok. Error: %d\n", actionDemoClickPath, inputError);
		printf_s(buf);
	}


	// get pose data
	vr::InputPoseActionData_t poseData;
	inputError = vr::VRInput()->GetPoseActionDataForNextFrame(m_actionDemoHandLeft, vr::TrackingUniverseStanding, &poseData, sizeof(poseData), vr::k_ulInvalidInputValueHandle);
	if (inputError == vr::VRInputError_None) {
		sprintf_s(buf, sizeof(buf), "%s | GetPoseActionData() Ok\n", actionDemoHandLeftPath);
		printf_s(buf);

		if (poseData.bActive) {
			vr::VRInputValueHandle_t activeOrigin = poseData.activeOrigin;
			bool bPoseIsValid = poseData.pose.bPoseIsValid;
			bool bDeviceIsConnected = poseData.pose.bDeviceIsConnected;
			sprintf_s(buf, sizeof(buf), "Origin: %d Validity: %d DeviceIsConnected: %d\n", activeOrigin, bPoseIsValid, bDeviceIsConnected);
			printf_s(buf);


		/* Code below is old ---> */
			vr::HmdVector3_t position;
			vr::HmdQuaternion_t quaternion;

			// get the position and rotation
			position = GetPosition(poseData.pose.mDeviceToAbsoluteTracking);
			quaternion = GetRotation(poseData.pose.mDeviceToAbsoluteTracking);

			// print the tracking data
			//if (printHmdTrackingData) {
			sprintf_s(buf, sizeof(buf), "\n%s Pose\nx: %.2f y: %.2f z: %.2f\n", actionDemoHandLeftPath, position.v[0], position.v[1], position.v[2]);
			printf_s(buf);
			sprintf_s(buf, sizeof(buf), "qw: %.2f qx: %.2f qy: %.2f qz: %.2f\n", quaternion.w, quaternion.x, quaternion.y, quaternion.z);
			printf_s(buf);
			//}
		/* <--- End of old code */


		}
		else {
			sprintf_s(buf, sizeof(buf), "%s | action not avail to be bound\n", actionDemoHandLeftPath);
			printf_s(buf);
		}
	}
	else {
		sprintf_s(buf, sizeof(buf), "%s | GetPoseActionData() Call Not Ok. Error: %d\n", actionDemoHandLeftPath, inputError);
		printf_s(buf);
	}
}

void LighthouseTracking::PrintDevices() {

	char buf[1024];
	sprintf_s(buf, sizeof(buf), "\nDevice list:\n---------------------------\n");
	printf_s(buf);

	// Loop over all conntected devices and print some stuff about them
	for (vr::TrackedDeviceIndex_t unDevice = 0; unDevice < vr::k_unMaxTrackedDeviceCount; unDevice++)
	{
		// if no HMD is connected in slot 0 just skip the rest of the code
		// since a HMD must be present.
		if (!m_pHMD->IsTrackedDeviceConnected(unDevice))
			continue;

		// Get what type of device it is and work with its data
		vr::ETrackedDeviceClass trackedDeviceClass = vr::VRSystem()->GetTrackedDeviceClass(unDevice);
		switch (trackedDeviceClass) {
		case vr::ETrackedDeviceClass::TrackedDeviceClass_HMD:
			// print stuff for the HMD here, see controller stuff in next case block

			char buf[1024];
			sprintf_s(buf, sizeof(buf), "Device %d: Class: [HMD]", unDevice);
			printf_s(buf);
			break;

		case vr::ETrackedDeviceClass::TrackedDeviceClass_Controller:
			// do stuff for a controller here
			sprintf_s(buf, sizeof(buf), "Device %d: Class: [Controller]", unDevice);
			printf_s(buf);

			break;

		case vr::ETrackedDeviceClass::TrackedDeviceClass_GenericTracker:
			// do stuff for a generic tracker here

			sprintf_s(buf, sizeof(buf), "Device %d: Class: [GenericTracker]", unDevice);
			printf_s(buf);
			break;

		case vr::ETrackedDeviceClass::TrackedDeviceClass_TrackingReference:
			/// do stuff for a tracking reference here

			sprintf_s(buf, sizeof(buf), "Device %d: Class: [TrackingReference]", unDevice);
			printf_s(buf);
			break;

		case vr::ETrackedDeviceClass::TrackedDeviceClass_DisplayRedirect:
			/// do stuff for a display redirect here

			sprintf_s(buf, sizeof(buf), "Device %d: Class: [DisplayRedirect]", unDevice);
			printf_s(buf);
			break;

		case vr::ETrackedDeviceClass::TrackedDeviceClass_Invalid:
			// do stuff for an invalid class

			sprintf_s(buf, sizeof(buf), "Device %d: Class: [Invalid]", unDevice);
			printf_s(buf);
			break;

		}

		// print some of the meta data for the device
		char manufacturer[1024];
		vr::VRSystem()->GetStringTrackedDeviceProperty(unDevice, vr::ETrackedDeviceProperty::Prop_ManufacturerName_String, manufacturer, sizeof(manufacturer));

		char modelnumber[1024];
		vr::VRSystem()->GetStringTrackedDeviceProperty(unDevice, vr::ETrackedDeviceProperty::Prop_ModelNumber_String, modelnumber, sizeof(modelnumber));

		char serialnumber[1024];
		vr::VRSystem()->GetStringTrackedDeviceProperty(unDevice, vr::ETrackedDeviceProperty::Prop_SerialNumber_String, serialnumber, sizeof(serialnumber));

		bool canPowerOff = vr::VRSystem()->GetBoolTrackedDeviceProperty(unDevice, vr::ETrackedDeviceProperty::Prop_DeviceCanPowerOff_Bool);

		sprintf_s(buf, sizeof(buf), " %s - %s [%s] can power off: %d\n", manufacturer, modelnumber, serialnumber, canPowerOff);
		printf_s(buf);
	}
	sprintf_s(buf, sizeof(buf), "---------------------------\nEnd of device list\n\n");
	printf_s(buf);

}
