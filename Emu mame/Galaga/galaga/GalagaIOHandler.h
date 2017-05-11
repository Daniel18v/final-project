#pragma once

#include <exception>

#include "../emulator/Emulator.h"

namespace Galaga
{
	class GalagaMachine;

    class GalagaIOHandler : public Emulator::IOHandler
    {
	public:
		GalagaIOHandler(const GalagaMachine& galaga)
        {
        }
		virtual ~GalagaIOHandler() {}

        // Emulator::IOHandler
        virtual void Write(int port, byte value)
		{
            //throw new std::exception("should never be called");
			return;
		}
        virtual byte Read(int port)
		{
            //throw new std::exception("should never be called");
			return 0;
		}
    };

} // Galaga
