/*
 * event.cpp
 *
 *  Created on: Aug 17, 2019
 *      Author: OS1
 */

#include "event.h"
#include "def.h"

Event::Event(IVTNo ivtNo) {
	lock;
	myImpl=new KernelEv(this,ivtNo);
	unlock;
}
Event::~Event(){
	lock;
	delete myImpl;
	unlock;
}
void Event::wait(){
	myImpl->wait();
}

void Event::signal(){
	myImpl->signal();
}

