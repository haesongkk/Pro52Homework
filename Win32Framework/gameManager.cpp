#include "WinApp.h"
#include "InputSystem.h"
#include "TimeSystem.h"
#include "RenderSystem.h"

#include "GameManager.h"

#include <string>

namespace game
{
	struct Object
	{
		float x;
		float y;
		float size;
		float speed;

		COLORREF color;

		void SetPos(float x, float y)
		{
			this->x = x;
			this->y = y;
		}

		void Move(float x, float y)
		{
			this->x += x;
			this->y += y;
		}
	};

	// player 인스턴스 값 초기화 ( 키보드 입력 )
	Object player = { global::GetWinApp().GetWidth() / 2 ,global::GetWinApp().GetHeight() / 2, 10, 10, RGB(255, 255, 0) };

	// blueCircles 인스턴스 생성 ( 마우스 입력 )
	const int blueCircleMax = 5000;
	int blueCircleCount = 0;
	Object blueCircles[blueCircleMax];

	// 플레이어 이동
	void UpdatePlayer()
	{
		if (input::IsKeyDown('A'))
		{
			player.Move(-player.speed, 0);
		}
		else if (input::IsKeyDown('D'))
		{
			player.Move(player.speed, 0);
		}
		if (input::IsKeyDown('W'))
		{
			player.Move(0, -player.speed);
		}
		else if (input::IsKeyDown('S'))
		{
			player.Move(0, player.speed);
		}
	}

	// 블루서클 업데이트
	void UpdateBlueCircle()
	{
		// 마우스 정보를 받아온다
		const input::MOUSESTATE& mouse = input::GetMouseState();
		const input::MOUSESTATE& prevmouse = input::GetPrevMouseState();

		if (input::IsSame(mouse, prevmouse))
		{
			return;
		}

		if (blueCircleCount < blueCircleMax && mouse.left)
		{
			blueCircles[blueCircleCount].SetPos(mouse.x, mouse.y);
			blueCircles[blueCircleCount].color = RGB(0, 0, 255);
			blueCircles[blueCircleCount].size = 10;
			blueCircles[blueCircleCount].speed = 0;
			blueCircleCount++;
		}
	}

	GAMEMANAGER* GAMEMANAGER::instance = nullptr;

	GAMEMANAGER::GAMEMANAGER()
	{
	}
	GAMEMANAGER::~GAMEMANAGER()
	{
	}

	// 게임 시작 (초기화)
	void GAMEMANAGER::Initialize()
	{
		input::InitInput();
		time::InitTime();
		render::InitRender();
	}

	// 업데이트
	void GAMEMANAGER::Update()
	{
		++updateCount;

		input::UpdateMouse();

		time::UpdateTime();

		UpdatePlayer();
		UpdateBlueCircle();

		input::ResetInput();

	}

	// 고정 업데이트
	void GAMEMANAGER::FixeUpdate()
	{
		static ULONGLONG elapsedTime;

		elapsedTime += time::GetDeltaTime();

		while (elapsedTime >= 20) //0.02초
		{
			++fixedUpdateCount;

			elapsedTime -= 20;
		}
	}

	// 출력하는 함수
	void GAMEMANAGER::Render()
	{
		render::BeginDraw();

		DrawFPS();
		DrawSomething();
		DrawPlayer();

		render::EndDraw();
	}

	// 게임 종료 전 메모리 정리
	void GAMEMANAGER::Finalize()
	{
		render::ReleaseRender();
	}

	// 게임 루프
	void GAMEMANAGER::Run()
	{
		// 메세지 처리 ( 키보드 입력 )
		MSG msg;
		while (true)
		{
			// getMessage : 메시지 없는 경우 대기 ...
			// peekMessage : 메시지 없는 경우 반환 !
			if (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
			{
				// 게임 종료 메시지가 들어온 경우
				if (msg.message == WM_QUIT) break;

				// 키보드 입력에 관한 메시지가 들어온 경우
				if (msg.message == WM_KEYDOWN)
				{
					input::KeyDown(msg.wParam);
				}
				else if (msg.message == WM_KEYUP)
				{
					input::KeyUp(msg.wParam);
				}
				
				// 이외 메시지의 경우 윈도우 프로시저로 전달한다
				else
				{
					DispatchMessage(&msg);
				}
			}
			// 메시지가 없는 경우
			else
			{
				FixeUpdate();
				Update();
				Render();
			}
		}
	}

	// 싱글톤 패턴 구현
	GAMEMANAGER* GAMEMANAGER::GetInstance()
	{
		if (instance == nullptr)
		{
			instance = new GAMEMANAGER();
		}
		return instance;
	}
	void GAMEMANAGER::DestroyInstance()
	{
		if (instance != nullptr)
		{
			delete instance;
			instance = nullptr;
		}
	}

	// FPS, update, fixedUpdate 정보 출력
	void GAMEMANAGER::DrawFPS()
	{
		static ULONGLONG elapsedTime;
		static int UpdateCount;
		static int FixedUpdateCount;

		elapsedTime += time::GetDeltaTime();

		if (elapsedTime >= 1000)
		{
			elapsedTime = 0;
			;
			UpdateCount = updateCount;
			FixedUpdateCount = fixedUpdateCount;

			updateCount = 0;
			fixedUpdateCount = 0;
		}

		std::string str = "FPS: " + std::to_string(time::GetFrameRate());
		str += "           Update " + std::to_string(UpdateCount);
		str += "           FixedUpdate " + std::to_string(FixedUpdateCount);

		render::DrawText(10, 10, str.c_str(), RGB(255, 0, 0));

	}

	// 플레이어를 그리는 함수
	void GAMEMANAGER::DrawPlayer()
	{
		render::DrawCircle(player.x, player.y, player.size, player.color);
	}

	// 무언가를 그리는 함수
	void GAMEMANAGER::DrawSomething()
	{
		// 파란 동그라미 출력
		for (int i = 0; i < blueCircleCount; i++)
		{
			render::DrawCircle(blueCircles[i].x, blueCircles[i].y, blueCircles[i].size, blueCircles[i].color);
		}
		// 플레이어 주변에 무언가를 출력
		render::DrawLine(player.x - 50, player.y + 50, player.x + 50, player.y + 50, RGB(255, 0, 0));
		render::DrawRect(player.x - 25, player.y - 25, 50, 50, RGB(255, 0, 255));

	}
}