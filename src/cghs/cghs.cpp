
#include "cghs/cghs.h"

namespace cghs
{

	void Motor::init() {

		set_encoder_units(pros::E_MOTOR_ENCODER_DEGREES);

		has_init = true;
	}

	bool Motor::get_init() {
		return has_init;
	}

	void Motor::toggle(bool state) {

		if (state) enabled = !enabled;

	}

	bool Motor::get_toggle() {
		return enabled;
	}


}