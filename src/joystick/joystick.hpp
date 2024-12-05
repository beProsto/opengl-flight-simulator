#pragma once

#include <OWL/Utility/Config.hpp>

#ifdef OWL_SYSTEM_WINDOWS
#include "windows/joystick.hpp"
using Joystick = WindowsJoystick;

#else
#include "linux/joystick.hpp"
using Joystick = LinuxJoystick;

#endif

// Something like a general JoystickManager class that would let the user pull individual Joysticks is needed.
// It should let them be pulled from a map by name or PID (i have to choose qwq), 
// it can't be just by id cuz that can change when connecting a new joystick or changing configuration or whtvr.


// // Basic interface (single joystick):
// class Joystick {
// public:
//     Joystick();
//     ~Joystick();
//     uint32_t getId();
//     float getAxis(uint8_t axisId);
//     bool getButton(uint8_t buttonId);
// };