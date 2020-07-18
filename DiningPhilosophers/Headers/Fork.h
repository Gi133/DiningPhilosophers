// FORK CLASS:
// Class containing a mutex and ownership information (as opposed to using unique_locks).
// ----------------------------------------------------------------------------------------
// Ownership Information:
// String : Name of the owner (if available).
// Atomic Boolean : Used to show if the mutex is locked or not.
//

#pragma once
#include <mutex>
#include <atomic>

class Fork
{
	std::string _name;
	std::mutex _mutex;
	std::atomic<bool> _in_use; // True if mutex is locked, false if unlocked.

public:
	Fork();
	~Fork();

	// Getters
	std::string get_name() const { return this->_name; }
	bool get_is_locked() const { return this->_in_use.load(); }

	// Setters
	void set_in_use_bool(bool new_state) { this->_in_use.store(new_state); }
	void set_name(std::string new_name) { this->_name = new_name; }

	// Mutex Controls
	void mutex_lock(std::string owner_name);
	void mutex_unlock();
};
