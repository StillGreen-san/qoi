#include "testdata.hpp"

#define CATCH_CONFIG_ENABLE_BENCHMARKING
#define CATCH_CONFIG_RUNNER
#include <catch2/catch.hpp>

int main(int argc, char* argv[])
{
	try
	{
		TestData::getInstance(); // load testdata

		int result = Catch::Session().run(argc, argv);

		return result;
	}
	catch(const std::exception& exception)
	{
		std::cout << "Unhandled Exception: " << exception.what() << std::endl;
	}
	catch(...)
	{
		std::cout << "Unhandled Exception: UNKNOWN" << std::endl;
	}
}