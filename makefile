all : main

Constraint.o : src/model/constraint/Constraint.cpp src/model/constraint/Constraint.h
	g++ -c src/model/constraint/Constraint.cpp -o exec/Constraint.o -W -Wall -Werror

Valuation.o : src/model/Valuation.cpp src/model/Valuation.h
	g++ -c src/model/Valuation.cpp -o exec/Valuation.o -W -Wall -Werror

SwapLog.o : src/model/SwapLog.cpp src/model/SwapLog.h
	g++ -c src/model/SwapLog.cpp -o exec/SwapLog.o -W -Wall -Werror

Post.o : src/model/Post.cpp src/model/Post.h
	g++ -c src/model/Post.cpp -o exec/Post.o -W -Wall -Werror

Agent.o : src/model/Agent.cpp src/model/Agent.h
	g++ -c src/model/Agent.cpp -o exec/Agent.o -W -Wall -Werror

Service.o : src/model/Service.cpp src/model/Service.h
	g++ -c src/model/Service.cpp -o exec/Service.o -W -Wall -Werror

ConstraintDaysSeq.o : src/model/constraint/ConstraintDaysSeq.cpp src/model/constraint/ConstraintDaysSeq.h
	g++ -c src/model/constraint/ConstraintDaysSeq.cpp -o exec/ConstraintDaysSeq.o -W -Wall -Werror

ConstraintInvolved.o : src/model/constraint/ConstraintInvolved.cpp src/model/constraint/ConstraintInvolved.h
	g++ -c src/model/constraint/ConstraintInvolved.cpp -o exec/ConstraintInvolved.o -W -Wall -Werror

ConstraintSeqMinMax.o : src/model/constraint/ConstraintSeqMinMax.cpp src/model/constraint/ConstraintSeqMinMax.h
	g++ -c src/model/constraint/ConstraintSeqMinMax.cpp -o exec/ConstraintSeqMinMax.o -W -Wall -Werror

Model.o : src/model/Model.cpp src/model/Model.h 
	g++ -c src/model/Model.cpp -o exec/Model.o -W -Wall -Werror

HeuristicToolBox.o : src/heuristic/HeuristicToolBox.cpp src/heuristic/HeuristicToolBox.h
	g++ -c src/heuristic/HeuristicToolBox.cpp -o exec/HeuristicToolBox.o -W -Wall -Werror

heuristicSolver.o : src/heuristic/heuristicSolver.cpp src/heuristic/heuristicSolver.h
	g++ -c src/heuristic/heuristicSolver.cpp -o exec/heuristicSolver.o -W -Wall -Werror

VariableData.o : src/LP/VariableData.cpp src/LP/VariableData.h
	g++ -c src/LP/VariableData.cpp -o exec/VariableData.o -W -Wall -Werror

LPSolver.o : src/LP/LPSolver.cpp src/LP/LPSolver.h
	g++ -c src/LP/LPSolver.cpp -o exec/LPSolver.o -W -Wall -Werror

main : heuristicSolver.o Constraint.o Valuation.o SwapLog.o Post.o Agent.o Service.o ConstraintInvolved.o ConstraintDaysSeq.o ConstraintSeqMinMax.o Model.o HeuristicToolBox.o VariableData.o
	g++ exec/heuristicSolver.o exec/LPSolver.o exec/Constraint.o exec/Valuation.o exec/SwapLog.o exec/Post.o exec/Agent.o exec/Service.o exec/ConstraintInvolved.o exec/ConstraintDaysSeq.o exec/ConstraintSeqMinMax.o exec/Model.o exec/HeuristicToolBox.o exec/VariableData.o src/main.cpp -o main -lm -W -Wall -Werror

clean :
	rm -f exec/*.o main