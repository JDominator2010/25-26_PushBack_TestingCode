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
    moveForward(42, {.async = false}); //14, 10
    moveBack(18);
    turnToHeading(276);
    moveForward(15);
    lowGoal();
    delay(2000);
    ladderOff();
    moveBack(49.5);  // changed from 48 to 50 ( slight distnace shortaage) to 49 (angle compensation)
    turnToHeading(226); // changed from 231 to 227 (check box) to 226
    matchLoad.extend();
    delay(200);
    intake();
    moveForward(12, {.timeout=1000}); // changed from 13 to 12 (check for ez flow) to 10 (timout being hit) to 11 (not far enough) to 12 (??????)
    moveBack(1.5); // changed from .5 to 1 (down x4) to 1.5 (down x4)
    moveForward(1.5);
    delay(100); // change from 500 to 100 (might be useless)
    moveBack(20); // changed from 10 to 14
    matchLoad.retract();
    turnToHeading(180);
    moveForward(8); // changed from 7 to 8 (inc dist)
    highGoal();
}

void blueHigh(){
    intake();
    moveForward(42, {.async = false}); //14, 10
    moveBack(18);
    turnToHeading(276);
    moveForward(15);
    lowGoal();
    delay(1500);
    ladderOff();
    moveBack(49.5);  // changed from 48 to 50 ( slight distnace shortaage) to 49 (angle compensation)
    turnToHeading(226); // changed from 231 to 227 (check box) to 226
    matchLoad.extend();
    delay(200);
    intake();
    moveForward(12, {.timeout=1000}); // changed from 13 to 12 (check for ez flow) to 10 (timout being hit) to 11 (not far enough) to 12 (??????)
    moveBack(1.5); // changed from .5 to 1 (down x4) to 1.5 (down x4)
    moveForward(1.5);
    delay(100); // change from 500 to 100 (might be useless)
    moveBack(20); // changed from 10 to 14
    matchLoad.retract();
    turnToHeading(180);
    moveForward(8); // changed from 7 to 8 (inc dist)
    highGoal();
}