#include "PhilosopherThread.h"
#include <string>
#include <iostream>

PhilosopherThread::PhilosopherThread(std::mutex* cout_mutex, bool benchmark_mode)
{
	Initialize(cout_mutex, benchmark_mode);
	_philosopher->set_num_forks(2); // Default value.
	random_resources_ = false;
	SetBenchmarkMode(benchmark_mode);
}

PhilosopherThread::PhilosopherThread(std::mutex* cout_mutex, bool benchmark_mode, int force_resources) // If this overload is called then force the required resources value.
{
	Initialize(cout_mutex, benchmark_mode);
	_philosopher->set_num_forks(force_resources);
	random_resources_ = false;
	SetBenchmarkMode(benchmark_mode);
}

PhilosopherThread::PhilosopherThread(std::mutex* cout_mutex, bool benchmark_mode, bool random_resources) : random_resources_(random_resources)
{
	Initialize(cout_mutex, benchmark_mode);
	_philosopher->GenerateForkNumber();
	SetBenchmarkMode(benchmark_mode);
}

PhilosopherThread::~PhilosopherThread()
{
	// Join the thread so it doesn't run rampant around the system.
	_thread->join();
	delete(_philosopher);
}

void PhilosopherThread::Initialize(std::mutex* cout_mutex, bool /*benchmark_mode*/)
{
	// Create a Philosopher for this Thread.
	_philosopher = new Philosopher();

	sync_flag.store(false);

	_active = true;
	_timer_start = false;

	think_message_sent = wait_message_sent = eat_message_sent = false;

	this->_cout_mutex = cout_mutex;
}

void PhilosopherThread::SetBenchmarkMode(bool benchmark_mode) const
{
	if (benchmark_mode)
	{
		// Set delays to 0 and repeats to 1.
		_philosopher->set_num_repeats(1);
		_philosopher->set_eating_time(0);
		_philosopher->set_thinking_time(0);
	}
}

void PhilosopherThread::StartThread()
{
	_thread = new std::thread(&PhilosopherThread::ThreadFunction, this);
}

void PhilosopherThread::StopThread() const
{
	_thread->join();
}

void PhilosopherThread::ThreadFunction() // Function used by the thread
{
	while (this->_active) // Main Loop for the thread.
	{
		switch (_philosopher->get_state())
		{
		case Philosopher::THINKING:
			onThinking();
			break;
		case Philosopher::WAITING:
			onWaiting();
			break;
		case Philosopher::EATTING:
			onEating();
			break;
		}

		// Check if the loop is still active.
		if (_philosopher->get_num_repeats_remaining() <= 0)
		{
			this->_active = false;
			_philosopher->set_time_starving(0);
		}
	}
	// Make sure to let main know that thread has finished.
	sync_flag = true;
}

void PhilosopherThread::SendThinkMessage()
{
	// Check if message has already been sent.
	if (!think_message_sent)
	{
		_cout_mutex->lock();
		std::cout << _philosopher->get_id() << " is thinking." << std::endl;
		_cout_mutex->unlock();

		// Flip boolean value to indicate that the message has been sent.
		think_message_sent = true;
	}
}

void PhilosopherThread::SendWaitMessage()
{
	// Check if message has already been sent.
	if (!wait_message_sent)
	{
		_cout_mutex->lock();
		std::cout << _philosopher->get_id() << " is waiting." << std::endl;
		_cout_mutex->unlock();

		// Flip boolean value to indicate that the message has been sent.
		wait_message_sent = true;
	}
}

void PhilosopherThread::SendEatMessage()
{
	// Check if message has already been sent.
	if (!eat_message_sent)
	{
		_cout_mutex->lock();
		std::cout << _philosopher->get_id() << " is eating." << std::endl;
		_cout_mutex->unlock();

		// Flip boolean value to indicate that the message has been sent.
		eat_message_sent = true;
	}
}

void PhilosopherThread::SendPickupFork() const
{
	_cout_mutex->lock();
	std::cout << _philosopher->get_id() << " picked up a fork." << std::endl;
	_cout_mutex->unlock();
}

void PhilosopherThread::SendDropFork() const
{
	_cout_mutex->lock();
	std::cout << _philosopher->get_id() << " let down a fork." << std::endl;
	_cout_mutex->unlock();
}

void PhilosopherThread::SendRepeatsRemaining() const
{
	_cout_mutex->lock();
	std::cout << _philosopher->get_id() << " has " << _philosopher->get_num_repeats_remaining() << " repeats remaining." << std::endl;
	_cout_mutex->unlock();
}

void PhilosopherThread::Reset()
{
	think_message_sent = wait_message_sent = eat_message_sent = false;

	// Send philosopher back to the thinking-verse.
	_philosopher->set_state(Philosopher::THINKING);

	// Reset timer boolean.
	_timer_start = false;
}

void PhilosopherThread::UpdateStarvingTime()
{
	// Check if the timer has started or not, if not then start it.
	if (_timer_start == false)
	{
		start = std::chrono::system_clock::now();
		_timer_start = true;
	}

	// Calculations to find the exact time the thread has spent waiting.
	auto elapsed_seconds = std::chrono::duration_cast<std::chrono::milliseconds>
		(std::chrono::system_clock::now() - start).count();

	_philosopher->set_time_starving(elapsed_seconds);
}

void PhilosopherThread::onThinking()
{
	// Send status message.
	SendThinkMessage();

	//Do while thinking.
	auto timer_end = std::chrono::steady_clock::now()
		+ std::chrono::milliseconds(_philosopher->get_thinking_time());

	while (std::chrono::steady_clock::now() < timer_end)
	{
		// Do nothing.
	}

	_philosopher->set_state(Philosopher::WAITING);
}

void PhilosopherThread::onWaiting()
{
	// Send status message.
	SendWaitMessage();

	// Update starving time.
	UpdateStarvingTime();

	// Check if the required number of resources have been acquired.
	if (_acquired_resources.size() == _philosopher->get_num_forks()) // Number of resources required matches the number acquired.
	{
		_philosopher->set_time_starved();
		_philosopher->set_state(Philosopher::EATTING);
	}
}

void PhilosopherThread::onEating()
{
	// Figure out how long we will be using the acquired resources.
	auto timer_end = std::chrono::steady_clock::now()
		+ std::chrono::milliseconds(_philosopher->get_eating_time());

	// Lock the mutexes.
	LockResources();

	while (std::chrono::steady_clock::now() < timer_end)
	{
		// Do while eating.

		// Send status message.
		SendEatMessage();
	}

	// Unlock the mutexes.
	UnlockResources();

	// Decrease loop remains.
	_philosopher->set_num_repeats_remaining(_philosopher->get_num_repeats_remaining() - 1);

	// Print the remainder of repeats.
	SendRepeatsRemaining();

	// Reset variables.
	Reset();
}

void PhilosopherThread::AcquireMutex(Fork* aquired_mutex) // Mutex passed in as a pointer and added to the list of mutexes associated with this thread.
{
	_acquired_resources.push_back(aquired_mutex);
}

void PhilosopherThread::LockResources()
{
	// Function to lock the resources in the _aquired_resources vector.
	// Also has some debugging and feedback functionality. Will attempt to try and lock all the mutexes before proceeding.
	unsigned num_resources_aquired = 0;

	do
	{
		for (auto it = _acquired_resources.begin(); it < _acquired_resources.end(); it++)
		{
			SendPickupFork();

			num_resources_aquired++;

			(*it)->mutex_lock(_philosopher->get_id());
		}
	} while (num_resources_aquired < _philosopher->get_num_forks());
}

void PhilosopherThread::UnlockResources()
{
	for (auto i : _acquired_resources)
	{
		// Unlock the mutexes from thread
		i->mutex_unlock();

		SendDropFork();
	}

	// Clear the resource vector. Leaving Resource mutexes intact for further use.
	_acquired_resources.clear();

	sync_flag.store(true);
}