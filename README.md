# 25-26_PushBack_TestingCode

A PROS-based VEX V5 project containing robot device definitions and a basic teleoperated control loop. This README explains the repository structure, code logic, device mappings, build/run instructions, dependencies, tests, and suggestions for next steps.

---

## Table of Contents

- Project Overview
- Repository Layout
- Key Dependencies and Libraries
- Hardware & Device Mapping (from `src/devices.cpp`)
- Code Walkthrough
  - `initialize()`
  - `opcontrol()` (teleop)
- Drive and Intake Logic Details
- Build, Upload and Debugging
- Quality Gates, Tests, and Validation
- Common Issues & Troubleshooting
- Next Steps and Suggested Improvements
- Contributing
- License

---

## Project Overview

This project is a VEX V5 robot firmware project built with PROS. It defines the hardware devices (motors, controller, pneumatics) in `src/devices.cpp` and implements initialization and teleoperation behavior in `src/main.cpp`.

The code implements:
- A basic initialization sequence that sets up the LCD.
- An `opcontrol()` loop that reads the controller inputs and drives the robot while operating an intake subsystem and a pneumatic match loader.

The project also includes LVGL headers under `include/liblvgl` (a graphics UI library often used for displays), though the present source files use only `pros::lcd` for a minimal UI.

---

## Repository Layout (important files and folders)

- `src/`
  - `main.cpp`      — Core competition functions: `initialize`, `opcontrol`, etc.
  - `devices.cpp`   — Global device objects: motors, controller, pneumatics.
- `include/`        — Project headers and included libraries (e.g., `liblvgl`)
- `project.pros`     — PROS project definition
- `Makefile`, `common.mk` — Build orchestration files (PROS uses `make`)
- `bin/`, `firmware/` — Build artifacts and linker files

Refer to these files when changing wiring, motor ports, or build configuration.

---

## Key Dependencies and Libraries

- PROS (C++ API for VEX V5): used across `pros::v5` types, `pros::Controller`, `pros::Motor`, `pros::adi::Pneumatics`, `pros::lcd`, and `pros::delay`.
- LVGL (Light and Versatile Graphics Library): headers included in `include/liblvgl` — present for UI/graphics support; not used heavily in the current code but available if you expand display functionality.

Note: The project uses the v5 API namespace (e.g., `pros::v5::MotorEncoderUnits::deg`) indicating v5 motor and encoder units.

---

## Hardware & Device Mapping

Device definitions are in `src/devices.cpp`. Current mappings (as declared in device objects):

- Controller
  - `Controller controller(E_CONTROLLER_MASTER);` — Master controller

- Intake motors
  - `Motor Low`  — port -13 (reversed), blue gearing (high torque)
  - `Motor Mid`  — port 12, blue gearing
  - `Motor High` — port -11 (reversed), blue gearing

- Drive motors
  - `Motor FL` — front-left: port -16 (reversed), green gears
  - `Motor FR` — front-right: port 17, green gears
  - `Motor RL` — rear-left: port -19 (reversed), green gears
  - `Motor RR` — rear-right: port -20 (reversed), green gears

- Pneumatics
  - `adi::Pneumatics matchLoad('A', false);` — ADI port A, initial state false

Comments about ports and signs:
- Negative port numbers (e.g., `-13`) indicate motor direction inversion applied at construction. If the robot moves opposite to expectation, flip the sign here or use `motor.set_reversed(true)`.

---

## Code Walkthrough

Files to read: `src/main.cpp`, `src/devices.cpp`.

### `initialize()`
- Called once at program start. It initializes the PROS LCD and writes a greeting.
- Keep `initialize()` fast; it blocks other modes until complete.

### `opcontrol()`
- Main teleop loop; runs continuously.
- Reads controller analog sticks and digital buttons.
- Drive input mapping:
  - `forward` = left stick Y (E_CONTROLLER_ANALOG_LEFT_Y)
  - `strafe`  = left stick X (E_CONTROLLER_ANALOG_LEFT_X)
  - `turn`    = right stick X (E_CONTROLLER_ANALOG_RIGHT_X)
- Motor mixing (drive math):
  - FL: `-strafe - forward +-turn`  (note: code contains `+-turn` token; intent is to include turn term with sign)
  - FR: `+strafe - forward + turn`
  - RL: `strafe - forward - turn`
  - RR: `strafe + forward - turn`

  This mixing combines forward/back, strafing, and rotation. The pattern indicates a holonomic/mecanum-style drive mixing, where strafing and rotation are combined per wheel with specific signs to achieve strafing and turning.

- Intake & goal control (buttons):
  - `A` toggles the `matchLoad` pneumatic via `matchLoad.toggle()`.
  - `R1` spins `Low` and `Mid` to intake (Low: +127, Mid: -127).
  - `R2` (Low goal) spins `Low` and `Mid` in the opposite directions.
  - `L2` (Mid goal) runs Low and Mid forward and High reversed.
  - `L1` (High goal) runs Low, Mid, High forward.
  - Default (no button): zero motor drive and call `.brake()` on intake motors to hold position.

- Loop delay of 20 ms (`pros::delay(20)`) to yield CPU.

---

## Drive and Intake Logic Details

- Drive mixing seems tailored for a 4-motor holonomic or mecanum chassis. If your robot uses tank drive, adapt mixing accordingly.
- Motor signs in `devices.cpp` indicate initial orientation; test in a safe environment and invert signs as needed.
- Intake logic uses simple full-power (127/-127) control; you can replace with ramped speed, PID, or velocity-control for smoother behavior.

Edge cases to consider:
- Controller bounce (button held across loops) — the code checks `get_digital()` each loop, and actions like pneumatic toggle will toggle every loop while pressed; consider edge detection (toggle only on transition from not-pressed to pressed) to prevent repeated toggles.
- Motor brake behaviour: code calls `.brake()` after setting zero — that's fine, but you might prefer `coast()` for less abrupt stopping if required.

---

## Build, Upload and Debugging

This is a PROS project. Typical steps on Windows (PowerShell):

```powershell
# Build the project
make

# Optionally clean
make clean

# Upload/flash to the V5 Brain (if configured)
# Many PROS setups support `make upload` or `make flash` — check your PROS toolchain and project config.
make upload
```

If you prefer a GUI, use the PROS Dashboard / PROS CLI tools available from the PROS documentation. Use the PROS Terminal to observe console prints or to interact with the robot during testing.

Debugging tips:
- Insert `pros::lcd::set_text()` or `printf()` statements to show controller values or motor outputs.
- Use `pros::delay()` in long-running loops to avoid hogging CPU.

---

## Quality Gates, Tests, and Validation

Suggested quick tests (manual):
- Build sanity: `make` should complete without errors.
- Motor direction test: With robot safely elevated, apply single-direction commands and observe wheel rotation; adjust sign in `devices.cpp` if necessary.
- Intake behavior: Press each intake button and verify the expected motors spin and direction matches the intended goal.
- Pneumatics: Press `A` and confirm the ADI channel toggles the piston state once per press (consider adding edge-detect code if toggle repeats).

Automated tests: PROS does not have a standardized unit-test harness out of the box for hardware integration; create small software-only unit tests for helper utilities, or mock device interfaces if you adopt a testing framework.

---

## Common Issues & Troubleshooting

- Unexpected wheel directions: change sign of port in `devices.cpp` (e.g., `-16` -> `16`) or set motor reversed at runtime.
- Repeated pneumatic toggles while holding `A`: implement a button-edge detector to toggle only on state change.
- Stuttering drive or jitter: ensure `pros::delay(20)` is present; tune loop delay and consider using velocity-based motor commands.

---

## Next Steps and Suggested Improvements

- Add a `buttonPressed()` edge-detect helper for clean toggles:
  - Track previous and current button states and only toggle on rising edge.
- Replace raw `move()` calls for intake with velocity control or closed-loop control for consistent shot power.
- Extract drive mixing into a separate `drive.setVelocity()` API with deadband and scaling.
- Add an autonomous routine in `autonomous()`.
- Add documentation for wiring and a small wiring diagram image in `docs/`.

---

## Contributing

Feel free to open issues or PRs. Please:
- Describe hardware changes (motor ports, gearing) in PRs.
- Keep `initialize()` short and deterministic.
- Add small unit tests for any new helper logic where practical.

---

## License

Add your project license here (e.g., MIT, Apache-2.0). If you don't choose one, many contributors and collaborators may be hesitant to reuse the code.

---

## Appendix: Quick Reference (from current code)

- Controller axes:
  - Left Y = forward/back
  - Left X = strafe
  - Right X = turning
- Intake buttons:
  - `R1` intake
  - `R2` low goal
  - `L2` mid goal
  - `L1` high goal
  - `A` toggle pneumatic

---

If you'd like, I can also:
- Update the code to add edge-detection for the pneumatic toggle.
- Add a small wiring diagram or a `docs/` page with port mapping and a sketch.
- Implement velocity-based intake control or ramping for smoother operation.

Tell me which of the above you'd like me to do next and I'll proceed.
