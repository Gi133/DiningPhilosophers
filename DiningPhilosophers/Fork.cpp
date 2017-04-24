#include "Fork.h"

Fork::Fork()
{
	_name = "NOT_SET";
	_in_use.store(false);
}

Fork::~Fork()
{
}

void Fork::mutex_lock(std::string owner_name)
{
	_in_use.store(true);
	_name = owner_name;

	// Lock the mutex.
	_mutex.lock();
}

void Fork::mutex_unlock()
{
	_in_use.store(false);
	_name = "NOT_SET";

	// Unlock the mutex.
	_mutex.unlock();
}