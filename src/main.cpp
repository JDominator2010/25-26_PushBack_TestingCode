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

/**
 * Runs while the robot is in the disabled state of Field Management System or
 * the VEX Competition Switch, following either autonomous or opcontrol. When
 * the robot is enabled, this task will exit.
 */
void disabled() {}

/**
 * Runs after initialize(), and before autonomous when connected to the Field
 * Management System or the VEX Competition Switch. This is intended for
 * competition-specific initialization routines, such as an autonomous selector
 * on the LCD.
 *
 * This task will exit when the robot is enabled and autonomous or opcontrol
 * starts.
 */
void competition_initialize() {}

/**
 * Runs the user autonomous code. This function will be started in its own task
 * with the default priority and stack size whenever the robot is enabled via
 * the Field Management System or the VEX Competition Switch in the autonomous
 * mode. Alternatively, this function may be called in initialize or opcontrol
 * for non-competition testing purposes.
 *
 * If the robot is disabled or communications is lost, the autonomous task
 * will be stopped. Re-enabling the robot will restart the task, not re-start it
 * from where it left off.
 */
void autonomous() {}

/**
 * Runs the operator control code. This function will be started in its own task
 * with the default priority and stack size whenever the robot is enabled via
 * the Field Management System or the VEX Competition Switch in the operator
 * control mode.
 *
 * If no competition control is connected, this function will run immediately
 * following initialize().
 *
 * If the robot is disabled or communications is lost, the
 * operator control task will be stopped. Re-enabling the robot will restart the
 * task, not resume it from where it left off.
 */

void opcontrol() {
	while (true) {

		// -- DRIVE CODE -- //
		std::int32_t forward = controller.get_analog(pros::E_CONTROLLER_ANALOG_LEFT_Y);
		std::int32_t strafe = controller.get_analog(pros::E_CONTROLLER_ANALOG_LEFT_X);
		std::int32_t turn = controller.get_analog(pros::E_CONTROLLER_ANALOG_RIGHT_X);

		FL.move(strafe + forward + turn);
		FR.move(-strafe + forward - turn);
		RL.move(-strafe + forward + turn);
		RR.move(strafe + forward - turn);



		// INTAKE R1
		if (controller.get_digital(pros::E_CONTROLLER_DIGITAL_R1)){
			Low.move(127);
			Mid.move(-127);
		}
		// Low GOAL R2
		else if (controller.get_digital(pros::E_CONTROLLER_DIGITAL_R2)){
			Low.move(-127);
			Mid.move(127);
		}
		// Mid GOAL L2
		else if (controller.get_digital(pros::E_CONTROLLER_DIGITAL_L2)){
			Low.move(127);
			Mid.move(127);
			High.move(-127);
		}
		// High GOAL L1
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