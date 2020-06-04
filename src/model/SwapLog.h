#pragma once
class SwapLog
{
public:
	SwapLog();
	virtual ~SwapLog();
	SwapLog(int agent1, int agent2, int day, int service);

	int getAgent1();
	int getAgent2();
	int getDay();
	int getService();

private:
	int agent1;
	int agent2;
	int day;
	int service;
};

