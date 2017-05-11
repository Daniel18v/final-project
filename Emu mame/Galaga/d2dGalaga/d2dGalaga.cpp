// d2dGalaga.cpp : Defines the entry point for the application.
//

#include "pch.h"
#include "d2dGalaga.h"
#include "../galaga/screendevice.h"
#include <mmsystem.h>

#include "../galaga/Frame.h"

int APIENTRY _tWinMain(HINSTANCE hInstance,
                       HINSTANCE hPrevInstance,
                       LPTSTR    lpCmdLine,
                       int       nCmdShow)
{
    // Use HeapSetInformation to specify that the process should
    // terminate if the heap manager detects an error in any heap used
    // by the process.
    // The return value is ignored, because we want to continue running in the
    // unlikely event that HeapSetInformation fails.
    HeapSetInformation(NULL, HeapEnableTerminationOnCorruption, NULL, 0);

    if (SUCCEEDED(CoInitialize(NULL)))
    {
        {
            GalagaApp app;

            if (SUCCEEDED(app.Initialize()))
            {
                app.RunMessageLoop();
            }
        }
        CoUninitialize();
    }

    return 0;
}

GalagaApp::GalagaApp() :
    m_hwnd(NULL),
    m_pDirect2dFactory(NULL),
    m_pRenderTarget(NULL),
	_frame(Galaga::ScreenDevice::Width, Galaga::ScreenDevice::Height),
	_bitmapBuffer(NULL),
	_pBitmap(NULL),
	galagaMachine(),
	_xaudioGalaga(std::make_unique<XAudioGalaga>())
{
	timeBeginPeriod(1);

	_bitmapBuffer = new UINT[Galaga::ScreenDevice::Width * Galaga::ScreenDevice::Height];
}

GalagaApp::~GalagaApp()
{
	timeEndPeriod(1);

	delete [] _bitmapBuffer;

    SafeRelease(&m_pDirect2dFactory);
    SafeRelease(&m_pRenderTarget);
}

void GalagaApp::RunMessageLoop()
{
    MSG msg;

    while (GetMessage(&msg, NULL, 0, 0))
    {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }
}

HRESULT GalagaApp::Initialize()
{
    HRESULT hr;

    // Initialize device-indpendent resources, such as the Direct2D factory.
    hr = CreateDeviceIndependentResources();

    if (SUCCEEDED(hr))
    {
        // Register the window class.
        WNDCLASSEX wcex = { sizeof(WNDCLASSEX) };
        wcex.style         = CS_HREDRAW | CS_VREDRAW;
        wcex.lpfnWndProc   = GalagaApp::WndProc;
        wcex.cbClsExtra    = 0;
        wcex.cbWndExtra    = sizeof(LONG_PTR);
        wcex.hInstance     = HINST_THISCOMPONENT;
        wcex.hbrBackground = NULL;
        wcex.lpszMenuName  = NULL;
        wcex.hCursor       = LoadCursor(NULL, IDI_APPLICATION);
        wcex.lpszClassName = L"D2DGalagaApp";

        RegisterClassEx(&wcex);


        // Because the CreateWindow function takes its size in pixels,
        // obtain the system DPI and use it to scale the window size.
        FLOAT dpiX, dpiY;

        // The factory returns the current system DPI. This is also the value it will use
        // to create its own windows.
        m_pDirect2dFactory->GetDesktopDpi(&dpiX, &dpiY);

        // Create the window.
        m_hwnd = CreateWindow(
            L"D2DGalagaApp",
            L"Direct2D Demo App",
            WS_OVERLAPPEDWINDOW,
            CW_USEDEFAULT,
            CW_USEDEFAULT,
            static_cast<UINT>(ceil(600.f * dpiX / 96.f)),
            static_cast<UINT>(ceil(720.f * dpiY / 96.f)),
            NULL,
            NULL,
            HINST_THISCOMPONENT,
            this
            );
        hr = m_hwnd ? S_OK : E_FAIL;
        if (SUCCEEDED(hr))
        {
            ShowWindow(m_hwnd, SW_SHOWNORMAL);
            UpdateWindow(m_hwnd);
        }

		if (SUCCEEDED(hr))
		{
			_timer = ::SetTimer(m_hwnd, 1, 17, NULL); // ~60 frames/sec
		}
    }

    return hr;
}

HRESULT GalagaApp::CreateDeviceIndependentResources()
{
    HRESULT hr = S_OK;

    // Create a Direct2D factory.
    hr = D2D1CreateFactory(D2D1_FACTORY_TYPE_SINGLE_THREADED, &m_pDirect2dFactory);

    return hr;
}

HRESULT GalagaApp::CreateDeviceResources()
{
	HRESULT hr = S_OK;

	if (! m_pRenderTarget)
	{
		RECT rc;
		GetClientRect(m_hwnd, &rc);

		D2D1_SIZE_U size = D2D1::SizeU(
			rc.right - rc.left,
			rc.bottom - rc.top
			);

		D2D1_PIXEL_FORMAT pixelFormat = D2D1::PixelFormat(DXGI_FORMAT_B8G8R8A8_UNORM, D2D1_ALPHA_MODE_IGNORE);

		// Create a Direct2D render target.
		hr = m_pDirect2dFactory->CreateHwndRenderTarget(
			D2D1::RenderTargetProperties(),//D2D1_RENDER_TARGET_TYPE_DEFAULT, pixelFormat),
			D2D1::HwndRenderTargetProperties(m_hwnd, size),
			&m_pRenderTarget);
		D2D1_PIXEL_FORMAT pf = m_pRenderTarget->GetPixelFormat();

		if (SUCCEEDED(hr))
		{
			hr = m_pRenderTarget->CreateBitmap(
				D2D1::SizeU(Galaga::ScreenDevice::Width, Galaga::ScreenDevice::Height),
				NULL, 0,
				D2D1::BitmapProperties(pixelFormat),
				&_pBitmap);	
		}
	}

	return hr;
}

LRESULT CALLBACK GalagaApp::WndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    LRESULT result = 0;

    if (message == WM_CREATE)
    {
        LPCREATESTRUCT pcs = (LPCREATESTRUCT)lParam;
        GalagaApp *pGalagaApp = (GalagaApp *)pcs->lpCreateParams;

        ::SetWindowLongPtrW(
            hwnd,
            GWLP_USERDATA,
            PtrToUlong(pGalagaApp)
            );

        result = 1;
    }
    else
    {
        GalagaApp *pGalagaApp = reinterpret_cast<GalagaApp *>(static_cast<LONG_PTR>(
            ::GetWindowLongPtrW(
                hwnd,
                GWLP_USERDATA
                )));

        bool wasHandled = false;

        if (pGalagaApp)
        {
            switch (message)
            {
            case WM_SIZE:
                {
                    UINT width = LOWORD(lParam);
                    UINT height = HIWORD(lParam);
                    pGalagaApp->OnResize(width, height);
                }
                result = 0;
                wasHandled = true;
                break;

            case WM_DISPLAYCHANGE:
                {
                    InvalidateRect(hwnd, NULL, FALSE);
                }
                result = 0;
                wasHandled = true;
                break;

            case WM_PAINT:
                {
                    pGalagaApp->OnRender();
                    ValidateRect(hwnd, NULL);
                }
                result = 0;
                wasHandled = true;
                break;

            case WM_DESTROY:
                {
                    PostQuitMessage(0);
                }
                result = 1;
                wasHandled = true;
                break;

			case WM_TIMER:
                result = 0;
                wasHandled = true;
                pGalagaApp->OnTimer();
				break;

			case WM_KEYDOWN:
			case WM_KEYUP:
                result = 0;
                wasHandled = true;
                pGalagaApp->OnKey(WM_KEYDOWN == message, wParam);
				break;
			}
        }

        if (!wasHandled)
        {
            result = DefWindowProc(hwnd, message, wParam, lParam);
        }
    }

    return result;
}

HRESULT GalagaApp::OnRender()
{
    HRESULT hr = S_OK;

    hr = CreateDeviceResources();
    if (SUCCEEDED(hr))
    {
        m_pRenderTarget->BeginDraw();
        //m_pRenderTarget->SetTransform(D2D1::Matrix3x2F::Identity());
//      D2D1_SIZE_F rtSize = m_pRenderTarget->GetSize();

		// Apply the rotation transform to the render target.
		D2D1::Matrix3x2F rotate = D2D1::Matrix3x2F::Rotation(90.0F,
			D2D1::Point2F(Galaga::ScreenDevice::Height / 2.0F, Galaga::ScreenDevice::Width / 2.0F));
		D2D1::Matrix3x2F scale = D2D1::Matrix3x2F::Scale(2.0F, 2.0F);
		m_pRenderTarget->SetTransform(rotate * scale);

		m_pRenderTarget->DrawBitmap(_pBitmap);
			//D2D1::RectF(0.0F, 0.0F, (float)Galaga::ScreenDevice::Width, (float)Galaga::ScreenDevice::Height)); 

		hr = m_pRenderTarget->EndDraw();
    }

	return hr;
}

void GalagaApp::OnResize(UINT width, UINT height)
{
    if (m_pRenderTarget)
    {
        // Note: This method can fail, but it's okay to ignore the
        // error here, because the error will be returned again
        // the next time EndDraw is called.
        m_pRenderTarget->Resize(D2D1::SizeU(width, height));
    }
}

static DWORD _totExecutionTime = 0;
static DWORD _totEmulatedTime = 0;
//static bool done = false;
void GalagaApp::OnTimer()
{
	static DWORD tOld = 0;
	DWORD t = ::timeGetTime();
	
	Galaga::AudioFrame audioFrame;
	DWORD delta = (tOld == 0 ? 20 : t - tOld);
	galagaMachine.Run(delta * 1000, _frame, audioFrame);

	_totEmulatedTime += delta;
	_totExecutionTime += (::timeGetTime() - t);
	//if (_totEmulatedTime >= 100000 && !done)
	//{
	//	TCHAR buff[256];
	//	wsprintfW(buff, L"%d %d", _totEmulatedTime, _totExecutionTime);
	//	done = true;
	//	MessageBox(nullptr, buff, L"!", MB_OK);
	//	exit(0);
	//}

	tOld = t;

	D2D1_RECT_U rect = D2D1::RectU(0, 0, Galaga::ScreenDevice::Width, Galaga::ScreenDevice::Height);
	int pitch = sizeof(UINT) * Galaga::ScreenDevice::Width;
	HRESULT hr = _pBitmap->CopyFromMemory(&rect, _frame.Pixels, pitch);

	_xaudioGalaga->RenderAudio(std::vector<int>(audioFrame.Samples, audioFrame.Samples + audioFrame.Size));

	InvalidateRect(m_hwnd, NULL, FALSE);
}

void GalagaApp::OnKey(bool fDown, int key)
{
    switch (key)
    {
        case '0': // insert coin
            galagaMachine.set_InsertCoin(fDown);
            break;

        case '1': // start 1 player
            galagaMachine.set_Start1Player(fDown);
            break;

        case '2': // start 2 player
            galagaMachine.set_Start2Player(fDown);
            break;

        case VK_LEFT:
            galagaMachine.set_MoveLeft(fDown);
            break;

        case VK_RIGHT:
            galagaMachine.set_MoveRight(fDown);
            break;

        case VK_CONTROL:
            galagaMachine.set_Button1(fDown);
            break;

        default:
            break;
    }
}
