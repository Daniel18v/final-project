#include "types.h"
#include "Namco51.h"
#include "galagamachine.h"
#include <algorithm>

namespace Galaga
{
    // joystick input mapping

    //  The joystick is parsed and a number corresponding to the direction is returned, according to the following table:
    //     0
    //   7   1
    // 6   8   2
    //   5   3
    //     4

    // static
	readonly byte Namco51::s_joyMap[16] = {
        //  LDRU,  LDR,  LDU,   LD,  LRU,   LR,   LU,    L,  DRU,   DR,   DU,    D,   RU,    R,    U, center */
            0x0f, 0x0e, 0x0d, 0x05, 0x0c, 0x09, 0x07, 0x06, 0x0b, 0x03, 0x0a, 0x04, 0x01, 0x02, 0x00, 0x08
    };

	Namco51::IO::IO(const Namco51& mcu, GalagaMachine& galagaMachine)// :
		//mcu(mcu)
	{
        readHandlers[0] = galagaMachine.GetPortByTag("IN0L");
        readHandlers[1] = galagaMachine.GetPortByTag("IN0H");
        readHandlers[2] = galagaMachine.GetPortByTag("IN1L");
        readHandlers[3] = galagaMachine.GetPortByTag("IN1H");

		writeHandlers[0] = [&galagaMachine](byte data){ galagaMachine.Out0PortWrite(data); };
        writeHandlers[1] = [&galagaMachine](byte data){ galagaMachine.Out1PortWrite(data); };
	}

	// IOHandler
	void Namco51::IO::Write(int port, byte data)
	{
        switch (port)
        {
            case 0:
            case 1:
                writeHandlers[port](data);
                break;

            default:
                break;
        }
	}
        
	byte Namco51::IO::Read(int port)
	{
        switch (port)
        {
            case 0:
            case 1:
            case 2:
            case 3:
                return readHandlers[port]->Read();

            default:
                return 0;
        }
	}

	Namco51::Namco51(GalagaMachine& galagaMachine) :
#pragma warning (disable: 4355) // 'this' : used in base member initializer list
        Inherited(galagaMachine.scheduler, 1,
				new Mb88xx::Processor(galagaMachine.scheduler,
				galagaMachine.ClockFrequency / 12,
				rom, RomLength,
				ram, RamLength,
				ioHandler,
				null, 0, // pla
				"51xx",
				*this,
				true)),
		ioHandler(*this, galagaMachine),
#pragma warning (default: 4355)
		mode(Mode::Switch),
		coincredMode(0)	{
		memset(ram, 0, RamLength);
		memset(rom, 0, RomLength);

		ByteArray rom51 = galagaMachine.ReadRom("51xx.bin");
		std::copy(begin(rom51), end(rom51), rom);

		galagaMachine.scheduler.RegisterDevice(this);
	}

	Namco51::~Namco51()
	{
		delete processor;
		processor = null;
	}

    byte Namco51::ReadPort(int port)
    {
        return ioHandler.Read(port);
    }

    void Namco51::WritePort(int port, byte data)
    {
        ioHandler.Write(port, data);
    }

    bool Namco51::ReadMemory(ushort offset, byte& value)
    {
        if (mode == Mode::Switch)
        {
            switch (offset)
            {
                case 0:
                    value = (byte)(ReadPort(0) | (ReadPort(1) << 4));
					break;

                case 1:
                    value = (byte)(ReadPort(2) | (ReadPort(3) << 4));
					break;

                case 2:
				default:
                    value = 0;	// nothing?
					break;

            }

			return true;
        }
        else
        {
            switch (offset)
            {
                case 0:	// number of credits in BCD format
                {
                    byte val = (byte)~((ReadPort(0) | (ReadPort(1) << 4)));
                    byte toggle = (byte)(val ^ lastCoins);
                    lastCoins = val;

                    if (coinsPerCred[0] > 0)
                    {
                        if (credits >= 99)
                        {
                            WritePort(1, 1);	// coin lockout
                        }
                        else
                        {
                            WritePort(1, 0);	// coin lockout
                            // check if the user inserted a coin //
                            if ((toggle & val & 0x10) != 0)
                            {
                                coins[0]++;
                                WritePort(0, 0x04);	// coin counter
                                WritePort(0, 0x0c);
                                if (coins[0] >= coinsPerCred[0])
                                {
                                    credits += credsPerCoin[0];
                                    coins[0] -= coinsPerCred[0];
                                }
                            }

                            if ((toggle & val & 0x20) != 0)
                            {
                                coins[1]++;
                                WritePort(0, 0x08);	// coin counter
                                WritePort(0, 0x0c);
                                if (coins[1] >= coinsPerCred[1])
                                {
                                    credits += credsPerCoin[1];
                                    coins[1] -= coinsPerCred[1];
                                }
                            }

                            if ((toggle & val & 0x40) != 0)
                            {
                                credits++;
                            }
                        }
                    }
                    else
                    {
                        credits = 100;	// free play
                    }

                    if (mode == Mode::Credits)
                    {
                        int on = 1; // machine is on?

                        if (credits >= 2)
                        {
                            WritePort(0, (byte)(0x0c | 3 * on));	// lamps
                        }
                        else if (credits >= 1)
                        {
                            WritePort(0, (byte)(0x0c | 2 * on));	// lamps
                        }
                        else
                        {
                            WritePort(0, 0x0c);	// lamps off
                        }

                        // check for 1 player start button //
                        if ((toggle & val & 0x04) != 0)
                        {
                            if (credits >= 1)
                            {
                                credits--;
                                mode = Mode::Play;
                                WritePort(0, 0x0c);	// lamps off
                            }
                        }
                        // check for 2 players start button //
                        else if ((toggle & val & 0x08) != 0)
                        {
                            if (credits >= 2)
                            {
                                credits -= 2;
                                mode = Mode::Play;
                                WritePort(0, 0x0c);	// lamps off
                            }
                        }
                    }

                    if ((~ReadPort(1) & 0x08) != 0)	// check test mode switch //
                    {
                        value = 0xbb;
						return true;
                    }

                    value = (byte)((credits / 10) * 16 + credits % 10);
					return true;
                }
					break;


                case 1:
                case 2:
                {
                    byte joy = (byte)(ReadPort((byte)(1 + offset)) & 0x0f);
                    byte val = (byte)~ReadPort(0);
                    byte toggle = (byte)(val ^ lastButtons);
                    lastButtons = (byte)((lastButtons & (3 - (int)offset)) | (val & (byte)offset));

                    // remap joystick //
                    if (fRemapJoystick)
                    {
                        joy = s_joyMap[joy];
                    }

                    // fire //
                    joy = (byte)(joy | (((toggle & val & offset) ^ offset) << (5 - (int)offset)));
                    joy = (byte)(joy | (((val & offset) ^ offset) << (6 - (int)offset)));

                    value = (byte)(joy);
					return true;
                }
					break;
            }
        }

        value = 0;
		return true;
    }

    bool Namco51::WriteMemory(ushort address, byte data)
    {
        data &= 0x07;

        if (coincredMode > 0)
        {
            switch (coincredMode--)
            {
                case 4:
                    coinsPerCred[0] = data;
                    break;
                case 3:
                    credsPerCoin[0] = data;
                    break;
                case 2:
                    coinsPerCred[1] = data;
                    break;
                case 1:
                    credsPerCoin[1] = data;
                    break;
                default:
                    // do nothing
                    break;
            }
        }
        else
        {
            switch (data)
            {
                case 0:	// nop
                    break;

                case 1:	// set coinage
                    coincredMode = 4;
                    credits = 0;
                    break;

                case 2:	// go in "credits" mode and enable start buttons
                    mode = Mode::Credits;
                    break;

                case 3:	// disable joystick remapping
                    fRemapJoystick = false;
                    break;

                case 4:	// enable joystick remapping
                    fRemapJoystick = true;
                    break;

                case 5:	// go in "switch" mode
                    mode = Mode::Switch;
                    break;

                default:
                    // do nothing
                    break;
            }
        }

		return true;
    }
/*
    byte Namco51::Read(uint offset)
    {
        if (mode == Mode::Switch)
        {
            switch (offset)
            {
                case 0:
                    return (byte)(ReadPort(0) | (ReadPort(1) << 4));
                case 1:
                    return (byte)(ReadPort(2) | (ReadPort(3) << 4));
                case 2:
                    return 0;	// nothing?
            }
        }
        else
        {
            switch (offset)
            {
                case 0:	// number of credits in BCD format
                {
                    byte val = (byte)~((ReadPort(0) | (ReadPort(1) << 4)));
                    byte toggle = (byte)(val ^ lastCoins);
                    lastCoins = val;

                    if (coinsPerCred[0] > 0)
                    {
                        if (credits >= 99)
                        {
                            WritePort(1, 1);	// coin lockout
                        }
                        else
                        {
                            WritePort(1, 0);	// coin lockout
                            // check if the user inserted a coin //
                            if ((toggle & val & 0x10) != 0)
                            {
                                coins[0]++;
                                WritePort(0, 0x04);	// coin counter
                                WritePort(0, 0x0c);
                                if (coins[0] >= coinsPerCred[0])
                                {
                                    credits += credsPerCoin[0];
                                    coins[0] -= coinsPerCred[0];
                                }
                            }

                            if ((toggle & val & 0x20) != 0)
                            {
                                coins[1]++;
                                WritePort(0, 0x08);	// coin counter
                                WritePort(0, 0x0c);
                                if (coins[1] >= coinsPerCred[1])
                                {
                                    credits += credsPerCoin[1];
                                    coins[1] -= coinsPerCred[1];
                                }
                            }

                            if ((toggle & val & 0x40) != 0)
                            {
                                credits++;
                            }
                        }
                    }
                    else
                    {
                        credits = 100;	// free play
                    }

                    if (mode == Mode::Credits)
                    {
                        int on = 1; // machine is on?

                        if (credits >= 2)
                        {
                            WritePort(0, (byte)(0x0c | 3 * on));	// lamps
                        }
                        else if (credits >= 1)
                        {
                            WritePort(0, (byte)(0x0c | 2 * on));	// lamps
                        }
                        else
                        {
                            WritePort(0, 0x0c);	// lamps off
                        }

                        // check for 1 player start button //
                        if ((toggle & val & 0x04) != 0)
                        {
                            if (credits >= 1)
                            {
                                credits--;
                                mode = Mode::Play;
                                WritePort(0, 0x0c);	// lamps off
                            }
                        }
                        // check for 2 players start button //
                        else if ((toggle & val & 0x08) != 0)
                        {
                            if (credits >= 2)
                            {
                                credits -= 2;
                                mode = Mode::Play;
                                WritePort(0, 0x0c);	// lamps off
                            }
                        }
                    }

                    if ((~ReadPort(1) & 0x08) != 0)	// check test mode switch //
                    {
                        return 0xbb;
                    }

                    return (byte)((credits / 10) * 16 + credits % 10);
                }

                case 1:
                case 2:
                {
                    byte joy = (byte)(ReadPort((byte)(1 + offset)) & 0x0f);
                    byte val = (byte)~ReadPort(0);
                    byte toggle = (byte)(val ^ lastButtons);
                    lastButtons = (byte)((lastButtons & (3 - (int)offset)) | (val & (byte)offset));

                    // remap joystick //
                    if (fRemapJoystick)
                    {
                        joy = s_joyMap[joy];
                    }

                    // fire //
                    joy = (byte)(joy | (((toggle & val & offset) ^ offset) << (5 - (int)offset)));
                    joy = (byte)(joy | (((val & offset) ^ offset) << (6 - (int)offset)));

                    return (byte)(joy);
                }
            }
        }

        return 0;
    }
*/
} // Galaga