
#define BOOST_TEST_DYN_LINK

#define BOOST_TEST_MODULE pageproc test 
#include <boost/test/unit_test.hpp>

#include <matpart.hpp>

BOOST_AUTO_TEST_CASE( constructor_test )
{
	MatPart part(); 
}

BOOST_AUTO_TEST_CASE(getters_test) 
{
	// TODO Test image and other things
}
