#ifndef SYSTEM_H
#define SYSTEM_H

#include"Entity_manager.h"

struct System
{
	Entity_manager* em;

	System(Entity_manager* e) : em(e) { }

	virtual void process(long previousFrameTime=0) =0;
};



#endif
