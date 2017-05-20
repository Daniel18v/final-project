#include "types.h"
#include "Namco06.h"
#include "galagamachine.h"
#include "..\emulator\deviceinput.h"
#include "..\emulator\timer.h"
#include "maincpu.h"
#include "namco51.h"
#include "namco54.h"

namespace Galaga
{
	Namco06::Namco06(GalagaMachine& galagaMachine) :
		namco51(galagaMachine),
		namco54(galagaMachine),
		mainCpu(*galagaMachine.mainCpu)
	{
		timer = new Emulator::CTimer("GenerateNmi", galagaMachine.scheduler,
			[this](INT64 param, INT64 currentTime) {
				// GenerateNmi(param);
				if (!mainCpu.IsSuspended((
					Emulator::SuspendReason::SUSPEND_REASON_HALT |
					Emulator::SuspendReason::SUSPEND_REASON_RESET |
					Emulator::SuspendReason::SUSPEND_REASON_DISABLE)))
				{
					mainCpu.get_Processor().get_NmiInputLine().SetState(Emulator::LineState::Pulse);
				}
			});
	}

	Namco06::~Namco06()
	{
		delete timer;
	}

    void Namco06::WriteMemory(ushort offset, byte data)
    {
        if (offset < 0x0100)
        {
            // ignore offset
            if ((control & 0x10) != 0)
            {
                return;
            }

            if (0 != (control & (1 << IndexNamco51Device)))
            {
                namco51.WriteMemory(0, data);
            }

            if (0 != (control & (1 << IndexNamco54Device)))
            {
                namco54.WriteMemory(0, data);
            }
        }
        else // offset == 0x0100
        {
            control = data;
            if (0 == (data & 0x0f))
            {
                timer->Stop();
            }
            else
            {
                timer->Start(200, true); // us
            }
        }
    }

    byte Namco06::ReadMemory(ushort offset)
    {
        if (offset < 0x0100)
        {
            // ignore offset

            if (0 == (control & 0x10))
            {
                return 0;
            }

            byte result = 0xff;
            int iDev = 0;
            if (0 != (control & (1 << iDev)))
            {
                byte data = 0;
				if (namco51.ReadMemory(offset, data)) {
					result &= data;
				}
            }

            return result;
        }
        else
        {
            // offset == 0x0100
            return control;
        }
    }

} // namespace Galaga