#include "main.h"
#include "devices.h"
#include "pros/rtos.hpp"
#include <atomic>
#include <cstdint>

/**
 * Runs initialization code. This occurs as soon as the program is started.
 *
 * All other competition modes are blocked by initialize; it is recommended
 * to keep execution time for this mode under a few seconds.
 */
void initialize() {
	pros::lcd::initialize();
	pros::lcd::set_text(1, "Hello PROS User!");
}

void disabled() {}


void competition_initialize() {}


void autonomous() {}


void opcontrol() {
	// Implement rising edge button detection
	bool prevA = false; 
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

		// TOGGLE PISTON (A)
		bool curA = controller.get_digital(pros::E_CONTROLLER_DIGITAL_A);
		if(curA && !prevA){
			matchLoad.toggle();
		}
		prevA = curA;

		// -- INTAKE CONTROLS -- //
		
		// INTAKE (R1)
		if (controller.get_digital(pros::E_CONTROLLER_DIGITAL_R1)){
			Low.move(127);
			Mid.move(-127);
		}
		// LOW GOAL (R2)
		else if (controller.get_digital(pros::E_CONTROLLER_DIGITAL_R2)){
			Low.move(-127);
			Mid.move(127);
		}
		// MID GOAL (L2)
		else if (controller.get_digital(pros::E_CONTROLLER_DIGITAL_L2)){
			Low.move(127);
			Mid.move(127);
			High.move(-127);
		}
		// HIGH GOAL (L1)
		else if (controller.get_digital(pros::E_CONTROLLER_DIGITAL_L1)){
			Low.move(127);
			Mid.move(127);
			High.move(127);			
		}
		else{
			Low.move(0);
			Mid.move(0);
			High.move(0);
			Low.brake();
			Mid.brake();
			High.brake();
		}
		pros::delay(20);
	}
}