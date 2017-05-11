#pragma once

#include "../emulator/emulator.h"
#include <string>
#include <map>

namespace Galaga
{
	class GalagaRoms :public  Emulator::IRomManager
	{
	public:
		GalagaRoms();

		// Emulator::IRomManager
		virtual const ByteArray& ReadRom(const std::string& rom);

	private:
		typedef std::map<std::string, ByteArray> Roms;
		Roms roms;
	};
}