
LIBRARY ?= libqm-dsp.a
CXX	?= g++
CC	?= gcc
AR	?= ar
RANLIB	?= ranlib

CFLAGS	:= $(CFLAGS) -I. -Iext/kissfft -Iext/kissfft/tools -Dkiss_fft_scalar=double
CXXFLAGS	:= $(CXXFLAGS) -I. -Iext/kissfft -Iext/kissfft/tools -Dkiss_fft_scalar=double

HEADERS := base/Pitch.h \
           base/Window.h \
           base/KaiserWindow.h \
           base/SincWindow.h \
           dsp/chromagram/Chromagram.h \
           dsp/chromagram/ConstantQ.h \
           dsp/keydetection/GetKeyMode.h \
           dsp/mfcc/MFCC.h \
           dsp/onsets/DetectionFunction.h \
           dsp/onsets/PeakPicking.h \
           dsp/phasevocoder/PhaseVocoder.h \
           dsp/rateconversion/Decimator.h \
           dsp/rateconversion/DecimatorB.h \
           dsp/rateconversion/Resampler.h \
           dsp/rhythm/BeatSpectrum.h \
           dsp/segmentation/cluster_melt.h \
           dsp/segmentation/ClusterMeltSegmenter.h \
           dsp/segmentation/cluster_segmenter.h \
           dsp/segmentation/Segmenter.h \
           dsp/segmentation/segment.h \
           dsp/signalconditioning/DFProcess.h \
           dsp/signalconditioning/Filter.h \
           dsp/signalconditioning/FiltFilt.h \
           dsp/signalconditioning/Framer.h \
           dsp/tempotracking/DownBeat.h \
           dsp/tempotracking/TempoTrack.h \
           dsp/tempotracking/TempoTrackV2.h \
           dsp/tonal/ChangeDetectionFunction.h \
           dsp/tonal/TCSgram.h \
           dsp/tonal/TonalEstimator.h \
           dsp/transforms/FFT.h \
           dsp/wavelet/Wavelet.h \
           hmm/hmm.h \
           maths/Correlation.h \
           maths/CosineDistance.h \
           maths/KLDivergence.h \
           maths/MathAliases.h \
           maths/MathUtilities.h \
           maths/Polyfit.h \
           maths/pca/pca.h \
           thread/AsynchronousTask.h \
           thread/BlockAllocator.h \
           thread/Thread.h \
           ext/kissfft/kiss_fft.h \
           ext/kissfft/tools/kiss_fftr.h

SOURCES := base/Pitch.cpp \
           base/KaiserWindow.cpp \
           base/SincWindow.cpp \
           dsp/chromagram/Chromagram.cpp \
           dsp/chromagram/ConstantQ.cpp \
           dsp/keydetection/GetKeyMode.cpp \
           dsp/mfcc/MFCC.cpp \
           dsp/onsets/DetectionFunction.cpp \
           dsp/onsets/PeakPicking.cpp \
           dsp/phasevocoder/PhaseVocoder.cpp \
           dsp/rateconversion/Decimator.cpp \
           dsp/rateconversion/DecimatorB.cpp \
           dsp/rateconversion/Resampler.cpp \
           dsp/rhythm/BeatSpectrum.cpp \
           dsp/segmentation/cluster_melt.c \
           dsp/segmentation/ClusterMeltSegmenter.cpp \
           dsp/segmentation/cluster_segmenter.c \
           dsp/segmentation/Segmenter.cpp \
           dsp/signalconditioning/DFProcess.cpp \
           dsp/signalconditioning/Filter.cpp \
           dsp/signalconditioning/FiltFilt.cpp \
           dsp/signalconditioning/Framer.cpp \
           dsp/tempotracking/DownBeat.cpp \
           dsp/tempotracking/TempoTrack.cpp \
           dsp/tempotracking/TempoTrackV2.cpp \
           dsp/tonal/ChangeDetectionFunction.cpp \
           dsp/tonal/TCSgram.cpp \
           dsp/tonal/TonalEstimator.cpp \
           dsp/transforms/FFT.cpp \
           dsp/wavelet/Wavelet.cpp \
           hmm/hmm.c \
           maths/Correlation.cpp \
           maths/CosineDistance.cpp \
           maths/KLDivergence.cpp \
           maths/MathUtilities.cpp \
           maths/pca/pca.c \
           thread/Thread.cpp \
           ext/kissfft/kiss_fft.c \
           ext/kissfft/tools/kiss_fftr.c

OBJECTS := $(SOURCES:.cpp=.o)
OBJECTS := $(OBJECTS:.c=.o)

$(LIBRARY):	$(OBJECTS)
		$(AR) cr $@ $^
		$(RANLIB) $@

clean:		
		rm -f $(OBJECTS)

distclean:	clean
		rm -f $(LIBRARY)
