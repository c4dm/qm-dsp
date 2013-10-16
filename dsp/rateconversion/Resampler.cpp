/* -*- c-basic-offset: 4 indent-tabs-mode: nil -*-  vi:set ts=8 sts=4 sw=4: */

#include "Resampler.h"

#include "qm-dsp/maths/MathUtilities.h"
#include "qm-dsp/base/KaiserWindow.h"
#include "qm-dsp/base/SincWindow.h"

#include <iostream>
#include <vector>
#include <map>

using std::vector;
using std::map;

//#define DEBUG_RESAMPLER 1

Resampler::Resampler(int sourceRate, int targetRate) :
    m_sourceRate(sourceRate),
    m_targetRate(targetRate)
{
    initialise();
}

Resampler::~Resampler()
{
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

    vector<double> filter(m_filterLength, 0.0);
    for (int i = 0; i < m_filterLength; ++i) filter[i] = 1.0;
    sw.cut(filter.data());
    kw.cut(filter.data());

    int inputSpacing = m_targetRate / m_gcd;
    int outputSpacing = m_sourceRate / m_gcd;

#ifdef DEBUG_RESAMPLER
    std::cerr << "resample " << m_sourceRate << " -> " << m_targetRate
	      << ": inputSpacing " << inputSpacing << ", outputSpacing "
	      << outputSpacing << ": filter length " << m_filterLength
	      << std::endl;
#endif

    m_phaseData = new Phase[inputSpacing];

    for (int phase = 0; phase < inputSpacing; ++phase) {

	Phase p;

	p.nextPhase = phase - outputSpacing;
	while (p.nextPhase < 0) p.nextPhase += inputSpacing;
	p.nextPhase %= inputSpacing;
	
	p.drop = int(ceil(std::max(0.0, double(outputSpacing - phase))
			  / inputSpacing));

	int filtZipLength = int(ceil(double(m_filterLength - phase)
				     / inputSpacing));
	for (int i = 0; i < filtZipLength; ++i) {
	    p.filter.push_back(filter[i * inputSpacing + phase]);
	}

	m_phaseData[phase] = p;
    }

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

    m_phase = (m_filterLength/2) % inputSpacing;
    
    m_buffer = vector<double>(m_phaseData[0].filter.size(), 0);
    m_bufferOrigin = 0;

    m_latency =
	((m_buffer.size() * inputSpacing) - (m_filterLength/2)) / outputSpacing
	+ m_phase;

#ifdef DEBUG_RESAMPLER
    std::cerr << "initial phase " << m_phase << " (as " << (m_filterLength/2) << " % " << inputSpacing << ")"
	      << ", latency " << m_latency << std::endl;
#endif
}

double
Resampler::reconstructOne()
{
    Phase &pd = m_phaseData[m_phase];
    double v = 0.0;
    int n = pd.filter.size();
    const double *const __restrict__ buf = m_buffer.data() + m_bufferOrigin;
    const double *const __restrict__ filt = pd.filter.data();
    for (int i = 0; i < n; ++i) {
	// NB gcc can only vectorize this with -ffast-math
	v += buf[i] * filt[i];
    }
    m_bufferOrigin += pd.drop;
    m_phase = pd.nextPhase;
    return v;
}

int
Resampler::process(const double *src, double *dst, int n)
{
    for (int i = 0; i < n; ++i) {
	m_buffer.push_back(src[i]);
    }

    int maxout = int(ceil(double(n) * m_targetRate / m_sourceRate));
    int outidx = 0;

#ifdef DEBUG_RESAMPLER
    std::cerr << "process: buf siz " << m_buffer.size() << " filt siz for phase " << m_phase << " " << m_phaseData[m_phase].filter.size() << std::endl;
#endif

    double scaleFactor = 1.0;
    if (m_targetRate < m_sourceRate) {
	scaleFactor = double(m_targetRate) / double(m_sourceRate);
    }

    while (outidx < maxout &&
	   m_buffer.size() >= m_phaseData[m_phase].filter.size() + m_bufferOrigin) {
	dst[outidx] = scaleFactor * reconstructOne();
	outidx++;
    }

    m_buffer = vector<double>(m_buffer.begin() + m_bufferOrigin, m_buffer.end());
    m_bufferOrigin = 0;
    
    return outidx;
}
    
std::vector<double>
Resampler::resample(int sourceRate, int targetRate, const double *data, int n)
{
    Resampler r(sourceRate, targetRate);

    int latency = r.getLatency();

    // latency is the output latency. We need to provide enough
    // padding input samples at the end of input to guarantee at
    // *least* the latency's worth of output samples. that is,

    int inputPad = int(ceil(double(latency * sourceRate) / targetRate));

    // that means we are providing this much input in total:

    int n1 = n + inputPad;

    // and obtaining this much output in total:

    int m1 = int(ceil(double(n1 * targetRate) / sourceRate));

    // in order to return this much output to the user:

    int m = int(ceil(double(n * targetRate) / sourceRate));
    
//    std::cerr << "n = " << n << ", sourceRate = " << sourceRate << ", targetRate = " << targetRate << ", m = " << m << ", latency = " << latency << ", m1 = " << m1 << ", n1 = " << n1 << ", n1 - n = " << n1 - n << std::endl;

    vector<double> pad(n1 - n, 0.0);
    vector<double> out(m1 + 1, 0.0);

    int got = r.process(data, out.data(), n);
    got += r.process(pad.data(), out.data() + got, pad.size());

#ifdef DEBUG_RESAMPLER
    std::cerr << "resample: " << n << " in, " << got << " out" << std::endl;
    for (int i = 0; i < got; ++i) {
	if (i % 5 == 0) std::cout << std::endl << i << "... ";
	std::cout << (float) out[i] << " ";
    }
    std::cout << std::endl;
#endif

    int toReturn = got - latency;
    if (toReturn > m) toReturn = m;

    return vector<double>(out.begin() + latency, 
			  out.begin() + latency + toReturn);
}

