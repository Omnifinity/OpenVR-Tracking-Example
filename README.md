#  What is this?
A rapid and simple example of how to start a background process for listening to HTC Vive Lighthouse tracking data. 

Uses OpenVR 1.3.22 which uses the new input system.

Compiled using Visual Studio 2013. Please adapt as suitable. 

##  How do I run it?
There is an executable in the Binary folder. Unarchive that and run it through a console after you've started Steam VR. Background processes are not allowed to start up SteamVR by themselves. 

If you supply the parameter "--showonlydeviceid <number>" you can choose to show data/events for a specific device. E.g. "--showonlydeviceid 0" would show only data for the HMD.

There are two manifest files inside the Binary folder.

##  How do I compile it?
1. Make sure that you point your includes and library bin folder to where you have openvr installed on your machine.
2. Make sure you've got the openvr_api.dll in the same folder as the built example

##  How do I use it?
1. Edit the source code to point to where your manifest file is, search for SetActionManifestPath() inside Lighthousetracking.cpp.
2. Start up Steam VR
3. Compile and start the example - it launches as a background application


##  Troubleshooting:

*Unable to init VR runtime: Hmd Not Found (108)*

Solution: Attach the HMD to the computer


*Unable to init VR runtime: Not starting vrserver for background app (121)*

Solution: Start Steam VR

