#include "types.h"
#include "namco54.h"
#include "galagamachine.h"
#include <algorithm>

namespace Galaga
{
	Namco54::Namco54(GalagaMachine& galagaMachine) :
#pragma warning (disable: 4355) // 'this' : used in base member initializer list
        Inherited(galagaMachine.scheduler, 1,
			new Mb88xx::Processor(galagaMachine.scheduler,
				galagaMachine.ClockFrequency / 12,
				rom, RomLength,
				ram, RamLength,
				ioHandler,
				null, 0, // pla
				"54xx",
				*this,
				false,
				[this](){
					return get_Processor().get_IrqInputLine().DefaultIrqCallback();
				}
			)
		),
		galagaMachine(galagaMachine),
		ioHandler(*this),
#pragma warning (default: 4355)
		latchedCmd(0),
		irqClearTimer(nullptr),
		setLatchedTimer(nullptr)
	{
		memset(ram, 0, RamLength);
		memset(rom, 0, RomLength);

		ByteArray rom54 = galagaMachine.ReadRom("54xx.bin");
		std::copy(begin(rom54), end(rom54), rom);

		irqClearTimer = new Emulator::CTimer("IrqClear", galagaMachine.scheduler,
			[this](INT64, INT64) {
				this->get_Processor().get_IrqInputLine().SetState(Emulator::LineState::Clear);	
		});
		setLatchedTimer = new Emulator::CTimer("SetLatched", galagaMachine.scheduler,
			[this](INT64 value, INT64 currentTime) {
				this->latchedCmd = (byte)value;
		});

		galagaMachine.scheduler.RegisterDevice(this);
	}

	Namco54::~Namco54()
	{
		delete setLatchedTimer;
		delete irqClearTimer;

		delete processor;
		processor = null;
	}

	bool Namco54::WriteMemory(ushort address, byte value)
    {
        //galagaMachine.scheduler.Synchronize([this](INT64 param){
		//	this->latchedCmd = (byte)param;
		//}, value);
		setLatchedTimer->Start(0, false, value);

	
		get_Processor().get_IrqInputLine().SetState(Emulator::LineState::Assert);

        // The execution time of one instruction is ~4us, so we must make sure to
        // give the cpu time to poll the /IRQ input before we clear it.
        // The input clock to the 06XX interface chip is 64H, that is
        // 18432000/6/64 = 48kHz, so it makes sense for the irq line to be
        // asserted for one clock cycle ~= 21us.
        irqClearTimer->Start(21, false);

		return true;
    }
    bool Namco54::ReadMemory(ushort address, byte& value)
	{
		// to do
		return false;
	}

	Emulator::AudioInput& Namco54::getAudioInput(int index)
	{
		return galagaMachine.getAudioInput(index);
	}

} // Galaga
