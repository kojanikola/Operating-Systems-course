/*
 * mainClass.cpp
 *
 *  Created on: Aug 11, 2019
 *      Author: OS1
 */

#include "thread.h"
#include "def.h"
#include "pcb.h"
#include "kernelev.h"
#include "event.h"s

extern int userMain(int argc,char*argv[]);

int main(int argc, char* argv[]){
	System::inic();
	int i=userMain(argc,argv);
	System::term();
}


