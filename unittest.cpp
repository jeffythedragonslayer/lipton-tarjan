#define BOOST_TEST_MODULE MyTest
#include <boost/test/included/unit_test.hpp>

BOOST_AUTO_TEST_CASE( my_test )
{
	BOOST_CHECK( 1 == 1 );
	BOOST_CHECK( 4 == 4 );
}
