#include "dpch.h"

#include "Interpreter/D++.h""


void signalHandler(int signal)
{
	std::cout << "Exiting D++ Interpreter!\n";
	
	std::exit(signal);
}

//argc is argument count, the no of arguments we pass from the command line and argv is a string array storing all the arguments
int main(int argc, char* argv[])
{

	//registering signal
	std::signal(SIGINT, signalHandler);

	//retreving the arg counts;
	std::vector<std::string> arg_counts{};
	arg_counts.reserve(argc);
	for (int i{ 0 }; i < argc; i++ ) {
		arg_counts.push_back(argv[i]);
	}
	
	//instantiating the iterpreter
	DXX::DXX* interpreter{ new DXX::DXX(argc, arg_counts) };
	interpreter->launch();
	delete interpreter;

	return 0;
}