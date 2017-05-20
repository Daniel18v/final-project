#pragma once

#include "..\emulator\emulator.h"
#include "..\emulator\timer.h"
#include "namco51.h"
#include "namco54.h"

namespace Galaga
{
	class MainCpu;
	class GalagaMachine;

	class Namco06
	{
	public:
		Namco06(GalagaMachine& galagaMachine);
		virtual ~Namco06();

		void WriteMemory(ushort offset, byte data);
		byte ReadMemory(ushort offset);

	private:
		static readonly int IndexNamco51Device = 0;
		static readonly int IndexNamco54Device = 3;
		byte control;
		Namco51 namco51;
		Namco54 namco54;
		MainCpu& mainCpu;
		Emulator::CTimer* timer;
	};

} // namespace Galaga
