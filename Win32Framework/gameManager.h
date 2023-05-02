#pragma once

namespace game
{
	class GAMEMANAGER
	{
	public:

		GAMEMANAGER();
		~GAMEMANAGER();

		void Initialize();
		void Update();
		void FixeUpdate();
		void Render();
		void Finalize();
		void Run();

		static GAMEMANAGER* GetInstance();
		static void DestroyInstance();

	private:

		void DrawFPS();
		void DrawPlayer();
		void DrawSomething();

		static GAMEMANAGER* instance;

		int updateCount = { 0 };
		int fixedUpdateCount = { 0 };
	};
}
