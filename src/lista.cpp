/*
 * lista.cpp
 *
 *  Created on: Aug 14, 2019
 *      Author: OS1
 */

#include "lista.h"
#include "def.h"

Lista::Lista() {
	lock;
	prvi=poslednji=NULL;
	unlock;
}
Lista::~Lista(){
	lock;
	while(prvi){
		Element *pom=prvi;
		prvi=prvi->next;
		delete pom;
	}
	prvi=poslednji=NULL;
	unlock;
}
void Lista::put(KernelSem* sem){
	lock;
	Element* pom=new Element(sem);
	if(prvi==NULL) prvi=pom;
	else poslednji->next=pom;
	poslednji=pom;
	unlock
}

KernelSem* Lista::get(){
	if(prvi==NULL) return 0;
	lock;
	Element* pom=prvi;
	KernelSem* ret=pom->sem;
	prvi=prvi->next;
	unlock;
	delete pom;
	return ret;
}

void Lista::izbaci(KernelSem* _sem){
	lock;
	Element* tek=prvi,* pret=NULL;
	while(tek && tek->sem!=_sem){
		pret=tek;
		tek=tek->next;
	}
	if(tek!=NULL){
		if(pret!=NULL)
			pret->next=tek->next;
		if(pret==NULL)
			prvi=tek->next;
		if(tek==poslednji)
			poslednji=pret;
	}
	unlock;
}
