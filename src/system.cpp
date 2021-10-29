/*
 * system.cpp
 *
 *  Created on: Aug 5, 2019
 *      Author: OS1
 */

#include "system.h"
#include <dos.h>


#define TIMER 0x08
#define MASK 0x200

unsigned oldTimerOFF,oldTimerSEG,tSEG,tOFF;

volatile char System::zahtevana_promena=0;
volatile PCB* System::running=NULL;
Thread* System::starting=NULL;
volatile Queue* System::redSvih=new Queue();
Idle* System::idle=NULL;
volatile Lista* System::listaSvihSem=new Lista();

class Idle: public Thread {
public:
	Idle():Thread(4096,20){};
	void run(){
		while(1){
			//cout<<PCB::brAktivnih;
		}
	}
	void start(){
		lock;
			myPCB->status=READY;
			myPCB->createStack();
			unlock;
	}
	PCB* getPCB(){
		return myPCB;
	}
};

void System::inic(){
	lock;
	tSEG=FP_SEG(timer);
	tOFF=FP_OFF(timer);
	asm{
		push es
		push ax
		mov ax,0
		mov es,ax
		mov ax,word ptr es:0022h
		mov word ptr oldTimerSEG,ax
		mov ax,word ptr es:0020h
		mov word ptr oldTimerOFF,ax
		mov ax,tSEG
		mov word ptr es:0022h, ax
		mov ax,tOFF
		mov word ptr es:0020h, ax
		mov ax,oldTimerSEG
		mov word ptr es:0182h,ax
		mov ax,oldTimerOFF
		mov word ptr es:0180h,ax
		pop ax
		pop es
	}
	starting=new Thread(4096,minTimeSlice());
	starting->myPCB->status=RUNNING;
	running=(PCB*) starting->myPCB;
	idle=new Idle();
	idle->start();
	unlock;
}

void System::term(){
	lock;
	//setvect(TIMER,oldRoutine);
	asm{
		push es
		push ax
		mov ax,0
		mov es,ax
		mov ax,word ptr oldTimerSEG
		mov word ptr es:0022h,ax
		mov ax, word ptr oldTimerOFF
		mov word ptr es:0020h,ax
		pop ax
		pop es
	}
	delete idle;
	delete starting;
	unlock;
}

void interrupt System::timer(...){
	static volatile unsigned int tsp,tss,tbp;
	static volatile PCB* newThread;

	if(!zahtevana_promena){       // kad hocu da promenim kontekst stavim zahtevana promena na 1 to znaci da postoji zahtev za promenu
									//ako je 0 onda ne postoji zahtev nego je timer otkucao koliko treba i treba da pozovem onu staru;
		tick();
		asm int 60h
	}
#ifndef BCC_BLOCK_IGNORE
	asm{
		cli
	};
#endif
	if(!zahtevana_promena){
		if(running->pcbTimeSlice!=0) {
			running->period-=1;
		}

		Element* tek=listaSvihSem->prvi;
				while(tek){
					KernelSem* pom=tek->sem;
					pom->semQueue->prvi;
					Elem* _tek=pom->semQueue->prvi,*_pret=NULL;
					while(_tek){

						if(_tek->pcb->uspavanaVremenski==1){
							_tek->pcb->timeToWait--;
							if(_tek->pcb->timeToWait==0){
								PCB* odBlokiraj= _tek->pcb;
								//Elem* brisi=tek;							ne brisem taj koji iyvadim iz reda kako da ga obrisem?
								if(_pret!=NULL){
									_pret->next=_tek->next;
								}
								if(_pret==NULL){
									pom->semQueue->prvi=pom->semQueue->prvi->next;
								}
								odBlokiraj->timerFlag=1;
								odBlokiraj->status=READY;
								if(odBlokiraj){
									//cout<<"ubacio iz liste\n";
									Scheduler::put(odBlokiraj);
								}

							}
						}
						_pret=_tek;
						_tek=_tek->next;
					}

					tek=tek->next;
				}

		if(running->period==0 || running->pcbTimeSlice==0){
			return;
		}

	}

	zahtevana_promena=0;

	if(running->status==READY && running!=idle->getPCB() ){
		running->status=READY;
		Scheduler::put((PCB*) running);
	}
	while(1){
		newThread= Scheduler::get();
		if(newThread==NULL) {

			newThread=idle->getPCB();
		}
		if(newThread->status!=READY) {

			continue;
		}

		asm{
			mov tsp,sp
			mov tss, ss
			mov tbp,bp
		}
		running->sp=tsp;
		running->ss=tss;
		running->bp=tbp;

		running=newThread;

		tsp= running->sp;
		tss= running->ss;
		tbp=running->bp;

		asm{
			mov sp,tsp
			mov ss,tss
			mov bp,tbp
		}

		running->period=running->pcbTimeSlice;       //  kod onih je stavljeno da je period nula ali valjda ne treba da bude 0
		break;
	}
	return;
}
