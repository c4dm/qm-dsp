/* -*- c-basic-offset: 4 indent-tabs-mode: nil -*-  vi:set ts=8 sts=4 sw=4: */

/*
    QM DSP Library

    Centre for Digital Music, Queen Mary, University of London.
    This file copyright 2008-2009 Matthew Davies and QMUL.
    All rights reserved.
*/

#include "DownBeat.h"

#include "maths/MathAliases.h"
#include "maths/MathUtilities.h"
#include "dsp/transforms/FFT.h"

#include <iostream>
#include <cstdlib>

DownBeat::DownBeat(float originalSampleRate,
                   size_t decimationFactor,
                   size_t dfIncrement) :
    m_rate(originalSampleRate),
    m_factor(decimationFactor),
    m_increment(dfIncrement),
    m_decimator1(0),
    m_decimator2(0),
    m_buffer(0),
    m_bufsiz(0),
    m_buffill(0),
    m_beatframesize(0),
    m_beatframe(0)
{
    // beat frame size is next power of two up from 1.3 seconds at the
    // downsampled rate (happens to produce 4096 for 44100 or 48000 at
    // 16x decimation, which is our expected normal situation)
    int bfs = int((m_rate / decimationFactor) * 1.3);
    m_beatframesize = 1;
    while (bfs) { bfs >>= 1; m_beatframesize <<= 1; }
    std::cerr << "rate = " << m_rate << ", bfs = " << m_beatframesize << std::endl;
    m_beatframe = new double[m_beatframesize];
    m_fftRealOut = new double[m_beatframesize];
    m_fftImagOut = new double[m_beatframesize];
}

DownBeat::~DownBeat()
{
    delete m_decimator1;
    delete m_decimator2;
    if (m_buffer) free(m_buffer);
    delete[] m_decbuf;
    delete[] m_beatframe;
    delete[] m_fftRealOut;
    delete[] m_fftImagOut;
}

void
DownBeat::makeDecimators()
{
    if (m_factor < 2) return;
    int highest = Decimator::getHighestSupportedFactor();
    if (m_factor <= highest) {
        m_decimator1 = new Decimator(m_increment, m_factor);
        return;
    }
    m_decimator1 = new Decimator(m_increment, highest);
    m_decimator2 = new Decimator(m_increment / highest, m_factor / highest);
    m_decbuf = new double[m_factor / highest];
}

void
DownBeat::pushAudioBlock(const double *audio)
{
    if (m_buffill + (m_increment / m_factor) > m_bufsiz) {
        if (m_bufsiz == 0) m_bufsiz = m_increment * 16;
        else m_bufsiz = m_bufsiz * 2;
        if (!m_buffer) {
            m_buffer = (double *)malloc(m_bufsiz * sizeof(double));
        } else {
            std::cerr << "DownBeat::pushAudioBlock: realloc m_buffer to " << m_bufsiz << std::endl;
            m_buffer = (double *)realloc(m_buffer, m_bufsiz * sizeof(double));
        }
    }
    if (!m_decimator1) makeDecimators();
    if (m_decimator2) {
        m_decimator1->process(audio, m_decbuf);
        m_decimator2->process(m_decbuf, m_buffer + m_buffill);
    } else {
        m_decimator1->process(audio, m_buffer + m_buffill);
    }
    m_buffill += m_increment / m_factor;
}
    
const double *
DownBeat::getBufferedAudio(size_t &length) const
{
    length = m_buffill;
    return m_buffer;
}

void
DownBeat::findDownBeats(const double *audio,
                        size_t audioLength,
                        const d_vec_t &beats,
                        i_vec_t &downbeats)
{
    // FIND DOWNBEATS BY PARTITIONING THE INPUT AUDIO FILE INTO BEAT SEGMENTS
    // WHERE THE AUDIO FRAMES ARE DOWNSAMPLED  BY A FACTOR OF 16 (fs ~= 2700Hz)
    // THEN TAKING THE JENSEN-SHANNON DIVERGENCE BETWEEN BEAT SYNCHRONOUS SPECTRAL FRAMES

    // IMPLEMENTATION (MOSTLY) FOLLOWS:
    //  DAVIES AND PLUMBLEY "A SPECTRAL DIFFERENCE APPROACH TO EXTRACTING DOWNBEATS IN MUSICAL AUDIO"
    //  EUSIPCO 2006, FLORENCE, ITALY

    d_vec_t newspec(m_beatframesize / 2); // magnitude spectrum of current beat
    d_vec_t oldspec(m_beatframesize / 2); // magnitude spectrum of previous beat
    d_vec_t specdiff;

    if (audioLength == 0) return;

    for (size_t i = 0; i + 1 < beats.size(); ++i) {

        // Copy the extents of the current beat from downsampled array
        // into beat frame buffer

        size_t beatstart = (beats[i] * m_increment) / m_factor;
        size_t beatend = (beats[i] * m_increment) / m_factor;
        if (beatend >= audioLength) beatend = audioLength - 1;
        if (beatend < beatstart) beatend = beatstart;
        size_t beatlen = beatend - beatstart;

        // Also apply a Hanning window to the beat frame buffer, sized
        // to the beat extents rather than the frame size.  (Because
        // the size varies, it's easier to do this by hand than use
        // our Window abstraction.)

        for (size_t j = 0; j < beatlen; ++j) {
            double mul = 0.5 * (1.0 - cos(TWO_PI * (double(j) / double(beatlen))));
            m_beatframe[j] = audio[beatstart + j] * mul;
        }

        for (size_t j = beatlen; j < m_beatframesize; ++j) {
            m_beatframe[j] = 0.0;
        }

        // Now FFT beat frame
        
        FFT::process(m_beatframesize, false,
                     m_beatframe, 0, m_fftRealOut, m_fftImagOut);
        
        // Calculate magnitudes

        for (size_t j = 0; j < m_beatframesize/2; ++j) {
            newspec[j] = sqrt(m_fftRealOut[j] * m_fftRealOut[j] +
                              m_fftImagOut[j] * m_fftImagOut[j]);
        }

        // Preserve peaks by applying adaptive threshold

        MathUtilities::adaptiveThreshold(newspec);

        // Calculate JS divergence between new and old spectral frames

        specdiff.push_back(measureSpecDiff(oldspec, newspec));

        // Copy newspec across to old

        for (size_t j = 0; j < m_beatframesize/2; ++j) {
            oldspec[j] = newspec[j];
        }
    }

    // We now have all spectral difference measures in specdiff

    uint timesig = 4;   // SHOULD REPLACE THIS WITH A FIND_METER FUNCTION - OR USER PARAMETER
    d_vec_t dbcand(timesig); // downbeat candidates

    // look for beat transition which leads to greatest spectral change
    for (int beat = 0; beat < timesig; ++beat) {
        for (int example = beat; example < specdiff.size(); ++example) {
            dbcand[beat] += (specdiff[example]) / timesig;
        }
    }

    // first downbeat is beat at index of maximum value of dbcand
    int dbind = MathUtilities::getMax(dbcand);

    // remaining downbeats are at timesig intervals from the first
    for (int i = dbind; i < beats.size(); i += timesig) {
        downbeats.push_back(i);
    }
}

double
DownBeat::measureSpecDiff(d_vec_t oldspec, d_vec_t newspec)
{
    // JENSEN-SHANNON DIVERGENCE BETWEEN SPECTRAL FRAMES

    uint SPECSIZE = 512;   // ONLY LOOK AT FIRST 512 SAMPLES OF SPECTRUM. 
    if (SPECSIZE > oldspec.size()/4) {
        SPECSIZE = oldspec.size()/4;
    }
    double SD = 0.;
    double sd1 = 0.;

    double sumnew = 0.;
    double sumold = 0.;
  
    for (uint i = 0;i < SPECSIZE;i++)
    {
        newspec[i] +=EPS;
        oldspec[i] +=EPS;
        
        sumnew+=newspec[i];
        sumold+=oldspec[i];
    } 
    
    for (uint i = 0;i < SPECSIZE;i++)
    {
        newspec[i] /= (sumnew);
        oldspec[i] /= (sumold);
        
        // IF ANY SPECTRAL VALUES ARE 0 (SHOULDN'T BE ANY!) SET THEM TO 1
        if (newspec[i] == 0)
        {
            newspec[i] = 1.;
        }
        
        if (oldspec[i] == 0)
        {
            oldspec[i] = 1.;
        }
        
        // JENSEN-SHANNON CALCULATION
        sd1 = 0.5*oldspec[i] + 0.5*newspec[i];	
        SD = SD + (-sd1*log(sd1)) + (0.5*(oldspec[i]*log(oldspec[i]))) + (0.5*(newspec[i]*log(newspec[i])));
    }
    
    return SD;
}

