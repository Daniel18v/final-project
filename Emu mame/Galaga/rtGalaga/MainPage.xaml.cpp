//
// MainPage.xaml.cpp
// Implementation of the MainPage class.
//

#include "pch.h"
#include "MainPage.xaml.h"
#include "../galaga/screendevice.h"
#include <Windows.h>
#include <initguid.h>
#include <robuffer.h>

using namespace Platform;
using namespace Windows::Foundation;
using namespace Windows::Foundation::Collections;
using namespace Windows::UI::Xaml;
using namespace Windows::UI::Xaml::Controls;
using namespace Windows::UI::Xaml::Controls::Primitives;
using namespace Windows::UI::Xaml::Data;
using namespace Windows::UI::Xaml::Input;
using namespace Windows::UI::Xaml::Media;
using namespace Windows::UI::Xaml::Navigation;
using namespace Microsoft::WRL;
using namespace rtGalaga;

//905a0fef-bc53-11df-8c49-001e4fc686da
DEFINE_GUID(IID_IBufferInternal, 0x905a0fef, 0xbc53, 0x11df, 
    0x8c, 0x49, 0x00, 0x1e, 0x4f, 0xc6, 0x86, 0xda);
struct IBufferInternal : IUnknown
{
	STDMETHOD(GetBuffer)(unsigned char** c) = 0;
};

MainPage::MainPage() :
	frame(Galaga::ScreenDevice::Width, Galaga::ScreenDevice::Height),
	timePrevFrame()
{
	InitializeComponent();
	Application::Current->DebugSettings->EnableFrameRateCounter = true;

	bitmap = ref new Windows::UI::Xaml::Media::Imaging::WriteableBitmap(Galaga::ScreenDevice::Height, Galaga::ScreenDevice::Width);

	img->Source = bitmap;
	
	_xaudioGalaga = std::make_unique<XAudioGalaga>();

	galaga = new Galaga::GalagaMachine();
}

MainPage::~MainPage()
{
	delete galaga;
}

void rtGalaga::MainPage::OnKey(Windows::System::VirtualKey key, bool fDown)
{
	switch (key)
	{
	case Windows::System::VirtualKey::Number0: // insert coin
		galaga->set_InsertCoin(fDown);
		break;

	case Windows::System::VirtualKey::Number1: // start 1 player
		galaga->set_Start1Player(fDown);
		break;

	case Windows::System::VirtualKey::Number2: // start 2 player
		galaga->set_Start2Player(fDown);
		break;

	case Windows::System::VirtualKey::Left:
		galaga->set_MoveLeft(fDown);
		break;

	case Windows::System::VirtualKey::Right:
		galaga->set_MoveRight(fDown);
		break;

	case Windows::System::VirtualKey::Control:
		galaga->set_Button1(fDown);
		break;

	default:
		break;
	}
}

void rtGalaga::MainPage::UserControl_Loaded(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e)
{
	CompositionTarget::Rendering += ref new EventHandler<Object^>(this,
		&MainPage::OnCompositionTargetRendered);
}

void rtGalaga::MainPage::OnCompositionTargetRendered(Platform::Object^ sender, Platform::Object^ args)
{
	// get elapsed time
	TimeSpan timeNextFrame = dynamic_cast<RenderingEventArgs^>(args)->RenderingTime;
	double time = (timeNextFrame.Duration - timePrevFrame.Duration) / 10.0; // us
	timePrevFrame = timeNextFrame;

	if (0 == time) {
		return;
	}

	Galaga::AudioFrame audioFrame;
	galaga->Run(time, frame, audioFrame);

	_xaudioGalaga->RenderAudio(std::vector<int>(audioFrame.Samples, audioFrame.Samples + audioFrame.Size));

	// RotateFrameAndCopyToBitmap
	Windows::Storage::Streams::IBuffer^ buffer = bitmap->PixelBuffer;
	ComPtr<Windows::Storage::Streams::IBufferByteAccess> pBufferByteAccess;
	ComPtr<IUnknown> pBuffer = (IUnknown*)buffer;
	if (!!pBuffer && SUCCEEDED(pBuffer.As(&pBufferByteAccess)))
	{
		// get pointer to pixel bytes
		BYTE* pBuff = nullptr;
		if (SUCCEEDED(pBufferByteAccess->Buffer(&pBuff)))
		{
			UINT* pDstPixels = (UINT*)pBuff;
			UINT* pSrcPixels = frame.Pixels;
            int i = 0;
            for (int x = 0; x < Galaga::ScreenDevice::Width; x++)
            {
                int j = (Galaga::ScreenDevice::Height - 1) * frame.Width + x;
                for (int y = Galaga::ScreenDevice::Height - 1; y >= 0; y--)
                {
                    pDstPixels[i++] = pSrcPixels[j];
                    j -= frame.Width;
                }
            }

			bitmap->Invalidate();
		}
	}
}

void rtGalaga::MainPage::LayoutRoot_KeyDown(Platform::Object^ sender, Windows::UI::Xaml::Input::KeyRoutedEventArgs^ e)
{
	OnKey(e->Key, true);
}

void rtGalaga::MainPage::LayoutRoot_KeyUp(Platform::Object^ sender, Windows::UI::Xaml::Input::KeyRoutedEventArgs^ e)
{
	OnKey(e->Key, false);
}
