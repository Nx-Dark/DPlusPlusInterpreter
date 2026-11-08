#include "dpch.h"

#include "Application.h"

#include "Signals.h"

void signalHandler(int signal)
{
	std::cout << "Exiting D++ Interpreter!\n";
	
	std::exit(0);
}

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
	try {
		DXX::Application application{ argc, arg_counts };
		application.launch();
	}
	catch (const DXX::ExitSignal&)
	{
		std::cout << "Exiting D++ Interpreter!\n";

		std::exit(0);
	}

	return 0;
}