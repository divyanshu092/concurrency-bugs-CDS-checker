include ../benchmarks.mk

BENCH := hashmap
NORMAL_TESTS := testcase1 testcase2 testcase3

WILDCARD_TESTS := $(patsubst %, %_wildcard, $(NORMAL_TESTS))

#TESTS := $(NORMAL_TESTS) $(WILDCARD_TESTS)
TESTS := $(NORMAL_TESTS)

all: $(TESTS)

$(WILDCARD_TESTS): CXXFLAGS += -DWILDCARD

$(BENCH).o : $(BENCH).h
	$(CXX) -o $@ $< $(CXXFLAGS) -c $(LDFLAGS)

$(BENCH)_wildcard.o : $(BENCH)_wildcard.h
	$(CXX) -o $@ $< $(CXXFLAGS) -c $(LDFLAGS)

$(WILDCARD_TESTS): %_wildcard : %.cc $(BENCH)_wildcard.o 
	$(CXX) -o $@ $< $(CXXFLAGS) $(LDFLAGS)

$(NORMAL_TESTS): % : %.cc $(BENCH).o
	$(CXX) -o $@ $< $(CXXFLAGS) $(LDFLAGS)

clean:
	rm -f *.o *.d $(TESTS)
