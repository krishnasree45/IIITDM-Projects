all:final_output

final_output:elegance.o module0.o module1.o  module2.o  module3.o  module_4.o queue.o stack.o
	g++ elegance.o module0.o module1.o module2.o module3.o module_4.o queue.o stack.o -o final_output
queue.o:queue.cpp
	g++ -c queue.cpp

stack.o:stack.cpp
	g++ -c stack.cpp

module.o:elegance.cpp
	g++ -c module0.cpp

module0.o:module0.cpp
	g++ -c module0.cpp

module1.o:module1.cpp
	g++ -c module1.cpp

module2.o:module2.cpp
	g++ -c module2.cpp

module3.o:module3.cpp
	g++ -c module3.cpp

module4.o:module_4.cpp
	g++ -c module_4.cpp

