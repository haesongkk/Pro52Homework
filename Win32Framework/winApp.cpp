#include <cstdlib>
#include "WinApp.h"
#include "GameManager.h"

namespace global
{
	WINAPP winApp;
	const WINAPP& GetWinApp() { return winApp; }
}

// 스크린 사이즈 (const 보다 강력함)
constexpr int SCREEN_START_LEFT = 10;
constexpr int SCREEN_START_TOP = 10;
constexpr int SCREEN_WIDTH = 1024;
constexpr int SCREEN_HEIGHT = 768;

// 메인 함수
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

// 윈도우 위치를 화면 중앙에 위치시킨다
void PlaceInCenterOfScreen(HWND window, DWORD style, DWORD exStyle)
{
	// 스크린 사이즈 값 받아오기
	int screenWidth = GetSystemMetrics(SM_CXSCREEN);
	int screenHeight = GetSystemMetrics(SM_CYSCREEN);

	// 클라이언트 영역 크기를 구조체 변수에 저장한다
	RECT clientRect;
	GetClientRect(window, &clientRect);

	int clientWidth = clientRect.right - clientRect.left;
	int clientHeight = clientRect.bottom - clientRect.top;

	// 윈도우의 위치를 스크린 사이즈와 클라이언트 영역 크기를 바탕으로 
	// 계산한 화면 중앙 지점에 위치시킨다
	SetWindowPos(window, NULL,
		screenWidth / 2 - clientWidth / 2,
		screenHeight / 2 - clientHeight / 2,
		clientWidth, clientHeight, 0
	);
}

// 메세지 처리
LRESULT CALLBACK WINAPP::WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
	// 윈도우 생성 이벤트
	case WM_CREATE:
	{
		PlaceInCenterOfScreen(hWnd, WS_OVERLAPPEDWINDOW, WS_EX_APPWINDOW | WS_EX_WINDOWEDGE);
	}
	break;

	// 윈도우 파괴 이벤트
	case WM_DESTROY:
		PostQuitMessage(0);
		break;

	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}

// 윈도우 클래스 등록 및 창 생성
void WINAPP::Initialize(HINSTANCE hInstance)
{
	const TCHAR* appName = TEXT("Test Game Framework");

	// 윈도우 클래스 인스턴스 생성
	WNDCLASS wndClass;

	// 인스턴스 초기화
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

	// 윈도우 클래스 등록
	RegisterClass(&wndClass);


	RECT rect{ SCREEN_START_LEFT, SCREEN_START_TOP,
	SCREEN_START_LEFT + SCREEN_WIDTH, SCREEN_START_TOP + SCREEN_HEIGHT };

	// rect의 값을 원하는 윈도우 사이즈로 변경한다
	// WS_OVERLAPPEDWINDOW : 윈도우 기본 스타일
	::AdjustWindowRect(&rect, WS_OVERLAPPEDWINDOW, FALSE);

	int width = rect.right - rect.left;
	int height = rect.bottom - rect.top;

	// 윈도우 생성
	// WS_OVERLAPPED | WS_SYSMENU : 일반적인 윈도우 스타일
	hWnd = CreateWindow(appName, appName, WS_OVERLAPPED | WS_SYSMENU,
		SCREEN_START_LEFT, SCREEN_START_TOP, width, height, NULL, NULL, hInstance, NULL);

	// 생성한 윈도우를 띄운다
	// SW_SHOWNORMAL : 일반적인 크기와 위치
	ShowWindow(hWnd, SW_SHOWNORMAL);

	// 윈도우를 다시 그린다
	UpdateWindow(hWnd);

	// 게임 시작 전 초기화
	game::GAMEMANAGER::GetInstance()->Initialize();
}

void WINAPP::Run()
{
	// 게임 루프
	game::GAMEMANAGER::GetInstance()->Run();
}

void WINAPP::Finalize()
{
	// 게임 종료 전 메모리 정리
	game::GAMEMANAGER::GetInstance()->Finalize();
}
