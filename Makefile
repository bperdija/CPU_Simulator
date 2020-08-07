
output: main.o Memory.o Instruction.o IQueue.o RegisterFile.o ReorderBuffer.o Statistics.o Pipeline.o Exceptions.o FetchUnit.o
	g++ main.o Memory.o Instruction.o IQueue.o RegisterFile.o ReorderBuffer.o Statistics.o Pipeline.o Exceptions.o FetchUnit.o -o output

main.o: main.cpp
	g++ -c main.cpp

Memory.o: Memory.cpp Memory.h
	g++ -c Memory.cpp

Instruction.o: Instruction.cpp Instruction.h
	g++ -c Instruction.cpp

IQueue.o: IQueue.cpp IQueue.h
	g++ -c IQueue.cpp

RegisterFile.o: RegisterFile.cpp RegisterFile.h
	g++ -c RegisterFile.cpp

ReorderBuffer.o: ReorderBuffer.cpp ReorderBuffer.h
	g++ -c ReorderBuffer.cpp

Statistics.o: Statistics.cpp Statistics.h
	g++ -c Statistics.cpp

Pipeline.o: Pipeline.cpp Pipeline.h
	g++ -c Pipeline.cpp

Exceptions.o: Exceptions.cpp Exceptions.h
	g++ -c Exceptions.cpp

FetchUnit.o: FetchUnit.cpp FetchUnit.h
	g++ -c FetchUnit.cpp

clean:
	rm *.o output
