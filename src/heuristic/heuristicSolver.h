/*
 * heuristicSolver.h
 *
 *  Created on: 9 mars 2020
 *      Author: Valars
 */

#ifndef SRC_HEURISTIC_HEURISTICSOLVER_H_
#define SRC_HEURISTIC_HEURISTICSOLVER_H_

#include "HeuristicToolBox.h"
#include <algorithm>
#include <chrono>
#include <random>
#include <typeinfo>
#include "../model/Valuation.h"

class heuristicSolver {
public:
	heuristicSolver();
	virtual ~heuristicSolver();

	static Model greedy(const Model m);
	static void nullTo(Model* m, Post* post);
	static Model predefinedGreedy(const Model m);
	static void checkFast(Model* m);
	static void checkFastMultiThread(Model* m); 
	static Model getNeighborSwap(Model* m, int range);
	static void neighborSwap(Model* m, int range);
	static void neighborSwapBlock(Model* m, int range);
	static Model getneighborRandom(Model* m, int range);
	static Model iterativeFast(const Model m, int nbIte, int range);
};

#endif /* SRC_HEURISTIC_HEURISTICSOLVER_H_ */
