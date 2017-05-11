#include "types.h"
#include "Cpu.h"
#include "DeviceInput.h"
#include "Timer.h"
#include <assert.h>
#include <algorithm>

namespace Emulator
{
	Cpu::Cpu(Scheduler& scheduler, int machineIndex, IProcessor* processor) :
		processor(processor),
		scheduler(scheduler),
		trigger(0),
		interruptTrigger(machineIndex - 2000),
		isInterruptEnabled(true),
		inputLines(processor->get_InputLines()),
		nextSuspend(0),
		clearAllLinesTimer(new CTimer("ClearAllLines", scheduler,
			[this](INT64, INT64) {
				this->ClearAllLines();	
		})),
		currentTime(0)
	{
	}

	Cpu::~Cpu()
	{
		delete clearAllLinesTimer;
	}

	//INT64 Cpu::Execute(INT64 untilTime)
 //   {
	//	assert(NULL != processor);

	//	if (untilTime > currentTime) {
	//		currentTime += processor->Execute(untilTime - currentTime);
	//	}
	//	else {
	//		currentTime = untilTime;
	//	}

	//	return currentTime;
 //   }

	void Cpu::stopTimeslice()
	{
        processor->AbortTimeslice();
	}

	void Cpu::set_IsInterruptEnabled(bool value) {
		isInterruptEnabled = value;
        if (!isInterruptEnabled)
        {
			clearAllLinesTimer->Start(0, false);
        }
	}

    void Cpu::ClearAllLines()
    {
        //// clear NMI
		processor->get_NmiInputLine().SetState(LineState::Clear, _DeviceInput::USE_STORED_VECTOR);

        // clear all other IRQ inputs
		processor->get_IrqInputLine().SetState(LineState::Clear, _DeviceInput::USE_STORED_VECTOR);
    }

} // Emulator
