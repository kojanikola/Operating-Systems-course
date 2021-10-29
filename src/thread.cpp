/*
 * thread.cpp
 *
 *  Created on: Aug 5, 2019
 *      Author: OS1
 */

#include "thread.h"

Thread::Thread(StackSize stackSize, Time timeSlice) {
	lock;
	myPCB=new PCB(this,stackSize,timeSlice);

	unlock;
}

Thread::~Thread() {
	lock;
	waitToComplete();
	delete myPCB;
	unlock;
}

void Thread::start(){
	lock;
	PCB::brAktivnih++;
	myPCB->setStatus(READY);
	myPCB->createStack();
	Scheduler::put(myPCB);
	unlock;
}

void Thread::waitToComplete(){
	lock;
	if(this==System::running){
		unlock;
		return;
	}
	if(this==System:: starting){
		unlock;
		return;
	}
	if(this==System::idle){
		unlock;
		return;
	}
	if(myPCB->status==FINISHED){
		unlock;
		return;
	}
	System::running->status=BLOCKED;
	myPCB->waitQueue->put((PCB*)System::running);
	PCB::brAktivnih--;
	System::dispatch();
	unlock;
}
ID Thread::getRunningId(){
	return System::running->ID;
}

ID Thread::getId(){
	return myPCB->ID;
}

Thread* Thread::getThreadById(ID i){
	Elem* pom= System::redSvih->prvi;
	while(pom!=NULL){
		if(pom->pcb->ID==i)return pom->pcb->myThread;
		pom=pom->next;

	}
	return NULL;
}

void dispatch(){
	lock;
	System::dispatch();
	unlock;
}

Time minTimeSlice(){
	return 55;
}


