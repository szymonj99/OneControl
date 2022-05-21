#include <gtest/gtest.h>
#include "../src/Program/OneControl.h"

namespace oc
{
    class OneControlTest : public ::testing::Test
    {
    protected:

        void SetUp() override { }
    };

    TEST_F(OneControlTest, MyTest)
    {
        auto x = std::make_unique<oc::Server>();
        EXPECT_EQ(1, 1);
    }
}
