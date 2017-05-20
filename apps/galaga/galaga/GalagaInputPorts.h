#pragma once

#include "../emulator/emulator.h"

namespace Galaga {

	class GalagaMachine;

    class IN0LPort : public Emulator::IInputPort
    {
	public:
        IN0LPort(GalagaMachine& galaga) :
			Button1(false),
			Start1Player(false),
			Start2Player(false)
        {
        }

        bool Button1;
        bool Start1Player;
        bool Start2Player;

        // IInputPort
        virtual byte Read();
    };

    class IN0HPort : public Emulator::IInputPort
    {
	public:
        IN0HPort(GalagaMachine& galaga) :
			InsertCoin(false)
        {
        }

        bool InsertCoin;

        // IInputPort
        virtual byte Read();
    };

    class IN1LPort : public Emulator::IInputPort
    {
	public:
        IN1LPort(GalagaMachine& galaga) :
			MoveLeft(false),
			MoveRight(false)
        {
        }

        bool MoveLeft;
        bool MoveRight;

        // IInputPort
        virtual byte Read();
    };

    class IN1HPort : public Emulator::IInputPort
    {
	public:
        IN1HPort(GalagaMachine& galaga)
        {
        }

        // IInputPort
        virtual byte Read();
    };

    class DSWAPort : public Emulator::IInputPort
    {
	public:
		DSWAPort(GalagaMachine& galaga) :
			MoveLeft(false),
			galaga(galaga)
		{
        }

        bool MoveLeft;

        // IInputPort
        virtual byte Read();

        GalagaMachine& galaga;
    };

    class DSWBPort : public Emulator::IInputPort
    {
	public:
        DSWBPort(GalagaMachine& galaga) :
			galaga(galaga)
        {
        }

        // IInputPort
        virtual byte Read();

        GalagaMachine& galaga;
    };

} // Galaga
