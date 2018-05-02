/* +------------------------------------------------------------------------+
   |                     Mobile Robot Programming Toolkit (MRPT)            |
   |                          http://www.mrpt.org/                          |
   |                                                                        |
   | Copyright (c) 2005-2018, Individual contributors, see AUTHORS file     |
   | See: http://www.mrpt.org/Authors - All rights reserved.                |
   | Released under BSD License. See details in http://www.mrpt.org/License |
   +------------------------------------------------------------------------+ */

#define MRPT_NO_WARN_BIG_HDR  // Yes, we really want to include all classes.
#include <mrpt/maps.h>

#include <mrpt/io/CMemoryStream.h>
#include <mrpt/serialization/CArchive.h>
#include <gtest/gtest.h>
#include <CTraitsTest.h>

using namespace mrpt;
using namespace mrpt::maps;
using namespace mrpt::obs;
using namespace mrpt::io;
using namespace mrpt::serialization;
using namespace std;

#define TEST_CLASS_MOVE_COPY_CTORS(_classname) \
	template class mrpt::CTraitsTest<_classname>

TEST_CLASS_MOVE_COPY_CTORS(CBeacon);
TEST_CLASS_MOVE_COPY_CTORS(CBeaconMap);
TEST_CLASS_MOVE_COPY_CTORS(CColouredPointsMap);
TEST_CLASS_MOVE_COPY_CTORS(CGasConcentrationGridMap2D);
TEST_CLASS_MOVE_COPY_CTORS(CWirelessPowerGridMap2D);
TEST_CLASS_MOVE_COPY_CTORS(CHeightGridMap2D);
TEST_CLASS_MOVE_COPY_CTORS(CReflectivityGridMap2D);
TEST_CLASS_MOVE_COPY_CTORS(COccupancyGridMap2D);
TEST_CLASS_MOVE_COPY_CTORS(CSimplePointsMap);
TEST_CLASS_MOVE_COPY_CTORS(CRandomFieldGridMap3D);
TEST_CLASS_MOVE_COPY_CTORS(CWeightedPointsMap);
TEST_CLASS_MOVE_COPY_CTORS(COctoMap);
TEST_CLASS_MOVE_COPY_CTORS(CColouredOctoMap);

// Create a set of classes, then serialize and deserialize to test possible
// bugs:
TEST(SerializeTestMaps, WriteReadToMem)
{
	const mrpt::rtti::TRuntimeClassId* lstClasses[] = {
		CLASS_ID(CBeacon),
		CLASS_ID(CBeaconMap),
		CLASS_ID(CColouredPointsMap),
		CLASS_ID(CGasConcentrationGridMap2D),
		CLASS_ID(CWirelessPowerGridMap2D),
		CLASS_ID(CHeightGridMap2D),
		CLASS_ID(CReflectivityGridMap2D),
		CLASS_ID(COccupancyGridMap2D),
		CLASS_ID(CSimplePointsMap),
		CLASS_ID(CRandomFieldGridMap3D),
		CLASS_ID(CWeightedPointsMap),
		CLASS_ID(COctoMap),
		CLASS_ID(CColouredOctoMap)};

	for (size_t i = 0; i < sizeof(lstClasses) / sizeof(lstClasses[0]); i++)
	{
		try
		{
			CMemoryStream buf;
			auto arch = mrpt::serialization::archiveFrom(buf);
			{
				CSerializable* o =
					static_cast<CSerializable*>(lstClasses[i]->createObject());
				arch << *o;
				delete o;
			}

			buf.Seek(0);
			std::variant<
				CBeacon,
				CBeaconMap,
				CColouredPointsMap,
				CGasConcentrationGridMap2D,
				CWirelessPowerGridMap2D,
				CHeightGridMap2D,
				CReflectivityGridMap2D,
				COccupancyGridMap2D,
				CSimplePointsMap,
				CRandomFieldGridMap3D,
				CWeightedPointsMap,
				COctoMap,
				CColouredOctoMap>  recons;
			arch >> recons;
		}
		catch (std::exception& e)
		{
			GTEST_FAIL() << "Exception during serialization test for class '"
						 << lstClasses[i]->className << "':\n"
						 << e.what() << endl;
		}
	}
}
