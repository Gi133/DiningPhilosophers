// Philosopher Thread class.
// Unlike the philosopher thread, that acts as a data container, this class has the functionality of each philosopher thread.
// It is to be used in conjunction with the Philosopher class.
//
// Philosopher thread has a Philosopher where it stores data. This is created on the constructor and stored in the Thread class.

#pragma once
#include <thread>
#include <mutex>
#include <chrono>
#include <atomic>

#include "Philosopher.h"
#include "Fork.h"

class PhilosopherThread
{
	Philosopher* _philosopher;
	std::thread* _thread;
	std::vector<Fork*> _acquired_resources;
	std::mutex* _cout_mutex;

	// Atomics
	std::atomic<bool> sync_flag; // Synchronization flag, true if main should proceed, false if thread is processing.

	// State messaging variables/functions.
	bool think_message_sent, wait_message_sent, eat_message_sent;
	bool random_resources_;
	void SendThinkMessage();
	void SendWaitMessage();
	void SendEatMessage();
	void SendPickupFork() const;
	void SendDropFork() const;
	void SendRepeatsRemaining() const;

	void Reset();

	bool _active;
	bool _timer_start;
	std::chrono::time_point<std::chrono::system_clock> start, end; // Used for timer.

	void Initialize(std::mutex* cout_mutex, bool benchmark_mode);
	void SetBenchmarkMode(bool benchmark_mode) const;
	void ThreadFunction();
	void onThinking();
	void onWaiting();
	void onEating();

	void UpdateStarvingTime();

	void LockResources();
	void UnlockResources();

public:
	PhilosopherThread(std::mutex* cout_mutex, bool benchmark_mode);
	PhilosopherThread(std::mutex* cout_mutex, bool benchmark_mode, int force_resources);
	PhilosopherThread(std::mutex* cout_mutex, bool benchmark_mode, bool random_resources);
	~PhilosopherThread();

	// Controllers
	void StartThread();
	void StopThread() const;

	// Mutex Acquisition
	void AcquireMutex(Fork* aquired_mutex);

	// Access atomic values.
	void set_sync_flag(bool set_sync_flag_state) { this->sync_flag.store(set_sync_flag_state); }
	bool get_sync_flag() const { return this->sync_flag.load(); }

	// Philosopher Class access.
	std::string get_id() const { return this->_philosopher->get_id(); }
	long long get_time_starving() const { return this->_philosopher->get_time_starving(); }
	int get_thinking_time() const { return this->_philosopher->get_thinking_time(); }
	int get_eating_time() const { return this->_philosopher->get_eating_time(); }
	unsigned int get_num_forks() const { return this->_philosopher->get_num_forks(); }
	unsigned int get_num_repeats() const { return this->_philosopher->get_num_repeats(); }
	unsigned int get_num_repeats_remaining() const { return this->_philosopher->get_num_repeats_remaining(); }
	bool get_active() const { return this->_active; }
	void PrintResults() const { _philosopher->PrintResults(); }
};