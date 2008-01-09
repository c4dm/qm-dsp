TEMPLATE = lib
CONFIG += release warn_on staticlib
CONFIG -= qt
OBJECTS_DIR = tmp_obj
MOC_DIR = tmp_moc

#DEPENDPATH += base \
#              dsp/chromagram \
#              dsp/keydetection \
#              dsp/maths \
#              dsp/onsets \
#              dsp/phasevocoder \
#              dsp/rateconversion \
#              dsp/signalconditioning \
#              dsp/tempotracking \
#              dsp/tonal \
#              dsp/transforms
INCLUDEPATH += . 

# Input
HEADERS += base/Pitch.h \
           base/Window.h \
           dsp/chromagram/Chromagram.h \
           dsp/chromagram/ChromaProcess.h \
           dsp/chromagram/ConstantQ.h \
           dsp/keydetection/GetKeyMode.h \
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
           dsp/transforms/FFT.h \
           maths/Correlation.h \
           maths/Histogram.h \
           maths/MathAliases.h \
           maths/MathUtilities.h \
           maths/Polyfit.h
SOURCES += base/Pitch.cpp \
           dsp/chromagram/Chromagram.cpp \
           dsp/chromagram/ChromaProcess.cpp \
           dsp/chromagram/ConstantQ.cpp \
           dsp/keydetection/GetKeyMode.cpp \
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
           dsp/transforms/FFT.cpp \
           maths/Correlation.cpp \
           maths/MathUtilities.cpp
