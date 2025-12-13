#pragma once
#include "lemlib/asset.hpp"
#include "lemlib/chassis/chassis.hpp"
#include "main.h" // IWYU pragma: keep
#include "pros/adi.hpp"
#include "pros/imu.hpp"
#include "lemlib/api.hpp" // IWYU pragma: keep
#include "pros/motor_group.hpp"
#include "pros/rotation.hpp"
#include "lemlib-tarball/api.hpp" // IWYU pragma: keep
#include "robodash/views/console.hpp"
#include "robodash/views/selector.hpp"
#include "liblvgl/lvgl.h"
#include "autons.h"
#include <string>

using namespace pros;
extern Controller controller;

// -- SENSORS -- //
extern Imu imu;
extern Rotation verticalRotation;
extern Rotation horizontalRotation;

// -- INTAKE MOTORS -- //
extern Motor Low;
extern Motor Mid;
extern Motor High;

// -- DRIVE MOTORS -- //
extern Motor FL;
extern Motor FR;
extern Motor RR;
extern Motor RL;

// -- LEMLIB DRIVETRAIN SETUP -- //
extern MotorGroup leftMotorGroup;
extern MotorGroup rightMotorGroup;

extern lemlib::Drivetrain drivetrain;

extern lemlib::TrackingWheel horizontalTracker;
extern lemlib::TrackingWheel verticalTracker;

extern lemlib::OdomSensors sensors;

extern lemlib::ControllerSettings lateral_controller;
extern lemlib::ControllerSettings angular_controller;

extern lemlib::Chassis chassis;

// -- LEMLIB TARBALL / PURE PURSUIT -- //

extern lemlib_tarball::Decoder blueDecoder;
extern lemlib_tarball::Decoder testingDecoder;


// --- ETC MOTORS -- //
extern Motor Disrupter;

// -- ADI PORTS -- //
extern adi::Pneumatics matchLoad;

// -- AUTON SELECTOR -- //
extern rd::Selector selector;
extern std::string selectedAutonName;

// -- IMAGES -- //
extern rd::Image appaR;
extern rd::Image appaB;

// -- RD CONSOLE -- //
extern rd::Console console;