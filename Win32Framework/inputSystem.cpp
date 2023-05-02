#include "WinApp.h"
#include "InputSystem.h"

namespace input
{
	bool isKeyDown[256];
	bool isKeyUp[256];
	bool isKey[256];

	MOUSESTATE curMouse;
	MOUSESTATE prevMouse;

	// 키 값 초기화
	void ResetInput()
	{
		for (int i = 0; i < 256; i++)
		{
			isKeyDown[i] = false;
			isKeyUp[i] = false;
		}
	}

	// setkey ( gamemanager -> run -> message ) 
	void KeyDown(unsigned int key)
	{
		isKeyDown[key] = true;
		isKey[key] = true;
	}
	void KeyUp(unsigned int key)
	{
		isKeyUp[key] = true;
		isKey[key] = false;
	}

	// getkey
	bool IsKeyDown(unsigned int key)
	{
		return isKeyDown[key];
	}
	bool IsKeyUp(unsigned int key)
	{
		return isKeyUp[key];
	}
	bool IsKey(unsigned int key)
	{
		return isKey[key];
	}

	// 마우스 초기화
	void InitMouse()
	{
		curMouse.x = global::GetWinApp().GetWidth() / 2;
		curMouse.y = global::GetWinApp().GetHeight() / 2;
		curMouse.wheel = 0;

		curMouse.left = false;
		curMouse.right = false;
		curMouse.middle = false;

		prevMouse = curMouse;

		SetCursorPos(curMouse.x, curMouse.y);
	}

	// 마우스 업데이트
	void UpdateMouse()
	{
		prevMouse = curMouse;

		POINT pt;
		GetCursorPos(&pt);
		ScreenToClient(global::GetWinApp().GetWindow(), &pt);

		curMouse.x = pt.x;
		curMouse.y = pt.y;
		curMouse.wheel = 0;

		curMouse.left = (GetKeyState(VK_LBUTTON) & 0x8000) != 0;
		curMouse.right = (GetKeyState(VK_RBUTTON) & 0x8000) != 0;
		curMouse.middle = (GetKeyState(VK_MBUTTON) & 0x8000) != 0;
	}

	// 현재 마우스 상태 반환
	const MOUSESTATE& GetMouseState()
	{
		return curMouse;
	}

	// 이전 마우스 상태 반환
	const MOUSESTATE& GetPrevMouseState()
	{
		return prevMouse;
	}

	// resetinput 왜 따로 있는지..?
	void InitInput()
	{
		for (int i = 0; i < 256; i++)
		{
			isKeyDown[i] = false;
			isKeyUp[i] = false;
			isKey[i] = false;
		}

		InitMouse();
	}

	bool IsSame(const MOUSESTATE& a, const MOUSESTATE& b)
	{
		return a.x == b.x && a.y == b.y && a.wheel == b.wheel && a.left == b.left && a.right == b.right && a.middle == b.middle;
	}
}