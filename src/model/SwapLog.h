#pragma once

#include "Post.h"

/*! \class SwapLog
 * 	This class is used to register changes in the planning (mostly swap between two agents) in order to optimize the checker
 */

class SwapLog
{
public:
	SwapLog(int agent1, int agent2, int day, int service1, int service2, Post* post1, Post* post2);
	virtual ~SwapLog();

	int getAgent1();
	int getAgent2();
	int getDay();
	int getService1();
	int getService2();
	Post* getPost1();
	Post* getPost2();

	void reverse();

private:
	int agent1;
	int agent2;
	int day;
	int service1;
	int service2;
	Post* post1;
	Post* post2;
};

