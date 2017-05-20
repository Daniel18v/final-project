#pragma once

#include "../emulator/emulator.h"
#include "../emulator/cpu.h"
#include "../mb88xx/mb88_processor.h"

namespace Galaga {

	class GalagaMachine;

	class Namco51 : public Emulator::Cpu
	{
		typedef Emulator::Cpu Inherited;

        enum Mode
        {
            Switch = 0,
            Credits = 1,
            Play = 2
        };

		class IO : public Emulator::IOHandler
		{
		public:
			IO(const Namco51& mcu, GalagaMachine& galagaMachine);

			// IOHandler
			void Write(int port, byte data);        
			byte Read(int port);

		private:
			//const Namco51& mcu;
            Emulator::IInputPort* readHandlers[4];
            std::function<void(byte)> writeHandlers[2];
		};

	public:
		Namco51(GalagaMachine& galagaMachine);
		~Namco51();

		// IMemoryHandler
        virtual bool WriteMemory(ushort address, byte value);
        virtual bool ReadMemory(ushort address, byte& value);

	protected:
        byte ReadPort(int port);
        void WritePort(int port, byte data);

	private:
		static const uint RomLength = 0x0400;
		static const uint RamLength = 0x40;

		//GalagaMachine& galagaMachine;
        IO ioHandler;
        byte rom[RomLength];
        byte ram[RamLength];

        // state
        Mode mode;
        int coincredMode;
        int credits;
        byte coinsPerCred[2];
        byte credsPerCoin[2];
        int coins[2];
        bool fRemapJoystick;
        byte lastCoins;
        byte lastButtons;

        static readonly byte s_joyMap[16];
	};

} // Galaga

