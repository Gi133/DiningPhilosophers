// Holds the philosopher's data values.
// See README.md for further info.
// ----------------------------------------
// Note on random number generation:
// Random Number generator will generate numbers for names that have already been used,
// this can be avoided by adding a vector of unsigned ints which will store line numbers that have already been used and force
// a new number to be generated, testing that number again and so on till one is found that hasn't been used before.
// This is the only real fix to some names appearing more than once, depending on time available the fix may be implemented.
// PRIORITY : LOW.
// ----------------------------------------

#pragma once
#include <random>
#include <functional>

class Philosopher
{
	// Variables
	std::string id; //Philosopher's name. Picked from a list of names.
	long long time_starving; //Time philosopher spent waiting for "forks".
	int thinking_time; //How long the philosopher will spend thinking, randomly generated.
	int eating_time; //How long the philosopher will spend eating, randomly generated.
	unsigned int num_forks; // Forks the Philosopher needs, could have more than 2 hands.
	unsigned int num_repeats; // Number of repeats (total).
	unsigned int num_repeats_remaining; // Number of repeats remaining.

	// Starving times
	std::vector<__int64> starving_times;

	// RNG Stuff
	unsigned int GenerateRandomNumber(int min_num, int max_num) const;

	void GeneratePhilosopherName();
	void GenerateDelays(); // Generate eating_time and thinking_time.
	void GenerateRepeats(); // Generate the number of times the philosopher will get hungry.

public:
	// Functions
	Philosopher();
	~Philosopher();

	enum StateText { THINKING, EATTING, WAITING } state;

	// Getters
	std::string get_id() const { return this->id; }
	StateText get_state() const { return this->state; }
	long long get_time_starving() const { return this->time_starving; }
	int get_thinking_time() const { return this->thinking_time; }
	int get_eating_time() const { return this->eating_time; }
	unsigned int get_num_forks() const { return this->num_forks; }
	unsigned int get_num_repeats() const { return this->num_repeats; }
	unsigned int get_num_repeats_remaining() const { return this->num_repeats_remaining; }

	// Setters
	void set_id(std::string id) { this->id = id; }
	void set_state(StateText state) { this->state = state; }
	void set_time_starving(__int64 time_starving) { this->time_starving = time_starving; }
	void set_time_starved() { this->starving_times.push_back(time_starving); }
	void set_eating_time(unsigned int eating_time) { this->eating_time = eating_time; }
	void set_thinking_time(int thinking_time) { this->thinking_time = thinking_time; }
	void set_num_forks(unsigned int num_forks) { this->num_forks = num_forks; }
	void set_num_repeats(unsigned int num_repeats) { this->num_repeats = this->num_repeats_remaining = num_repeats; }
	void set_num_repeats_remaining(unsigned int num_repeats_remaining) { this->num_repeats_remaining = num_repeats_remaining; }
	void GenerateForkNumber(); // Generate a random number of required forks.

	// Results
	void PrintResults();
};