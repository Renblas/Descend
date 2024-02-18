#pragma once

//namespace Drive chassis
namespace cghs::drive
{
	// Chassis constructor
	ez::Drive chassis(
		// Left Chassis Ports (negative port will reverse it!)
		//   the first port is used as the sensor
		{ 1, 2, 3 }

		// Right Chassis Ports (negative port will reverse it!)
		//   the first port is used as the sensor
		, { -4, -5, -6 }

		// IMU Port
		, 7

		// Wheel Diameter (Remember, 4" wheels without screw holes are actually 4.125!)
		, 3.25

		// Cartridge RPM
		, 600

		// External Gear Ratio (MUST BE DECIMAL) This is WHEEL GEAR / MOTOR GEAR

		, 1.6667
	);
}