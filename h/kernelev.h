/*
 * kernelev.h
 *
 *  Created on: Aug 17, 2019
 *      Author: OS1
 */

#ifndef KERNELEV_H_
#define KERNELEV_H_
#include <dos.h>
#include <stdlib.h>
#include "SCHEDULE.H"
#include "pcb.h"
#include "queue.h"


typedef void interrupt (*InterruptHandler)(...);
typedef unsigned char IVTNo;

class KernelEv;
class IVTEntry;
class Event;

extern IVTEntry* ulazi[256];


class IVTEntry {
private:
	IVTNo brojUlaza;

public:

	IVTEntry(IVTNo ivtNo,void interrupt(*prekidna)(...));
	~IVTEntry();
	static IVTEntry * get(IVTNo ivtNo) {
		return ulazi[ivtNo];
	}
	void inic();
	void restore();
	void signal();
	InterruptHandler staraRutina, novaRutina;
	KernelEv* myEvent;
};

class KernelEv {
private:
	Queue* blocked;
	PCB* myPCB;
	IVTEntry* myEntry;
	int vred;
	Event* myEvent;
	IVTNo brUlaza;
public:
	KernelEv(Event* myEvent, IVTNo ivtNo);
	~KernelEv();
	void wait();
	void signal();
	void signalAll();
};

#endif /* KERNELEV_H_ */
