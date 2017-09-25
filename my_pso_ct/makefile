#specify compiler
CC = g++ -std=c++11

#Target
Target = my_pso_ct

#Sources & Objects
Common = ${wildcard common/*.cpp}
Conventional = ${wildcard conventional/*.cpp}
Utils = ${wildcard utils/*.cpp}

Objects = ${Common: .cpp = .o} ${Conventional: .cpp = .o} ${Utils: .cpp = .o}

#Target
.PHONY : all
all : $(Target)

$(Target) : $(Objects) main.o
	$(CC) -O2 $(Objects) main.o -o ${Target}

main.o : main.cpp
	$(CC) -O2 -c main.cpp -o main.o
	
#Clean target
.PHONY : clean
clean :
	rm main.o $(Target)