
#include "devices.h"
#include "lemlib/chassis/chassis.hpp"
#include "main.h" // IWYU pragma: keep
#include "pros/adi.hpp"
#include "pros/imu.hpp"
#include "lemlib/api.hpp" // IWYU pragma: keep
#include "pros/motor_group.hpp"
#include "pros/rotation.hpp"


using namespace pros;
Controller controller(E_CONTROLLER_MASTER);

// -- SENSORS -- //
Imu inertial(0);
Rotation verticalRotation(0);  // PORTS TBD
Rotation horizontalRotation(0);

// -- INTAKE MOTORS -- //
Motor Low(-4, v5::MotorGears::blue, pros::v5::MotorEncoderUnits::deg);
Motor Mid(5, v5::MotorGears::blue, pros::v5::MotorEncoderUnits::deg);
Motor High(-6, v5::MotorGears::blue, pros::v5::MotorEncoderUnits::deg);

// -- DRIVE MOTORS -- //
Motor FL(-7, v5::MotorGears::green, pros::v5::MotorEncoderUnits::deg);
Motor FR(8, v5::MotorGears::green, pros::v5::MotorEncoderUnits::deg);
Motor RR(-9, v5::MotorGears::green, pros::v5::MotorEncoderUnits::deg);
Motor RL(-10, v5::MotorGears::green, pros::v5::MotorEncoderUnits::deg);

// -- LEMLIB DRIVETRAIN SETUP -- //
MotorGroup leftMotorGroup({-7, -10}, pros::MotorGears::green);
MotorGroup rightMotorGroup({8, -9}, pros::MotorGears::green);

lemlib::Drivetrain drivetrain(&leftMotorGroup, // left motor group
                              &rightMotorGroup, // right motor group
                              10, // 10 inch track width
                              lemlib::Omniwheel::NEW_4, // using new 4" omnis
                              360, // drivetrain rpm is 360
                              2 // horizontal drift is 2 (for now)
);

lemlib::TrackingWheel horizontalTracker(&horizontalRotation, lemlib::Omniwheel::NEW_275, -5.75);
lemlib::TrackingWheel verticalTracker(&verticalRotation, lemlib::Omniwheel::NEW_275, -5.75);

lemlib::OdomSensors sensors(&horizontalTracker, // vertical tracking wheel 1, set to null
                            nullptr,
                            &verticalTracker, // horizontal tracking wheel 1
                            nullptr, // horizontal tracking wheel 2, set to nullptr as we don't have a second one
                            &inertial 
);

lemlib::ControllerSettings lateral_controller(10, // proportional gain (kP)
                                              0, // integral gain (kI)
                                              3, // derivative gain (kD)
                                              3, // anti windup
                                              1, // small error range, in inches
                                              100, // small error range timeout, in milliseconds
                                              3, // large error range, in inches
                                              500, // large error range timeout, in milliseconds
                                              20 // maximum acceleration (slew)
);

lemlib::ControllerSettings angular_controller(2, // proportional gain (kP)
                                              0, // integral gain (kI)
                                              10, // derivative gain (kD)
                                              3, // anti windup
                                              1, // small error range, in degrees
                                              100, // small error range timeout, in milliseconds
                                              3, // large error range, in degrees
                                              500, // large error range timeout, in milliseconds
                                              0 // maximum acceleration (slew)
);

lemlib::Chassis chassis(drivetrain, // drivetrain settings
                        lateral_controller, 
                        angular_controller, 
                        sensors
);

// --- ETC MOTORS -- //
Motor Disrupter(3, pros::v5::MotorGears::green, pros::v5::MotorEncoderUnits::deg);

// -- ADI PORTS -- //
adi::Pneumatics matchLoad('A', false);