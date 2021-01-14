include ../benchmarks.mk

all: seqlock

seqlock: seqlock.c
	$(CXX) -o $@ $< $(CXXFLAGS) $(LDFLAGS)

clean:
	rm -f seqlock
