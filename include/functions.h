#pragma once
#include "devices.h"
#include "functions.h"
#include "main.h" // IWYU pragma: keep
#include "lemlib/api.hpp" // IWYU pragma: keep
#include "lemlib-tarball/api.hpp" // IWYU pragma: keep

extern bool goalActiveAuton;

struct moveForwardOptions {
    float timeout = 4000;
    bool forwards = true;
    float maxSpeed = 127;
    float minSpeed = 0;
    float earlyExitRange = 0;
    bool async = false;
};

struct moveBackOptions {
    float timeout = 4000;
    bool forwards = false;
    float maxSpeed = 127;
    float minSpeed = 0;
    float earlyExitRange = 0;
    bool async = false;
};

inline const moveForwardOptions defaultMoveForwardOptions = { // IWYU pragma: keep
    .timeout = 4000,
    .forwards = true,
    .maxSpeed = 127,
    .minSpeed = 0,
    .earlyExitRange = 0,
    .async = false
};

inline const moveBackOptions defaultMoveBackOptions = { // IWYU pragma: keep
    .timeout = 4000,
    .forwards = false,
    .maxSpeed = 127,
    .minSpeed = 0,
    .earlyExitRange = 0,
    .async = false
};

extern void windshieldWiperTask(void* param);

extern void moveForward(float inches, moveForwardOptions options = defaultMoveForwardOptions);
extern void turnToHeading(float theta, int timeoutMS = 4000);

extern void moveBack(float inches, moveBackOptions options = defaultMoveBackOptions);

extern void intake();

extern void lowGoal();

extern void midGoal();

extern void highGoal();

extern void ladderOff();
