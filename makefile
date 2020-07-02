SCIPDIR = ~/bin/scip

all : main

Constraint.o : src/model/constraint/Constraint.cpp src/model/constraint/Constraint.h
	g++ -c src/model/constraint/Constraint.cpp -o Constraint.o -W -Wall -Werror

Valuation.o : src/model/Valuation.cpp src/model/Valuation.h
	g++ -c src/model/Valuation.cpp -o Valuation.o -W -Wall -Werror

SwapLog.o : src/model/SwapLog.cpp src/model/SwapLog.h
	g++ -c src/model/SwapLog.cpp -o SwapLog.o -W -Wall -Werror

Post.o : src/model/Post.cpp src/model/Post.h
	g++ -c src/model/Post.cpp -o Post.o -W -Wall -Werror

Agent.o : src/model/Agent.cpp src/model/Agent.h
	g++ -c src/model/Agent.cpp -o Agent.o -W -Wall -Werror

Service.o : src/model/Service.cpp src/model/Service.h
	g++ -c src/model/Service.cpp -o Service.o -W -Wall -Werror

ConstraintDaysSeq.o : src/model/constraint/ConstraintDaysSeq.cpp src/model/constraint/ConstraintDaysSeq.h
	g++ -c src/model/constraint/ConstraintDaysSeq.cpp -o ConstraintDaysSeq.o -W -Wall -Werror

ConstraintInvolved.o : src/model/constraint/ConstraintInvolved.cpp src/model/constraint/ConstraintInvolved.h
	g++ -c src/model/constraint/ConstraintInvolved.cpp -o ConstraintInvolved.o -W -Wall -Werror

ConstraintSeqMinMax.o : src/model/constraint/ConstraintSeqMinMax.cpp src/model/constraint/ConstraintSeqMinMax.h
	g++ -c src/model/constraint/ConstraintSeqMinMax.cpp -o ConstraintSeqMinMax.o -W -Wall -Werror

Model.o : src/model/Model.cpp src/model/Model.h 
	g++ -c src/model/Model.cpp -o Model.o -W -Wall -Werror

HeuristicToolBox.o : src/heuristic/HeuristicToolBox.cpp src/heuristic/HeuristicToolBox.h
	g++ -c src/heuristic/HeuristicToolBox.cpp -o HeuristicToolBox.o -W -Wall -Werror

heuristicSolver.o : src/heuristic/heuristicSolver.cpp src/heuristic/heuristicSolver.h
	g++ -c src/heuristic/heuristicSolver.cpp -o heuristicSolver.o -W -Wall -Werror

VariableData.o : src/LP/VariableData.cpp src/LP/VariableData.h
	g++ -c src/LP/VariableData.cpp -o VariableData.o -W -Wall -Werror

LPSolver.o : src/LP/LPSolver.cpp src/LP/LPSolver.h
	g++ -I/$(SCIPDIR) -L/$(SCIPDIR) -c src/LP/LPSolver.cpp -o LPSolver.o -lm -lscip -W -Wall -Werror

main : heuristicSolver.o LPSolver.o Constraint.o Valuation.o SwapLog.o Post.o Agent.o Service.o ConstraintInvolved.o ConstraintDaysSeq.o ConstraintSeqMinMax.o Model.o HeuristicToolBox.o VariableData.o
	g++ -I/$(SCIPDIR) -L/$(SCIPDIR) heuristicSolver.o LPSolver.o Constraint.o Valuation.o SwapLog.o Post.o Agent.o Service.o ConstraintInvolved.o ConstraintDaysSeq.o ConstraintSeqMinMax.o Model.o HeuristicToolBox.o VariableData.o src/main.cpp -o main -lm -lscip -W -Wall -Werror

clean :
	rm -f *.o main