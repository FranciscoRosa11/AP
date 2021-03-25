UNAME_S := $(shell uname -s)

COMPILER := gcc
#COMPILER := intel
#COMPILER := cuda
OMP := no
VEC := no
DEBUG := no
PAPI := no

DEBUG_FLAGS=
INCLUDES=
PAPI_INC=
PAPI_LIB=
OMP_INC=
OMP_LIB=
MPI_LIB=
MPI_INC=
OPTIMIZATION := -O3

SRC=src/
EXP=experimental/
TESTDIR=test/
BIN=bin/

ifeq ($(DEBUG),yes)
		DEBUG_FLAGS=-DDEBUG -DPAPI_LOG_INFO=true -DPAPI_LOG_VERBOSE=true
endif

ifeq ($(OMP),yes)
		OMPFLAGS=-DMATRIX_OMP
endif

ifeq ($(UNAME_S),Linux)
	ifeq ($(PAPI), yes)
		# Papi libs and includes
		PAPI_INC=-I/share/apps/papi/5.5.0/include
		PAPI_LIB=-L/share/apps/papi/5.5.0/lib -lpapi
	endif

	ifeq ($(COMPILER),intel)
		#Intel requires module load intel/2020
		# BUT that CANNOT be done on search node (no qsub -I before make)
		CXX = icpc
		LD  = icpc
		MPICC=mpiicc
		MPI_LIB=
#		MPI_INC=/share/apps/intel/compilers_and_libraries_2020.0.166/linux/mpi/intel64/include
		OMP_FLAGS=-qopenmp $(OMP_EXTRA)
		ifeq ($(VEC),yes)
			CXXFLAGS_VECTOR  = -O3 -qopenmp -g -Wall -Wextra -std=c++11 -Wno-unused-parameter -qopt-report=5 -qopt-report-phase=vec $(DEBUG_FLAGS)
		else
#			CXXFLAGS  = $(OPTIMIZATION) -g -Wall -Wextra -std=c99 -Wno-unused-parameter $(DEBUG_FLAGS)
			CXXFLAGS  = $(OPTIMIZATION) -qopenmp -g -Wall -Wextra -std=c99 -Wno-unused-parameter -qopt-report=2 $(DEBUG_FLAGS)
		endif
		#	include directories
		# library directories
		ifeq ($(DYNAMIC),yes)
			CXXFLAGS += -DD_DYNAMIC
		endif
		ifeq ($(IRREGULAR),yes)
			CXXFLAGS += -DD_IRREGULAR
		endif
	endif
	ifeq ($(COMPILER),cuda)
		# GPU NVidia compiler
		CXX = nvcc
		LD  = nvcc
		CXXFLAGS = $(OPTIMIZATION) -DD_GPU -arch=sm_30
		ifeq ($(DEBUG),yes)
			CXXFLAGS += -ggdb3
		endif
	endif
	ifeq ($(COMPILER),gcc)
		# GCC
		CXX=gcc
		# OPT: CXXFLAGS= $(OPTIMIZATION) -std=c99 -g -fopenmp $(INCLUDES) $(DEBUG_FLAGS)
		# NOOPT CXXFLAGS= -std=c99 -g -fopenmp $(INCLUDES) $(DEBUG_FLAGS)
		OMP_FLAGS=-fopenmp $(OMP_EXTRA)
		# -lm needed for sqrt on gcc
		CXXFLAGS=$(OPTIMIZATION) -std=c99 -g $(DEBUG_FLAGS) $(OMP_FLAGS) -lm
		# MPI requires "module load gnu/openmpi_eth/1.8.4"
		MPICC=mpicc
	endif
endif
ifeq ($(UNAME_S),Darwin)
	CXX=/usr/local/bin/gcc-10
	MPICC=/opt/openmpi/bin/mpicc
	MPI_INC=-I /opt/openmpi/include
	INCLUDES=
	OMP_FLAGS= -fopenmp
#	CXXFLAGS= $(OPTIMIZATION) -std=c99 -g
	CXXFLAGS= $(OPTIMIZATION) -std=c99 -g $(OMP_FLAGS) -v
#	MPI_LIB=-L/opt/openmpi/lib -l
endif
#CXXFLAGS= -O3 -std=c++11 -mavx -pg -qopenmp -qopt-report5 $(INCLUDES)
#
PROGS=$(BIN)kmeans

.PHONY: all
all: $(BIN) noop

noop:
	$(CXX) $(CXXFLAGS) -o $(BIN)rooms_noop $(SRC)rooms_main.c $(SRC)config.c $(SRC)matrix_support.c \
										   $(SRC)rooms_noop_impl.c $(HEADERS) $(LIBS)
#
#montecarlo:
#	$(CXX) $(CXXFLAGS) -o $(BIN)rooms_montecarlo $(SRC)rooms_main.c \
#						  $(SRC)config.c $(SRC) $(SRC)rooms_noop_implc.c \
#						  $(HEADERS) $(LIBS)


$(BIN):
	mkdir $(BIN)

.PHONY: clean
clean:
	rm -r $(BIN)

