#include "main.h"
#include "devices.h"
#include "pros/motors.h"
#include "pros/rtos.hpp"
#include <atomic> // IWYU pragma: keep
#include <cstdint>


void initialize() {
	pros::lcd::initialize();
	pros::lcd::set_text(1, "Hello PROS User!");
	Disrupter.tare_position();
}

void disabled() {}

void competition_initialize() {}

void autonomous() {}

void opcontrol() {
	bool prevA = false; 
	bool pulseDir = false;  // false = left, true = right
	uint32_t lastPulseTime = 0;
	const uint32_t pulseInterval = 300; // ms between pulses
	const double leftPos = 15.0;
	const double rightPos = -900.0;
	const double parkTolerance = 8.0;
	const int pulseVel = 200; 

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
		else if (L2) {
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
