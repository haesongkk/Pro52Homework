#include "WinApp.h"
#include "RenderSystem.h"

#pragma comment(lib, "msimg32.lib")

namespace render
{
	HWND hWnd;

	// ������۸��� ���� �յ� DC �ڵ�
	HDC frontMemDC;
	HDC backMemDC;

	// ����۴� ������ ��Ʈ�� �ڵ��� �ʿ��ѵ�?
	HBITMAP backBitmap = nullptr;

	void InitRender()
	{
		hWnd = global::GetWinApp().GetWindow();

		// ����Ʈ���ۿ� ���� ������ �ڵ���
		// ����ۿ� ������ ���� ������ �ڵ��� �����Ѵ�
		frontMemDC = GetDC(hWnd);
		backMemDC = CreateCompatibleDC(frontMemDC);

		// ������� �̹����� �����ϱ� ���� ��Ʈ�� �ڵ��� �����Ѵ�
		backBitmap = CreateCompatibleBitmap(frontMemDC, global::GetWinApp().GetWidth(), global::GetWinApp().GetHeight());
	}

	// ȭ���� �Ͼ������ ���´�
	void ClearScreen()
	{
		::PatBlt(backMemDC, 0, 0, global::GetWinApp().GetWidth(), global::GetWinApp().GetHeight(), WHITENESS);
	}

	// ����� ���� -> ���� ����ϴ� ������ ����ۿ� �׷�����
	void BeginDraw()
	{
		ClearScreen();
		::SelectObject(backMemDC, backBitmap);
	}


	// ����ۿ� �׷����� ������ ����Ʈ���۷� �����Ѵ�
	void EndDraw()
	{
		::BitBlt(frontMemDC, 0, 0, global::GetWinApp().GetWidth(), global::GetWinApp().GetHeight(), backMemDC, 0, 0, SRCCOPY);
	}

	// ����� ���� ���� �޸𸮿� �ڿ� ����
	void ReleaseRender()
	{
		DeleteObject(backBitmap);
		DeleteDC(backMemDC);
		ReleaseDC(hWnd, frontMemDC);
	}

	// ���� �׸��� �Լ�
	void DrawLine(int x1, int y1, int x2, int y2, COLORREF color)
	{
		HPEN hPen = CreatePen(PS_SOLID, 5, color);

		HPEN hOldPen = (HPEN)SelectObject(backMemDC, hPen);

		MoveToEx(backMemDC, x1, y1, NULL);

		LineTo(backMemDC, x2, y2);

		SelectObject(backMemDC, hOldPen);

		DeleteObject(hPen);
	}

	// �簢���� �׸��� �Լ�
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

	// ���׶�̸� �׸��� �Լ�
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

	// ������ �׸��� �Լ�
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

	// ���ڸ� �׸��� �Լ�
	void DrawText(int x, int y, const char* text, COLORREF color)
	{
		SetTextColor(backMemDC, color);
		TextOutA(backMemDC, x, y, text, strlen(text));
	}

}