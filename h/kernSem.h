/*
 * kernSem.h
 *
 *  Created on: Aug 14, 2019
 *      Author: OS1
 */

#ifndef KERNSEM_H_
#define KERNSEM_H_

#include "queue.h"
#include "def.h"
#include "semaphor.h"
#include "system.h"

class Queue;

class KernelSem {
private:
	int val;
public:
	Queue* semQueue;
	KernelSem(int init);
	~KernelSem();

	int signal(int n=0);
	int wait(Time maxTimeToWait);
	void signalAll();
	void sleep(Time maxTimeToWait);
protected:
	friend class Semaphore;
	friend class System;
};

#endif /* KERNSEM_H_ */
