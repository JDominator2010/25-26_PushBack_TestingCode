#include "main.h"
#include "devices.h"
#include "functions.h"
#include "lemlib/pose.hpp"
#include "liblvgl/llemu.hpp"
#include "pros/misc.h"
#include "pros/motors.h"
#include "pros/rtos.h" // IWYU pragma: keep
#include "pros/rtos.hpp"
#include <atomic> // IWYU pragma: keep
#include <cstdint>
#include <cstdio>
#include <ctime>
#include <string>
#include "lemlib/api.hpp" // IWYU pragma: keep


void moveForward(float inches, moveForwardOptions options){
    chassis.setPose(0,0,0);
    chassis.moveToPoint(0, inches, options.timeout, {.forwards = options.forwards, .maxSpeed = options.maxSpeed, .minSpeed = options.minSpeed, .earlyExitRange = options.earlyExitRange}, options.async);
}

void moveBack(float inches, moveBackOptions options){
    chassis.setPose(0, inches, 0);
    chassis.moveToPoint(0, 0, options.timeout, {.forwards = options.forwards, .maxSpeed = options.maxSpeed, .minSpeed = options.minSpeed, .earlyExitRange = options.earlyExitRange}, options.async);
}

void turnToHeading(float theta, int timeoutMS){
    chassis.turnToHeading(theta, timeoutMS, {}, false);
}

void intake(){
    Low.move(127);
    Mid.move(-127);
}

void midGoal(){
    Low.move(85);
    Mid.move(127);
    High.move(-85);
    FL.set_brake_mode(E_MOTOR_BRAKE_BRAKE);
    FR.set_brake_mode(E_MOTOR_BRAKE_BRAKE);
    RL.set_brake_mode(E_MOTOR_BRAKE_BRAKE);
    RR.set_brake_mode(E_MOTOR_BRAKE_BRAKE);
}

void ladderOff(){
    Low.move(0);
    Mid.move(0);
    High.move(0);
    Low.brake();
    Mid.brake();
    High.brake();
}