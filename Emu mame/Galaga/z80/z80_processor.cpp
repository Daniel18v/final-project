#include "types.h"
#include "z80_processor.h"

#include "z80_processor_impl.h"

namespace Z80 {

	Processor::Processor(double clockFrequency, byte ram[], Emulator::IOHandler& ioHandler,
		const std::string tag, Emulator::IMemoryHandler* memoryHandler, Emulator::Cpu& cpu) :
		_impl(new Z80ProcessorImpl(clockFrequency, ram, ioHandler, tag, memoryHandler, cpu))
	{
	}

	Processor::~Processor()
	{
	}

	void Processor::Reset() {
		_impl->Reset();
	}
	INT64 Processor::get_ExecutionTimeInCurrentTimeslice() const {
		return _impl->get_ExecutionTimeInCurrentTimeslice();
	}
	INT64 Processor::Execute(INT64 us) {
		return _impl->Execute(us);
	}
	void Processor::AbortTimeslice() {
		_impl->AbortTimeslice();
	}
	Emulator::IDeviceInputLine& Processor::get_IrqInputLine() {
		return _impl->get_IrqInputLine();
	}
	Emulator::IDeviceInputLine& Processor::get_ResetInputLine() {
		return _impl->get_ResetInputLine();
	}
	Emulator::IDeviceInputLine& Processor::get_NmiInputLine() {
		return _impl->get_NmiInputLine();
	}
	std::vector<Emulator::IDeviceInputLine*> Processor::get_InputLines() {
		return _impl->get_InputLines();
	}
}
