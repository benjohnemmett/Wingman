## -*- Makefile -*-
##
## User: benjo
## Time: Jul 15, 2022 3:09:51 PM
## Makefile created by Oracle Solaris Studio.
##
## This file is generated automatically.
##
CC = g++

FC_PROJ = "../FlightController.X"
INC = -I$(FC_PROJ)
OPTS = -std=c++11

TESTS_EXE = PidControllerTests.exe

all: tests

build: $(TESTS_EXE)
	$(info "Build complete")

PidControllerTests.exe : PidControllerTests.c
	$(CC) $(INC) $(OPTS) $(FC_PROJ)/PidController.c PidControllerTests.c -o PidControllerTests.exe

tests: build
	$(info "Running PidControllerTests.exe...")
	./PidControllerTests.exe

.PHONY: clean
clean:
	rm -f ./*.exe


# Enable dependency checking
.KEEP_STATE:
.KEEP_STATE_FILE:.make.state.GNU-amd64-Windows

