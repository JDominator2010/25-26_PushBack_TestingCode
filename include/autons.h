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


extern void blueLow();
extern void blueMid();
extern void blueHigh();
extern void redLow();
extern void redMid();
extern void redHigh();

extern void skillsAuton();

extern pros::Task windshield;