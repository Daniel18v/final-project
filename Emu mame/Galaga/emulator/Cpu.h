#pragma once

#include "emulator.h"
#include "deviceinput.h"

namespace Emulator
{
	class Scheduler;
	class DeviceInput;

	class Cpu : public IMemoryHandler, public IDevice
	{
	public:
		Cpu(Scheduler& scheduler, int machineIndex, IProcessor* processor);
		virtual ~Cpu();

		// IDevice
        virtual void Reset() {
            processor->Reset();
        }
		virtual INT64 get_ExecutionTimeInCurrentTimeslice() const {
			return processor->get_ExecutionTimeInCurrentTimeslice();
		}
		virtual INT64 Execute(INT64 untilTime) // us
		{
			//assert(NULL != processor);

			if (untilTime > currentTime) {
				currentTime += processor->Execute(untilTime - currentTime);
			}
			else {
				currentTime = untilTime;
			}

			return currentTime;
		}
		virtual void stopTimeslice();

		bool get_IsInterruptEnabled() const {
			return isInterruptEnabled;
		}
		void set_IsInterruptEnabled(bool value);

        IProcessor& get_Processor() {
			return *processor;
		}

		Scheduler& get_Scheduler() {
			return scheduler;
		}

        bool IsSuspended(uint reason = SuspendReason::SUSPEND_ANY_REASON) {
            return ((nextSuspend & reason) != 0);
        }

        void Suspend(uint reason)
        {
            nextSuspend |= reason;
            processor->AbortTimeslice();
        }

        void Resume(uint reason)
        {
            nextSuspend &= ~reason;
            processor->AbortTimeslice();
        }

        void SignalInterruptTrigger()
        {
            processor->AbortTimeslice();

            // see if this is a matching trigger
            if ((nextSuspend & (uint)SuspendReason::SUSPEND_REASON_TRIGGER) != 0 && trigger == interruptTrigger)
            {
                Resume((uint)SuspendReason::SUSPEND_REASON_TRIGGER);
                trigger = 0;
            }
        }

	private:
		void EmptyEventQueues();
		void ClearAllLines();

	protected:
        IProcessor* processor;

	private:
		Scheduler& scheduler;
        uint trigger;
        int interruptTrigger;
		bool isInterruptEnabled;
		std::vector<IDeviceInputLine*> inputLines;
		uint nextSuspend;
		CTimer* clearAllLinesTimer;
		INT64 currentTime;
	};

} // Emulator
