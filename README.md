#Dining Philosophers Operating Systems Assessment

##Problem:
Five philosophers spend all their life eating and doing what philosophers do.
Their dining table has 5 forks/chopsticks on it for the same amount of philosophers
(5 philosophers but can be N if the algorithm is done correctly to avoid deadlock).
Each philosopher thinks for a while, then goes to eat. When he sits down he picks up 2 chopsticks,
if 2 chopsticks are not available then he waits for a pair to free up before eating. After eating,
he puts down the chopsticks and goes back to thinking.

Main problem is that there are not enough chopsticks for all philosophers so the algorithm has to
stop them from starving to death while waiting for a chopstick to free up.

###Solutions:
Use mutexes as chopsticks and make sure a pair is ready for each thread and/or use a semaphore as a waiter 
so that the philosophers have to go through before sitting down to eat. Using a semaphore will also help with further
optimization to avoid deadlocks.

##IMPLEMENTATION:
	-Application States:
		-Initialization and Input
		-Simulation
		-Results
	
 	-Philosopher Class:
		-ID := String (use names from a list of known philosophers.
		-State := Enum (Eating, Thinking, Waiting).
		-Wait := long long int (Time spent starving to death).
		-Thinking_Time := unsigned int (Time the philosopher will spend thinking).
		-Eating_Time := unsigned int (Time the philosopher needs to sate his hunger).
	
	-Chopstick Array := Mutex array. 
		 -Waiter := Semaphore (Dude who checks and prioritizes the philosophers), most probably handled through program logic.

###How will stuff work:
The program will take in a number of threads (philosophers) and shared memory (forks in this case). After this is done it will initialize all the philosophers and forks.
After this is done and initialized and the philosophers -classes in the code- are given their thinking times then the program will switch to it's next state where the waiter (semaphore) 
starts checking which threads are "starving" and crosschecking whether there are enough forks for them. All philosophers will start thinking for a random amount of time then become hungry.
Once they become hungry they enter a "waiting" state, at this state a timer starts off to count the time the philosopher has spent starving. This value will be accessible by the waiter.

A sorted vector of handles could be used to prioritize the threads depending on how long they have spent waiting to "eat".
So, priority will be given to threads that have waited the longest and the semaphore will let them in. The philosophers will then eat for whatever amount of time was randomly generated for Eating_Time
and go back to thinking until they become hungry again.

This process could go on forever but ideally there will be a goal. Maybe once a philosopher ate X number of times then he will enter a "finished" state and won't go hungry again.
Once all philosophers have entered this state then the program will end and enter a "Results" state where all the benchmark data will be shown.

##Further Deadlock Prevention:
Weighting threads according to their "waiting" time should solve most of the deadlock issues found even with mutexes or semaphores and other synchronization objects. This adds an extra layer of protection
to multiple threads that share the same resources. The algorithm will try to keep all threads working with as less down-time as possible. A step further would be the ability to "kick" a thread but depending
programming language and OS limitations that may or may not be feasible.
