#pragma once

#include "../emulator/emulator.h"
#include "../emulator/timer.h"
#include "Tile.h"
#include <memory>

namespace Galaga {

	class GalagaMachine;
	class Starfield;

    //ROM_REGION( 0x1000, "gfx1", 0 )
    //ROM_LOAD( "2600j.bin",    0x0000, 0x1000, CRC(58b2f47c) SHA1(62f1279a784ab2f8218c4137c7accda00e6a3490) )
    //ROM_REGION( 0x2000, "gfx2", 0 )
    //ROM_LOAD( "2800l.bin",    0x0000, 0x1000, CRC(ad447c80) SHA1(e697c180178cabd1d32483c5d8889a40633f7857) )
    //ROM_LOAD( "2700k.bin",    0x1000, 0x1000, CRC(dd6f1afc) SHA1(c340ed8c25e0979629a9a1730edc762bd72d0cff) )
    //ROM_REGION( 0x0220, "proms", 0 )
    //ROM_LOAD( "prom-5.5n",    0x0000, 0x0020, CRC(54603c6b) SHA1(1a6dea13b4af155d9cb5b999a75d4f1eb9c71346) )	// palette 
    //ROM_LOAD( "prom-4.2n",    0x0020, 0x0100, CRC(59b6edab) SHA1(0281de86c236c88739297ff712e0a4f5c8bf8ab9) )	// char lookup table 
    //ROM_LOAD( "prom-3.1c",    0x0120, 0x0100, CRC(4a04bb6b) SHA1(cdd4bc1013f5c11984fdc4fd10e2d2e27120c1e5) )	// sprite lookup table 

	class ScreenDevice
	{
        //static readonly int ScreenTotalWidth = 384;
        static readonly int ScreenTotalHeight = 264;
        static readonly INT64 FramePeriod = (long)(1e6 / 60); // 60Hz (in us)
        static readonly INT64 ScanTime = FramePeriod / ScreenTotalHeight;

        // visible area
	public:
        static readonly int Width = 288;
        static readonly int Columns = Width / 8;
        static readonly int Height = 224;
        static readonly int Rows = Height / 8;
	private:
        static readonly int VisibleAreaFirstLine = (ScreenTotalHeight - Height) / 2;
        static readonly int VisibleAreaLastLine = VisibleAreaFirstLine + Height - 1;
        static readonly int VBlank0Line = VisibleAreaLastLine + 1;
        static readonly int TotSprites = 64;
		static readonly int TotTiles = 8192;

        static const ushort VideoRamBase0 = 0x8000;
        static const ushort VideoRamLength0 = 0x800;
        static const ushort VideoRamBase1 = 0x8800;
        static const ushort VideoRamLength1 = 0x400;
        static const ushort VideoRamBase2 = 0x9000;
        static const ushort VideoRamLength2 = 0x400;
        static const ushort VideoRamBase3 = 0x9800;
        static const ushort VideoRamLength3 = 0x400;

		struct TileData
		{
			byte code;	// 7 bits
			byte color;	// 6 bits

			TileData() : code(255), color(0) {}

			int Index() const {
				return code << 6 | color;
			}
			operator bool() const {
				return code != 255;
			}
			bool operator == (const TileData& rhs) const {
				return code == rhs.code && color == rhs.color;
			}
		};

        struct SpriteData
        {
            int code;	// 7 bits
            int color;	// 6 bits
            bool flipx;
            bool flipy;
            byte sizex;
            byte sizey;

            bool operator == (const SpriteData& rhs) const
            {
                return (rhs.code == code &&
					rhs.color == color &&
					rhs.flipx == flipx &&
					rhs.flipy == flipy &&
					rhs.sizex == sizex &&
					rhs.sizey == sizey);
            }
            bool operator != (const SpriteData& rhs) const {
			   return ! operator == (rhs);
		    }
            bool operator < (const SpriteData& rhs) const
            {
				uint l = (((((code | (color << 8)) | (flipx ? 0x10000 : 0)) | (flipy ? 0x20000 : 0)) |
					((sizex / 0x10) << 0x12)) | ((sizey / 0x10) << 20));
				uint r = (((((rhs.code | (rhs.color << 8)) | (rhs.flipx ? 0x10000 : 0)) | (rhs.flipy ? 0x20000 : 0)) |
					((rhs.sizex / 0x10) << 0x12)) | ((rhs.sizey / 0x10) << 20));

                return l < r;
            }
        };

	public:
		ScreenDevice(GalagaMachine& galaga, byte* ram);
		virtual ~ScreenDevice();

		void DrawSprites(Frame& frame);
		void DrawStars(Frame& frame);
		void DrawTiles(Frame& frame);

		INT64 TimeUntilScanline(int scanline) const;

		bool WriteMemory(ushort address, byte value);

		Starfield* getStarfield() const {
			return starfield;
		}

	protected:
		void Reset();
		bool InitRoms();
		int GenerateTile(int code, int colorIndex);
		std::unique_ptr<Sprite> GenerateSprite(SpriteData spriteData) const;
		void GetTileInfo(int tileIndex, int& code, int& color);
		static int MapTile(int col, int row, int totColumns, int totRows);
		void InitMappings();
		void UpdateMappings();
		void MarkTileDirty(int memIndex);
		void OnVblankBegin(INT64 param);
		INT64 get_TimeUntilVblankStart() const;
		
		static bool readbit(const ByteArray& src, int bitnum)
        {
            return 0 != (src[bitnum / 8] & (0x80 >> (bitnum % 8)));
        }

	private:
		GalagaMachine& galaga;
		byte* ram;
        ByteArray romTxt;
        ByteArray romSprites;
        int* romPalette;
        ByteArray romColorTable;

        int tileMap[Rows][Columns];
        bool* dirtyMap;
        int maxMemoryIndex;
        int maxLogicalIndex;
        int* memoryToLogical;
        int* logicalToMemory;

        Tile tileCache[TotTiles];
		TileData tileData[TotTiles];

        std::unique_ptr<Sprite> sprites[TotSprites];
        SpriteData spritesData[TotSprites];

        Starfield* starfield;
		bool flipScreen; // ignored

        Emulator::CTimer* vblankBeginTimer;
        long vblankStartTime;
	};

} // namespace Galaga
