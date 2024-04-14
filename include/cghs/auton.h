#pragma once

extern cghs::Motor intake_Motor;

extern pros::ADIDigitalOut flaps_front;

/*
	AUTON UTILITY FUNCTIONS
*/

// drive x inches
// intake auton

/*
	AUTONS
*/

#define TURN_SPEED 90.0
#define DRIVE_SPEED 110.0
#define SWING_SPEED 90.0 

// blue side goal auton epic

void auton_blue_offense() {

	intake_Motor.spin(true, 100.0);
	pros::delay(200);
	intake_Motor.spin(false, 0.0);

	chassis.pid_drive_set(24_in, DRIVE_SPEED);
	chassis.pid_wait();

  	chassis.pid_turn_set(90_deg, TURN_SPEED);
  	chassis.pid_wait();

    chassis.pid_swing_set(ez::LEFT_SWING, 45_deg, SWING_SPEED, 45);
    chassis.pid_wait();

}


void auton_defense() {

	
}