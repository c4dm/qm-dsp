/* -*- c-basic-offset: 4 indent-tabs-mode: nil -*-  vi:set ts=8 sts=4 sw=4: */

#include "Resampler.h"

#include "qm-dsp/maths/MathUtilities.h"
#include "qm-dsp/base/KaiserWindow.h"
#include "qm-dsp/base/SincWindow.h"

#include <iostream>

Resampler::Resampler(int sourceRate, int targetRate) :
    m_sourceRate(sourceRate),
    m_targetRate(targetRate)
{
    initialise();
}

Resampler::~Resampler()
{
    delete[] m_buffer;
    delete[] m_phaseData;
}

void
Resampler::initialise()
{
    int higher = std::max(m_sourceRate, m_targetRate);
    int lower = std::min(m_sourceRate, m_targetRate);

    m_gcd = MathUtilities::gcd(lower, higher);

    int peakToPole = higher / m_gcd;

    KaiserWindow::Parameters params =
	KaiserWindow::parametersForBandwidth(100, 0.02, peakToPole);

    params.length =
	(params.length % 2 == 0 ? params.length + 1 : params.length);
    
    m_filterLength = params.length;
    
    KaiserWindow kw(params);
    SincWindow sw(m_filterLength, peakToPole * 2);

    double *filter = new double[m_filterLength];
    for (int i = 0; i < m_filterLength; ++i) filter[i] = 1.0;
    sw.cut(filter);
    kw.cut(filter);

    int inputSpacing = m_targetRate / m_gcd;
    int outputSpacing = m_sourceRate / m_gcd;

    m_latency = int((m_filterLength / 2) / outputSpacing);

    m_bufferLength = 0;

    m_phaseData = new Phase[inputSpacing];

    for (int phase = 0; phase < inputSpacing; ++phase) {

	Phase p;

	p.nextPhase = phase - outputSpacing;
	while (p.nextPhase < 0) p.nextPhase += inputSpacing;
	p.nextPhase %= inputSpacing;
	
	p.drop = int(ceil(std::max(0, outputSpacing - phase) / inputSpacing));
	p.take = int((outputSpacing +
		      ((m_filterLength - 1 - phase) % inputSpacing))
		     / outputSpacing);

	int filtZipLength = int(ceil((m_filterLength - phase) / inputSpacing));
	if (filtZipLength > m_bufferLength) {
	    m_bufferLength = filtZipLength;
	}
	
	for (int i = 0; i < filtZipLength; ++i) {
	    p.filter.push_back(filter[i * inputSpacing + phase]);
	}

	m_phaseData[phase] = p;
    }

    delete[] filter;

    // The May implementation of this uses a pull model -- we ask the
    // resampler for a certain number of output samples, and it asks
    // its source stream for as many as it needs to calculate
    // those. This means (among other things) that the source stream
    // can be asked for enough samples up-front to fill the buffer
    // before the first output sample is generated.
    // 
    // In this implementation we're using a push model in which a
    // certain number of source samples is provided and we're asked
    // for as many output samples as that makes available. But we
    // can't return any samples from the beginning until half the
    // filter length has been provided as input. This means we must
    // either return a very variable number of samples (none at all
    // until the filter fills, then half the filter length at once) or
    // else have a lengthy declared latency on the output. We do the
    // latter. (What do other implementations do?)

    m_phase = m_filterLength % inputSpacing;
    m_buffer = new double[m_bufferLength];
    for (int i = 0; i < m_bufferLength; ++i) m_buffer[i] = 0.0;
}

double
Resampler::reconstructOne(const double **srcptr)
{
    Phase &pd = m_phaseData[m_phase];
    double *filt = pd.filter.data();
    int n = pd.filter.size();
    double v = 0.0;
    for (int i = 0; i < n; ++i) {
	v += m_buffer[i] * filt[i];
    }
    for (int i = pd.drop; i < n; ++i) {
	m_buffer[i - pd.drop] = m_buffer[i];
    }
    for (int i = 0; i < pd.take; ++i) {
	m_buffer[n - pd.drop + i] = **srcptr;
	++ *srcptr;
    }
    m_phase = pd.nextPhase;
    return v;
}

int
Resampler::process(const double *src, double *dst, int n)
{
    int m = 0;
    const double *srcptr = src;

    while (n > m_phaseData[m_phase].take) {
	std::cerr << "n = " << n << ", m = " << m << ", take = " << m_phaseData[m_phase].take << std::endl;
	n -= m_phaseData[m_phase].take;
	dst[m] = reconstructOne(&srcptr);
	std::cerr << "n -> " << n << std::endl;
	++m;
    }

    //!!! save any excess 

    return m;
}

