#pragma once

#include <string>
#include <map>
#include <set>
#include <vector>
#include <algorithm>
#include <functional>

#include <assert.h>
#define ASSERT(x) assert( (x) )

#include "Emulator.h"

namespace Emulator
{
	class CTimer;
	typedef std::function<void(INT64, INT64)> TimerCallback;

    class Scheduler
    {
	public:
        Scheduler(long quantum) //us
        {
            Quantum = quantum;
            CurrentTime = 0;
			executingDevice = nullptr;
        }

        void Reset()
        {
            this->CurrentTime = 0L;
        }

		void RegisterTimer(CTimer* timer);

        void UnregisterTimer(CTimer* timer)
        {
            for (auto it = timers.begin(); it != timers.end(); it++)
			{
				if (*it == timer) {
					timers.erase(it);
					return;
				}
            }
        }

        void RegisterDevice(Emulator::IDevice* device)
        {
            devices.push_back(device);
        }

        void UnregisterDevice(Emulator::IDevice* device)
        {
			for (std::vector<Emulator::IDevice*>::iterator it = devices.begin();
				 it != devices.end(); it++)
			{
				if (*it == device) {
					devices.erase(it);
					break;
				}
			}
        }

        void Timeslice()
        {
            ExecuteTimers();

			INT64 untilTime = CurrentTime + Quantum;
			for (size_t i = 0; i < devices.size(); i++)
			{
				Emulator::IDevice* device = devices[i];

				INT64 timeNextTimer = GetTimeNextTimer();
				if (timeNextTimer > 0 && timeNextTimer < untilTime)
				{
					untilTime = timeNextTimer;
				}
			
				if (untilTime > CurrentTime)
				{
					executingDevice = device;
					INT64 t = device->Execute(untilTime);

					if (t < untilTime) {
						untilTime = t;
					}

					executingDevice = nullptr;
				}
			}
				
			CurrentTime = (long)untilTime;
        }

		long get_CurrentTime() const
		{
			// if we're active, add in the time from the current slice
			long result = CurrentTime;
			if (executingDevice != nullptr) {
				result = result + (long)executingDevice->get_ExecutionTimeInCurrentTimeslice();
			}
			return result;
		}

	private:
		void ExecuteTimers();
		INT64 GetTimeNextTimer();

		long CurrentTime; // us
		long Quantum; // us
		std::vector<CTimer*> timers; // weak refs

        std::vector<IDevice*> devices; // weak refs
		IDevice* executingDevice; // weak ref
    };

	class CTimer
	{
	public:
		CTimer(const std::string& tag, Emulator::Scheduler& scheduler, TimerCallback callback) :
			ExpirationTime(0),
			Enabled(false),
			tag(tag),
			Scheduler(scheduler),
			Callback(callback),
			Period(0),
			IsPeriodic(false)
		{
			scheduler.RegisterTimer(this);
		}
		~CTimer();

		void Start(INT64 period, bool isPeriodic, long param = 0)
		{
			ASSERT(period >= 0);

			Param = param;
			Period = period;
			IsPeriodic = isPeriodic;
			Enabled = true;
			Scheduler.UnregisterTimer(this);
			ExpirationTime = Scheduler.get_CurrentTime() + Period;
			Scheduler.RegisterTimer(this);
		}

		void Stop()
		{
			Enabled = false;
		}

		void OnTick(long currentTime);

	public:
		INT64 ExpirationTime;
		bool Enabled;
	private:
		std::string tag;
		Scheduler& Scheduler;
		TimerCallback Callback;
		INT64 Period; // us
		bool IsPeriodic;
		long Param;
	};

} // Emulator
