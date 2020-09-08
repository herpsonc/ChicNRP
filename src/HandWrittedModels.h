#pragma once

#include "model\Model.h"

class HandWrittedModels
{
public:
	static void addConsecutiveSamePost(Agent* a, Post* p, int d_start, int d_end);

	//MARCH 2020
	static Model generateGhrMarch20();
	static Model addServiceSDCMarch20(Model m);
	static Model addServiceUKMarch20(Model m);
	static Model addServiceSDNMarch20(Model m);
	static Model addServiceCSMarch20(Model m);
	static Model addServiceExploMarch20(Model m);
	static Model addServicePoolMarch20(Model m);
	static Model generateMarch20();

	//APRIL 2020
	static Model generateGhrApril20();
	static Model addServiceSDCApril20(Model m);
	static Model addServiceUKApril20(Model m);
	static Model addServiceSDNApril20(Model m);
	static Model addServiceCSApril20(Model m);
	static Model addServiceExploApril20(Model m);
	static Model addServicePoolApril20(Model m);
	static Model generateApril20();
};

