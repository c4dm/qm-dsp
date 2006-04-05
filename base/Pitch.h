/* -*- c-basic-offset: 4 indent-tabs-mode: nil -*-  vi:set ts=8 sts=4 sw=4: */

/*
    QM DSP library
    Centre for Digital Music, Queen Mary, University of London.
    This file Copyright 2006 Chris Cannam.
    All rights reserved.
*/

#ifndef _PITCH_H_
#define _PITCH_H_

class Pitch
{
public:
    static float getFrequencyForPitch(int midiPitch,
				      float centsOffset = 0,
				      float concertA = 440.0);

    static int getPitchForFrequency(float frequency,
				    float *centsOffsetReturn = 0,
				    float concertA = 440.0);
};


#endif
