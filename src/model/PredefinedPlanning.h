#pragma once

#include "Post.h"
#include <array>
#include <iostream>

class PredefinedPlanning
{
public:
	PredefinedPlanning();
	void addAgent();
	void setPlanningDay(Post* post, const int idAgent, const int day);
	const std::vector<std::array<Post*, 31>>* getPlanning();
	void printPlanning();
private:
	std::vector<std::array<Post*, 31>> planning;
};

