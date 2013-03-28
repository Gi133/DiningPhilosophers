#include <iostream>
#include <mutex>
#include <vector>

#include "PhilosopherThread.h"

// Global Variables.
enum PROGRAM_STATE {INITIALIZATION, USER_INPUT, SETUP, SIMULATION, RESULTS} state;
unsigned int num_philosophers = 0;
unsigned int num_forks  = 0;

// Threading/Mutexes
std::vector<std::mutex*> fork_vector; //The "forks" go here.
std::vector<PhilosopherThread*> philosopherthread_vector;

void Init()
{
	// Do any initializations or first-time computations here.

	state = USER_INPUT;
}

void UserInput()
{
	// Take in user input.
	std::cout << "Welcome to the Dining Philosopher application\n";
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
	else
		num_forks = 2;

	std::cout << "Thank you, please wait... \n\n";
}

void SimulationSetup()
{
	// This function sets up all the remaining program parameters and variables according to user input.
	// Initialize Philosophers and push them into a vector.
	for (int i = 0; i < num_philosophers; i++)
		philosopherthread_vector.push_back(new PhilosopherThread());

	for (int i = 0; i < num_forks; i++)
		fork_vector.push_back(new std::mutex());
}

void main()
{
	state = INITIALIZATION;

	switch (state)
	{
	case INITIALIZATION:
		{
			Init();
		}
	case USER_INPUT:
		{
			UserInput();
		}
	case SETUP:
		{
			SimulationSetup();
		}
	case SIMULATION:
		{
		}
	case RESULTS:
		{
		}
	}
}