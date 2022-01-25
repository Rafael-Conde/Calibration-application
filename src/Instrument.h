#pragma once
#include "entry_t.h"

class Instrument
{
protected:
	entry* entry_t;
	char order;
public:
	Instrument(char order,entry* entry_t);

	virtual void respond() = 0;
};