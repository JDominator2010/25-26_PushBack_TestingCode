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
    intake();
	chassis.moveToPoint(0, 16, 4000, {}, false);
	chassis.turnToHeading(33, 4000, {}, false);
	printf("X: %f, Y: %f, Theta: %f\n", chassis.getPose().x, chassis.getPose().y, chassis.getPose().theta);
	moveForward(11.3, {.maxSpeed=32, .async=false});	
	chassis.turnToHeading(315, 400, {}, false);
	chassis.moveToPose(-0.547, 36.658, 315, 1000, {.lead=0.1}, false); //-1.784, 39.309
	lowGoal();
	delay(2000);
	// chassis.moveToPoint(18.722, 18.803, 4000, {.forwards=false}, false);
	moveBack(44.5);
	// chassis.turnToPoint(26, 0, 3000, {}, false);
	chassis.turnToHeading(180.02, 4000, {}, false);
	// chassis.moveToPose(28.5, 6, 180, 3000, {.lead=0.1}, false);
	intake();
	// ladderOff();
	matchLoad.extend();
	delay(300);
	// // moveForward(11);
	chassis.moveToPoint(29.626, 2.1, 4000, {}, false);
	// moveBack(1);
	// moveForward(1);
	delay(550);
	moveBack(20.5);
	ladderOff();
	matchLoad.retract();
	chassis.turnToHeading(2, 4000, {}, false);
	// chassis.moveToPose(26.908, 19.149, 0, 4000, {.lead=0.01}, false);
	moveForward(4);
	highGoal();
	delay(1270);
	ladderOff();
	// chassis.turnToPoint(28.125, 17.50, 4000, {}, false);
	// chassis.moveToPose(28.125, 17.50, 0, 4000, {.lead=0.1}, false);
	// highGoal();
}

void blueHigh(){
    intake();
	chassis.moveToPoint(0, 16, 4000, {}, false);
	chassis.turnToHeading(33, 4000, {}, false);
	printf("X: %f, Y: %f, Theta: %f\n", chassis.getPose().x, chassis.getPose().y, chassis.getPose().theta);
	moveForward(11.3, {.maxSpeed=32, .async=false});	
	chassis.turnToHeading(315, 400, {}, false);
	chassis.moveToPose(-0.547, 36.658, 315, 1000, {.lead=0.1}, false); //-1.784, 39.309
	lowGoal();
	delay(2000);
	// chassis.moveToPoint(18.722, 18.803, 4000, {.forwards=false}, false);
	moveBack(44.5);
	// chassis.turnToPoint(26, 0, 3000, {}, false);
	chassis.turnToHeading(180.02, 4000, {}, false);
	// chassis.moveToPose(28.5, 6, 180, 3000, {.lead=0.1}, false);
	intake();
	// ladderOff();
	matchLoad.extend();
	delay(300);
	// // moveForward(11);
	chassis.moveToPoint(29.626, 2.1, 4000, {}, false);
	// moveBack(1);
	// moveForward(1);
	delay(550);
	moveBack(20.5);
	ladderOff();
	matchLoad.retract();
	chassis.turnToHeading(2, 4000, {}, false);
	// chassis.moveToPose(26.908, 19.149, 0, 4000, {.lead=0.01}, false);
	moveForward(4);
	highGoal();
	delay(1270);
	ladderOff();
	// chassis.turnToPoint(28.125, 17.50, 4000, {}, false);
	// chassis.moveToPose(28.125, 17.50, 0, 4000, {.lead=0.1}, false);
	// highGoal();
}