#include "main.h"
#include "devices.h"
#include "lemlib/pose.hpp"
#include "liblvgl/llemu.hpp"
#include "pros/misc.h"
#include "pros/motors.h"
#include "pros/rtos.h" // IWYU pragma: keep
#include "pros/rtos.hpp"
#include <algorithm>
#include <atomic> // IWYU pragma: keep
#include <cstdint>
#include <cstdio>
#include <cstring>
#include <ctime>
#include <string>
#include "lemlib/api.hpp" // IWYU pragma: keep
#include "functions.h"

void initialize() {
	pros::lcd::initialize();
	pros::lcd::set_text(1, "Hello PROS User!");
	Disrupter.tare_position();
	chassis.calibrate(true);
	Task windshield(windshieldWiperTask, (void*)"PROS", TASK_PRIORITY_DEFAULT, TASK_STACK_DEPTH_DEFAULT, "Windshield Wiper Task");
	// imu.tare();
	// while (imu.is_calibrating()){
	// 	pros::delay(20);
	// }
    // pros::Task screen_task([=]() {
    //     while (true) {
    //         // print robot location to the brain screen
    //         pros::lcd::print(0, "X: %f", chassis.getPose().x); // x
    //         pros::lcd::print(1, "Y: %f", chassis.getPose().y); // y
    //         pros::lcd::print(2, "Theta: %f", chassis.getPose().theta); // heading
    //         // delay to save resources
    //         pros::delay(20);
    //     }
    // });
}

void disabled() {}

void competition_initialize() {}

void autonomous() {
	bool pulseDir = false;  // false = left, true = right
	uint32_t lastPulseTime = 0;
	const uint32_t pulseInterval = 300; // ms between pulses
	const double leftPos = 15.0;
	const double rightPos = -200.0;
	const double parkTolerance = 8.0;
	const int pulseVel = 150; 

	lemlib::Pose startPos(0, 0, 0);
	chassis.setPose(startPos);

	// BACKUP CODE / RETURN POINT / 12/11/2025
	// intake();
	// moveForward(42, {.async = false}); //14, 10
	// moveBack(18.25);
	// turnToHeading(88);
	// matchLoad.toggle();
	// moveForward(15.5);
	// midGoal();

	intake();
	moveForward(42, {.async = false}); //14, 10
	moveBack(18);
	turnToHeading(88);
	matchLoad.toggle();
	delay(250);
	moveForward(15.5);
	midGoal();





	// long start = millis();
	// long end = start + 5000;

	// double currentPos = Disrupter.get_position();

	// while (millis() <= end) {
	// 	uint32_t now = pros::millis();
	// 	if (now - lastPulseTime >= pulseInterval) {
	// 		pulseDir = !pulseDir;
	// 		lastPulseTime = now;
	// 		Disrupter.move_absolute(pulseDir ? leftPos : rightPos, pulseVel);
	// 	}
	// } 

	// bool inLeftRange = fabs(currentPos - leftPos) <= parkTolerance;

	// if (!inLeftRange) {
	// 	// move to left park position
	// 	Disrupter.move_absolute(leftPos, pulseVel);
	// } else {
	// 	// once parked properly on the left, hold it there (no coasting)
	// 	Disrupter.set_brake_mode(pros::E_MOTOR_BRAKE_HOLD);
	// 	Disrupter.move_velocity(0);
	// }

    // while (true) { // infinite loop
	// 	pros::lcd::print(1, "Horizontal Rotation Sensor: %f", horizontalTracker.getDistanceTraveled());
	// 	pros::lcd::print(2, "Vertical Rotation Sensor: %f", verticalTracker.getDistanceTraveled());
	// 	printf("Horizontal Rotation Sensor: %f, Vertical Rotation Sensor: %f\n",
	// 		   horizontalTracker.getDistanceTraveled(), verticalTracker.getDistanceTraveled());
	// 	printf("\033[2J\033[H"); // ANSI clear screen and move cursor home
	// 	fflush(stdout);
    //     pros::delay(10); // delay to save resources. DO NOT REMOVE
    // }

	// pros::lcd::clear();
	// printf("Autonomous Start\n");
	// std::string msg = "X: " + std::to_string(chassis.getPose().x) + " Y: " + std::to_string(chassis.getPose().y) + " Theta: " + std::to_string(chassis.getPose().theta);
	// printf("%s\n", msg.c_str());
	// pros::lcd::set_text(1, "X: " + std::to_string(chassis.getPose().x) + " Y: " + std::to_string(chassis.getPose().y) + " Theta: " + std::to_string(chassis.getPose().theta));
	// printf("Moving to point...\n");

	// chassis.moveToPoint(0, 48, 4000, {}, false);
	// chassis.turnToHeading(180, 4000);
	// chassis.moveToPoint(0, 24, 4000, {}, false);
	// chassis.moveToPose(0, 48, 0, 4000);
	// chassis.turnToHeading(90, 100000);


	// printf("done with move\n");
	// pros::lcd::set_text(2, msg.c_str());
	// printf("%s\n", msg.c_str());
	// printf("Autonomous End\n");
}


void opcontroldebug(){
	controller.clear();
	int lookahead = 1;
	controller.print(0, 0, "Lookahead: %i", lookahead);
	bool lastUp = false;
	bool lastDown = false;
	bool lastA = false;
	bool lastB = false;

	chassis.setPose(0, -24, 0);

	while (true) {
		bool upPressed   = controller.get_digital(pros::E_CONTROLLER_DIGITAL_UP);
		bool downPressed = controller.get_digital(pros::E_CONTROLLER_DIGITAL_DOWN);
		bool aPressed    = controller.get_digital(pros::E_CONTROLLER_DIGITAL_A);
		bool bPressed    = controller.get_digital(pros::E_CONTROLLER_DIGITAL_B);
		
		if (upPressed && !lastUp) {
			lookahead += 1;
			controller.print(0, 0, "Lookahead: %i", lookahead);
		}
		if (downPressed && !lastDown) {
			if (lookahead > 0){
				lookahead -= 1;
				
			}
			controller.print(0, 0, "Lookahead: %i", lookahead);
		}
		if (aPressed && !lastA) {
			chassis.follow(testingDecoder["Path"], lookahead, 50000, true, false);
		}
		if (bPressed && !lastB) {
			chassis.cancelAllMotions();
			chassis.setPose(0,0,0);
		}
		lastUp = upPressed;
		lastDown = downPressed;
		lastA = aPressed;
		lastB = bPressed;
		pros::delay(20);
}

}

void opcontrol() {
	bool prevA = false; 
	bool pulseDir = false;  // false = left, true = right
	uint32_t lastPulseTime = 0;
	const uint32_t pulseInterval = 300; // ms between pulses
	const double leftPos = 10.0;
	const double rightPos = -200.0;
	const double parkTolerance = 8.0;
	const int pulseVel = 125; 

	while (true) {
		// -- DRIVE CODE -- //
		std::int32_t forward = controller.get_analog(pros::E_CONTROLLER_ANALOG_LEFT_Y);
		std::int32_t strafe = controller.get_analog(pros::E_CONTROLLER_ANALOG_LEFT_X);
		std::int32_t turn = controller.get_analog(pros::E_CONTROLLER_ANALOG_RIGHT_X);
		FL.move(-strafe - forward +-turn);
		FR.move(+strafe - forward + turn);
		RL.move(strafe - forward - turn);
		RR.move(strafe + forward - turn);

		// -- MATCH LOADER CONTROLS -- //
		bool curA = controller.get_digital(pros::E_CONTROLLER_DIGITAL_A);
		if (curA && !prevA) {
			matchLoad.toggle();
		}
		prevA = curA; 

		// -- INTAKE CONTROLS -- //
		bool R1 = controller.get_digital(pros::E_CONTROLLER_DIGITAL_R1);
		bool R2 = controller.get_digital(pros::E_CONTROLLER_DIGITAL_R2);
		bool L1 = controller.get_digital(pros::E_CONTROLLER_DIGITAL_L1);
		bool L2 = controller.get_digital(pros::E_CONTROLLER_DIGITAL_L2);

		if (R1) {
			Low.move(127);
			Mid.move(-127);
		}
		else if (R2) {
			Low.move(-127);
			Mid.move(127);
			FL.set_brake_mode(E_MOTOR_BRAKE_BRAKE);
			FR.set_brake_mode(E_MOTOR_BRAKE_BRAKE);
			RL.set_brake_mode(E_MOTOR_BRAKE_BRAKE);
			RR.set_brake_mode(E_MOTOR_BRAKE_BRAKE);
		}
		else if (L2 && matchLoad.is_extended()) {
			Low.move(85);
			Mid.move(127);
			High.move(-85);
			FL.set_brake_mode(E_MOTOR_BRAKE_BRAKE);
			FR.set_brake_mode(E_MOTOR_BRAKE_BRAKE);
			RL.set_brake_mode(E_MOTOR_BRAKE_BRAKE);
			RR.set_brake_mode(E_MOTOR_BRAKE_BRAKE);
		}
		else if (L1) {
			Low.move(127);
			Mid.move(127);
			High.move(127);
			FL.set_brake_mode(E_MOTOR_BRAKE_BRAKE);
			FR.set_brake_mode(E_MOTOR_BRAKE_BRAKE);
			RL.set_brake_mode(E_MOTOR_BRAKE_BRAKE);
			RR.set_brake_mode(E_MOTOR_BRAKE_BRAKE);
		}
		else {
			Low.move(0);
			Mid.move(0);
			High.move(0);
			Low.brake();
			Mid.brake();
			High.brake();
			FL.set_brake_mode(E_MOTOR_BRAKE_COAST);
			FR.set_brake_mode(E_MOTOR_BRAKE_COAST);
			RL.set_brake_mode(E_MOTOR_BRAKE_COAST);
			RR.set_brake_mode(E_MOTOR_BRAKE_COAST);
		}

		// -- DISRUPTER CONTROL -- //
		bool goalActive = (R2 || L1 || L2) && !R1;
		double currentPos = Disrupter.get_position();

		if (goalActive) {
			uint32_t now = pros::millis();
			if (now - lastPulseTime >= pulseInterval) {
				pulseDir = !pulseDir;
				lastPulseTime = now;
				Disrupter.move_absolute(pulseDir ? leftPos : rightPos, pulseVel);
			}
		} 
		else {
			// always park on the left side when idle
			bool inLeftRange = fabs(currentPos - leftPos) <= parkTolerance;

			if (!inLeftRange) {
				// move to left park position
				Disrupter.move_absolute(leftPos, pulseVel);
			} else {
				// once parked properly on the left, hold it there (no coasting)
				Disrupter.set_brake_mode(pros::E_MOTOR_BRAKE_HOLD);
				Disrupter.move_velocity(0);
			}
		}

		pros::delay(20);
	}
}
