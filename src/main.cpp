#include "main.h"
#include "devices.h"
#include "lemlib/chassis/chassis.hpp"
#include "lemlib/pose.hpp"
#include "pros/abstract_motor.hpp"
#include "pros/adi.h"
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
#include "robodash/api.h"
#include "autons.h"
#include "robodash/views/image.hpp"
#include "robodash/views/selector.hpp"
#include "liblvgl/lvgl.h"


pros::Task windshield(windshieldWiperTask, (void*)"PROS", TASK_PRIORITY_DEFAULT, TASK_STACK_DEPTH_DEFAULT, "Windshield Wiper Task");



void initialize() {
	// pros::lcd::initialize();
	// pros::lcd::set_text(1, "Jasmine Dragons | 95872B");
	Disrupter.tare_position();
	chassis.calibrate(true);
	selector.on_select([](std::optional<rd::Selector::routine_t> sel) {
	if (sel.has_value()) {
		selectedAutonName = sel->name; // store the name
		console.printf("Selected: %s", selectedAutonName.c_str());
	} else {
		selectedAutonName.clear();     // deselected
	}
	});

	// FL.set_brake_mode(pros::E_MOTOR_BRAKE_BRAKE);
	// FR.set_brake_mode(pros::E_MOTOR_BRAKE_BRAKE);
	// RR.set_brake_mode(pros::E_MOTOR_BRAKE_BRAKE);
	// RL.set_brake_mode(pros::E_MOTOR_BRAKE_BRAKE);
	// imu.tare();;
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

void disabled() {
	ladderOff();
}

void competition_initialize() {}

void autonomous() {
	lemlib::Pose startPos(0, 0, 0);
	chassis.setPose(startPos);
	selector.run_auton();


	// intake();
	// chassis.moveToPoint(6, 38, 4000, {}, false);
	// chassis.moveToPoint(chassis.getPose().x, 3, 4000, {.minSpeed=100}, false);
	// printf("Reached point 1");
	// chassis.moveToPose(-1.5, 20, 33, 1500, {.lead=0.1}, false);
	// printf("Reached point 2");
	// chassis.moveToPoint(4.5, 29.2, 2000, {.maxSpeed=35}, false);	
	// printf("Reached point 3");
	// chassis.moveToPose(-0.4, 41.4, 315, 4000, {.lead=0.1}, false);
	// float start = millis();
	// chassis.moveToPose(-10, 24, 0, 40000, {.lead=0.1}, false);
	// printf("Time to shoot: %f\n", millis() - start);
	// chassis.turnToHeading(90, 4000, {}, false);
	// printf("Time: %f\n", millis() - start);
	// chassis.moveToPoint(0, chassis.getPose().y, 4000, {}, false);
	// printf("Time: %f\n", millis() - start);
	// ladderOff();


	// float start = millis();

	// chassis.moveToPoint(0, 16, 4000, {}, false);
	// chassis.turnToHeading(33, 4000, {}, false);
	// printf("X: %f, Y: %f, Theta: %f\n", chassis.getPose().x, chassis.getPose().y, chassis.getPose().theta);
	// moveForward(11.3, {.maxSpeed=32, .async=false});	
	// chassis.turnToHeading(315, 400, {}, false);
	// chassis.moveToPose(-0.547, 36.658, 315, 1000, {.lead=0.1}, false); //-1.784, 39.309
	// lowGoal();
	// delay(2000);
	// chassis.moveToPoint(18.722, 18.803, 4000, {.forwards=false}, false);
	// moveBack(44.5);
	// chassis.turnToPoint(26, 0, 3000, {}, false);
	// chassis.turnToHeading(180.02, 4000, {}, false);
	// chassis.moveToPose(28.5, 6, 180, 3000, {.lead=0.1}, false);
	// intake();
	// ladderOff();
	// matchLoad.extend();
	// delay(300);
	// // moveForward(11);
	// chassis.moveToPoint(29.626, 2.1, 4000, {}, false);
	// moveBack(1);
	// moveForward(1);
	// delay(550);
	// moveBack(20.5);
	// ladderOff();
	// matchLoad.retract();
	// chassis.turnToHeading(0.09, 4000, {}, false);
	// chassis.moveToPose(26.908, 19.149, 0, 4000, {.lead=0.01}, false);
	// moveForward(3.808);
	// highGoal();
	// delay(1270);
	// ladderOff();
	// chassis.turnToPoint(28.125, 17.50, 4000, {}, false);
	// chassis.moveToPose(28.125, 17.50, 0, 4000, {.lead=0.1}, false);
	// highGoal();

	// printf("time: %f", millis() - start);

	// controller.print(0, 0, "time: %f", millis() - start);


	// chassis.turnToPoint(-1.7, 42, 4000, {}, false);
	// chassis.moveToPoint(-1.7, 42, 4000, {}, false);
}


void opcontroldebug(){
	controller.clear();
	// int lookahead = 1;
	controller.print(0, 0, "curSel kP: %i", angular_controller.kP);
	bool lastUp = false;
	bool lastDown = false;
	bool lastRight = false;
	bool lastA = false;
	bool lastB = false;
	std::string curSel = "kp";
	chassis.setPose(0, -24, 0);

	while (true) {
		bool upPressed   = controller.get_digital(pros::E_CONTROLLER_DIGITAL_UP);
		bool downPressed = controller.get_digital(pros::E_CONTROLLER_DIGITAL_DOWN);
		bool rightPressed = controller.get_digital(pros::E_CONTROLLER_DIGITAL_RIGHT);
		bool aPressed    = controller.get_digital(pros::E_CONTROLLER_DIGITAL_A);
		bool bPressed    = controller.get_digital(pros::E_CONTROLLER_DIGITAL_B);
		
		if (curSel == "kp"){
			controller.print(0, 0, "curSel kP: %f", angular_controller.kP);
		}
		else if (curSel == "kd"){
			controller.print(0, 0, "curSel kD: %f", angular_controller.kD);
		}

		if (upPressed && !lastUp){
			if (curSel == "kp"){
				angular_controller.kP += .5;
			}
			else if (curSel == "kd"){
				angular_controller.kD += .5;
			}
		}
		if (downPressed && !lastDown){
			if (curSel == "kp"){
				angular_controller.kP -= .5;
			}
			else if (curSel == "kd"){
				angular_controller.kD -= .5;
			}
		}

		if (rightPressed && !lastRight){
			if (curSel == "kp"){
				curSel = "kd";
			}
			else if (curSel == "kd"){
				curSel = "kp";
			}
		}

		if (aPressed && !lastA){
			chassis.setPose(0,0,0);
			chassis.turnToHeading(270, 10000);
		}

		lastDown = downPressed;
		lastUp = upPressed;
		lastRight = rightPressed;
		lastA = aPressed;
		pros::delay(20);
	}

}

void opcontrol() {
	bool prevA = false; 
	bool pulseDir = false;  // false = left, true = right
	bool prevDown = false;
	uint32_t lastPulseTime = 0;
	const uint32_t pulseInterval = 300; // ms between pulses
	const double leftPos = 0.0;
	const double rightPos = -180.0;
	const double parkTolerance = 8.0;
	const float pulseVel = 127; 
	
	windshield.remove();

	if (selectedAutonName.find("Red") != std::string::npos){
		appaR.focus();
	}
	else if (selectedAutonName.find("Blue") != std::string::npos){
		appaB.focus();
	}
	

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

		bool down = controller.get_digital(pros::E_CONTROLLER_DIGITAL_DOWN);

		if (R1) {
			Low.move(127);
			Mid.move(-127);
		}
		else if (R2) {
			Low.move(-127);
			Mid.move(127);
			FL.set_brake_mode(pros::E_MOTOR_BRAKE_BRAKE);
			FR.set_brake_mode(pros::E_MOTOR_BRAKE_BRAKE);
			RL.set_brake_mode(pros::E_MOTOR_BRAKE_BRAKE);
			RR.set_brake_mode(pros::E_MOTOR_BRAKE_BRAKE);
		}
		else if (L2 && matchLoad.is_extended()) {
			Low.move(85);
			Mid.move(127);
			High.move(-85);
			FL.set_brake_mode(pros::E_MOTOR_BRAKE_BRAKE);
			FR.set_brake_mode(pros::E_MOTOR_BRAKE_BRAKE);
			RL.set_brake_mode(pros::E_MOTOR_BRAKE_BRAKE);
			RR.set_brake_mode(pros::E_MOTOR_BRAKE_BRAKE);
		}
		else if (L1) {
			Low.move(127);
			Mid.move(127);
			High.move(127);
			FL.set_brake_mode(pros::E_MOTOR_BRAKE_BRAKE);
			FR.set_brake_mode(pros::E_MOTOR_BRAKE_BRAKE);
			RL.set_brake_mode(pros::E_MOTOR_BRAKE_BRAKE);
			RR.set_brake_mode(pros::E_MOTOR_BRAKE_BRAKE);
		}
		else {
			Low.move(0);
			Mid.move(0);
			High.move(0);
			Low.brake();
			Mid.brake();
			High.brake();
			FL.set_brake_mode(E_MOTOR_BRAKE_BRAKE);
			FR.set_brake_mode(pros::E_MOTOR_BRAKE_BRAKE);
			RL.set_brake_mode(pros::E_MOTOR_BRAKE_BRAKE);
			RR.set_brake_mode(pros::E_MOTOR_BRAKE_BRAKE);
		}

		// -- QUICK TURNS -- //
		if (down && !prevDown){
			quickTurn180();
		}

		// -- DISRUPTER CONTROL -- //
		bool goalActive = (R2 || L1 || L2 || R1);
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
				Disrupter.set_brake_mode(pros::E_MOTOR_BRAKE_BRAKE);
				Disrupter.move_velocity(0);
			}
		}
		prevDown = down;
		pros::delay(20);
	}
}
