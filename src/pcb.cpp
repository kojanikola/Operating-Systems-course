/*
 * pcb.cpp
 *
 *  Created on: Aug 5, 2019
 *      Author: OS1
 */

#include "pcb.h"

ID PCB::ukID=0;
volatile int PCB::brAktivnih=0;

PCB::PCB(Thread* nit, StackSize stackSize, Time timeSlice) {
	lock;
	ID=++ukID;
	myThread=nit;
	mySP=NULL;
	status=NEW;
	period=0;
	pcbTimeSlice=timeSlice;
	pcbStackSize=stackSize;
	waitQueue=new Queue();
	timerFlag=0;
	uspavanaVremenski=0;
	unlock;
}
PCB::PCB(Thread* thread):mySP(0),pcbTimeSlice(1),uspavanaVremenski(0){
	lock;
	myThread=thread;
	ID=++ukID;
	status=READY;
	period=0.;
	System::redSvih->put(this);
	brAktivnih++;
	unlock;
}

PCB::~PCB(){
	lock;
	delete waitQueue;
	delete mySP;
	unlock;
}
void PCB::run(){
	myThread->run();
	lock;
	System::running->status=FINISHED;
	brAktivnih--;
	while(waitQueue->getSize()>0){

		PCB* pom= waitQueue->get();
		pom->status=READY;
		Scheduler::put(pom);
	}

	System::dispatch();
}


void PCB::createStack(){
	lock;

	System::redSvih->put(this);						// kako da ga ubacim kad ga kreiram u ono sto vodi racuna o svima
	static volatile unsigned tsp, tss,tbp, tip, tcs,oldss,oldsp;
	static unsigned char* tempStack;
	tempStack=new unsigned char[pcbStackSize];
	this->mySP=tempStack;
	tsp=this->sp=FP_OFF(tempStack+pcbStackSize);
	tss=this->ss=FP_SEG(tempStack+pcbStackSize);
	tip=FP_OFF(&(wrapper));
	tcs=FP_SEG(&(wrapper));

	asm{
		mov oldsp,sp
		mov oldss,ss
		mov sp,tsp
		mov ss,tss
		pushf
		pop ax
		or ax,0000001000000000b
		push ax
		mov ax,tcs
		push ax
		mov ax,tip
		push ax
		mov ax,0
		push ax
		push bx
		push cs
		push dx
		push es
		push ds
		push si
		push di
		push bp
		mov tsp,sp
		mov sp,oldsp
		mov ss, oldss
	}
	//gde da ubacim bp=sp?
	this->sp=tsp;
	this->bp=this->sp;
	unlock;
}

void PCB::wrapper(){
	System::running->run();         // kako funckije da pozivam za running
}




