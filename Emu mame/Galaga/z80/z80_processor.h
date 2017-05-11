#pragma once

#include "../emulator/emulator.h"
#include "../emulator/deviceinput.h"
#include <memory>

namespace Z80 {

	class Z80ProcessorImpl;

	class Processor  : public Emulator::IProcessor
	{
	public:
		Processor(double clockFrequency, byte ram[], Emulator::IOHandler& ioHandler,
			const std::string tag, Emulator::IMemoryHandler* memoryHandler, Emulator::Cpu& cpu);
		~Processor() override;

		// IProcessor
		void Reset() override;
		INT64 get_ExecutionTimeInCurrentTimeslice() const override;
		INT64 Execute(INT64 us) override;
		void AbortTimeslice() override;
		Emulator::IDeviceInputLine& get_IrqInputLine() override;
		Emulator::IDeviceInputLine& get_ResetInputLine() override;
		Emulator::IDeviceInputLine& get_NmiInputLine() override;
		std::vector<Emulator::IDeviceInputLine*> get_InputLines() override;

	private:
		std::unique_ptr<Z80ProcessorImpl> _impl;
	};

} // Z80