#include "Philosopher.h"

Philosopher::Philosopher(void)
{
	this->time_starving = this->thinking_time = this->eating_time = 0;

	GeneratePhilosopherName();
	GenerateDelays();
	GenerateRepeats();

	this->state = THINKING;
}

Philosopher::~Philosopher(void)
{
}

void Philosopher::GeneratePhilosopherName()
{
	//Generate a random line number for the name.
	std::uniform_int_distribution<int> distribution(0, 106);
	std::mt19937 engine; // Using Mersenne twister MT19937
	auto generator = std::bind(distribution, engine);

	int line = generator(); // Generate a number.

	std::ifstream myfile ("PhilosopherNames.txt");

	for (int i = 0; i <= (line+1); i++)
	{
		std::getline(myfile, id);
	}

	myfile.close();
}

void Philosopher::GenerateDelays()
{
	std::uniform_int_distribution<int> distribution(0, 350);
	std::mt19937 engine;

	auto generator = std::bind(distribution, engine);

	// Generate a time (in ms).
	thinking_time = distribution(engine);
	eating_time = distribution(engine);
}

void Philosopher::GenerateRepeats()
{
	std::uniform_int_distribution<int> distribution(0, 5);
	std::mt19937 engine;

	auto generator = std::bind(distribution, engine);

	num_repeats = num_repeats_remaining = generator();
}