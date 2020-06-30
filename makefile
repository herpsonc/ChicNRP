all : main

Constraint.o : src/model/constraint/Constraint.cpp src/model/constraint/Constraint.h
	g++ -o Constraint.o -c src/model/constraint/Constraint.cpp -Wall

Valuation.o : src/model/Valuation/cpp src/model/Valuation.h
	g++ -o Valuation.o -c src/model/Valuation.cpp -Wall

SwapLog.o : src/model/SwapLog.cpp src/model/SwapLog.h
	g++ -o SwapLog.o -c src/model/SwapLog.cpp -Wall

Post.o : src/model/Post.cpp src/model/Post.h Constraint.o
	g++ Constraint.o -o Post.o -c src/model/Post.cpp -Wall

Agent.o : src/model/Agent.cpp src/model/Agent.h Post.o Valuation.o
	g++ Post.o Valuation.o -o Agent.o -c src/model/Agent.cpp -Wall

Service.o : src/model/Service.cpp src/model/Service.h Agent.o
	g++ Agent.o -o Service.o -c src/model/Service.cpp -Wall

ConstraintDaysSeq.o : src/model/constraint/ConstraintDaysSeq.cpp src/model/constraint/ConstraintDaysSeq.h Agent.o
	g++ Agent.o -o ConstraintDaysSeq.o -c src/model/constraint/ConstraintDaysSeq.cpp -Wall

ConstraintInvolved.o : src/model/constraint/ConstraintInvolved.cpp src/model/constraint/ConstraintInvolved.h Agent.o
	g++ Agent.o -o ConstraintInvolved.o -c src/model/constraint/ConstraintInvolved.cpp -Wall

ConstraintSeqMinMax.o : src/model/constraint/ConstraintSeqMinMax.cpp src/model/constraint/ConstraintSeqMinMax.h Model.o Constraint.o
	g++ Model.o Constraint.o -o ConstraintSeqMinMax.o -c src/model/constraint/ConstraintSeqMinMax.cpp -Wall

Model.o : src/model/Model.cpp src/model/Model.h Service.o SwapLog.o ConstraintInvolved.o ConstraintSeqMinMax.o ConstraintDaysSeq.o 
	g++ Service.o SwapLog.o ConstraintInvolved.o ConstraintSeqMinMax.o ConstraintDaysSeq.o -o Model.o -c src/model/Model.cpp -Wall

heuristicSolver.o : src/heuristic/heuristicSolver.cpp src/heuristic/heuristicSolver.h Model.o
	g++ Model.o -o heuristicSolver.o -c src/heuristic/heuristicSolver.cpp -Wall

VariableData.o : src/LP/VariableData.cpp src/LP/VariableData.h Agent.o
	g++ Agent.o -o VariableData.o -c src/LP/VariableData.cpp

LPSolver.o : src/LP/LPSolver.cpp src/LP/LPSolver.h VariableData.o Model.o
	g++ VariableData.o Model.o -o LPSolver.o -c src/LP/LPSolver.cpp

main.o : src/main.cpp heuristicSolver.o LPSolver.o
	g++ heuristicSolver.o LPSolver.o -o main.o -c src/main.cpp -Wall

main : main.o
	g++ main.o -o main

clean :
	rm -f *.o main