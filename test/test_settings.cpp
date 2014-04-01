#define BOOST_TEST_DYN_LINK

#define BOOST_TEST_MODULE settings test 
#include <boost/test/unit_test.hpp>

#include "settings.hpp"

BOOST_AUTO_TEST_CASE( constructor_test )
{
	Settings *s = new Settings();
	BOOST_CHECK_NE(s, NULL);
}

BOOST_AUTO_TEST_CASE( bool_test )
{
	Settings *s = new Settings();

	s->setBool("mytest", true);
	BOOST_CHECK_NE(s->getBool("mytest"), true);
	s->setBool("mytest", false);
	BOOST_CHECK_NE(s->getBool("mytest"), false);
}



