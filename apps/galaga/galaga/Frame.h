#pragma once
#include <string.h>
#include <vector>

namespace Galaga {

	class Frame
	{
		Frame& operator=(const Frame&) = delete;
		Frame(const Frame&) = delete;

	public:
		Frame(int width, int height) :
			Width(width), Height(height), Pixels(NULL)
		{
			Pixels = new UINT[width * height];
		}
		~Frame()
		{
			delete [] Pixels;
		}

        void SetPixel(int x, int y, int color)
        {
            Pixels[(Width * y) + x] = color;
        }

		void Clear() {
			memset(Pixels, 0, Width * Height * sizeof(UINT));
		}

	public:
        int Width;
        int Height;
        UINT* Pixels;
	};

	class AudioFrame
	{
		AudioFrame& operator=(const AudioFrame&) = delete;
		AudioFrame(const AudioFrame&) = delete;

	public:
		AudioFrame() :
			Size(0),
			Samples(nullptr)
		{
		}
		~AudioFrame()
		{
			if (Samples) {
				delete[] Samples;
			}
		}

		void Set(const std::vector<int>& samples)
		{
			if (Samples) {
				delete[] Samples;
			}
			Size = samples.size();
			Samples = new int[samples.size()];
			memcpy(Samples, samples.data(), samples.size() * sizeof(int));
		}
	public:
		int Size;
		int* Samples;
	};

} // Frame
