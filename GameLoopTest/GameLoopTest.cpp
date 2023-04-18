#include <stdio.h>
#include <windows.h>

namespace global
{
    namespace time
    {
        ULONGLONG previousTime;
        ULONGLONG currentTime;
        ULONGLONG deltaTime;

        // 전역 변수는 자동으로 초기화 된다

        int updateCount;
        int fixedUpdateCount;

        int updateUpdateCountPerSecond;
        int updateFixedCountPerSecond;

        void InitTime()
        {

            // 지역 변수는 반드시 초기화를 해주자

            previousTime = currentTime = GetTickCount64();

        }

        void UpdateTime()
        {
            previousTime = currentTime;

            currentTime = GetTickCount64();

            deltaTime = currentTime - previousTime;
        }

        ULONGLONG GetDeltaTime()
        {
            return deltaTime;
        }
    }
}



void StartGame()
{

}



bool IsGameRun()
{
    return true;
}




void ProcessInput()
{

}



void FixedUpdate()
{
    static ULONGLONG elapsedTime;

    // 지역 변수는 해당 함수가 호출 당하면 생성되고 실행 종료되면 존재 안 함
    // 전역 변수는 프로그램이 실행이 되면 항상 존재함
    // 정적 변수는 메모리 상에 관리하는 영역이 다르다
    // 정적 변수는 지역 변수로 생성하여도 함수 종료 후까지 메모리에 존재한다 (값 유지)

    elapsedTime += global::time::GetDeltaTime();

    while (elapsedTime >= 20) // 0.2s
    {
        global::time::fixedUpdateCount += 1;
        elapsedTime -= 20;
    }
}



void Update()
{
    global::time::updateCount += 1;
}



void PrintCountsPerSecond()
{
    static ULONGLONG elapsedTime;

    elapsedTime += global::time::GetDeltaTime();
    if (elapsedTime >= 1000)
    {
        printf("updateCount : %d \t fixedUpdateCount : %d \n", global::time::updateUpdateCountPerSecond, global::time::updateFixedCountPerSecond);

        global::time::updateUpdateCountPerSecond = global::time::updateCount;
        global::time::updateFixedCountPerSecond = global::time::fixedUpdateCount;

        elapsedTime = 0;
        global::time::fixedUpdateCount = 0;
        global::time::fixedUpdateCount = 0;
    }
}



void EndGame()
{

}



int main()
{
    global::time::InitTime();

    StartGame();

    while (IsGameRun())
    {
        global::time::UpdateTime();

        ProcessInput();

        FixedUpdate();

        Update();

        PrintCountsPerSecond();
    }

    EndGame();

    return 0;
}