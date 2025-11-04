#include "main.h"
#include "pros/adi.hpp"
#include "pros/misc.h"
#include "pros/misc.hpp"
#include "devices.h"

using namespace pros;

Controller controller(E_CONTROLLER_MASTER);
// -- INTAKE MOTORS -- //
Motor Low(-13, v5::MotorGears::blue, pros::v5::MotorEncoderUnits::deg);
Motor Mid(12, v5::MotorGears::blue, pros::v5::MotorEncoderUnits::deg);
Motor High(-11, v5::MotorGears::blue, pros::v5::MotorEncoderUnits::deg);

// -- DRIVE MOTORS -- //
Motor FL(-16, v5::MotorGears::green, pros::v5::MotorEncoderUnits::deg);
Motor FR(17, v5::MotorGears::green, pros::v5::MotorEncoderUnits::deg);
Motor RR(-20, v5::MotorGears::green, pros::v5::MotorEncoderUnits::deg);
Motor RL(-19, v5::MotorGears::green, pros::v5::MotorEncoderUnits::deg);

adi::Pneumatics matchLoad('h', false);