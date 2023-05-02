#include "WinApp.h"
#include "TimeSystem.h"
#include <cmath>

namespace time
{
	ULONGLONG previousTime;
	ULONGLONG currentTime;
	ULONGLONG deltaTime;

	// �ð� �ʱ�ȭ
	void InitTime()
	{
		previousTime = currentTime = GetTickCount64();
	}
	// �ð� ������Ʈ
	void UpdateTime()
	{
		previousTime = currentTime;
		currentTime = GetTickCount64();
		deltaTime = currentTime - previousTime;
	}

	// �ʴ� ������ ���
	const float GetFrameRate()
	{
		if (deltaTime == 0) return 0;
		return ceil(((1000.0f / deltaTime) * 1000) / 1000);
	}
	
	// �ð� ��ȭ�� �޾ƿ���
	const ULONGLONG GetDeltaTime() { return deltaTime; }

}