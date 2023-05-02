#include "WinApp.h"
#include "RenderSystem.h"

#pragma comment(lib, "msimg32.lib")

namespace render
{
	HWND hWnd;

	// 더블버퍼링을 위한 앞뒤 DC 핸들
	HDC frontMemDC;
	HDC backMemDC;

	// 백버퍼는 별도의 비트맵 핸들이 필요한듯?
	HBITMAP backBitmap = nullptr;

	void InitRender()
	{
		hWnd = global::GetWinApp().GetWindow();

		// 프론트버퍼에 현재 윈도우 핸들을
		// 백버퍼에 생성한 가상 윈도우 핸들을 저장한다
		frontMemDC = GetDC(hWnd);
		backMemDC = CreateCompatibleDC(frontMemDC);

		// 백버퍼의 이미지를 관리하기 위한 비트맵 핸들을 생성한다
		backBitmap = CreateCompatibleBitmap(frontMemDC, global::GetWinApp().GetWidth(), global::GetWinApp().GetHeight());
	}

	// 화면을 하얀색으로 덮는다
	void ClearScreen()
	{
		::PatBlt(backMemDC, 0, 0, global::GetWinApp().GetWidth(), global::GetWinApp().GetHeight(), WHITENESS);
	}

	// 백버퍼 선택 -> 이후 출력하는 내용은 백버퍼에 그려진다
	void BeginDraw()
	{
		ClearScreen();
		::SelectObject(backMemDC, backBitmap);
	}


	// 백버퍼에 그려놓은 내용을 프론트버퍼로 복사한다
	void EndDraw()
	{
		::BitBlt(frontMemDC, 0, 0, global::GetWinApp().GetWidth(), global::GetWinApp().GetHeight(), backMemDC, 0, 0, SRCCOPY);
	}

	// 출력을 위해 쓰인 메모리와 자원 해제
	void ReleaseRender()
	{
		DeleteObject(backBitmap);
		DeleteDC(backMemDC);
		ReleaseDC(hWnd, frontMemDC);
	}

	// 선을 그리는 함수
	void DrawLine(int x1, int y1, int x2, int y2, COLORREF color)
	{
		HPEN hPen = CreatePen(PS_SOLID, 5, color);

		HPEN hOldPen = (HPEN)SelectObject(backMemDC, hPen);

		MoveToEx(backMemDC, x1, y1, NULL);

		LineTo(backMemDC, x2, y2);

		SelectObject(backMemDC, hOldPen);

		DeleteObject(hPen);
	}

	// 사각형을 그리는 함수
	void DrawRect(int x, int y, int width, int height, COLORREF color)
	{
		HPEN hPen = CreatePen(PS_SOLID, 1, color);

		HPEN hOldPen = (HPEN)SelectObject(backMemDC, hPen);

		HBRUSH hBrush = CreateSolidBrush(color);

		HBRUSH hOldBrush = (HBRUSH)SelectObject(backMemDC, hBrush);

		Rectangle(backMemDC, x, y, x + width, y + height);

		SelectObject(backMemDC, hOldPen);
		SelectObject(backMemDC, hOldBrush);

		DeleteObject(hPen);
		DeleteObject(hBrush);
	}

	// 동그라미를 그리는 함수
	void DrawCircle(int x, int y, int radius, COLORREF color)
	{
		HPEN hPen = CreatePen(PS_SOLID, 1, color);

		HPEN hOldPen = (HPEN)SelectObject(backMemDC, hPen);

		HBRUSH hBrush = CreateSolidBrush(color);

		HBRUSH hOldBrush = (HBRUSH)SelectObject(backMemDC, hBrush);

		Ellipse(backMemDC, x - radius, y - radius, x + radius, y + radius);

		SelectObject(backMemDC, hOldPen);
		SelectObject(backMemDC, hOldBrush);

		DeleteObject(hPen);
		DeleteObject(hBrush);

	}

	// 도형을 그리는 함수
	void DrawPolygon(POINT points[], int count, COLORREF color)
	{
		HPEN hPen = CreatePen(PS_SOLID, 1, color);

		HPEN hOldPen = (HPEN)SelectObject(backMemDC, hPen);

		HBRUSH hBrush = CreateSolidBrush(color);

		HBRUSH hOldBrush = (HBRUSH)SelectObject(backMemDC, hBrush);

		Polygon(backMemDC, points, count);

		SelectObject(backMemDC, hOldPen);
		SelectObject(backMemDC, hOldBrush);

		DeleteObject(hPen);
		DeleteObject(hBrush);

	}

	// 글자를 그리는 함수
	void DrawText(int x, int y, const char* text, COLORREF color)
	{
		SetTextColor(backMemDC, color);
		TextOutA(backMemDC, x, y, text, strlen(text));
	}

}