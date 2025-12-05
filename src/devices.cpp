
#include "devices.h"
#include "lemlib/chassis/chassis.hpp"
#include "lemlib/chassis/trackingWheel.hpp"
#include "main.h" // IWYU pragma: keep
#include "pros/adi.hpp"
#include "pros/imu.hpp"
#include "lemlib/api.hpp" // IWYU pragma: keep
#include "pros/motor_group.hpp"
#include "pros/rotation.hpp"


using namespace pros;
Controller controller(E_CONTROLLER_MASTER);

// -- SENSORS -- //
Imu imu(20);
Rotation verticalRotation(-2);  // PORTS TBD
Rotation horizontalRotation(1);

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
MotorGroup leftMotorGroup({7, 10}, pros::MotorGears::green);
MotorGroup rightMotorGroup({-8, -9}, pros::MotorGears::green);

lemlib::Drivetrain drivetrain(&leftMotorGroup, // left motor group
                              &rightMotorGroup, // right motor group
                              14.5, // 14.5 inch track width
                              lemlib::Omniwheel::OLD_4, // using new 4" omnis
                              200, // drivetrain rpm is 200
                              2 // horizontal drift is 2 (for now)
);

lemlib::TrackingWheel horizontalTracker(&horizontalRotation, lemlib::Omniwheel::NEW_2, 2.25);
lemlib::TrackingWheel verticalTracker(&verticalRotation, lemlib::Omniwheel::NEW_2, 2.25);

lemlib::OdomSensors sensors(&verticalTracker, // vertical tracking wheel 1
                            nullptr,
                            &horizontalTracker, // horizontal tracking wheel 1
                            nullptr, // horizontal tracking wheel 2, set to nullptr as we don't have a second one
                            &imu 
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

lemlib::ControllerSettings angular_controller(7, // proportional gain (kP)
                                              0, // integral gain (kI)
                                              30, // derivative gain (kD)
                                              3, // anti windup
                                              1, // small error range, in inches
                                              100, // small error range timeout, in milliseconds
                                              3, // large error range, in inches
                                              500, // large error range timeout, in milliseconds
                                              0 // maximum acceleration (slew)
);

lemlib::Chassis chassis(drivetrain, // drivetrain settings
                        lateral_controller, 
                        angular_controller, 
                        sensors
);

// -- LEMLIB TARBALL / PURE PURSUIT -- //

ASSET(bluepath_txt);
ASSET(curve_txt);
lemlib_tarball::Decoder blueDecoder(bluepath_txt);
lemlib_tarball::Decoder testingDecoder(curve_txt);

// --- ETC MOTORS -- //
Motor Disrupter(3, pros::v5::MotorGears::green, pros::v5::MotorEncoderUnits::deg);

// -- ADI PORTS -- //
adi::Pneumatics matchLoad('A', false);