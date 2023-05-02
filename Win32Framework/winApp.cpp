#include <cstdlib>
#include "WinApp.h"
#include "GameManager.h"

namespace global
{
	WINAPP winApp;
	const WINAPP& GetWinApp() { return winApp; }
}

// ��ũ�� ������ (const ���� ������)
constexpr int SCREEN_START_LEFT = 10;
constexpr int SCREEN_START_TOP = 10;
constexpr int SCREEN_WIDTH = 1024;
constexpr int SCREEN_HEIGHT = 768;

// ���� �Լ�
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

// ������ ��ġ�� ȭ�� �߾ӿ� ��ġ��Ų��
void PlaceInCenterOfScreen(HWND window, DWORD style, DWORD exStyle)
{
	// ��ũ�� ������ �� �޾ƿ���
	int screenWidth = GetSystemMetrics(SM_CXSCREEN);
	int screenHeight = GetSystemMetrics(SM_CYSCREEN);

	// Ŭ���̾�Ʈ ���� ũ�⸦ ����ü ������ �����Ѵ�
	RECT clientRect;
	GetClientRect(window, &clientRect);

	int clientWidth = clientRect.right - clientRect.left;
	int clientHeight = clientRect.bottom - clientRect.top;

	// �������� ��ġ�� ��ũ�� ������� Ŭ���̾�Ʈ ���� ũ�⸦ �������� 
	// ����� ȭ�� �߾� ������ ��ġ��Ų��
	SetWindowPos(window, NULL,
		screenWidth / 2 - clientWidth / 2,
		screenHeight / 2 - clientHeight / 2,
		clientWidth, clientHeight, 0
	);
}

// �޼��� ó��
LRESULT CALLBACK WINAPP::WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
	// ������ ���� �̺�Ʈ
	case WM_CREATE:
	{
		PlaceInCenterOfScreen(hWnd, WS_OVERLAPPEDWINDOW, WS_EX_APPWINDOW | WS_EX_WINDOWEDGE);
	}
	break;

	// ������ �ı� �̺�Ʈ
	case WM_DESTROY:
		PostQuitMessage(0);
		break;

	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}

// ������ Ŭ���� ��� �� â ����
void WINAPP::Initialize(HINSTANCE hInstance)
{
	const TCHAR* appName = TEXT("Test Game Framework");

	// ������ Ŭ���� �ν��Ͻ� ����
	WNDCLASS wndClass;

	// �ν��Ͻ� �ʱ�ȭ
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

	// ������ Ŭ���� ���
	RegisterClass(&wndClass);


	RECT rect{ SCREEN_START_LEFT, SCREEN_START_TOP,
	SCREEN_START_LEFT + SCREEN_WIDTH, SCREEN_START_TOP + SCREEN_HEIGHT };

	// rect�� ���� ���ϴ� ������ ������� �����Ѵ�
	// WS_OVERLAPPEDWINDOW : ������ �⺻ ��Ÿ��
	::AdjustWindowRect(&rect, WS_OVERLAPPEDWINDOW, FALSE);

	int width = rect.right - rect.left;
	int height = rect.bottom - rect.top;

	// ������ ����
	// WS_OVERLAPPED | WS_SYSMENU : �Ϲ����� ������ ��Ÿ��
	hWnd = CreateWindow(appName, appName, WS_OVERLAPPED | WS_SYSMENU,
		SCREEN_START_LEFT, SCREEN_START_TOP, width, height, NULL, NULL, hInstance, NULL);

	// ������ �����츦 ����
	// SW_SHOWNORMAL : �Ϲ����� ũ��� ��ġ
	ShowWindow(hWnd, SW_SHOWNORMAL);

	// �����츦 �ٽ� �׸���
	UpdateWindow(hWnd);

	// ���� ���� �� �ʱ�ȭ
	game::GAMEMANAGER::GetInstance()->Initialize();
}

void WINAPP::Run()
{
	// ���� ����
	game::GAMEMANAGER::GetInstance()->Run();
}

void WINAPP::Finalize()
{
	// ���� ���� �� �޸� ����
	game::GAMEMANAGER::GetInstance()->Finalize();
}
