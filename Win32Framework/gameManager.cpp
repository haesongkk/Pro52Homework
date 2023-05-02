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

	// player �ν��Ͻ� �� �ʱ�ȭ ( Ű���� �Է� )
	Object player = { global::GetWinApp().GetWidth() / 2 ,global::GetWinApp().GetHeight() / 2, 10, 10, RGB(255, 255, 0) };

	// blueCircles �ν��Ͻ� ���� ( ���콺 �Է� )
	const int blueCircleMax = 5000;
	int blueCircleCount = 0;
	Object blueCircles[blueCircleMax];

	// �÷��̾� �̵�
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

	// ��缭Ŭ ������Ʈ
	void UpdateBlueCircle()
	{
		// ���콺 ������ �޾ƿ´�
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

	// ���� ���� (�ʱ�ȭ)
	void GAMEMANAGER::Initialize()
	{
		input::InitInput();
		time::InitTime();
		render::InitRender();
	}

	// ������Ʈ
	void GAMEMANAGER::Update()
	{
		++updateCount;

		input::UpdateMouse();

		time::UpdateTime();

		UpdatePlayer();
		UpdateBlueCircle();

		input::ResetInput();

	}

	// ���� ������Ʈ
	void GAMEMANAGER::FixeUpdate()
	{
		static ULONGLONG elapsedTime;

		elapsedTime += time::GetDeltaTime();

		while (elapsedTime >= 20) //0.02��
		{
			++fixedUpdateCount;

			elapsedTime -= 20;
		}
	}

	// ����ϴ� �Լ�
	void GAMEMANAGER::Render()
	{
		render::BeginDraw();

		DrawFPS();
		DrawSomething();
		DrawPlayer();

		render::EndDraw();
	}

	// ���� ���� �� �޸� ����
	void GAMEMANAGER::Finalize()
	{
		render::ReleaseRender();
	}

	// ���� ����
	void GAMEMANAGER::Run()
	{
		// �޼��� ó�� ( Ű���� �Է� )
		MSG msg;
		while (true)
		{
			// getMessage : �޽��� ���� ��� ��� ...
			// peekMessage : �޽��� ���� ��� ��ȯ !
			if (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
			{
				// ���� ���� �޽����� ���� ���
				if (msg.message == WM_QUIT) break;

				// Ű���� �Է¿� ���� �޽����� ���� ���
				if (msg.message == WM_KEYDOWN)
				{
					input::KeyDown(msg.wParam);
				}
				else if (msg.message == WM_KEYUP)
				{
					input::KeyUp(msg.wParam);
				}
				
				// �̿� �޽����� ��� ������ ���ν����� �����Ѵ�
				else
				{
					DispatchMessage(&msg);
				}
			}
			// �޽����� ���� ���
			else
			{
				FixeUpdate();
				Update();
				Render();
			}
		}
	}

	// �̱��� ���� ����
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

	// FPS, update, fixedUpdate ���� ���
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

	// �÷��̾ �׸��� �Լ�
	void GAMEMANAGER::DrawPlayer()
	{
		render::DrawCircle(player.x, player.y, player.size, player.color);
	}

	// ���𰡸� �׸��� �Լ�
	void GAMEMANAGER::DrawSomething()
	{
		// �Ķ� ���׶�� ���
		for (int i = 0; i < blueCircleCount; i++)
		{
			render::DrawCircle(blueCircles[i].x, blueCircles[i].y, blueCircles[i].size, blueCircles[i].color);
		}
		// �÷��̾� �ֺ��� ���𰡸� ���
		render::DrawLine(player.x - 50, player.y + 50, player.x + 50, player.y + 50, RGB(255, 0, 0));
		render::DrawRect(player.x - 25, player.y - 25, 50, 50, RGB(255, 0, 255));

	}
}