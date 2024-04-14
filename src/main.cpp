#include "cghs/cghs.h"

/* ========================================================================== */
/*                                    Ports                                   */
/* ========================================================================== */

// NO NEGATIVES
#define PORT_INTAKE 7

// Chassis (negatives can reverse)
#define PORT_CHASSIS_L_F 11
#define PORT_CHASSIS_L_M 2
#define PORT_CHASSIS_L_B 3

#define PORT_CHASSIS_R_F -4
#define PORT_CHASSIS_R_M -16
#define PORT_CHASSIS_R_B -6

// NO NEGATIVES
#define PORT_FRONT_FLAPS 'A'
#define PORT_BACK_FLAPS 'B'
#define PORT_PTL_PISTON 'C'
#define PORT_INTAKE_LIFT 'D'


/* ========================================================================== */
/*                                    Setup                                   */
/* ========================================================================== */

// Device Declarations / definitions
cghs::Motor intake_Motor(PORT_INTAKE, pros::E_MOTOR_GEAR_BLUE, false, pros::E_MOTOR_ENCODER_DEGREES);

// Pneumatics stuffs
pros::ADIDigitalOut flaps_front(PORT_FRONT_FLAPS, false);
pros::ADIDigitalOut back_flaps(PORT_BACK_FLAPS, false);
pros::ADIDigitalOut pto_piston(PORT_PTL_PISTON, false);
pros::ADIDigitalOut intake_lift(PORT_INTAKE_LIFT, false);

/* ========================================================================== */
/*                                   Chassis                                  */
/* ========================================================================== */
ez::Drive chassis(

	// Left Chassis Ports (negative port will reverse it!)
	//   the first port is the sensored port (when trackers are not used!)
	{-11, -2, -3}

	// Right Chassis Ports (negative port will reverse it!)
	//   the first port is the sensored port (when trackers are not used!)
	,
	{4, 16, 6}

	// IMU Port
	,
	7

	// Wheel Diameter (Remember, 4" wheels are actually 4.125!)
	,
	2.75

	// Cartridge RPM
	,
	600

	// External Gear Ratio (MUST BE DECIMAL)
	// eg. if your drive is 84:36 where the 36t is powered, your RATIO would be 2.333.
	// eg. if your drive is 36:60 where the 60t is powered, your RATIO would be 0.6.
	,
	0.75

);

/* ========================================================================== */
/*                                 Initialize                                 */
/* ========================================================================== */
void initialize()
{

	pros::delay(500); // Stop the user from doing anything while legacy ports configure

	// Configure your chassis controls
	chassis.opcontrol_curve_buttons_toggle(true); // Enables modifying the controller curve with buttons on the joysticks
	chassis.opcontrol_drive_activebrake_set(0);	  // Sets the active brake kP. We recommend 0.1.
	chassis.opcontrol_curve_default_set(0, 0);	  // Defaults for curve. If using tank, only the first parameter is used. (Comment this line out if you have an SD card!)
	default_constants();						  // Set the drive to your own constants from autons.cpp!

	// These are already defaulted to these buttons, but you can change the left/right curve buttons here!
	// chassis.opcontrol_curve_buttons_left_set (pros::E_CONTROLLER_DIGITAL_LEFT, pros::E_CONTROLLER_DIGITAL_RIGHT); // If using tank, only the left side is used.
	// chassis.opcontrol_curve_buttons_right_set(pros::E_CONTROLLER_DIGITAL_Y,    pros::E_CONTROLLER_DIGITAL_A);

	auton_blue_offense();
	auton_defense();

	// Initialize chassis and auton selector
	chassis.initialize();
	master.rumble(".");
}

/* ========================================================================== */
/*                                  Disabled                                  */
/* ========================================================================== */
void disabled()
{
	// . . .
}

/* ========================================================================== */
/*                           Competition Initialize                           */
/* ========================================================================== */
void competition_initialize()
{
	// . . .
}

/* ========================================================================== */
/*                                 Autonomous                                 */
/* ========================================================================== */
void autonomous()
{
	chassis.pid_targets_reset();			   // Resets PID targets to 0
	chassis.drive_imu_reset();				   // Reset gyro position to 0
	chassis.drive_sensor_reset();			   // Reset drive sensors to 0
	chassis.drive_brake_set(MOTOR_BRAKE_HOLD); // Set motors to hold.  This helps autonomous consistency

	ez::as::auton_selector.selected_auton_call(); // Calls selected auton from autonomous selector
}

/* ========================================================================== */
/*                                User Control                                */
/* ========================================================================== */
void opcontrol()
{

	/*
		Setup
	*/

	// This is preference to what you like to drive on
	chassis.drive_brake_set(MOTOR_BRAKE_COAST);

	// Device Declarations / definitions
	cghs::Motor intake_Motor(PORT_INTAKE, pros::E_MOTOR_GEAR_BLUE, false, pros::E_MOTOR_ENCODER_DEGREES);

	// Pneumatics stuffs
	// ez::Piston front_flaps(PORT_FRONT_FLAPS, false);
	// ez::Piston back_flaps(PORT_BACK_FLAPS, false);
	// ez::Piston PTO_piston(PORT_PTL_PISTON, false);

	/*
		While True
	*/
	while (true)
	{

		// PID Tuner
		// After you find values that you're happy with, you'll have to set them in auton.cpp
		if (!pros::competition::is_connected() && false)
		{
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
		/*
		 *	IF USING get_digital_new_press() FOR MORE THAN ONE MOTOR / PISTON, MAKE SURE TO STORE IN INTERMIDIATE VARIABLE
		 *
		 * 	bool flaps_enabled = master.get_digital_new_press(DIGITAL_A)
		 * 	front_flaps.set(flaps_enabled);
		 *	back_flaps.set(flaps_enabled);
		 */

		// INTAKE //
		// intake_Motor.set_speed_percent(100);
		intake_Motor.spin(master.get_digital(DIGITAL_L1), 100.0);
		intake_Motor.spin(master.get_digital(DIGITAL_L2), -100.0);
		// intake_Motor.toggle(false);

		// Front Flaps //

		// ez::Piston PTO_piston(master.get_digital_new_press(DIGITAL_UP));

		// Tank control
		chassis.opcontrol_tank();

		pros::delay(ez::util::DELAY_TIME); // This is used for timer calculations!  Keep this ez::util::DELAY_TIME

		{
			bool front_flaps = master.get_digital_new_press(DIGITAL_R1);
			bool intake_lift = master.get_digital_new_press(DIGITAL_A);
			bool pto_piston = master.get_digital_new_press(DIGITAL_Y);
			// front_flaps.set(flaps_enabled);
			// back_flaps.set(flaps_enabled);

			// front_flaps(master.get_digital_new_press(DIGITAL_R1));
			// back_flaps(master.get_digital_new_press(DIGITAL_R2));

			// Tank control
			chassis.opcontrol_tank();

			pros::delay(ez::util::DELAY_TIME); // This is used for timer calculations!  Keep this ez::util::DELAY_TIME
		}
	}
}