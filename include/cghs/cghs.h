#pragma once

#include "main.h"
#include "drive.h"

// Namespace cghs
namespace cghs
{

	class Motor : public pros::Motor
	{
		private:
		bool has_init = false;
		bool enabled = false;

		public:
		using pros::Motor::Motor;

		/**
		 *@brief Initializes Motor (for now just sets motor encoding units to degrees)
		 *
		 */
		void init();

		/**
		 *@brief Get the init object
		 *
		 * @return true
		 * @return false
		 */
		bool get_init();

		/**
		 *@brief Toggles Motor
		 *
		 * @param state whether to toggle this frame or not
		 */
		void toggle(bool state);

		/**
		 *@brief Get whether the motor is enabled or not
		 *
		 * @return true
		 * @return false
		 */
		bool get_toggle();

	};
}

/*
class A_Motor: public pros::Motor {
		public:
		using Motor::Motor;
		bool has_init = false;

		void init();
		float get_temp();
		void spin_percent(float percent);
		float get_percent_velocity();
		float get_percent_torque();
	};
*/