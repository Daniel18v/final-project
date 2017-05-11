#pragma once

#include "../emulator/emulator.h"
#include "../emulator/cpu.h"
#include "../emulator/audioinput.h"
#include "../mb88xx/mb88_processor.h"
#include "discreteaudio.h"

namespace Galaga {

	class GalagaMachine;

	class Namco54 : public Emulator::Cpu
	{
		typedef Emulator::Cpu Inherited;

		class IO : public Emulator::IOHandler
		{
		public:
			IO(Namco54& mcu) : mcu(mcu) {}

			// IOHandler
			void Write(int port, byte data)
			{
				switch (port)
				{
					case Mb88xx::Port::MB88_PORTO:
						{
							byte outVal = (byte)(data & 0x0f);
							if ((data & 0x10) != 0)
							{
								mcu.getAudioInput(1).input_write(outVal);
							}
							else
							{
								mcu.getAudioInput(0).input_write(outVal);
							}
						}
						break;

					case Mb88xx::Port::MB88_PORTR1:
						{
							byte outVal = (byte)(data & 0x0f);
							mcu.getAudioInput(2).input_write(outVal);
						}
						break;

					default:
						break;
				}
			}
        
			byte Read(int port)
			{
				switch (port)
				{
					case Mb88xx::Port::MB88_PORTK:
						return (byte)(mcu.latchedCmd >> 4);

					case Mb88xx::Port::MB88_PORTR0:
						return (byte)(mcu.latchedCmd & 0x0f);

					default:
						return 0;
				}
			}

		private:
			Namco54& mcu;
		};

	public:
		Namco54(GalagaMachine& galagaMachine);
		virtual ~Namco54();

		// IMemoryHandler
        virtual bool WriteMemory(ushort address, byte value);
        virtual bool ReadMemory(ushort address, byte& value);

		Emulator::AudioInput& getAudioInput(int index);

	private:
		static const uint RomLength = 0x0400;
		static const uint RamLength = 0x40;

		GalagaMachine& galagaMachine;
        IO ioHandler;
        byte rom[RomLength];
        byte ram[RamLength];
        byte latchedCmd;
		Emulator::CTimer* irqClearTimer;
		Emulator::CTimer* setLatchedTimer;
	};

} // Galaga
