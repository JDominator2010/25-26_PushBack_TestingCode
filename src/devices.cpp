#include "main.h"
#include "pros/misc.h"
#include "pros/misc.hpp"
#include "devices.h"

using namespace pros;

Controller controller(E_CONTROLLER_MASTER);
// -- INTAKE MOTORS -- //
Motor Low(-10, v5::MotorGears::blue, pros::v5::MotorEncoderUnits::deg);
Motor Mid(9, v5::MotorGears::blue, pros::v5::MotorEncoderUnits::deg);
Motor High(-8, v5::MotorGears::blue, pros::v5::MotorEncoderUnits::deg);

// -- DRIVE MOTORS -- //
Motor FL(-10, v5::MotorGears::blue, pros::v5::MotorEncoderUnits::deg);
Motor FR(9, v5::MotorGears::blue, pros::v5::MotorEncoderUnits::deg);
Motor RR(-8, v5::MotorGears::blue, pros::v5::MotorEncoderUnits::deg);
Motor RL(-8, v5::MotorGears::blue, pros::v5::MotorEncoderUnits::deg);
