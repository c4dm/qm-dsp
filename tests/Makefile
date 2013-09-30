
CFLAGS := -I.. $(CFLAGS)
CXXFLAGS := -I.. $(CXXFLAGS)

LDFLAGS := $(LDFLAGS) -lboost_unit_test_framework
LIBS := ../libqm-dsp.a

TESTS	:= test-window test-fft

all: $(TESTS)
	for t in $(TESTS); do echo "Running $$t"; ./"$$t" || exit 1; done

test-window: TestWindow.o $(LIBS)
	$(CXX) -o $@ $^ $(LDFLAGS)

test-fft: TestFFT.o $(LIBS)
	$(CXX) -o $@ $^ $(LDFLAGS)

clean: 
	rm *.o $(TESTS)

