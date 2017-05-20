#pragma once

#include "../emulator/emulator.h"
#include "../emulator/cpu.h"

namespace Galaga {

	class GalagaMachine;

    //ROM_REGION( 0x10000, "sub", 0 )     // 64k for the second CPU 
    //ROM_LOAD( "3600e.bin",    0x0000, 0x1000, CRC(bc556e76) SHA1(0d3d68243c4571d985b4d8f7e0ea9f6fcffa2116) )
	class Cpu3 : public Emulator::Cpu
	{
		typedef Emulator::Cpu Inherited;

	public:
		Cpu3(GalagaMachine& galaga, byte* ram, uint ramLength, Emulator::IOHandler& ioHandler);
		virtual ~Cpu3();

		// IMemoryHandler
        virtual bool WriteMemory(ushort address, byte value);
        virtual bool ReadMemory(ushort address, byte& value);

        void NmiLinePulse();

	private:
		GalagaMachine& galagaMachine;
		static const uint romLength = 4 * 1024;
        byte rom[romLength];
		//uint ramLength;
        byte* ram;
	};

} // Galaga
