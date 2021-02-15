
#include <wsjcpp_core.h>
#include <wsjcpp_unit_tests.h>
#include "sea5kg_triangulation.h"

// ---------------------------------------------------------------------
// UnitTestSea5kgTriangulationPoint

class UnitTestSea5kgTriangulationPoint : public WsjcppUnitTestBase {
    public:
        UnitTestSea5kgTriangulationPoint();
        virtual bool doBeforeTest() override;
        virtual void executeTest() override;
        virtual bool doAfterTest() override;
};

REGISTRY_WSJCPP_UNIT_TEST(UnitTestSea5kgTriangulationPoint)

UnitTestSea5kgTriangulationPoint::UnitTestSea5kgTriangulationPoint()
    : WsjcppUnitTestBase("UnitTestSea5kgTriangulationPoint") {
}

// ---------------------------------------------------------------------

bool UnitTestSea5kgTriangulationPoint::doBeforeTest() {
    // do something before test
    return true;
}

// ---------------------------------------------------------------------

void UnitTestSea5kgTriangulationPoint::executeTest() {
    Sea5kgTriangulationPoint p(0,0);

    compare("X", p.getXint(), 0);
    compare("Y", p.getYint(), 0);

    // TODO unit test source code here
}

// ---------------------------------------------------------------------

bool UnitTestSea5kgTriangulationPoint::doAfterTest() {
    // do somethig after test
    return true;
}


