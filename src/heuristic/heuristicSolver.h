/*
 * heuristicSolver.h
 *
 *  Created on: 9 mars 2020
 *      Author: Valars
 */

#ifndef SRC_HEURISTIC_HEURISTICSOLVER_H_
#define SRC_HEURISTIC_HEURISTICSOLVER_H_

#include "../model/Model.h"
#include <algorithm>
#include <chrono>
#include <random>
#include <typeinfo>

class heuristicSolver {
public:
	heuristicSolver();
	virtual ~heuristicSolver();

	static Model greedy(const Model m);
	static void nullTo(Model* m, Post* post);
	static bool check(Model* m);
};

#endif /* SRC_HEURISTIC_HEURISTICSOLVER_H_ */
