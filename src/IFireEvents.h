#pragma once

class IFireEvents
{
public:
	virtual void FireGroupEvent(const int dispid) = 0;
};