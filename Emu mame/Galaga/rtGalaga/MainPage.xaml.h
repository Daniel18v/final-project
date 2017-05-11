//
// MainPage.xaml.h
// Declaration of the MainPage class.
//

#pragma once

#include "pch.h"
#include "MainPage.g.h"
#include "../galaga/GalagaMachine.h"
#include "../common/xaudiogalaga.h"

namespace rtGalaga
{
	/// <summary>
	/// An empty page that can be used on its own or navigated to within a Frame.
	/// </summary>
	public ref class MainPage sealed
	{
	public:
		MainPage();
        virtual ~MainPage();

	protected:
		void OnKey(Windows::System::VirtualKey key, bool fDown);

	private:
		void UserControl_Loaded(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e);
		void OnCompositionTargetRendered(Platform::Object^ sender, Platform::Object^ args);
		void LayoutRoot_KeyDown(Platform::Object^ sender, Windows::UI::Xaml::Input::KeyRoutedEventArgs^ e);
		void LayoutRoot_KeyUp(Platform::Object^ sender, Windows::UI::Xaml::Input::KeyRoutedEventArgs^ e);

		Windows::UI::Xaml::Media::Imaging::WriteableBitmap^ bitmap;
		Galaga::GalagaMachine* galaga;
		Galaga::Frame frame;
		Windows::Foundation::TimeSpan timePrevFrame;

		std::unique_ptr<XAudioGalaga> _xaudioGalaga;
	};
}
