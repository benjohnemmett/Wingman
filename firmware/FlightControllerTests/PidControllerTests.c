/* 
 * File:   PidControllerTests.c
 * Author: Ben Emmett
 *
 * Created on July 15, 2022, 2:39 PM
 */

#define CATCH_CONFIG_MAIN
#include <cmath>
#include "catch.hpp"
#include "PidController.h"

static const float epsilon = 1e-6;

TEST_CASE( "P = 1.0 Updates ", "[run]") {
    PidControllerState pOnly;
    pOnly.Kp = 1.0;
    pOnly.Ki = 0.0;
    pOnly.Kd = 0.0;
    REQUIRE( RunPidController(&pOnly, 1.2, 1.0) == Approx(1.2));
    REQUIRE( RunPidController(&pOnly, -1.2, 1.0) == Approx(-1.2));
    REQUIRE( abs(RunPidController(&pOnly, 0.0, 1.0) - 0.0f) < epsilon);
    REQUIRE( abs(RunPidController(&pOnly, 0.0, 2.0) - 0.0f) < epsilon);
}

TEST_CASE( "P = 0.5 Updates ", "[run]") {
    PidControllerState pOnly;
    pOnly.Kp = 0.5;
    pOnly.Ki = 0.0;
    pOnly.Kd = 0.0;
    REQUIRE( RunPidController(&pOnly, 1.2, 1.0) == Approx(0.6));
    REQUIRE( RunPidController(&pOnly, -1.2, 1.0) == Approx(-0.6));
    REQUIRE( abs(RunPidController(&pOnly, 0.0, 1.0) - 0.0f) < epsilon);
    REQUIRE( abs(RunPidController(&pOnly, 0.0, 2.0) - 0.0f) < epsilon);
}

TEST_CASE( "D = 1.0 Constant input ", "[run]") {
    PidControllerState pid;
    pid.Kp = 0;
    pid.Ki = 0;
    pid.Kd = 1.0;
    
    float inputs[] = {1.0, 1.0, 1.0};
    float expectedOutputs[] = {0.0, 0.0, 0.0};
    
    REQUIRE( abs(RunPidController(&pid, inputs[0], 1.0) - expectedOutputs[0]) < epsilon);
    REQUIRE( abs(RunPidController(&pid, inputs[1], 1.0) - expectedOutputs[1]) < epsilon);
    REQUIRE( abs(RunPidController(&pid, inputs[2], 1.0) - expectedOutputs[2]) < epsilon);
}

TEST_CASE( "D = 1.0 Non-constant input ", "[run]") {
    PidControllerState pid;
    pid.Kp = 0;
    pid.Ki = 0;
    pid.Kd = 1.0;
    
    float inputs[]          = {1.0, 1.5, 1.3, 1.3};
    float expectedOutputs[] = {0.0, 0.5, -0.2, 0.0};
    
    REQUIRE( abs(RunPidController(&pid, inputs[0], 1.0) - expectedOutputs[0]) < epsilon);
    REQUIRE( abs(RunPidController(&pid, inputs[1], 1.0) - expectedOutputs[1]) < epsilon);
    REQUIRE( abs(RunPidController(&pid, inputs[2], 1.0) - expectedOutputs[2]) < epsilon);
    REQUIRE( abs(RunPidController(&pid, inputs[3], 1.0) - expectedOutputs[3]) < epsilon);
}

TEST_CASE( "I = 0.1 Constant dt", "[run]") {
    PidControllerState pid;
    pid.Kp = 0;
    pid.Ki = 0.1;
    pid.Kd = 0;
    pid.integrator_min = -1.0;
    pid.integrator_max = 1.0;
    
    float inputs[]          = {1.0, 0.5, -0.3};
    float expectedOutputs[] = {0.1, 0.15, 0.12};
    
    REQUIRE( abs(RunPidController(&pid, inputs[0], 1.0) - expectedOutputs[0]) < epsilon);
    REQUIRE( abs(RunPidController(&pid, inputs[1], 1.0) - expectedOutputs[1]) < epsilon);
    REQUIRE( abs(RunPidController(&pid, inputs[2], 1.0) - expectedOutputs[2]) < epsilon);
}