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

public:
	//Functions
	Philosopher(void);
	~Philosopher(void);

	enum StateText {THINKING, EATTING, WAITING} state;

	void GeneratePhilosopherName();
	void GenerateDelays(); //Generate eating_time and thinking_time.

	//Getters
	std::string get_id(){return id;}
	StateText get_state(){return state;}
	__int64 get_time_starving(){return time_starving;}
	int get_thinking_time(){return thinking_time;}
	int get_eating_time(){return eating_time;}

	//Setters
	void set_id(std::string id){this->id = id;}
	void set_state(StateText state){this->state = state;}
	void set_time_starving(__int64 time_starving){this->time_starving = time_starving;}
	void set_thinking_time(int thinking_time){this->thinking_time = thinking_time;}
}; 