#include <cstdlib>
#include "WinApp.h"
#include "GameManager.h"

namespace global
{
	WINAPP winApp;

	// 레퍼런스로 호출 되었기 때문에 extern 안 쓰고 변수에 접근할 수 있다
	const WINAPP& GetWinApp()
	{
		return winApp;
	}
}

// const 보다 강력한 constexpr
constexpr int SCREEN_START_LEFT = 10;

constexpr int SCREEN_START_TOP = 10;

constexpr int SCREEN_WIDTH = 1024;

constexpr int SCREEN_HEIGHT = 768;

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
	_In_opt_ HINSTANCE hPrevInstance,
	_In_ LPWSTR    lpCmdLine,
	_In_ int       nCmdShow)
{

	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);

	global::winApp.Initialize(hInstance);
	global::winApp.Run();

	global::winApp.Finalize();

	return EXIT_SUCCESS;
}

void PlaceInCenterOfScreen(HWND window, DWORD style, DWORD exStyle)
{
	int screenWidth = GetSystemMetrics(SM_CXSCREEN);
	int screenHeight = GetSystemMetrics(SM_CYSCREEN);

	RECT clientRect;
	GetClientRect(window, &clientRect);

	int clientWidth = clientRect.right - clientRect.left;
	int clientHeight = clientRect.bottom - clientRect.top;

	SetWindowPos(window, NULL,
		screenWidth / 2 - clientWidth / 2,
		screenHeight / 2 - clientHeight / 2,
		clientWidth, clientHeight, 0
	);
}

LRESULT CALLBACK WINAPP::WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
	case WM_CREATE:
	{
		PlaceInCenterOfScreen(hWnd, WS_OVERLAPPEDWINDOW, WS_EX_APPWINDOW | WS_EX_WINDOWEDGE);
	}
	break;

	case WM_DESTROY:
		PostQuitMessage(0);
		break;

	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}


void WINAPP::Initialize(HINSTANCE hInstance)
{
	hInstance = hInstance;

	const TCHAR* appName = TEXT("Test Game Framework");

	//Step 1: Registering the Window Class

	WNDCLASS wndClass;

	wndClass.style = CS_HREDRAW | CS_VREDRAW;
	wndClass.lpfnWndProc = WndProc;
	wndClass.cbClsExtra = 0;
	wndClass.cbWndExtra = 0;
	wndClass.hInstance = hInstance;
	wndClass.hIcon = LoadIcon(hInstance, IDI_APPLICATION);
	wndClass.hCursor = LoadCursor(NULL, IDC_ARROW);
	wndClass.hbrBackground = static_cast<HBRUSH>(GetStockObject(WHITE_BRUSH));
	wndClass.lpszMenuName = NULL;
	wndClass.lpszClassName = appName;

	RegisterClass(&wndClass);




	// Step 2: Creating the Window

	RECT rect{ SCREEN_START_LEFT, SCREEN_START_TOP,
	SCREEN_START_LEFT + SCREEN_WIDTH, SCREEN_START_TOP + SCREEN_HEIGHT };

	::AdjustWindowRect(&rect, WS_OVERLAPPEDWINDOW, FALSE);

	int width = rect.right - rect.left;
	int height = rect.bottom - rect.top;

	hWnd = CreateWindow(appName, appName, WS_OVERLAPPED | WS_SYSMENU,
		SCREEN_START_LEFT, SCREEN_START_TOP, width, height, NULL, NULL, hInstance, NULL);

	ShowWindow(hWnd, SW_SHOWNORMAL);
	UpdateWindow(hWnd);

	// Step 3: Game Initialize Here
	game::GAMEMANAGER::GetInstance()->Initialize();
}

void WINAPP::Run()
{
	// Step 4: Game Loop Here
	game::GAMEMANAGER::GetInstance()->Run();
}

void WINAPP::Finalize()
{
	// Step 5: Game Finalize Here
	game::GAMEMANAGER::GetInstance()->Finalize();
}
