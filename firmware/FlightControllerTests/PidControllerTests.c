/* 
 * File:   PidControllerTests.c
 * Author: benjo
 *
 * Created on July 15, 2022, 2:39 PM
 */

#define CATCH_CONFIG_MAIN
#include "catch.hpp"
#include "PidController.h"

TEST_CASE( "P = 1.0 Updates ", "[run]") {
    PidControllerState pOnly;
    pOnly.Kp = 1.0;
    REQUIRE( RunPidController(&pOnly, 1.2, 1.0) == Approx(1.2));
}
