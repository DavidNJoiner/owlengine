#include <core/application.hpp>
#include <core/logger.hpp>
#include <core/asserts.hpp>

//std
#include <cstdlib>
#include <iostream>
#include <stdexcept>

int main()
{
	lve::application_config config{ 800,800 };
	lve::Application owlEngineEditor{config};

	try {
		owlEngineEditor.run();
	}
	catch (const std::exception& e) {
		std::cerr << e.what() << '\n';
		return EXIT_FAILURE;
	}

	return EXIT_SUCCESS;
}