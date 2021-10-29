/*
 * kernSem.cpp
 *
 *  Created on: Aug 14, 2019
 *      Author: OS1
 */

#include "kernSem.h"
#include "thread.h"

KernelSem::KernelSem(int init) {
	val = init;
	semQueue = new Queue();
	System::listaSvihSem->put(this);
}

KernelSem::~KernelSem() {
	signalAll();
	delete semQueue;
}

int KernelSem::wait(Time maxTimeToWait) {
	if (--val < 0){
		sleep(maxTimeToWait);
	}

	if (System::running->timerFlag == 1) //timerFlag ti je da l je odblokiran od tajmera ili obicno
		return 0;
	return 1;
}

void KernelSem::sleep(Time maxTimeToWait) {
	if (maxTimeToWait > 0) {
		System::running->uspavanaVremenski = 1;
		System::running->timeToWait = maxTimeToWait;
		System::running->status = BLOCKED;
		semQueue->put((PCB*) System::running);
		System::dispatch();
	} else {
		System::running->status = BLOCKED;
		semQueue->put((PCB*) System::running);
		System::dispatch();
	}
}

int KernelSem::signal(int n) {
	if (n < 0)
		return n;
	if (n == 0) {
		if (++val <= 0) {
			PCB* pom = semQueue->get();
			if (pom) {
				pom->uspavanaVremenski=0;
				pom->status = READY;
				Scheduler::put(pom);
			}
			return 0;
		}
	}
	if (n > 0) {
		if (n >= abs(val)) {
			int odb = val;
			for (int i = 0; i < val; i++) {
				PCB* pom = semQueue->get();
				if (pom) {
					pom->uspavanaVremenski=0;
					pom->status = READY;
					Scheduler::put(pom);
				}
			}
			val += n;
			return odb;
		}
		if (n < abs(val)) {
			for (int i = 0; i < n; i++) {
				PCB* pom = semQueue->get();
				if (pom) {
					pom->uspavanaVremenski=0;
					pom->status = READY;
					Scheduler::put(pom);
				}
			}
			val += n;
			return n;
		}
	}

}

void KernelSem::signalAll() {
	while (semQueue->prvi) {
		PCB* pom = semQueue->get();
		pom->status = READY;
		Scheduler::put(pom);
	}
}
//dodaj u timer za azuriranje
