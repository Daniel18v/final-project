#include "types.h"
#include "Timer.h"

namespace Emulator
{
	CTimer::~CTimer()
	{
		Scheduler.UnregisterTimer(this);
	}

	void CTimer::OnTick(long currentTime)
	{
		if (Enabled && ExpirationTime <= currentTime)
		{
			if (IsPeriodic)
			{
				Scheduler.UnregisterTimer(this);
				while (ExpirationTime <= currentTime) {
					ExpirationTime = ExpirationTime + Period;
				}
				Scheduler.RegisterTimer(this);
			}
			else
			{
				Enabled = false;
			}

			Callback(Param, currentTime);
		}
	}

	void Scheduler::RegisterTimer(CTimer* timer)
	{
		bool stopTimeslice = timer->ExpirationTime < GetTimeNextTimer();
		timers.push_back(timer);

		if (stopTimeslice) {
			if (executingDevice != nullptr) {
				executingDevice->stopTimeslice();
			}
		}
	}

	void Scheduler::ExecuteTimers()
	{
		for (size_t i = 0; i < timers.size(); i++)
		{
			timers[i]->OnTick(CurrentTime);
		}
	}

	INT64 Scheduler::GetTimeNextTimer()
	{
		INT64 time = INT64_MAX;
		for (size_t i = 0; i < timers.size(); i++)
		{
			if (timers[i]->Enabled && timers[i]->ExpirationTime < time) {
				time = timers[i]->ExpirationTime;
			}
		}
		return (time == INT64_MAX ? 0 : time);
	}

} // Emulator
