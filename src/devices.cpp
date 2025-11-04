#include "main.h"
#include "pros/adi.hpp"
#include "pros/imu.hpp"
#include "pros/misc.h"
#include "pros/misc.hpp"
#include "devices.h"

using namespace pros;

Controller controller(E_CONTROLLER_MASTER);

// -- SENSORS -- //
Imu inertial(0);

// -- INTAKE MOTORS -- //
Motor Low(-4, v5::MotorGears::blue, pros::v5::MotorEncoderUnits::deg);
Motor Mid(5, v5::MotorGears::blue, pros::v5::MotorEncoderUnits::deg);
Motor High(-6, v5::MotorGears::blue, pros::v5::MotorEncoderUnits::deg);

// -- DRIVE MOTORS -- //
Motor FL(-7, v5::MotorGears::green, pros::v5::MotorEncoderUnits::deg);
Motor FR(8, v5::MotorGears::green, pros::v5::MotorEncoderUnits::deg);
Motor RR(-9, v5::MotorGears::green, pros::v5::MotorEncoderUnits::deg);
Motor RL(-10, v5::MotorGears::green, pros::v5::MotorEncoderUnits::deg);


// -- ADI PORTS -- //
adi::Pneumatics matchLoad('A', false);