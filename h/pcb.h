/*
 * pcb.h
 *
 *  Created on: Aug 5, 2019
 *      Author: OS1
 */

#ifndef _PCB_H_
#define _PCB_H_

#include <iostream.h>
#include <stdlib.h>

#include "thread.h"
#include "def.h"
#include "queue.h"
#include "system.h"
#include "SCHEDULE.H"

class Queue;
class Thread;

class PCB {
public:
	PCB();
	Queue* waitQueue;
	PCB(Thread* nit,StackSize stackSize, Time timeSlice);
	//PCB(void(*)(),StackSize,Time);
	PCB(Thread* thread);
	~PCB();
	void createStack();
	void setStatus(ID _status) {
		status = _status;
	}
	int getStatus() {
		return status;
	}
	inline ID getID() {
		return ID;
	}
	static void wrapper();
	inline void run();
	static ID ukID;
	static volatile int brAktivnih;
	ID ID;
	volatile int uspavanaVremenski;
	volatile int status;
protected:
	friend class Thread;
	friend class Idle;
	friend class KernelSem;
	friend class System; /*::dispatch();*/
private:
	int timerFlag;
	Thread* myThread;
	unsigned int sp, ss, bp;

	volatile Time period;
	volatile int timeToWait; //brojac
	Time pcbTimeSlice;
	StackSize pcbStackSize;
	unsigned char* mySP;

};

#endif /* PCB_H_ */
