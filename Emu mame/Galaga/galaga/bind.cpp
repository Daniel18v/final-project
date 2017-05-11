#include <emscripten/bind.h>
using namespace emscripten;

#include "types.h"
#include "galagamachine.h"

namespace Galaga {

	// Binding code
	EMSCRIPTEN_BINDINGS(galaga_machine)
	{
		class_<GalagaMachine>("GalagaMachine")
			.constructor<>()
			.function("Run", &GalagaMachine::Run)
			.function("set_InsertCoin", &GalagaMachine::set_InsertCoin)
			.function("set_Start1Player", &GalagaMachine::set_Start1Player)
			.function("set_Start2Player", &GalagaMachine::set_Start2Player)
			.function("set_MoveLeft", &GalagaMachine::set_MoveLeft)
			.function("set_MoveRight", &GalagaMachine::set_MoveRight)
			.function("set_Button1", &GalagaMachine::set_Button1)
			;
	}

} // Galaga
