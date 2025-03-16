CXX=g++
CXXFLAGS=--std=c++11 -W -Wall -O3 -DNDEBUG -fPIC
LDLIBS=

SRCS=Solver.cpp
OBJS=$(subst .cpp,.o,$(SRCS))

ifeq ($(OS),Windows_NT) # Windows
		SHARED_LIB_EXT=dll
		LDFLAGS=-Wl,--output-def,solver_c_interface.def -Wl,--export-all-symbols -Wl,--out-implib,solver_c_interface.lib
else # Linux/macOS
		SHARED_LIB_EXT=so
		LDFLAGS=
endif

all: c4solver generator c4solver_c_interface

c4solver: $(OBJS) main.o
				$(CXX) $(CXXFLAGS) $(LDFLAGS) -o c4solver main.o $(OBJS) $(LDLIBS)

generator: generator.o
				$(CXX) $(CXXFLAGS) $(LDFLAGS) -o generator generator.o $(LDLIBS)

c4solver_c_interface: $(OBJS) solver_c_interface.o
				$(CXX) $(CXXFLAGS) $(LDFLAGS) -shared -o c4solver_c_interface.$(SHARED_LIB_EXT) solver_c_interface.o $(OBJS) $(LDLIBS)

.depend: $(SRCS)
				$(CXX) $(CXXFLAGS) -MM $^ > ./.depend

-include .depend

clean:
ifeq ($(OS),Windows_NT)
				del *.o .depend c4solver.exe generator.exe c4solver_c_interface.dll c4solver_c_interface.def c4solver_c_interface.lib
else
				rm -f *.o .depend c4solver generator c4solver_c_interface.so
endif

PHONY: clean all