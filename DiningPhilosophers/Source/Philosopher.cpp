#include <string>
#include <fstream>
#include <iostream>

#include <Philosopher.h>

Philosopher::Philosopher()
{
	this->time_starving = this->thinking_time = this->eating_time = 0;

	GeneratePhilosopherName();
	GenerateDelays();
	GenerateRepeats();

	this->state = THINKING;
}

Philosopher::~Philosopher()
{
}

void Philosopher::GenerateForkNumber()
{
	this->num_forks = GenerateRandomNumber(1, 4);
}

unsigned int Philosopher::GenerateRandomNumber(int min_num, int max_num) const
{
	std::random_device rd;
	std::mt19937 _rng_engine(rd());

	//Get Distribution with min and max values.
	std::uniform_int_distribution<unsigned int> _distribution(min_num, max_num);
	std::function<unsigned int()> _random_num = bind(_distribution, _rng_engine);
	return _random_num();
}

void Philosopher::GeneratePhilosopherName()
{
	std::ifstream myfile("PhilosopherNames.txt");

	for (unsigned i = 0; i <= GenerateRandomNumber(1, 107); i++)
		getline(myfile, id);

	myfile.close();
}

void Philosopher::GenerateDelays()
{
	// Generate a time (in ms).
	thinking_time = GenerateRandomNumber(0, 350);
	eating_time = GenerateRandomNumber(0, 350);
}

void Philosopher::GenerateRepeats()
{
	num_repeats = num_repeats_remaining = GenerateRandomNumber(1, 5);
}

void Philosopher::PrintResults()
{
	long long total_time = 0;

	// Go through the vector with the starvation times and print the times.
	std::cout << id << " has starved " << starving_times.size() << " time/s" << std::endl;
	std::cout << "Times the philosopher spent starving: ";
	for (auto i : starving_times)
	{
		std::cout << i << "ms ";
		total_time += i;
	}
	std::cout << "." << std::endl;
	std::cout << "On average, time spent starving :" << total_time / starving_times.size() << "ms." << std::endl << std::endl;
}