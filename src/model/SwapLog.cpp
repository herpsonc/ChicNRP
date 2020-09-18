#include "SwapLog.h"

SwapLog::SwapLog(int agent1, int agent2, int day, int service1, int service2, Post* post1, Post* post2)
{
	this->agent1 = agent1;
	this->agent2 = agent2;
	this->day = day;
	this->service1 = service1;
	this->service2 = service2;
	this->post1 = post1;
	this->post2 = post2;
}

SwapLog::~SwapLog() {

}

//! \return first agent concerned by the swap
int SwapLog::getAgent1()
{
	return agent1;
}

//! \return second agent concerned by the swap
int SwapLog::getAgent2()
{
	return agent2;
}

//! \return day of the swap
int SwapLog::getDay()
{
	return day;
}

//! \return service where the swap happened
int SwapLog::getService1()
{
	return service1;
}

//! \return service where the swap happened
int SwapLog::getService2()
{
	return service2;
}

Post* SwapLog::getPost1()
{
	return post1;
}

Post* SwapLog::getPost2()
{
	return post2;
}

//! exchange post1 and post2
void SwapLog::reverse()
{
	Post* p = post1;
	post1 = post2;
	post2 = p;
}
