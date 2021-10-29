/*
 * kernelev.cpp
 *
 *  Created on: Aug 17, 2019
 *      Author: OS1
 */

#include "kernelev.h"
#include "def.h"
#include <dos.h>
#include "pcb.h"
#include "queue.h"
IVTEntry* ulazi[256];
extern volatile int zahtevana_promena;

KernelEv::KernelEv(Event* _myEvent, IVTNo ivtNo) {
	blocked = new Queue();
	vred = 0;
	myEntry = ulazi[ivtNo];
	myEntry->myEvent=this;
	myEvent = _myEvent;
	brUlaza = ivtNo;
	myPCB = ((PCB*)System::running);
}
KernelEv::~KernelEv() {
	delete blocked;
}
void KernelEv::wait() {
	lock;
	if (System::running == myPCB) {
		if (--vred < 0) {
			PCB* p=myPCB;
			System::running->status = BLOCKED;
			PCB::brAktivnih--;
			blocked->put(p);
		}
	}unlock;
	System::dispatch();
}
void KernelEv::signal() {
	lock;
	if (vred++ < 0) {
		PCB* pom = blocked->get();
		pom->status = READY;
		if (pom != NULL){
			Scheduler::put(pom);
		}
		myPCB = NULL;
	} unlock;
	System::dispatch();
}

IVTEntry::IVTEntry(IVTNo ivtNo, void interrupt (*prekidna)(...)) {
	lock;
	if (ivtNo >= 0 && ivtNo < 265) {
		brojUlaza = ivtNo;
		staraRutina = getvect(ivtNo);
		novaRutina = prekidna;
		ulazi[ivtNo] = this;
		inic();
	}unlock;
}
IVTEntry::~IVTEntry() {
	lock;
	restore();
	ulazi[brojUlaza] = 0;
	staraRutina();
	unlock;
}
void IVTEntry::inic() {
	lock;
	setvect(brojUlaza, novaRutina);
	unlock;
}

void IVTEntry::restore() {
	lock;
	setvect(brojUlaza, staraRutina);
	unlock;
}
void IVTEntry::signal(){
	myEvent->signal();
}
