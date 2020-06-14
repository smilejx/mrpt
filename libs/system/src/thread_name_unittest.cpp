/* +------------------------------------------------------------------------+
   |                     Mobile Robot Programming Toolkit (MRPT)            |
   |                          https://www.mrpt.org/                         |
   |                                                                        |
   | Copyright (c) 2005-2020, Individual contributors, see AUTHORS file     |
   | See: https://www.mrpt.org/Authors - All rights reserved.               |
   | Released under BSD License. See: https://www.mrpt.org/License          |
   +------------------------------------------------------------------------+ */

#include <gtest/gtest.h>
#include <mrpt/system/thread_name.h>

TEST(thread_name, set_get_current_thread)
{
	const std::string testValue = "CoolName";
	const std::string testValue2 = "HotName";
	// Set:
	mrpt::system::thread_name(testValue);
	// Get:
	const std::string ret = mrpt::system::thread_name();
	EXPECT_EQ(ret, testValue);

	// Set:
	mrpt::system::thread_name(testValue2);
	// Get:
	const std::string ret2 = mrpt::system::thread_name();
	EXPECT_EQ(ret2, testValue2);
}
