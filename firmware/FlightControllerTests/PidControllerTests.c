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
    PidControllerState pid;
    ResetPidControllerState(&pid);
    pid.Kp = 1.0;
    pid.Ki = 0.0;
    pid.Kd = 0.0;

    REQUIRE( RunPidController(&pid, 1.2, 1.0) == Approx(1.2));
    REQUIRE( RunPidController(&pid, -1.2, 1.0) == Approx(-1.2));
    REQUIRE( abs(RunPidController(&pid, 0.0, 1.0) - 0.0f) < epsilon);
    REQUIRE( abs(RunPidController(&pid, 0.0, 2.0) - 0.0f) < epsilon);
}

TEST_CASE( "P = 0.5 Updates ", "[run]") {
    PidControllerState pid;
    ResetPidControllerState(&pid);
    pid.Kp = 0.5;
    pid.Ki = 0.0;
    pid.Kd = 0.0;

    REQUIRE( RunPidController(&pid, 1.2, 1.0) == Approx(0.6));
    REQUIRE( RunPidController(&pid, -1.2, 1.0) == Approx(-0.6));
    REQUIRE( abs(RunPidController(&pid, 0.0, 1.0) - 0.0f) < epsilon);
    REQUIRE( abs(RunPidController(&pid, 0.0, 2.0) - 0.0f) < epsilon);
}

TEST_CASE( "D = 1.0 Constant input ", "[run]") {
    PidControllerState pid;
    ResetPidControllerState(&pid);
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
    ResetPidControllerState(&pid);
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
    ResetPidControllerState(&pid);
    pid.Kp = 0;
    pid.Ki = 0.1;
    pid.Kd = 0;
    pid.integrator_min = -1.0;
    pid.integrator_max = 1.0;
    float dt = 1.0;
    
    float inputs[]          = {1.0, 0.5, -0.3};
    float expectedOutputs[] = {0.1, 0.15, 0.12};
    
    REQUIRE( abs(RunPidController(&pid, inputs[0], dt) - expectedOutputs[0]) < epsilon);
    REQUIRE( abs(RunPidController(&pid, inputs[1], dt) - expectedOutputs[1]) < epsilon);
    REQUIRE( abs(RunPidController(&pid, inputs[2], dt) - expectedOutputs[2]) < epsilon);
}

TEST_CASE( "I = 0.5 integrator upper bound", "[run]") {
    PidControllerState pid;
    ResetPidControllerState(&pid);
    pid.Kp = 0;
    pid.Ki = 0.5;
    pid.Kd = 0;
    pid.integrator_min = -1.0;
    pid.integrator_max = 1.0;
    float dt = 1.0;
    
    float inputs[]          = {1.0, 0.5, 0.6};
    float expectedOutputs[] = {0.5, 0.75, 1.0};
    
    REQUIRE( abs(RunPidController(&pid, inputs[0], dt) - expectedOutputs[0]) < epsilon);
    REQUIRE( abs(RunPidController(&pid, inputs[1], dt) - expectedOutputs[1]) < epsilon);
    REQUIRE( abs(RunPidController(&pid, inputs[2], dt) - expectedOutputs[2]) < epsilon);
}

TEST_CASE( "I = 0.5 integrator lower bound", "[run]") {
    PidControllerState pid;
    ResetPidControllerState(&pid);
    pid.Kp = 0;
    pid.Ki = 0.5;
    pid.Kd = 0;
    pid.integrator_min = -1.0;
    pid.integrator_max = 1.0;
    float dt = 1.0;
    
    float inputs[]          = {-0.6, -0.5, -1.0};
    float expectedOutputs[] = {-0.3, -0.55, -1.0};
    
    REQUIRE( abs(RunPidController(&pid, inputs[0], dt) - expectedOutputs[0]) < epsilon);
    REQUIRE( abs(RunPidController(&pid, inputs[1], dt) - expectedOutputs[1]) < epsilon);
    REQUIRE( abs(RunPidController(&pid, inputs[2], dt) - expectedOutputs[2]) < epsilon);
}