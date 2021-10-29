/*
 * event.h
 *
 *  Created on: Aug 17, 2019
 *      Author: OS1
 */

#ifndef EVENT_H_
#define EVENT_H_

#include <dos.h>
#include <iostream.h>
#include "kernelev.h"

typedef unsigned char IVTNo;


#define PREPAREENTRY(numIVT,oldINT)\
	void interrupt PREK_##numIVT(...);\
	IVTEntry ulaz##numIVT(numIVT,PREK_##numIVT);\
	void interrupt PREK_##numIVT(...){\
		if(oldINT)ulazi[numIVT]->staraRutina();\
		ulazi[numIVT]->myEvent->signal();\
	}\


class Event {
public:
	Event(IVTNo ivtNO);
	~Event();
	void wait();
protected:
	friend class KernelEv;
	void signal();
private:
	KernelEv* myImpl;
};

#endif /* EVENT_H_ */
