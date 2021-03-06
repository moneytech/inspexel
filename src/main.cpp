#include <sargparse/ArgumentParsing.h>
#include <sargparse/Parameter.h>
#include <iostream>

namespace {

#define VERSION "1.3.0"
#define DATE    "16 October 2018"

#define TERM_RED                        "\033[31m"
#define TERM_RESET                      "\033[0m"

auto printHelp  = sargp::Parameter<std::optional<std::string>>{{}, "help", "print this help add a string which will be used in a grep-like search through the parameters"};

auto cmdVersion = sargp::Command("version", "prints the version", []() {
	std::cout << "inspexel version " << VERSION << " - " << DATE << "\n";
});

}

int main(int argc, char** argv)
{
	if (std::string(argv[argc-1]) == "--bash_completion") {
		auto hints = sargp::getNextArgHint(argc-2, argv+1);
		for (auto const& hint : hints) {
			std::cout << hint << " ";
		}
		return 0;
	}
	if (argc == 2 and std::string(argv[1]) == "--groff") {
		std::cout << ".TH man 1 \"" << DATE << "\" \"" << VERSION << "\" inspexel man page\"\n";
		std::cout << sargp::generateGroffString();
		return 0;
	}

	try {
		// pass everything except the name of the application
		sargp::parseArguments(argc-1, argv+1);

		if (printHelp) {
			std::cout << "inspexel version " << VERSION << " - " << DATE << "\n";
			std::cout << sargp::generateHelpString(std::regex{".*" + printHelp->value_or("") + ".*"});
			return 0;
		}
		sargp::callCommands();
	} catch (std::exception const& e) {
		std::cerr << "exception: " << TERM_RED << e.what() << TERM_RESET "\n";
	}
	return 0;
}
