#pragma once

#include <sdkddkver.h>

#define WIN32_LEAN_AND_MEAN
#include <Windows.h>

class WINAPP
{
public:
	WINAPP() = default;
	~WINAPP() = default;

	void Initialize(HINSTANCE hinstance);

	void Run();

	void Finalize();

	HINSTANCE GetInstance()const { return hInstance; }

	HWND GetWindow() const { return hWnd; }

	const int GetWidth() const { return width; }
	const int GetHeight() const { return height; }

	// 함수에 붙은 static 은 선언은 내부에서 했지만 소속은 외부에 있음 (?)
	static LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

private:
	HINSTANCE hInstance = { 0 };  
	HWND hWnd = { 0 };			

	int width = 1024;
	int height = 768;
};

namespace global
{
	const WINAPP& GetWinApp();
}