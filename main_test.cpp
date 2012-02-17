#include <omni/omni.hpp>

#include <test/test.hpp>
#include <test/calc.hpp>
#include <test/conf.hpp>
#include <test/util.hpp>

#include <iostream>

// locals
namespace
{

///////////////////////////////////////////////////////////////////////////////
/// @brief Print the compiler information to standard output stream.
void print_compiler_info()
{
#if defined(__INTEL_COMPILER)
	std::cout << "Intel compiler: " << int(__INTEL_COMPILER) << "\n";
#endif
#if defined(__BORLANC__)
	std::cout << "Borland compiler: " << int(__BORLANDC__) << "\n";
#endif
#if defined(__GNUC__)
	std::cout << "GCC: " << int(__GNUC__) << "\n";
#endif
#if defined(__EDG__) && defined(__COMO__) // Comeau C++
	std::cout << "Comeau C++: " << int(__COMO__) << "\n";
#endif
#if defined(_MSC_VER)
	std::cout << "MS VS compiler: " << int(_MSC_VER) << "\n";
#endif
}

} // locals


///////////////////////////////////////////////////////////////////////////////
/// @brief The application entry point.
/**
@param[in] argc The number of command line arguments.
@param[in] argv The command line arguments.
*/
int main(int argc, char const* argv[])
{
	print_compiler_info();
	std::cout << "\n";

	try
	{
		if (0) // explicit test
		{
			//if (!test_conf(std::cout))
			//	std::cout << "test FAILED\n";
			//else
			//	std::cout << "test SUCCESS\n";

			return 0;
		}

		//omni::rnd::randomize();
		omni::test::UnitTest::testAll(std::cout);

#if !OMNI_DEBUG
		//omni::test::SpeedTest::testAll(std::cout);
#endif

	}
	catch (std::exception const& ex)
	{
		std::cerr << "\nERROR: "
			<< ex.what() << "\n";
	}
	catch (...)
	{
		std::cerr << "\nFATAL ERROR!\n";
	}

	return 0;
}
