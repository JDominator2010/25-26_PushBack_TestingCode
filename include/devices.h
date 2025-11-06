#pragma once
#include "main.h"
#include "pros/adi.hpp"
#include "pros/imu.hpp"

using namespace pros;
extern Controller controller;

// -- SENSORS -- //
extern Imu inertial;

// -- INTAKE MOTORS -- //
extern Motor Low;
extern Motor Mid;
extern Motor High;

// -- DRIVE MOTORS -- //
extern Motor FL;
extern Motor FR;
extern Motor RR;
extern Motor RL;

// --- ETC MOTORS -- //
extern Motor Disrupter;

// -- ADI PORTS -- //
extern adi::Pneumatics matchLoad;