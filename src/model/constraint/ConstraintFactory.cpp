/*
 * ConstraintFactory.cpp
 *
 *  Created on: 10 févr. 2020
 *      Author: Valars
 */

#include "ConstraintFactory.h"

using namespace std;

ConstraintFactory::ConstraintFactory() {
	// TODO Auto-generated constructor stub

}

ConstraintFactory::~ConstraintFactory() {
	// TODO Auto-generated destructor stub
}

ConstraintDaysSeq* ConstraintFactory::createConstraintDaysSeq(vector<Post>& seq, int priority)
{
	return new ConstraintDaysSeq(seq, priority);
}
