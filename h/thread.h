#ifndef _thread_h_
#define _thread_h_

typedef unsigned long StackSize;
const StackSize defaultStackSize = 4096;
typedef unsigned int Time; // time, x 55ms
const Time defaultTimeSlice = 2; // default = 2*55ms
typedef int ID;

#include "pcb.h"
#include "def.h"
//#include "idle.h"
#include <stdlib.h>

class PCB;
//class Idle;

class Thread {
public:
	Thread(StackSize stackSize = defaultStackSize, Time timeSlice =
			defaultTimeSlice);
	void start();
	void waitToComplete();
	virtual ~Thread();
	ID getId();
	static ID getRunningId();
	static Thread * getThreadById(ID id);
	PCB* myPCB;
protected:
	friend class PCB;
	friend class System;
	//friend class Idle;
	virtual void run() {
	}
private:

};

void dispatch();
extern void tick();
Time minTimeSlice();

#endif
