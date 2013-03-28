#include "PhilosopherThread.h"

PhilosopherThread::PhilosopherThread(void)
{
	// Create a Philosopher for this Thread.
	_philosopher = new Philosopher();

	_active = true;
	_timer_start = false;
	_required_resources = 2; // Default value.
	_aquired_resources.clear(); // Make sure the vector is clear.
}

PhilosopherThread::PhilosopherThread(int force_resources) // If this overload is called then force the required resources value.
{
	// Create a Philosopher for this Thread.
	_philosopher = new Philosopher();

	_active = true;
	_timer_start = false;

	this->_required_resources = force_resources; 
}

PhilosopherThread::PhilosopherThread(bool random_resources)
{
	// Create a Philosopher for this Thread.
	_philosopher = new Philosopher();

	_active = true;
	_timer_start = false;

	std::uniform_int_distribution<int> distribution(2, 4); // Boundaries
	std::mt19937 engine; // Using Mersenne twister MT19937
	auto generator = std::bind(distribution, engine);

	_required_resources = generator(); // Generate a random number.
}

PhilosopherThread::~PhilosopherThread(void)
{
	// Join the thread so it doesn't run rampant around the system.
	_thread->join();
}

void PhilosopherThread::StartThread()
{
	_thread = new std::thread(&PhilosopherThread::ThreadFunction, this);
}

void PhilosopherThread::ThreadFunction() // Function used by the thread
{
	while (this->_active) // Main Loop for the thread.
	{
		switch (_philosopher->get_state())
		{
		case _philosopher->THINKING:
			{
				onThinking();
			}
		case _philosopher->WAITING:
			{
				onWaiting();
			}
		case _philosopher->EATTING:
			{
				onEating();
			}
		}
	}
}

void PhilosopherThread::onThinking()
{
	//Do while thinking.
	std::chrono::steady_clock::time_point timer_end = std::chrono::steady_clock::now()
		+ std::chrono::milliseconds(_philosopher->get_thinking_time());

	while (std::chrono::steady_clock::now() < timer_end)
	{
		// Do nothing.
	}

	_philosopher->set_state(Philosopher::WAITING);
}

void PhilosopherThread::onWaiting()
{
	if (_timer_start == FALSE)
	{
		start = std::chrono::system_clock::now();
		_timer_start = true;
	}

	// Check if the required number of resources have been acquired.
	if (_aquired_resources.size() == _required_resources) // Number of resources required matches the number acquired.
		_philosopher->set_state(Philosopher::EATTING);
}

void PhilosopherThread::onEating()
{
	// Find out how long the thread has been starving.
	if (_philosopher->get_time_starving() == 0) // Make sure we have done this only once.
		CalculateWaitTime();

	// Figure out how long we will be using the acquired resources.
	std::chrono::steady_clock::time_point timer_end = std::chrono::steady_clock::now()
		+ std::chrono::milliseconds(_philosopher->get_eating_time());

	// Lock the mutexes.
	LockResources();
	

	while (std::chrono::steady_clock::now() < timer_end)
	{
		// Do while eating.
	}

	// Unlock the mutexes.
	UnlockResources();
}

void PhilosopherThread::CalculateWaitTime()
{
	end = std::chrono::system_clock::now();

	__int64 elapsed_seconds = std::chrono::duration_cast<std::chrono::milliseconds>
		(end-start).count();

	_philosopher->set_time_starving(elapsed_seconds);
}

void PhilosopherThread::AquireMutex(std::mutex* aquired_mutex) // Mutex passed in as a pointer and added to the list of mutexes associated with this thread.
{
	_aquired_resources.push_back(aquired_mutex);
}

void PhilosopherThread::LockResources()
{
	// Function to lock the resources in the _aquired_resources vector. 
	// Also has some debugging and feedback functionality. Will attempt to try and lock all the mutexes before proceeding.
	int num_resources_aquired = 0;

	do 
	{
		for (std::mutex* i : _aquired_resources)
		{
			if (!i->try_lock()) // Attempt to lock the mutex.
			{
				std::cout << _philosopher->get_id() << " locked a mutex.";
				num_resources_aquired ++;
			}
			else
				std::cout << _philosopher->get_id() << " failed to lock a mutex.";
		}
	} while (num_resources_aquired < _required_resources);
}

void PhilosopherThread::UnlockResources()
{
	for (std::mutex* i : _aquired_resources )
		i->unlock();
}