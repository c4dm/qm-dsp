
CFLAGS := -I.. $(CFLAGS)
CXXFLAGS := -I.. -Wall -g $(CXXFLAGS)

LDFLAGS := $(LDFLAGS) -lboost_unit_test_framework
LIBS := ../libqm-dsp.a

TESTS	:= test-mathutilities test-window test-fft test-pvoc

#VG	:= valgrind

all: $(TESTS)
	for t in $(TESTS); do echo "Running $$t"; $(VG) ./"$$t" || exit 1; done

test-mathutilities: TestMathUtilities.o $(LIBS)
	$(CXX) -o $@ $^ $(LDFLAGS)

test-window: TestWindow.o $(LIBS)
	$(CXX) -o $@ $^ $(LDFLAGS)

test-fft: TestFFT.o $(LIBS)
	$(CXX) -o $@ $^ $(LDFLAGS)

test-pvoc: TestPhaseVocoder.o $(LIBS)
	$(CXX) -o $@ $^ $(LDFLAGS)

TestMathUtilities.o: $(LIBS)
TestWindow.o: $(LIBS)
TestFFT.o: $(LIBS)
TestPhaseVocoder.o: $(LIBS)

clean: 
	rm -f *.o $(TESTS)

