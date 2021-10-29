/*
 * lista.h
 *
 *  Created on: Aug 14, 2019
 *      Author: OS1
 */

#ifndef LISTA_H_
#define LISTA_H_

#include <iostream.h>
#include <stdlib.h>

class KernelSem;

struct Element{
	KernelSem* sem;
	Element* next;
	Element(KernelSem* _sem, Element* _next=NULL){
		sem=_sem;
		next=_next;
	}
};
class Lista {
private:
	Element *prvi,*poslednji;
public:
	Lista();
	~Lista();
	void put(KernelSem* sem);
	KernelSem* get();
	void izbaci(KernelSem* sem);
protected:
	friend class System;
};

#endif /* LISTA_H_ */
