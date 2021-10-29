/*
 * system.h
 *
 *  Created on: Aug 5, 2019
 *      Author: OS1
 */

#ifndef SYSTEM_H_
#define SYSTEM_H_

#include <iostream.h>
#include <dos.h>
#include "queue.h"
#include "thread.h"
#include "pcb.h"
#include "def.h"
#include "kernSem.h"
//#include "idle.h"
#include "lista.h"
//#include "kernelev.h"

typedef void interrupt (*sInterruptRoutine)();

class Idle;
class Queue;

class System {
private:
	friend class Thread;
	friend class PCB;
	friend class KernelSem;
	friend class KernerEvent;

	static volatile char zahtevana_promena;
public:
	static Thread* starting;
	static Idle* idle;
	static volatile PCB* running;
	static volatile Queue* redSvih; //dodaj u init ovo da napravis red svih i da stavis ukID na nulu
	static volatile Lista* listaSvihSem;
	static void inic();
	static void term();

	static void interrupt timer(...);
	static void dispatch(){
		lock;
		zahtevana_promena=1;
		timer();
		unlock;
	}

};

#endif /* SYSTEM_H_ */
