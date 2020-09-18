#ifndef SRC_HEURISTIC_HEURISTICSOLVER_H_
#define SRC_HEURISTIC_HEURISTICSOLVER_H_

#include "HeuristicToolBox.h"
#include <algorithm>
#include <chrono>
#include <random>
#include <typeinfo>
#include "../model/Valuation.h"

/*!	\class heuristicSolver
 * 	This class contain algorithms used to solve the planning problem
 */

class heuristicSolver {
public:
	heuristicSolver();
	virtual ~heuristicSolver();

	static Model greedy(const Model m);
	static void nullTo(Model* m, Post* post);
	static Model removeExtraPosts(const Model* m);
	static Model predefinedGreedy(const Model m);
	static void checkFast(Model* m);
	static void neighborSwap(Model* m, int range);
	static void neighborSwapBlock(Model* m, int range);
	static void neighborSwapPool(Model* m, int range, Service* pool);
	static Model getneighborRandom(Model* m, int range);
	static Model iterativeFast(const Model m, int nbIte, int range, int idPool = -1);
};

#endif /* SRC_HEURISTIC_HEURISTICSOLVER_H_ */
