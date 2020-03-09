#include "REF.h"



Ref::Ref() : REFCOUNT(0), m_bDebug(false)
{
}


Ref::~Ref()
{
}

void Ref::DebugCheck()
{
	if (true == m_bDebug)
	{
		int a = 0;
	}
}

void Ref::HADDREF()
{
	++REFCOUNT;
}

void Ref::HDECREF()
{
	--REFCOUNT;

	if (0 >= REFCOUNT)
	{
		delete this;
	}
}