#define BOOST_TEST_DYN_LINK

#define BOOST_TEST_MODULE pageproc test 
#include <boost/test/unit_test.hpp>
int add(int x, int y){return x+y;} 

BOOST_AUTO_TEST_CASE( add_test )
{
	BOOST_CHECK_EQUAL(add(1, 1), 2); 
	BOOST_CHECK_EQUAL(add(1, 0), 1);
	BOOST_CHECK_EQUAL(add(0, 0), 0);
	BOOST_CHECK_EQUAL(add(1, -1), 0);
	BOOST_CHECK_EQUAL(add(0, -100), -100);
}

