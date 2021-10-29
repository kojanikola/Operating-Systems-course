/*
 * def.h
 *
 *  Created on: Aug 5, 2019
 *      Author: OS1
 */

#ifndef DEF_H_
#define DEF_H_

#include <iostream.h>
#include <dos.h>

#define NEW 0
#define READY 1
#define RUNNING 2
#define BLOCKED 3
#define FINISHED 4

#define lock
	asm{
		pushf;
		cli;
	}

#define unlock
	asm{
		popf;
	}


#endif /* DEF_H_ */
