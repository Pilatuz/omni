#include <test/test.hpp>
#include <omni/rand.hpp>

#include <iostream>

// test functions
bool test_SIMD(std::ostream &os);
bool test_calc(std::ostream &os);
bool test_conf(std::ostream &os);
bool test_pool(std::ostream &os);
bool test_smart(std::ostream &os);
bool test_util(std::ostream &os);
bool test_matrix(std::ostream &os);


//////////////////////////////////////////////////////////////////////////
/// @brief The application entry point.
int main()
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
	std::cout << "MS compiler: " << int(_MSC_VER) << "\n";
#endif
	std::cout << "\n";

	try
	{
		if (0) // explicit test
		{
			if (!test_conf(std::cout))
				std::cout << "test FAILED\n";
			else
				std::cout << "test SUCCESS\n";

			return 0;
		}

		omni::rnd::randomize();
		omni::test::UnitTest::testAll(std::cout);

#if !OMNI_DEBUG
		omni::test::SpeedTest::testAll(std::cout);
#endif

	}
	catch (std::exception const& e)
	{
		std::cerr << "ERROR: "
			<< e.what() << "\n";
	}
	catch (...)
	{
		std::cerr << "FATAL ERROR!\n";
	}

	return 0;
}