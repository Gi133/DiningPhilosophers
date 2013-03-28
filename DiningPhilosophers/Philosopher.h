// Holds the philosopher's data values.
// See README.md for further info.

#pragma once
#include <string>
#include <random>
#include <functional>
#include <iostream>
#include <fstream>

class Philosopher
{
private:
	//Variables
	std::string id; //Philosopher's name. Picked from a list of names.
	__int64 time_starving; //Time philosopher spent waiting for "forks".
	int thinking_time; //How long the philosopher will spend thinking, randomly generated.
	int eating_time; //How long the philosopher will spend eating, randomly generated.
	unsigned int num_forks; // Forks the Philosopher needs, could have more than 2 hands.
	unsigned int num_repeats; // Number of repeats (total).
	unsigned int num_repeats_remaining; // NUmber of repeats remaining.

	void GeneratePhilosopherName();
	void GenerateDelays(); // Generate eating_time and thinking_time.
	void GenerateRepeats(); // Generate the number of times the philosopher will get hungry.

public:
	//Functions
	Philosopher(void);
	~Philosopher(void);

	enum StateText {THINKING, EATTING, WAITING} state;

	//Getters
	std::string get_id(){return this->id;}
	StateText get_state(){return this->state;}
	__int64 get_time_starving(){return this->time_starving;}
	int get_thinking_time(){return this->thinking_time;}
	int get_eating_time(){return this->eating_time;}
	unsigned int get_num_forks(){return this->num_forks;}
	unsigned int get_num_repeats(){return this->num_repeats;}
	unsigned int get_num_repeats_remaining(){return this->num_repeats_remaining;}

	//Setters
	void set_id(std::string id){this->id = id;}
	void set_state(StateText state){this->state = state;}
	void set_time_starving(__int64 time_starving){this->time_starving = time_starving;}
	void set_thinking_time(int thinking_time){this->thinking_time = thinking_time;}
	void set_num_forks(unsigned int num_forks){this->num_forks = num_forks;}
	void set_num_repeats(unsigned int num_repeats){this->num_repeats = num_repeats;}
	void set_num_repeats_remaining(unsigned int num_repeats_remaining){this->num_repeats_remaining = num_repeats_remaining;}
}; 