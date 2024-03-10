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
		int cur_speed = 100.0;

		public:
		using pros::Motor::Motor;

		/**
		*@brief Converts percent speed to rpm based on current motor gearset
		*
		* @param percent	value 0.0 to 100.0, percent speed
		* @return int	rpm based on percent
		*/
		int speed_percent_to_rpm(float percent) {

			float rpm = percent;
			if (get_gearing() == pros::E_MOTOR_GEAR_RED) rpm *= 1.0;
			if (get_gearing() == pros::E_MOTOR_GEAR_GREEN) rpm *= 2.0;
			if (get_gearing() == pros::E_MOTOR_GEAR_BLUE) rpm *= 6.0;
			return (int)rpm;
		}

		/**
		 *@brief Converts rpm speed to percent based on current motor gearset
		 *
		 * @param rpm
		 * @return int
		 */
		int speed_rpm_to_percent(int rpm) {

			float percent = rpm;
			if (get_gearing() == pros::E_MOTOR_GEAR_RED) percent /= 1.0;
			if (get_gearing() == pros::E_MOTOR_GEAR_GREEN) percent /= 2.0;
			if (get_gearing() == pros::E_MOTOR_GEAR_BLUE) percent /= 6.0;
			return (int)rpm;
		}

		/**
		 *@brief Set the speed percent
		 *
		 * @param percent int 0-100
		 */
		void set_speed_percent(float percent) {
			cur_speed = speed_percent_to_rpm(percent);
		}

		/**
		 *@brief Set the speed rpm
		 *
		 * @param rpm float 0-100, 0-200, or 0-300 based on motor
		 */
		void set_speed_rpm(float rpm) {
			cur_speed = rpm;
		}

		/**
		 *@brief use this to toggle motor
		 *
		 * @param state whether to toggle this frame or not
		 */
		void toggle(bool state) {
			if (state) enabled = !enabled;
			if (enabled) move_velocity(speed_percent_to_rpm(cur_speed));
		}

		/**
		 *@brief Spins Motor, for press and hold
		 *
		 * @param state
		 */
		void spin(bool state) {
			enabled = state;
			if (enabled) move_velocity(speed_percent_to_rpm(cur_speed));
		}

		/**
		 *@brief Get whether the motor is enabled or not
		 *
		 * @return true
		 * @return false
		 */
		bool get_toggle() {
			return enabled;
		}

	};
}
