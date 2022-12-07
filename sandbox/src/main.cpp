#include <first_app.hpp>
#include <core/logger.hpp>
#include <core/asserts.hpp>

//std
#include <cstdlib>
#include <iostream>
#include <stdexcept>

int main()
{
	LVE_FATAL("A test message: %f", 3.14f);
	LVE_ERROR("A test message: %f", 3.14f);
	LVE_WARN("A test message: %f", 3.14f);
	LVE_INFO("A test message: %f", 3.14f);
	LVE_DEBUG("A test message: %f", 3.14f);
	LVE_TRACE("A test message: %f", 3.14f);

	LVE_ASSERT(1 == 0);

	lve::FirstApp app{};

	try {
		app.run();
	}
	catch (const std::exception& e) {
		std::cerr << e.what() << '\n';
		return EXIT_FAILURE;
	}

	return EXIT_SUCCESS;
}