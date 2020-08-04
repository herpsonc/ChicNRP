#pragma once

#include "Post.h"
class SwapLog
{
public:
	SwapLog();
	virtual ~SwapLog();
	SwapLog(int agent1, int agent2, int day, int service, Post* post1, Post* post2);

	int getAgent1();
	int getAgent2();
	int getDay();
	int getService();
	Post* getPost1();
	Post* getPost2();

	void reverse();

private:
	int agent1;
	int agent2;
	int day;
	int service;
	Post* post1;
	Post* post2;
};

