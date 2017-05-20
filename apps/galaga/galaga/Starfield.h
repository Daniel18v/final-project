#pragma once

#include "../emulator/emulator.h"

namespace Galaga
{
	class Frame;

    class Starfield
    {
		struct Star
		{
			ushort x;
			ushort y;
			byte col;
			byte set;

			Star(ushort x, ushort y, byte col, byte set) :
				x(x),
				y(y),
				col(col),
				set(set)
			{
			}
		};

	public:
        Starfield();
		~Starfield() {}

		void DrawStars(Frame& frame);
		void SetControl(int offset, byte data) {
			_control[offset] = data;
		}

	private:
        void Move();

	private:
        static readonly Star starSeedTab[];
		static const size_t PaletteSize = 0x40;
        UINT palette[PaletteSize];
        byte _control[6];
        int _starsScrollX;
    };

} // Galaga
