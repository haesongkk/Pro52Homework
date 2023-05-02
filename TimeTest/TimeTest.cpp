#include <iostream>
#include <Windows.h>
#include <conio.h>
#include <mmsystem.h>
#pragma comment(lib, "winmm.lib")

class SimpleClock
{
public:
	SimpleClock()
	{
		m_StartTime = m_EndTime = 0;
	}

	void Start()
	{
		m_StartTime = clock();
	}

	void Stop()
	{
		m_EndTime = clock();
	}

	double GetElapsedTime()
	{
		return (double)(m_EndTime - m_StartTime) / (double)CLOCKS_PER_SEC;
	}

	double GetElapsedTimeMS()
	{
		return (double)(m_EndTime - m_StartTime) / (double)CLOCKS_PER_SEC * 1000.0;
	}

private:
	clock_t m_StartTime;
	clock_t m_EndTime;
};

class TickClock
{
public:

	TickClock()
	{
		m_StartTime = m_EndTime = 0;
	}
	void Start()
	{
		m_StartTime = GetTickCount64();
	}
	void Stop()
	{
		m_EndTime = GetTickCount64();
	}

	double GetElapsedTime()
	{
		return (double)(m_EndTime - m_StartTime) / (double)CLOCKS_PER_SEC;;
	}

	double GetElapsedTimeMS()
	{
		return (double)(m_EndTime - m_StartTime) / (double)CLOCKS_PER_SEC * 1000.0;
	}

private:
	ULONGLONG m_StartTime;
	ULONGLONG m_EndTime;
};

class WinMMClock
{
public:
	WinMMClock()
	{
		timeBeginPeriod(1);
		m_StartTime = m_EndTime = 0;
	}

	~WinMMClock()
	{
		timeEndPeriod(1);
	}

	void Start()
	{
		m_StartTime = timeGetTime();
	}

	void Stop()
	{
		m_EndTime = timeGetTime();
	}

	double GetElapsedTime()
	{
		return (double)(m_EndTime - m_StartTime) / (double)CLOCKS_PER_SEC;
	}

	double GetElapsedTimeMS()
	{
		return (double)(m_EndTime - m_StartTime) / (double)CLOCKS_PER_SEC * 1000.0;
	}

private:
	DWORD m_StartTime;
	DWORD m_EndTime;
};

class QueryClock
{
public:
	QueryClock()
	{
		QueryPerformanceFrequency(&m_Frequency);
		QueryPerformanceCounter(&m_StartTime);
	}
	void Start()
	{
		QueryPerformanceCounter(&m_StartTime);
	}
	void Stop()
	{
		QueryPerformanceCounter(&m_EndTime);
	}

	double GetElapsedTime()
	{
		return (double)(m_EndTime.QuadPart - m_StartTime.QuadPart) / (double)m_Frequency.QuadPart;
	}

	double GetElapsedTimeMS()
	{
		return (double)(m_EndTime.QuadPart - m_StartTime.QuadPart) / (double)m_Frequency.QuadPart * 1000.0f;
	}

private:
	LARGE_INTEGER m_StartTime = { 0 };
	LARGE_INTEGER m_EndTime = { 0 };
	LARGE_INTEGER m_Frequency = { 0 };
};

void TestSleep()
{
	SimpleClock simpleClock;
	TickClock tickClock;
	WinMMClock winMMClock;
	QueryClock queryClock;

	const int SLEEP_TIME = 1000;

	simpleClock.Start();
	Sleep(SLEEP_TIME);
	simpleClock.Stop();

	std::cout << "SimpleClock: " << simpleClock.GetElapsedTime() << std::endl;

	tickClock.Start();
	Sleep(SLEEP_TIME);
	tickClock.Stop();

	std::cout << "TickClock: " << tickClock.GetElapsedTime() << std::endl;

	winMMClock.Start();
	Sleep(SLEEP_TIME);
	winMMClock.Stop();

	std::cout << "WinMMClock: " << winMMClock.GetElapsedTime() << std::endl;

	queryClock.Start();
	Sleep(SLEEP_TIME);
	queryClock.Stop();

	std::cout << "QueryClock: " << queryClock.GetElapsedTime() << std::endl;
}

void DummyLoop()
{
	_getch();
}

void TestLoop()
{
	SimpleClock simpleClock;
	TickClock tickClock;
	WinMMClock winMMClock;
	QueryClock queryClock;

	simpleClock.Start();

	DummyLoop();

	simpleClock.Stop();

	std::cout << "SimpleClock: " << simpleClock.GetElapsedTime() << std::endl;

	tickClock.Start();

	DummyLoop();

	tickClock.Stop();

	std::cout << "TickClock: " << tickClock.GetElapsedTime() << std::endl;

	winMMClock.Start();

	DummyLoop();

	winMMClock.Stop();

	std::cout << "WinMMClock: " << winMMClock.GetElapsedTime() << std::endl;

	queryClock.Start();

	DummyLoop();

	queryClock.Stop();

	std::cout << "QueryClock: " << queryClock.GetElapsedTime() << std::endl;
}

void TestDelta()
{
	SimpleClock simpleClock;
	TickClock tickClock;
	WinMMClock winMMClock;
	QueryClock queryClock;

	const int LOOP_COUNT = 10;

	double simpleDelta = 0.0;

	simpleClock.Start();

	for (int i = 0; i < LOOP_COUNT; ++i)
	{
		DummyLoop();

		simpleClock.Stop();
		simpleDelta += simpleClock.GetElapsedTimeMS();
		simpleClock.Start();
	}
	simpleClock.Stop();

	std::cout << "SimpleClock: " << simpleDelta << std::endl;


	double tickDelta = 0.0;

	tickClock.Start();

	for (int i = 0; i < LOOP_COUNT; ++i)
	{
		DummyLoop();

		tickClock.Stop();
		tickDelta += tickClock.GetElapsedTimeMS();
		tickClock.Start();
	}

	tickClock.Stop();

	std::cout << "TickClock: " << tickDelta << std::endl;

	double winMMDelta = 0.0;

	winMMClock.Start();

	for (int i = 0; i < LOOP_COUNT; ++i)
	{
		DummyLoop();

		winMMClock.Stop();
		winMMDelta += winMMClock.GetElapsedTimeMS();
		winMMClock.Start();
	}

	winMMClock.Stop();

	std::cout << "WinMMClock: " << winMMDelta << std::endl;

	double queryDelta = 0.0;

	queryClock.Start();

	for (int i = 0; i < LOOP_COUNT; ++i)
	{
		DummyLoop();

		queryClock.Stop();
		queryDelta += queryClock.GetElapsedTimeMS();
		queryClock.Start();
	}

	queryClock.Stop();

	std::cout << "QueryClock: " << queryDelta << std::endl;

}

void TestLoopCount()
{
	const int CHECK_TIME = 1000; // ms

	SimpleClock simpleClock;
	TickClock tickClock;
	WinMMClock winMMClock;
	QueryClock queryClock;

	double elapsedTime = 0;
	int loopCount = 0;

	simpleClock.Start();

	while (elapsedTime < CHECK_TIME)
	{
		++loopCount;

		simpleClock.Stop();
		elapsedTime = simpleClock.GetElapsedTimeMS();
	}

	std::cout << "SimpleClock:\t " << elapsedTime << "  \t: " << loopCount << std::endl;

	elapsedTime = 0;
	loopCount = 0;

	tickClock.Start();

	while (elapsedTime < CHECK_TIME)
	{
		++loopCount;
		tickClock.Stop();
		elapsedTime = tickClock.GetElapsedTimeMS();
	}

	std::cout << "TickClock:\t " << elapsedTime << "  \t: " << loopCount << std::endl;

	elapsedTime = 0;
	loopCount = 0;

	winMMClock.Start();

	while (elapsedTime < CHECK_TIME)
	{
		++loopCount;
		winMMClock.Stop();
		elapsedTime = winMMClock.GetElapsedTimeMS();
	}

	std::cout << "WinMMClock:\t " << elapsedTime << "  \t: " << loopCount << std::endl;

	elapsedTime = 0;
	loopCount = 0;

	queryClock.Start();

	while (elapsedTime < CHECK_TIME)
	{
		++loopCount;
		queryClock.Stop();
		elapsedTime = queryClock.GetElapsedTimeMS();
	}

	std::cout << "QueryClock:\t " << elapsedTime << " \t: " << loopCount << std::endl;
}

int main()
{
	while (true)
	{
		//TestLoop();
		//TestSleep();
		//TestDelta();
		TestLoopCount();
	}
	
}