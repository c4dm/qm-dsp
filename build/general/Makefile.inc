
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
           dsp/transforms/DCT.h \
           dsp/transforms/FFT.h \
           dsp/wavelet/Wavelet.h \
           hmm/hmm.h \
           maths/Correlation.h \
           maths/CosineDistance.h \
           maths/KLDivergence.h \
           maths/MathAliases.h \
           maths/MathUtilities.h \
	   maths/MedianFilter.h \
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
           dsp/transforms/DCT.cpp \
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
		rm -f $@
		$(AR) cr $@ $^
		$(RANLIB) $@

depend:
		makedepend -fbuild/general/Makefile.inc -Y -- $(CFLAGS) -- $(SOURCES)

clean:		
		rm -f $(OBJECTS)

distclean:	clean
		rm -f $(LIBRARY)


# DO NOT DELETE

base/Pitch.o: base/Pitch.h
base/KaiserWindow.o: base/KaiserWindow.h maths/MathUtilities.h
base/KaiserWindow.o: maths/nan-inf.h
base/SincWindow.o: base/SincWindow.h
dsp/chromagram/Chromagram.o: maths/MathUtilities.h maths/nan-inf.h
dsp/chromagram/Chromagram.o: dsp/chromagram/Chromagram.h dsp/transforms/FFT.h
dsp/chromagram/Chromagram.o: base/Window.h dsp/chromagram/ConstantQ.h
dsp/chromagram/Chromagram.o: maths/MathAliases.h
dsp/chromagram/ConstantQ.o: dsp/chromagram/ConstantQ.h maths/MathAliases.h
dsp/chromagram/ConstantQ.o: maths/MathUtilities.h maths/nan-inf.h
dsp/chromagram/ConstantQ.o: dsp/transforms/FFT.h
dsp/keydetection/GetKeyMode.o: dsp/keydetection/GetKeyMode.h
dsp/keydetection/GetKeyMode.o: dsp/rateconversion/Decimator.h
dsp/keydetection/GetKeyMode.o: dsp/chromagram/Chromagram.h
dsp/keydetection/GetKeyMode.o: dsp/transforms/FFT.h base/Window.h
dsp/keydetection/GetKeyMode.o: dsp/chromagram/ConstantQ.h maths/MathAliases.h
dsp/keydetection/GetKeyMode.o: maths/MathUtilities.h maths/nan-inf.h
dsp/keydetection/GetKeyMode.o: base/Pitch.h
dsp/mfcc/MFCC.o: dsp/mfcc/MFCC.h base/Window.h dsp/transforms/FFT.h
dsp/onsets/DetectionFunction.o: dsp/onsets/DetectionFunction.h
dsp/onsets/DetectionFunction.o: maths/MathUtilities.h maths/nan-inf.h
dsp/onsets/DetectionFunction.o: maths/MathAliases.h
dsp/onsets/DetectionFunction.o: dsp/phasevocoder/PhaseVocoder.h base/Window.h
dsp/onsets/PeakPicking.o: dsp/onsets/PeakPicking.h maths/MathUtilities.h
dsp/onsets/PeakPicking.o: maths/nan-inf.h maths/MathAliases.h
dsp/onsets/PeakPicking.o: dsp/signalconditioning/DFProcess.h
dsp/onsets/PeakPicking.o: dsp/signalconditioning/FiltFilt.h
dsp/onsets/PeakPicking.o: dsp/signalconditioning/Filter.h maths/Polyfit.h
dsp/phasevocoder/PhaseVocoder.o: dsp/phasevocoder/PhaseVocoder.h
dsp/phasevocoder/PhaseVocoder.o: dsp/transforms/FFT.h maths/MathUtilities.h
dsp/phasevocoder/PhaseVocoder.o: maths/nan-inf.h
dsp/rateconversion/Decimator.o: dsp/rateconversion/Decimator.h
dsp/rateconversion/DecimatorB.o: dsp/rateconversion/DecimatorB.h
dsp/rateconversion/DecimatorB.o: maths/MathUtilities.h maths/nan-inf.h
dsp/rateconversion/Resampler.o: dsp/rateconversion/Resampler.h
dsp/rateconversion/Resampler.o: maths/MathUtilities.h maths/nan-inf.h
dsp/rateconversion/Resampler.o: base/KaiserWindow.h base/SincWindow.h
dsp/rateconversion/Resampler.o: thread/Thread.h
dsp/rhythm/BeatSpectrum.o: dsp/rhythm/BeatSpectrum.h maths/CosineDistance.h
dsp/segmentation/cluster_melt.o: dsp/segmentation/cluster_melt.h
dsp/segmentation/ClusterMeltSegmenter.o: dsp/segmentation/ClusterMeltSegmenter.h
dsp/segmentation/ClusterMeltSegmenter.o: dsp/segmentation/segment.h
dsp/segmentation/ClusterMeltSegmenter.o: dsp/segmentation/Segmenter.h
dsp/segmentation/ClusterMeltSegmenter.o: hmm/hmm.h base/Window.h
dsp/segmentation/ClusterMeltSegmenter.o: dsp/segmentation/cluster_segmenter.h
dsp/segmentation/ClusterMeltSegmenter.o: dsp/segmentation/cluster_melt.h
dsp/segmentation/ClusterMeltSegmenter.o: maths/pca/pca.h dsp/transforms/FFT.h
dsp/segmentation/ClusterMeltSegmenter.o: dsp/chromagram/ConstantQ.h
dsp/segmentation/ClusterMeltSegmenter.o: maths/MathAliases.h
dsp/segmentation/ClusterMeltSegmenter.o: maths/MathUtilities.h
dsp/segmentation/ClusterMeltSegmenter.o: maths/nan-inf.h
dsp/segmentation/ClusterMeltSegmenter.o: dsp/rateconversion/Decimator.h
dsp/segmentation/ClusterMeltSegmenter.o: dsp/mfcc/MFCC.h
dsp/segmentation/cluster_segmenter.o: dsp/segmentation/cluster_segmenter.h
dsp/segmentation/cluster_segmenter.o: dsp/segmentation/segment.h
dsp/segmentation/cluster_segmenter.o: dsp/segmentation/cluster_melt.h
dsp/segmentation/cluster_segmenter.o: hmm/hmm.h maths/pca/pca.h
dsp/segmentation/Segmenter.o: dsp/segmentation/Segmenter.h
dsp/signalconditioning/DFProcess.o: dsp/signalconditioning/DFProcess.h
dsp/signalconditioning/DFProcess.o: dsp/signalconditioning/FiltFilt.h
dsp/signalconditioning/DFProcess.o: dsp/signalconditioning/Filter.h
dsp/signalconditioning/DFProcess.o: maths/MathUtilities.h maths/nan-inf.h
dsp/signalconditioning/Filter.o: dsp/signalconditioning/Filter.h
dsp/signalconditioning/FiltFilt.o: dsp/signalconditioning/FiltFilt.h
dsp/signalconditioning/FiltFilt.o: dsp/signalconditioning/Filter.h
dsp/signalconditioning/Framer.o: dsp/signalconditioning/Framer.h
dsp/tempotracking/DownBeat.o: dsp/tempotracking/DownBeat.h
dsp/tempotracking/DownBeat.o: dsp/rateconversion/Decimator.h
dsp/tempotracking/DownBeat.o: maths/MathAliases.h maths/MathUtilities.h
dsp/tempotracking/DownBeat.o: maths/nan-inf.h maths/KLDivergence.h
dsp/tempotracking/DownBeat.o: dsp/transforms/FFT.h
dsp/tempotracking/TempoTrack.o: dsp/tempotracking/TempoTrack.h
dsp/tempotracking/TempoTrack.o: dsp/signalconditioning/DFProcess.h
dsp/tempotracking/TempoTrack.o: dsp/signalconditioning/FiltFilt.h
dsp/tempotracking/TempoTrack.o: dsp/signalconditioning/Filter.h
dsp/tempotracking/TempoTrack.o: maths/Correlation.h
dsp/tempotracking/TempoTrack.o: dsp/signalconditioning/Framer.h
dsp/tempotracking/TempoTrack.o: maths/MathAliases.h maths/MathUtilities.h
dsp/tempotracking/TempoTrack.o: maths/nan-inf.h
dsp/tempotracking/TempoTrackV2.o: dsp/tempotracking/TempoTrackV2.h
dsp/tempotracking/TempoTrackV2.o: maths/MathUtilities.h maths/nan-inf.h
dsp/tonal/ChangeDetectionFunction.o: dsp/tonal/ChangeDetectionFunction.h
dsp/tonal/ChangeDetectionFunction.o: dsp/tonal/TCSgram.h
dsp/tonal/ChangeDetectionFunction.o: dsp/tonal/TonalEstimator.h
dsp/tonal/TCSgram.o: dsp/tonal/TCSgram.h dsp/tonal/TonalEstimator.h
dsp/tonal/TCSgram.o: maths/MathUtilities.h maths/nan-inf.h
dsp/tonal/TonalEstimator.o: dsp/tonal/TonalEstimator.h
dsp/transforms/FFT.o: dsp/transforms/FFT.h maths/MathUtilities.h
dsp/transforms/FFT.o: maths/nan-inf.h ext/kissfft/kiss_fft.h
dsp/transforms/FFT.o: ext/kissfft/tools/kiss_fftr.h
dsp/wavelet/Wavelet.o: dsp/wavelet/Wavelet.h
hmm/hmm.o: ./include/clapack.h maths/nan-inf.h ./include/cblas.h hmm/hmm.h
maths/Correlation.o: maths/Correlation.h
maths/CosineDistance.o: maths/CosineDistance.h
maths/KLDivergence.o: maths/KLDivergence.h
maths/MathUtilities.o: maths/MathUtilities.h maths/nan-inf.h
maths/pca/pca.o: maths/pca/pca.h
thread/Thread.o: thread/Thread.h
ext/kissfft/kiss_fft.o: ext/kissfft/_kiss_fft_guts.h ext/kissfft/kiss_fft.h
ext/kissfft/tools/kiss_fftr.o: ext/kissfft/tools/kiss_fftr.h
ext/kissfft/tools/kiss_fftr.o: ext/kissfft/kiss_fft.h
ext/kissfft/tools/kiss_fftr.o: ext/kissfft/_kiss_fft_guts.h
