#include "main.h"
#include "devices.h"
#include "lemlib/pose.hpp"
#include "pros/adi.h"
#include "pros/misc.h"
#include "pros/motors.h"
#include "pros/rtos.h" // IWYU pragma: keep
#include "pros/rtos.hpp"
#include <algorithm>
#include <atomic> // IWYU pragma: keep
#include <cmath>
#include <cstdint>
#include <cstdio>
#include <cstring>
#include <ctime>
#include <string>
#include "lemlib/api.hpp" // IWYU pragma: keep
#include "functions.h"
#include "robodash/api.h"
#include "autons.h"


void blueLow(){
    intake();
    moveForward(42, {.async = false}); //14, 10
    moveBack(19);
    turnToHeading(276);
    moveForward(15);
    lowGoal();
}
void blueMid(){
    intake();
    moveForward(42, {.async = false}); //14, 10
    moveBack(19);
    turnToHeading(88);
    matchLoad.toggle();
    delay(250);
    moveForward(15);
    midGoal();
}
void redLow(){
    intake();
    moveForward(42, {.async = false}); //14, 10
    moveBack(18);
    turnToHeading(276);
    moveForward(15);
    lowGoal();
}
void redMid(){
    intake();
    moveForward(42, {.async = false}); //14, 10
    moveBack(18);
    turnToHeading(88);
    matchLoad.toggle();
    delay(250);
    moveForward(15.5);
    midGoal();
}

void redHigh(){
	chassis.moveToPoint(0, 16, 4000, {}, false);
	chassis.turnToHeading(33, 4000, {}, false);
	printf("X: %f, Y: %f, Theta: %f\n", chassis.getPose().x, chassis.getPose().y, chassis.getPose().theta);
	chassis.moveToPoint(3.67, 26.69, 4000, {.maxSpeed=32}, false);	
	chassis.turnToHeading(315, 400, {}, false);
	chassis.moveToPose(-2.13745, 39.66255, 315, 2000, {.lead=0.1}, false);
}

void blueHigh(){
	chassis.moveToPoint(0, 16, 4000, {}, false);
	chassis.turnToHeading(33, 4000, {}, false);
	printf("X: %f, Y: %f, Theta: %f\n", chassis.getPose().x, chassis.getPose().y, chassis.getPose().theta);
	chassis.moveToPoint(3.67, 26.69, 4000, {.maxSpeed=32}, false);	
	chassis.turnToHeading(315, 400, {}, false);
	chassis.moveToPose(-2.491, 40.0161, 315, 2000, {.lead=0.1}, false);
}