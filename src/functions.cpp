#include "main.h"
#include "devices.h"
#include "functions.h"
#include "lemlib/pose.hpp"
#include "pros/adi.h"
#include "pros/misc.h"
#include "pros/motors.h"
#include "pros/rtos.h" // IWYU pragma: keep
#include "pros/rtos.hpp"
#include <atomic> // IWYU pragma: keep
#include <cstdint>
#include <cstdio>
#include <ctime>
#include <cmath>
#include <string>
#include "lemlib/api.hpp" // IWYU pragma: keep

bool goalActiveAuton = false;
float thetaError = 0;

void windshieldWiperTask(void* param) {
	bool prevA = false; 
	bool pulseDir = false;  // false = left, true = right
	uint32_t lastPulseTime = 0;
	const uint32_t pulseInterval = 300; // ms between pulse
	const double leftPos = 0.0;
	const double rightPos = -150.0;
	const double parkTolerance = 8.0;
	const float pulseVel = 127; 
    
    while (true) {
        console.clear();

        console.printf("X: %f Y: %f, Theta: %f", chassis.getPose().x, chassis.getPose().y, chassis.getPose().theta);
        if (goalActiveAuton) {
            // Pulsing mode
            uint32_t now = pros::millis();
            if (now - lastPulseTime >= pulseInterval) {
                pulseDir = !pulseDir;
                lastPulseTime = now;
                Disrupter.move_absolute(pulseDir ? leftPos : rightPos, pulseVel);
            }
        } else {
            // Parking mode (same as opcontrol)
            double currentPos = Disrupter.get_position();
            bool inLeftRange = fabs(currentPos - leftPos) <= parkTolerance;
            
            if (!inLeftRange) {
                Disrupter.move_absolute(leftPos, pulseVel);
            } else {
                Disrupter.set_brake_mode(pros::E_MOTOR_BRAKE_HOLD);
                Disrupter.move_velocity(0);
            }
        }
        pros::delay(20);
    }
}

// void moveForward(float inches, moveForwardOptions options){
//     chassis.setPose(0,0,thetaError);
//     chassis.moveToPoint(0, inches, options.timeout, {.forwards = options.forwards, .maxSpeed = options.maxSpeed, .minSpeed = options.minSpeed, .earlyExitRange = options.earlyExitRange}, options.async);
// }

// void moveBack(float inches, moveBackOptions options){
//     chassis.setPose(0, inches, thetaError);
//     chassis.moveToPoint(0, 0, options.timeout, {.forwards = options.forwards, .maxSpeed = options.maxSpeed, .minSpeed = options.minSpeed, .earlyExitRange = options.earlyExitRange}, options.async);
// }

void moveForward(float inches, moveForwardOptions options){
    lemlib::Pose cPose = chassis.getPose();
    double oX = cPose.x;
    double oY = cPose.y;
    double theta = cPose.theta;
    double thetaRad = theta * (M_PI / 180);

    double dX = inches * std::sin(thetaRad);
    double dY = inches * std::cos(thetaRad);

    double nX = oX + dX;
    double nY = oY + dY;

    chassis.moveToPoint(nX, nY, options.timeout, {.forwards=options.forwards, .maxSpeed=options.maxSpeed, .minSpeed=options.minSpeed, .earlyExitRange=options.earlyExitRange}, false);
    return;
}

void moveBack(float inches, moveBackOptions options){
    lemlib::Pose cPose = chassis.getPose();
    double oX = cPose.x;
    double oY = cPose.y;
    double theta = cPose.theta;
    double thetaRad = theta * (M_PI / 180);

    inches = inches * -1;

    double dX = inches * std::sin(thetaRad);
    double dY = inches * std::cos(thetaRad);

    double nX = oX + dX;
    double nY = oY + dY;

    chassis.moveToPoint(nX, nY, options.timeout, {.forwards=options.forwards, .maxSpeed=options.maxSpeed, .minSpeed=options.minSpeed, .earlyExitRange=options.earlyExitRange}, false);
    return;
}

void intake(){
    Low.move(127);
    Mid.move(-127);
    goalActiveAuton = true;
}

void highGoal(){
    Low.move(127);
    Mid.move(127);
    High.move(127);
    FL.set_brake_mode(E_MOTOR_BRAKE_BRAKE);
    FR.set_brake_mode(E_MOTOR_BRAKE_BRAKE);
    RL.set_brake_mode(E_MOTOR_BRAKE_BRAKE);
    RR.set_brake_mode(E_MOTOR_BRAKE_BRAKE);
    goalActiveAuton = true;
}

void midGoal(){
    Low.move(85);
    Mid.move(127);
    High.move(-85);
    FL.set_brake_mode(E_MOTOR_BRAKE_BRAKE);
    FR.set_brake_mode(E_MOTOR_BRAKE_BRAKE);
    RL.set_brake_mode(E_MOTOR_BRAKE_BRAKE);
    RR.set_brake_mode(E_MOTOR_BRAKE_BRAKE);
    goalActiveAuton = true;
}

void lowGoal(){
    Low.move(-101);
    Mid.move(101);
    FL.set_brake_mode(E_MOTOR_BRAKE_BRAKE);
    FR.set_brake_mode(E_MOTOR_BRAKE_BRAKE);
    RL.set_brake_mode(E_MOTOR_BRAKE_BRAKE);
    RR.set_brake_mode(E_MOTOR_BRAKE_BRAKE);
    goalActiveAuton = true;
}

void ladderOff(){
    Low.move(0);
    Mid.move(0);
    High.move(0);
    Low.brake();
    Mid.brake();
    High.brake();
    goalActiveAuton=false;
}

void pulseLowOn(){
    Low.move(-127);
    goalActiveAuton = true;
}

void midConstant(){
    Mid.move(64);
    goalActiveAuton = true; 
}

void pulseLowOff(){
    Low.move(0);
    Low.brake();
}

void ladderOffPulse(){
    Low.move(0);
    Mid.move(0);
    High.move(0);
    Low.brake();
    Mid.brake();
    High.brake();
    goalActiveAuton=true;
}