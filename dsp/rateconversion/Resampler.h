/* -*- c-basic-offset: 4 indent-tabs-mode: nil -*-  vi:set ts=8 sts=4 sw=4: */

#ifndef RESAMPLER_H
#define RESAMPLER_H

#include <vector>

class Resampler
{
public:
    /**
     * Construct a Resampler to resample from sourceRate to
     * targetRate.
     */
    Resampler(int sourceRate, int targetRate);
    virtual ~Resampler();

    /**
     * Read n input samples from src and write resampled data to
     * dst. The return value is the number of samples written, which
     * will be no more than ceil((n * targetRate) / sourceRate). The
     * caller must ensure the dst buffer has enough space for the
     * samples returned.
     */
    int process(const double *src, double *dst, int n);

    /**
     * Return the number of samples of latency at the output due by
     * the filter. (That is, the output will be delayed by this number
     * of samples relative to the input.)
     */
    int getLatency() const { return m_latency; }

    /**
     * Carry out a one-off resample of a single block of n
     * samples. The output is latency-compensated.
     */
    static std::vector<double> resample
    (int sourceRate, int targetRate, const double *data, int n);

private:
    int m_sourceRate;
    int m_targetRate;
    int m_gcd;
    int m_filterLength;
    int m_bufferLength;
    int m_latency;
    
    struct Phase {
        int nextPhase;
        std::vector<double> filter;
        int drop;
    };

    Phase *m_phaseData;
    int m_phase;
    std::vector<double> m_buffer;

    void initialise();
    double reconstructOne();
};

#endif
    
