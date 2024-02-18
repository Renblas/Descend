#include "cghs/cghs.h"


/* ========================================================================== */
/*                                 Initialize                                 */
/* ========================================================================== */
void initialize() {

	pros::delay(500); // Stop the user from doing anything while legacy ports configure

	// Configure your chassis controls
	chassis.opcontrol_curve_buttons_toggle(true); // Enables modifying the controller curve with buttons on the joysticks
	chassis.opcontrol_drive_activebrake_set(0); // Sets the active brake kP. We recommend 0.1.
	chassis.opcontrol_curve_default_set(0, 0); // Defaults for curve. If using tank, only the first parameter is used. (Comment this line out if you have an SD card!)  
	default_constants(); // Set the drive to your own constants from autons.cpp!

	// These are already defaulted to these buttons, but you can change the left/right curve buttons here!
	// chassis.opcontrol_curve_buttons_left_set (pros::E_CONTROLLER_DIGITAL_LEFT, pros::E_CONTROLLER_DIGITAL_RIGHT); // If using tank, only the left side is used. 
	// chassis.opcontrol_curve_buttons_right_set(pros::E_CONTROLLER_DIGITAL_Y,    pros::E_CONTROLLER_DIGITAL_A);

	// Autonomous Selector using LLEMU
	ez::as::auton_selector.autons_add({
	  Auton("Example Drive\n\nDrive forward and come back.", drive_example),
	  Auton("Example Turn\n\nTurn 3 times.", turn_example),
	  Auton("Drive and Turn\n\nDrive forward, turn, come back. ", drive_and_turn),
	  Auton("Drive and Turn\n\nSlow down during drive.", wait_until_change_speed),
	  Auton("Swing Example\n\nSwing in an 'S' curve", swing_example),
	  Auton("Combine all 3 movements", combining_movements),
	  Auton("Interference\n\nAfter driving forward, robot performs differently if interfered or not.", interfered_example),
		});

	// Initialize chassis and auton selector
	chassis.initialize();
	ez::as::initialize();
	master.rumble(".");
}


/* ========================================================================== */
/*                                  Disabled                                  */
/* ========================================================================== */
void disabled() {
	// . . .
}


/* ========================================================================== */
/*                           Competition Initialize                           */
/* ========================================================================== */
void competition_initialize() {
	// . . .
}


/* ========================================================================== */
/*                                 Autonomous                                 */
/* ========================================================================== */
void autonomous() {
	chassis.pid_targets_reset(); // Resets PID targets to 0
	chassis.drive_imu_reset(); // Reset gyro position to 0
	chassis.drive_sensor_reset(); // Reset drive sensors to 0
	chassis.drive_brake_set(MOTOR_BRAKE_HOLD); // Set motors to hold.  This helps autonomous consistency

	ez::as::auton_selector.selected_auton_call(); // Calls selected auton from autonomous selector
}


/* ========================================================================== */
/*                                User Control                                */
/* ========================================================================== */
void opcontrol() {

	// This is preference to what you like to drive on
	chassis.drive_brake_set(MOTOR_BRAKE_COAST);

	while (true) {

		// PID Tuner
		// After you find values that you're happy with, you'll have to set them in auton.cpp
		if (!pros::competition::is_connected()) {
			// Enable / Disable PID Tuner
			//  When enabled: 
			//  * use A and Y to increment / decrement the constants
			//  * use the arrow keys to navigate the constants
			if (master.get_digital_new_press(DIGITAL_X))
				chassis.pid_tuner_toggle();

			// Trigger the selected autonomous routine
			if (master.get_digital_new_press(DIGITAL_B))
				autonomous();

			chassis.pid_tuner_iterate(); // Allow PID Tuner to iterate
		}




		chassis.opcontrol_tank(); // Tank control



		pros::delay(ez::util::DELAY_TIME); // This is used for timer calculations!  Keep this ez::util::DELAY_TIME
	}
}
