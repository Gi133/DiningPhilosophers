// Notes:
// A possibility to optimize code even more would be to map out the mutex vector,
// knowing where groups of open mutexes begin and end would make the program able
// to assign mutexes to multiple threads in the same loop cycle instead of wasting
// cycles/time giving mutexes to one thread at a time.
// Also, the Benchmark mode needs work, at the moment it's more of a gimmick that
// an actual working thing. To turn it to an actual benchmark mode all randomness
// and wait times will need to be removed from the philosophers so things are executed
// instantly and it all falls onto the CPU, this can be done by passing an extra
// value in the philosopher constructor that will initialize the object without any
// delay times.
// -----------------------------------------------------------------------------------
// PROBLEM DURING INITIAL RELEASE:
// The main thread would loop a great amount of times, assigning the same unique
// locks to threads that were not up to that step yet. Threads are acting
// unexpectedly slow, also due to bad code and inexperience the synchronization
// needed was not taken into account initially.
//	POSSIBLE FIXES:
//		* Attempt to synchronize the threads with the main thread so that when a
//		thread is assigned mutexes the main loop waits for that thread to
//		acknowledge the fact and lock them before moving on.
//			* In C++10 this could be achieved with a barrier or semaphore,
//			which would be used to make sure that threads and main thread are up
//			to speed with each other or just lock main while thread caught up.
//			* C++11 provides a lot of ways to signal between threads and main thread,
//			things like atomic_flag and others. An atomic boolean was picked to fix
//			the problem, which would be stored either in main and accessed by the
//			thread to tell main that it's OK to proceed or it can be stored in thread
//			class where the thread associated to the class would update it and main
//			would just wait for the flag to be flipped in order to move on. (The last
//			method was used).
//		* Another way would be to wrap a custom template over the mutex, instead of
//		using unique_lock that would hold a boolean and use that boolean for the
//		different tests and assigning of mutexes to threads.
//		* A cheap way of working around this would also be to make the threads
//		aware of each other and work that way but that is not the optimal way to
//		work on multi-threading because usually you don't have specifics, don't
//		have something as trivial as the example here and it is simply brute forcing
//		the issue. So this fix has been noted down as a possibility but ignored.
// -----------------------------------------------------------------------------------
// ADDITIONAL NOTES: Builds from this project will not be able to run on a machine
// that does not include Visual C++ Redistributable for Visual Studio 2012 or does
// not have Visual Studio 2012 installed (which also installs the redistributable
// package on the machine.
// Link to most recent redistributable pack:
// http://www.microsoft.com/en-us/download/details.aspx?id=30679
//

#include <string>
#include <iostream>
#include <algorithm>
#include <mutex>
#include <vector>
#include <atomic>

#include "PhilosopherThread.h"
#include "Fork.h"
#include <cctype>

// Global Variables.
enum PROGRAM_STATE { INITIALIZATION, USER_INPUT, SETUP, SIMULATION, RESULTS } state;
unsigned int num_philosophers = 0;
unsigned int num_forks = 0;

// Benchmarking Values.
bool benchmark_mode = false;
std::chrono::time_point<std::chrono::system_clock> benchmark_start;
long long benchmark_time = 0;

// Threading/Mutexes
std::vector<Fork*> fork_vector; //The "forks" go here.
std::vector<PhilosopherThread*> philosopherthread_vector;
std::vector<PhilosopherThread*> finished_philosopher_vector;
std::mutex* cout_mutex;

// Mutex Section Checks
std::vector<Fork*>::iterator fork_section_start, fork_section_finish; // Variables to record mutex_section_start and mutex_section_finish of each mutex section.
bool fork_section_loop = false; // If the mutex section loops around from end to beginning.

void Init()
{
	// Do any initializations or first-time computations here.
	cout_mutex = new std::mutex();

	state = USER_INPUT;
}

void UserInput()
{
	std::string mode;

	cout_mutex->lock();
	// Take in user input.
	std::cout << "Welcome to the Dining Philosopher application" << std::endl;
	std::cout << "Select which mode you wish to use, type benchmark or manual: ";
	std::cin >> mode;
	std::cout << std::endl;

	for (auto letter : mode)
		tolower(letter);

	if (mode == "benchmark")
	{
		std::cout << "Selected 'Benchmark'\n";
		std::cout << "Number of threads (2-64), defaults to 64: ";
		std::cin >> num_forks;
		if (num_forks < 2 || num_forks > 64)
		{
			// Initialize benchmark values
			num_philosophers = num_forks = 64;
		}
		else
		{
			// Initialize benchmark values
			num_philosophers = num_forks;
		}
		std::cout << std::endl;
		benchmark_mode = true;
	}
	else
	{
		std::cout << "Selected 'Manual'\n";
		std::cout << "Input the number of Philosophers (2-64): ";
		std::cin >> num_philosophers;

		// Check if philosopher number is valid.
		if (num_philosophers > 64)
			num_philosophers = 64;
		else if (num_philosophers < 2)
			num_philosophers = 2;

		std::cout << "Input the number of Forks (use 0 to have forks be same number as philosophers and can not be more than Philosophers): ";
		std::cin >> num_forks;

		// Check if number of forks is valid. Safeguard for extreme numbers.
		if (num_forks > 64)
			num_forks = 64;
		else if (num_forks == 0)
			num_forks = num_philosophers;
		else if (num_forks <= 2)
			num_forks = 2;

		benchmark_mode = false;
	}

	std::cout << std::endl << "Thank you, please wait... \n\n";
	cout_mutex->unlock();

	state = SETUP;
}

void SimulationSetup()
{
	// This function sets up all the remaining program parameters and variables according to user input.
	// Initialize Philosophers and push them into a vector.
	for (unsigned i = 0; i < num_philosophers; i++)
		philosopherthread_vector.push_back(new PhilosopherThread(cout_mutex, benchmark_mode));

	for (unsigned i = 0; i < num_forks; i++)
		fork_vector.push_back(new Fork());

	state = SIMULATION;
}

bool CheckMutexes(unsigned int num_needed)
{
	// Return true if the required number of mutexes are available, false if not.
	// Keep going through the vector if the first group didn't match the required number.

	// Mutex check variables
	auto _has_fork_section_start = false;
	unsigned int num_mutex_available = 0;

	// Go through mutex vector looking for enough open forks for the thread.
	for (auto it = fork_vector.begin(); it < fork_vector.end(); ++it)
	{
		if ((*it)->get_is_locked()) // If mutex is locked
		{
			if (_has_fork_section_start) // and there's already an "opened" section.
			{
				fork_section_finish = it - 1; // End of open mutex section is the current iterator - 1.

				if (num_mutex_available == num_needed)
					return true;
				_has_fork_section_start = false;
			}
		}
		else // If mutex is unlocked
		{
			if (!_has_fork_section_start) // and current group hasn't mutex_section_started.
			{
				_has_fork_section_start = true;
				fork_section_start = it; // mutex_section_start the mutex group.
				num_mutex_available++;
			}
			else // and current group has a mutex_section_start
			{
				num_mutex_available++;
				if (num_mutex_available == num_needed)
				{
					fork_section_finish = it; // End of open mutex section is the current iterator.
					return true;
				}
			}

			// If iterator is at the end of the vector, and end is unlocked, check beginning and link up.
			if (it == fork_vector.end())
			{
				if (fork_vector.front()->get_is_locked()) // First mutex is also unlocked, so add it to the available number.
				{
					num_mutex_available++;
					fork_section_loop = true;
				}
			}
		}
	}
	// If out of loop without a return true then required number of mutexes is not available.
	fork_section_loop = false;
	return false;
}

void AssignMutexes()
{
	// Initial philosopher
	auto active_philosopher = philosopherthread_vector.begin();

	// Find the required number of mutexes needed for thread.
	auto active_required_resources = (*active_philosopher)->get_num_forks();

	auto _success = false;

	do
	{
		// Check mutexes
		if (CheckMutexes(active_required_resources))
		{
			// Assign mutexes in section to thread class
			for (std::vector<Fork*>::iterator it = fork_section_start; it <= fork_section_finish; it++)
			{
				(*active_philosopher)->AcquireMutex((*it));
			}
			// Also, check if loop boolean is true. If so, add the front mutex too.
			if (fork_section_loop)
			{
				(*active_philosopher)->AcquireMutex(fork_vector.front());
			}
			_success = true;
		} // if false, then move to next one.
		else
		{
			if (active_philosopher == philosopherthread_vector.end())
				active_philosopher = philosopherthread_vector.begin();
			else
				++active_philosopher;
		}
	} while (!_success);

	if ((*active_philosopher)->get_active())
	{
		// Wait for sync flag.
		while (!(*active_philosopher)->get_sync_flag())
		{
			// Do nothing.
		}

		// Reset the sync flag as knowledgeable.
		(*active_philosopher)->set_sync_flag(false);
	}
}

void SortPhilosopherThreads()
{
	// Sort the thread vector according to wait time.
	std::sort(philosopherthread_vector.begin(), philosopherthread_vector.end(),
		[](PhilosopherThread* lhs, PhilosopherThread* rhs)
	{
		if (lhs->get_time_starving() > rhs->get_time_starving())
			return true;
		return false;
	});
}

bool CheckSimulationActive()
{
	// To check if the simulation is still active, set the variable to false (signify it's not active)
	// and go through all the thread class objects, if an active one is encountered then the simulation
	// is still active so break; and keep working on simulation.
	// Return TRUE if the simulation is still on-going, FALSE if it's not.

	for (auto i : philosopherthread_vector)
	{
		if (i->get_active())
			return true;
	}
	return false;
}

void Simulation()
{
	// Function that will handle all the simulation in the philosopher's problem.
	auto _simulation_active = true;

	// mutex_section_start up all the threads.
	for (auto i : philosopherthread_vector)
		i->StartThread();

	while (_simulation_active)
	{
		SortPhilosopherThreads();

		AssignMutexes();

		_simulation_active = CheckSimulationActive();
	}

	state = RESULTS;
}

void Results()
{
	cout_mutex->lock();
	std::cout << std::endl << "RESULTS MODE!\n" << std::endl;
	cout_mutex->unlock();

	// Go through all philosophers calling PrintResults();
	for (auto i : philosopherthread_vector)
	{
		i->PrintResults();
	}

	// If benchmarking was enabled then also print the time taken.
	if (benchmark_mode)
		std::cout << std::endl << "BENCHMARK TIME: " << benchmark_time << "ms." << std::endl;
}

int main()
{
	state = INITIALIZATION;
	auto program_active = true;

	while (program_active)
	{
		switch (state)
		{
		case INITIALIZATION:
		{
			std::thread InitThread(Init);
			InitThread.join();
			break;
		}
		case USER_INPUT:
		{
			std::thread UserInputThread(UserInput);
			UserInputThread.join();
			break;
		}
		case SETUP:
		{
			std::thread SimulationSetupThread(SimulationSetup);
			SimulationSetupThread.join();
			break;
		}
		case SIMULATION:
		{
			// Start the benchmark clock if benchmark mode is enabled.
			if (benchmark_mode)
				benchmark_start = std::chrono::system_clock::now();

			Simulation();

			// Calculate time needed for simulation to finish.
			if (benchmark_mode)
			{
				benchmark_time = std::chrono::duration_cast<std::chrono::milliseconds>
					(std::chrono::system_clock::now() - benchmark_start).count();
			}

			break;
		}
		case RESULTS:
		{
			Results();
			program_active = false;
			break;
		}
		}
	}

	return 0;
}