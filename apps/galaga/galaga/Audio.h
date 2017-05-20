#pragma once

#include "../emulator/timer.h"

namespace Galaga {

	// forward declarations:
	class GalagaMachine;

	class Audio
	{
	public:
		Audio(GalagaMachine& galagaMachine);
		~Audio();

		void update(INT64 currentTime);

		std::vector<int> GetAudio() {
			return std::move(_buffer);
		}

	private:
		GalagaMachine& _galagaMachine;
		Emulator::CTimer* _timer;
		std::vector<int> _buffer;
	};

}