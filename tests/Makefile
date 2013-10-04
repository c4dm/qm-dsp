
CFLAGS := -I.. $(CFLAGS)
CXXFLAGS := -I.. -Wall -g $(CXXFLAGS)

LDFLAGS := $(LDFLAGS) -lboost_unit_test_framework
LIBS := ../libqm-dsp.a

TESTS	:= test-window test-fft test-pvoc

all: $(TESTS)
	for t in $(TESTS); do echo "Running $$t"; valgrind ./"$$t" || exit 1; done

test-window: TestWindow.o $(LIBS)
	$(CXX) -o $@ $^ $(LDFLAGS)

test-fft: TestFFT.o $(LIBS)
	$(CXX) -o $@ $^ $(LDFLAGS)

test-pvoc: TestPhaseVocoder.o $(LIBS)
	$(CXX) -o $@ $^ $(LDFLAGS)

TestWindow.o: $(LIBS)
TestFFT.o: $(LIBS)
TestPhaseVocoder.o: $(LIBS)

clean: 
	rm -f *.o $(TESTS)

