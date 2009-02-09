/* -*- c-basic-offset: 4 indent-tabs-mode: nil -*-  vi:set ts=8 sts=4 sw=4: */

/*
    QM DSP Library

    Centre for Digital Music, Queen Mary, University of London.
    This file copyright 2008-2009 Matthew Davies and QMUL.
    All rights reserved.
*/


#ifndef TEMPOTRACKV2_H
#define TEMPOTRACKV2_H

#include <vector>

using std::vector;

class TempoTrackV2  
{
public:
    TempoTrackV2();
    ~TempoTrackV2();

    void calculateBeatPeriod(const vector<double> &df,
                             vector<double> &beatPeriod,
                             vector<double> &tempi);

    void calculateBeats(const vector<double> &df,
                        const vector<double> &beatPeriod,
                        vector<double> &beats);

private:
    typedef vector<int> i_vec_t;
    typedef vector<vector<int> > i_mat_t;
    typedef vector<double> d_vec_t;
    typedef vector<vector<double> > d_mat_t;

    void adapt_thresh(d_vec_t &df);
    double mean_array(const d_vec_t &dfin, int start, int end);
    void filter_df(d_vec_t &df);
    void get_rcf(const d_vec_t &dfframe, const d_vec_t &wv, d_vec_t &rcf);
    void viterbi_decode(const d_mat_t &rcfmat, const d_vec_t &wv,
                        d_vec_t &bp, d_vec_t &tempi);
    double get_max_val(const d_vec_t &df);
    int get_max_ind(const d_vec_t &df);
    void normalise_vec(d_vec_t &df);
};

#endif
