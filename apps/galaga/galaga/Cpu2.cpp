#include "types.h"
#include "Cpu2.h"
#include "galagamachine.h"
#include "../z80/z80_processor.h"
#include "namco06.h"
#include "screendevice.h"
#include <algorithm>

namespace Galaga {

	Cpu2::Cpu2(GalagaMachine& galaga, byte* ram, uint ramLength, Emulator::IOHandler& ioHandler) :
#pragma warning (disable: 4355) // 'this' : used in base member initializer list
        Inherited(galaga.scheduler, 1,
			new Z80::Processor(galaga.ClockFrequency, ram, ioHandler, "cpu2", this, *this)),
#pragma warning (default: 4355)
		galagaMachine(galaga),
		//ramLength(ramLength),
		ram(ram)
    {
		ByteArray rom3600e = galaga.ReadRom("3600e.bin");
		std::copy(begin(rom3600e), end(rom3600e), rom);

        galagaMachine.scheduler.RegisterDevice(this);
    }

	Cpu2::~Cpu2()
	{
		delete processor;
		processor = null;
	}

    bool Cpu2::WriteMemory(ushort address, byte value)
	{
        if (address < 0x1000)
        {
            // rom - do nothing
            return false;
        }
		else if (address >= 0x6800 && address < 0x6820)
		{
			galagaMachine.getNamcoSound().WriteMemory((ushort)(address - 0x6800), value);
			return true;
		}
		else if (address >= 0x6820 && address < 0x6828)
        {
            galagaMachine.WriteLatch(address - 0x6820, value);
        }
        //else if (address == 0x6830)
        //{
        //    galaga.ResetWatchdogTimer();
        //}
        else if (address >= 0x7000 && address < 0x7101)
        {
            galagaMachine.namco06xx->WriteMemory((ushort)(address - 0x7000), value);
            return true;
        }
		else if (!galagaMachine.screenDevice->WriteMemory(address, value))
        {
            ram[address] = value;
        }

        return true;
	}

    bool Cpu2::ReadMemory(ushort address, byte& value)
	{
        if (address < 0x1000)
        {
            value = rom[address];
            return true;
        }
        else if (address >= 0x6800 && address < 0x6808)
        {
            value = galagaMachine.ReadDsw(address - 0x6800);
            return true;
        }
        else if (address >= 0x7000 && address < 0x7101)
        {
            value = galagaMachine.namco06xx->ReadMemory((ushort)(address - 0x7000));
            return true;
        }
        else
        {
            value = ram[address];
            return true;
        }
	}

    void Cpu2::OnVblankStart()
    {
        if (!IsSuspended(Emulator::SuspendReason::SUSPEND_REASON_HALT |
						 Emulator::SuspendReason::SUSPEND_REASON_RESET |
						 Emulator::SuspendReason::SUSPEND_REASON_DISABLE))
        {
            get_Processor().get_IrqInputLine().SetState(Emulator::LineState::Assert);
        }
    }

} // Galaga
