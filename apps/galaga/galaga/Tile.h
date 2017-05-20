#pragma once
#include "Frame.h"

namespace Galaga
{
	class Tile
	{
	public:
		void Draw(Frame& frame, int left, int top)
		{
			int ymin = top < 0 ? -top : 0;
			int ymax = top + Height > frame.Height ? frame.Height - (top + Height) : Height;
			int xmin = left < 0 ? -left : 0;
			int xmax = left + Width > frame.Width ? frame.Width - (left + Width) : Width;

			for (int y = ymin; y < ymax; y++)
			{
				int src = Width * y;
				int dest = (top + y) * frame.Width + left;
				for (int x = xmin; x < xmax; x++)
				{
					if ((pixels[src + x] & 0xff000000) != 0)
					{
						frame.Pixels[dest + x] = pixels[src + x];
					}
				}
			}
		}

		static const int Width = 8;
		static const int Height = 8;
        int pixels[Width * Height];
	};

	class Sprite
	{
	public:
		Sprite(int width, int height) :
			width(width),
			height(height),
			pixels(NULL)
		{
			pixels = new int[width * height];
		}
		~Sprite()
		{
			delete[] pixels;
		}

		void Draw(Frame& frame, int left, int top)
		{
			int ymin = top < 0 ? -top : 0;
			int ymax = top + height > frame.Height ? frame.Height - (top + height) : height;
			int xmin = left < 0 ? -left : 0;
			int xmax = left + width > frame.Width ? frame.Width - (left + width) : width;

			for (int y = ymin; y < ymax; y++)
			{
				int src = width * y;
				int dest = (top + y) * frame.Width + left;
				for (int x = xmin; x < xmax; x++)
				{
					if ((pixels[src + x] & 0xff000000) != 0)
					{
						frame.Pixels[dest + x] = pixels[src + x];
					}
				}
			}
		}

		int width;
		int height;
		int* pixels;
	};

} // Frame
