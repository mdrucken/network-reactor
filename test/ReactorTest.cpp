#include <gtest/gtest.h>

#include "Reactor.h"

class ReactorTest : public ::testing::Test
{
};

TEST_F(ReactorTest, HandleEvents_WhereNoHandlerRegistered_ThenReturns)
{
    Reactor r;
    r.HandleEvents();
}
