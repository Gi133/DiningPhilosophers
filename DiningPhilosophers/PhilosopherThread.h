// Philosopher Thread class.
// Unlike the philosopher thread, that acts as a data container, this class has the functionality of each philosopher thread.
// It is to be used in conjunction with the Philosopher class.
//
// Philosopher thread has a Philosopher where it stores data. This is created on the constructor and stored in the Thread class.

#pragma once
#include <thread>
#include <mutex>
#include <windows.h>
#include <chrono>
#include "Philosopher.h"

class PhilosopherThread
{
private:
	Philosopher* _philosopher;
	std::thread* _thread;
	std::vector<std::mutex*> _aquired_resources;

	bool _active;
	int _required_resources;
	bool _timer_start;
	std::chrono::time_point<std::chrono::system_clock> start, end; // Used for timer.

	void ThreadFunction();
	void onThinking();
	void onWaiting();
	void onEating();

	void StartThread();
	void LockResources();
	void UnlockResources();

	void CalculateWaitTime();

public:
	PhilosopherThread(void);
	PhilosopherThread(int force_resources);
	PhilosopherThread(bool random_resources);
	~PhilosopherThread(void);

	//Mutex Acquisition
	void AquireMutex(std::mutex* aquired_mutex);
};