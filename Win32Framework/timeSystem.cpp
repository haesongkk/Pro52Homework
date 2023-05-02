#include "WinApp.h"
#include "TimeSystem.h"
#include <cmath>

namespace time
{
	ULONGLONG previousTime;
	ULONGLONG currentTime;
	ULONGLONG deltaTime;

	// 시간 초기화
	void InitTime()
	{
		previousTime = currentTime = GetTickCount64();
	}
	// 시간 업데이트
	void UpdateTime()
	{
		previousTime = currentTime;
		currentTime = GetTickCount64();
		deltaTime = currentTime - previousTime;
	}

	// 초당 프레임 계산
	const float GetFrameRate()
	{
		if (deltaTime == 0) return 0;
		return ceil(((1000.0f / deltaTime) * 1000) / 1000);
	}
	
	// 시간 변화량 받아오기
	const ULONGLONG GetDeltaTime() { return deltaTime; }

}