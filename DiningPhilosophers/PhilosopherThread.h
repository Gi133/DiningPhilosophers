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
	std::vector<std::unique_lock<std::mutex>*> _aquired_resources;

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

	// Mutex Acquisition
	void AquireMutex(std::unique_lock<std::mutex>* aquired_mutex);

	// Philosopher Class access.
	std::string get_id(){return this->_philosopher->get_id();}
	__int64 get_time_starving(){return this->_philosopher->get_time_starving();}
	int get_thinking_time(){return this->_philosopher->get_thinking_time();}
	int get_eating_time(){return this->_philosopher->get_eating_time();}
	unsigned int get_num_forks(){return this->_philosopher->get_num_forks();}
	unsigned int get_num_repeats(){return this->_philosopher->get_num_repeats();}
	unsigned int get_num_repeats_remaining(){return this->_philosopher->get_num_repeats_remaining();}
};