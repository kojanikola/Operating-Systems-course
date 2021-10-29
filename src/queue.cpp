/*
 * queue.cpp
 *
 *  Created on: Aug 9, 2019
 *      Author: OS1
 */

#include "queue.h"

Queue::Queue() {
	lock;
	prvi = poslednji = NULL;
	size = 0;
	unlock;
}

Queue::~Queue() {
	lock;
	Elem* pom;
	while (pom != NULL) {
		pom = prvi;
		prvi = prvi->next;
		delete pom;
		size--;
	}
	prvi = poslednji = NULL;
	unlock;
}

void Queue::put(PCB* pcb) {
	lock;
	Elem* pom = new Elem(pcb);
	unlock;
	if (prvi == NULL){
		prvi = pom;
	}

	else{
		poslednji->next = pom;
	}
	poslednji = pom;
	size++;
}

PCB* Queue::get() {
	if (prvi == NULL){

		return 0;
	}
	lock;
	Elem* pom = prvi;
	prvi = prvi->next;
	PCB* pcb = pom->pcb;
	delete pom;
	size--;
	unlock;
	return pcb;
}


