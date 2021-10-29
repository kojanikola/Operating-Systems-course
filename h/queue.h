/*
 * queue.h
 *
 *  Created on: Aug 9, 2019
 *      Author: OS1
 */

#ifndef QUEUE_H_
#define QUEUE_H_

#include <iostream.h>
#include <stdlib.h>
#include "thread.h"
#include "pcb.h"

class PCB;

struct Elem {
	PCB* pcb;
	Elem* next;
	Elem(PCB* p, Elem* s = NULL) {
		pcb = p;
		next = s;
	}
};

class Queue {
private:
	int size;

	Queue(const Queue& q) {
	}
	;
	Queue& operator =(const Queue& q) {
		return *this;
	}
public:
	Elem* prvi, *poslednji;
	friend class Thread;
	friend class KernelSem;
	Queue();
	~Queue();
	PCB* get();
	void put(PCB* pcb);
	int getSize() {
		return size;
	}
	PCB* getThreadByID(ID id);

};

#endif /* QUEUE_H_ */
