
#include "devices.h"
#include "lemlib/chassis/chassis.hpp"
#include "lemlib/chassis/trackingWheel.hpp"
#include "main.h" // IWYU pragma: keep
#include "pros/adi.hpp"
#include "pros/imu.hpp"
#include "lemlib/api.hpp" // IWYU pragma: keep
#include "pros/motor_group.hpp"
#include "pros/rotation.hpp"
#include "robodash/views/console.hpp"
#include "robodash/views/selector.hpp"
#include "liblvgl/lvgl.h"
#include "autons.h"

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
Motor FL(-7, v5::MotorGears::red, pros::v5::MotorEncoderUnits::deg);
Motor FR(8, v5::MotorGears::red, pros::v5::MotorEncoderUnits::deg);
Motor RR(-9, v5::MotorGears::red, pros::v5::MotorEncoderUnits::deg);
Motor RL(-10, v5::MotorGears::red, pros::v5::MotorEncoderUnits::deg);

// -- LEMLIB DRIVETRAIN SETUP -- //
MotorGroup leftMotorGroup({7, 10}, pros::MotorGears::red);
MotorGroup rightMotorGroup({-8, -9}, pros::MotorGears::red);

lemlib::Drivetrain drivetrain(&leftMotorGroup, // left motor group
                              &rightMotorGroup, // right motor group
                              14.25, // 14.5 inch track width
                              4.0982398, // using new 4" omnis
                              200, // drivetrain rpm is 200
                              2 // horizontal drift is 2 (for now)
);

lemlib::TrackingWheel horizontalTracker(&horizontalRotation, 2, 2.75);
lemlib::TrackingWheel verticalTracker(&verticalRotation, 2, 3.125);

lemlib::OdomSensors sensors(&verticalTracker, // vertical tracking wheel 1
                            nullptr,
                            &horizontalTracker, // horizontal tracking wheel 1
                            nullptr, // horizontal tracking wheel 2, set to nullptr as we don't have a second one
                            &imu 
);

lemlib::ControllerSettings lateral_controller(11, // proportional gain (kP)
                                              0.05, // integral gain (kI)
                                              6, // derivative gain (kD)
                                              3, // anti windup
                                              1, // small error range, in inches
                                              200, // small error range timeout, in milliseconds
                                              1.5, // large error range, in inches
                                              500, // large error range timeout, in milliseconds
                                              20 // maximum acceleration (slew)
);

// lemlib::ControllerSettings angular_controller(7, // proportional gain (kP)
//                                               0, // integral gain (kI)
//                                               30, // derivative gain (kD)
//                                               3, // anti windup
//                                               1, // small error range, in inches
//                                               100, // small error range timeout, in milliseconds
//                                               3, // large error range, in inches
//                                               500, // large error range timeout, in milliseconds
//                                               0 // maximum acceleration (slew)
// );

lemlib::ControllerSettings angular_controller(10, // proportional gain (kP)
                                              0, // integral gain (kI)
                                              41, // derivative gain (kD)
                                              0, // anti windup
                                              .5, // small error range, in inches
                                              100, // small error range timeout, in milliseconds
                                              1.5, // large error range, in inches
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

// -- AUTON SELECTOR -- //
rd::Selector selector({
    {"Red High", redHigh},
    {"Blue High", blueHigh},
    {"Skills Auton", skillsAuton}
});

std::string selectedAutonName = "";

// -- IMAGES -- //
rd::Image appaR("appaP.bin", "appaP");
rd::Image appaB("appaS.bin", "appaS");

// -- RD CONSOLE -- //
rd::Console console("Console");