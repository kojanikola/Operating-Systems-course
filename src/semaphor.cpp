/*
 * semaphor.cpp
 *
 *  Created on: Aug 14, 2019
 *      Author: OS1
 */

#include "semaphor.h"
#include "def.h"
#include "kernSem.h"

Semaphore::Semaphore(int init) {
	lock;
	myImpl=new KernelSem(init);
	unlock;
}
Semaphore::~Semaphore(){
	lock;
	delete myImpl;
	unlock;
}

int Semaphore::wait(Time maxTimeToWait){
	int pom;
	lock;
	pom=myImpl->wait(maxTimeToWait);
	unlock;
	return pom;
}

int Semaphore::signal(int n){
	int pom;
	lock;
	pom=myImpl->signal(n);
	unlock;
	return pom;
}

int Semaphore::val()const{
	return myImpl->val;
}
