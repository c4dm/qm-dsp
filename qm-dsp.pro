TEMPLATE = lib
CONFIG += release warn_on staticlib
OBJECTS_DIR = tmp_obj
MOC_DIR = tmp_moc

DEPENDPATH += base \
              dsp/chromagram \
              dsp/maths \
              dsp/onsets \
              dsp/phasevocoder \
              dsp/rateconversion \
              dsp/signalconditioning \
              dsp/tempotracking \
              dsp/tonal \
              dsp/transforms
INCLUDEPATH += . \
               base \
               dsp/maths \
               dsp/chromagram \
               dsp/transforms \
               dsp/onsets \
               dsp/phasevocoder \
               dsp/signalconditioning \
               dsp/rateconversion \
               dsp/tempotracking \
               dsp/tonal

# Input
HEADERS += base/Pitch.h \
           base/Window.h \
           dsp/chromagram/Chromagram.h \
           dsp/chromagram/ChromaProcess.h \
           dsp/chromagram/ConstantQ.h \
           dsp/maths/Correlation.h \
           dsp/maths/Histogram.h \
           dsp/maths/MathAliases.h \
           dsp/maths/MathUtilities.h \
           dsp/maths/Polyfit.h \
           dsp/onsets/DetectionFunction.h \
           dsp/onsets/PeakPicking.h \
           dsp/phasevocoder/PhaseVocoder.h \
           dsp/rateconversion/Decimator.h \
           dsp/signalconditioning/DFProcess.h \
           dsp/signalconditioning/Filter.h \
           dsp/signalconditioning/FiltFilt.h \
           dsp/signalconditioning/Framer.h \
           dsp/tempotracking/TempoTrack.h \
           dsp/tonal/ChangeDetectionFunction.h \
           dsp/tonal/TCSgram.h \
           dsp/tonal/TonalEstimator.h \
           dsp/transforms/FFT.h
SOURCES += base/Pitch.cpp \
           dsp/chromagram/Chromagram.cpp \
           dsp/chromagram/ChromaProcess.cpp \
           dsp/chromagram/ConstantQ.cpp \
           dsp/maths/Correlation.cpp \
           dsp/maths/MathUtilities.cpp \
           dsp/onsets/DetectionFunction.cpp \
           dsp/onsets/PeakPicking.cpp \
           dsp/phasevocoder/PhaseVocoder.cpp \
           dsp/rateconversion/Decimator.cpp \
           dsp/signalconditioning/DFProcess.cpp \
           dsp/signalconditioning/Filter.cpp \
           dsp/signalconditioning/FiltFilt.cpp \
           dsp/signalconditioning/Framer.cpp \
           dsp/tempotracking/TempoTrack.cpp \
           dsp/tonal/ChangeDetectionFunction.cpp \
           dsp/tonal/TCSgram.cpp \
           dsp/tonal/TonalEstimator.cpp \
           dsp/transforms/FFT.cpp 
