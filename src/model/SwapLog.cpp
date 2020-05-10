#include "SwapLog.h"

SwapLog::SwapLog() 
{
	this->agent1 = -1;
	this->agent2 = -1;
	this->day = -1;
}

SwapLog::~SwapLog() {

}
SwapLog::SwapLog(int agent1, int agent2, int day, int service)
{
	this->agent1 = agent1;
	this->agent2 = agent2;
	this->day = day;
	this->service = service;
}

int SwapLog::getAgent1()
{
	return agent1;
}

int SwapLog::getAgent2()
{
	return agent2;
}

int SwapLog::getDay()
{
	return day;
}

int SwapLog::getService()
{
	return service;
}
