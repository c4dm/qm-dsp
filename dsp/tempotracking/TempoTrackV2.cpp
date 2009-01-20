/* -*- c-basic-offset: 4 indent-tabs-mode: nil -*-  vi:set ts=8 sts=4 sw=4: */

/*
    QM DSP Library

    Centre for Digital Music, Queen Mary, University of London.
    This file copyright 2008-2009 Matthew Davies and QMUL.
    All rights reserved.
*/

#include "TempoTrackV2.h"

#include <cmath>
#include <cstdlib>


//#define		FRAMESIZE	512
//#define		BIGFRAMESIZE	1024
#define   TWOPI 6.283185307179586232
#define   EPS 0.0000008 // just some arbitrary small number

TempoTrackV2::TempoTrackV2() { }
TempoTrackV2::~TempoTrackV2() { }

void
TempoTrackV2::adapt_thresh(d_vec_t &df)
{

  d_vec_t smoothed(df.size());
	
	int p_post = 7;
	int p_pre = 8;

	int t = std::min(static_cast<int>(df.size()),p_post);	// what is smaller, p_post of df size. This is to avoid accessing outside of arrays

	// find threshold for first 't' samples, where a full average cannot be computed yet 
	for (int i = 0;i <= t;i++)
	{	
            int k = std::min((i+p_pre),static_cast<int>(df.size()));
		smoothed[i] = mean_array(df,1,k);
	}
	// find threshold for bulk of samples across a moving average from [i-p_pre,i+p_post]
	for (uint i = t+1;i < df.size()-p_post;i++)
	{
		smoothed[i] = mean_array(df,i-p_pre,i+p_post);
	}
	// for last few samples calculate threshold, again, not enough samples to do as above
	for (uint i = df.size()-p_post;i < df.size();i++)
	{
            int k = std::max((static_cast<int> (i) -p_post),1);
		smoothed[i] = mean_array(df,k,df.size());
	}

	// subtract the threshold from the detection function and check that it is not less than 0
	for (uint i = 0;i < df.size();i++)
	{
		df[i] -= smoothed[i];
		if (df[i] < 0)
		{
			df[i] = 0;
		}
	}
}

double
TempoTrackV2::mean_array(const d_vec_t &dfin,int start,int end)
{

	double sum = 0.;
	
	// find sum
	for (int i = start;i < end+1;i++)
	{
		sum += dfin[i];
	}

	return static_cast<double> (sum / (end - start + 1) );	// average and return
}

void
TempoTrackV2::filter_df(d_vec_t &df)
{


  d_vec_t a(3);
  d_vec_t b(3);
  d_vec_t	lp_df(df.size());

  //equivalent in matlab to [b,a] = butter(2,0.4);
	a[0] = 1.0000;
	a[1] = -0.3695;
	a[2] = 0.1958;
	b[0] = 0.2066;
	b[1] = 0.4131;
	b[2] = 0.2066;
	
  double inp1 = 0.;
  double inp2 = 0.;
  double out1 = 0.;
  double out2 = 0.;


  // forwards filtering
	for (uint i = 0;i < df.size();i++)
	{
    lp_df[i] =  b[0]*df[i] + b[1]*inp1 + b[2]*inp2 - a[1]*out1 - a[2]*out2;
    inp2 = inp1;
    inp1 = df[i];
    out2 = out1;
    out1 = lp_df[i];
	}


  // copy forwards filtering to df...
  // but, time-reversed, ready for backwards filtering
	for (uint i = 0;i < df.size();i++)
	{
    df[i] = lp_df[df.size()-i];    
  }

	for (uint i = 0;i < df.size();i++)
	{
    lp_df[i] = 0.;    
  }

  inp1 = 0.; inp2 = 0.;
  out1 = 0.; out2 = 0.;

  // backwards filetering on time-reversed df
	for (uint i = 0;i < df.size();i++)
	{
    lp_df[i] =  b[0]*df[i] + b[1]*inp1 + b[2]*inp2 - a[1]*out1 - a[2]*out2;
    inp2 = inp1;
    inp1 = df[i];
    out2 = out1;
    out1 = lp_df[i];
	}

  // write the re-reversed (i.e. forward) version back to df
	for (uint i = 0;i < df.size();i++)
	{
    df[i] = lp_df[df.size()-i];    
  }


}


void
TempoTrackV2::calculateBeatPeriod(const d_vec_t &df, d_vec_t &beat_period)
{

// to follow matlab.. split into 512 sample frames with a 128 hop size
// calculate the acf,
// then the rcf.. and then stick the rcfs as columns of a matrix
// then call viterbi decoding with weight vector and transition matrix
// and get best path

  uint wv_len = 128;
  double rayparam = 43.;

  // make rayleigh weighting curve
  d_vec_t wv(wv_len);
  for (uint i=0; i<wv.size(); i++)
  {
    wv[i] = (static_cast<double> (i) / pow(rayparam,2.)) * exp((-1.*pow(-static_cast<double> (i),2.)) / (2.*pow(rayparam,2.)));
  }


  uint winlen = 512;
  uint step = 128;

  d_mat_t rcfmat;
  int col_counter = -1;
  // main loop for beat period calculation
  for (uint i=0; i<(df.size()-winlen); i+=step)
  {
    // get dfframe
    d_vec_t dfframe(winlen);
    for (uint k=0; k<winlen; k++)
    {
      dfframe[k] = df[i+k];
    }
    // get rcf vector for current frame
    d_vec_t rcf(wv_len);    
    get_rcf(dfframe,wv,rcf);
  
    rcfmat.push_back( d_vec_t() ); // adds a new column
    col_counter++;
    for (uint j=0; j<rcf.size(); j++)
    {
      rcfmat[col_counter].push_back( rcf[j] );
    }

  }
  
  // now call viterbi decoding function
  viterbi_decode(rcfmat,wv,beat_period);



}


void
TempoTrackV2::get_rcf(const d_vec_t &dfframe_in, const d_vec_t &wv, d_vec_t &rcf)
{
  // calculate autocorrelation function
  // then rcf
  // just hard code for now... don't really need separate functions to do this

  // make acf

  d_vec_t dfframe(dfframe_in);

  adapt_thresh(dfframe);

  d_vec_t acf(dfframe.size());


  for (uint lag=0; lag<dfframe.size(); lag++)
  {

    double sum = 0.;
    double tmp = 0.;

    for (uint n=0; n<(dfframe.size()-lag); n++)
    {
      tmp = dfframe[n] * dfframe[n+lag];    
      sum += tmp;
    }
    acf[lag] = static_cast<double> (sum/ (dfframe.size()-lag));
  }


//  for (uint i=0; i<dfframe.size(); i++)
//  {
//    cout << dfframe[i] << " " << acf[i]  << endl;
//  }

//    cout << "~~~~~~~~~~~~~~" << endl;





  // now apply comb filtering
	int numelem = 4;
	
//	for (uint i = 1;i < 118;i++) // max beat period
	for (uint i = 2;i < rcf.size();i++) // max beat period
	{
		for (int a = 1;a <= numelem;a++) // number of comb elements
		{
			for (int b = 1-a;b <= a-1;b++) // general state using normalisation of comb elements
			{
				rcf[i-1] += ( acf[(a*i+b)-1]*wv[i-1] ) / (2.*a-1.);	// calculate value for comb filter row
			}
		}
	}
  
  // apply adaptive threshold to rcf
  adapt_thresh(rcf);
  
  double rcfsum =0.;
  for (uint i=0; i<rcf.size(); i++)
  {
 //  rcf[i] *= acf[i];
    rcf[i] += EPS ;
    rcfsum += rcf[i];
  }

  // normalise rcf to sum to unity
  for (uint i=0; i<rcf.size(); i++)
  {
    rcf[i] /= (rcfsum + EPS);
  }



}

void
TempoTrackV2::viterbi_decode(const d_mat_t &rcfmat, const d_vec_t &wv, d_vec_t &beat_period)
{

  // make transition matrix
	d_mat_t tmat;
	for (uint i=0;i<wv.size();i++)
	{
		tmat.push_back ( d_vec_t() ); // adds a new column
		for (uint j=0; j<wv.size(); j++)
		{	
			tmat[i].push_back(0.); // fill with zeros initially
		}
	}

  double sigma = 8.;
	for (uint i=20;i <wv.size()-20; i++)
	{
		for (uint j=20; j<wv.size()-20; j++)
		{	
      double mu = static_cast<double>(i);
			tmat[i][j] = exp( (-1.*pow((j-mu),2.)) / (2.*pow(sigma,2.)) );
		}
	}

  d_mat_t delta;
  i_mat_t psi;
	for (uint i=0;i <rcfmat.size(); i++)
	{
    delta.push_back( d_vec_t());
    psi.push_back( i_vec_t());
		for (uint j=0; j<rcfmat[i].size(); j++)
		{	
			delta[i].push_back(0.); // fill with zeros initially
			psi[i].push_back(0); // fill with zeros initially
		}
	}


  uint T = delta.size();
  uint Q = delta[0].size();

  // initialize first column of delta
  for (uint j=0; j<Q; j++)
  {
    delta[0][j] = wv[j] * rcfmat[0][j];
    psi[0][j] = 0;
  }

  double deltasum = 0.;
  for (uint i=0; i<Q; i++)
  {
    deltasum += delta[0][i];
  }      
  for (uint i=0; i<Q; i++)
  {
    delta[0][i] /= (deltasum + EPS);
  }      



  for (uint t=1; t<T; t++)
  {
    d_vec_t tmp_vec(Q);

    for (uint j=0; j<Q; j++)
    {

      for (uint i=0; i<Q; i++)
      {
        tmp_vec[i] = delta[t-1][i] * tmat[j][i];
      }      
   
      delta[t][j] = get_max_val(tmp_vec);    

      psi[t][j] = get_max_ind(tmp_vec);
 
      delta[t][j] *= rcfmat[t][j];


    }

    double deltasum = 0.;
    for (uint i=0; i<Q; i++)
    {
      deltasum += delta[t][i];
    }      
    for (uint i=0; i<Q; i++)
    {
      delta[t][i] /= (deltasum + EPS);
    }      




  }


//  ofstream tmatfile;
//  tmatfile.open("/home/matthewd/Desktop/tmat.txt");

// 	for (uint i=0;i <delta.size(); i++)
//	{
//		for (uint j=0; j<delta[i].size(); j++)
//		{	
//      tmatfile << rcfmat[i][j] << endl;
//		}
//	}

//  tmatfile.close();

  i_vec_t bestpath(T);
  d_vec_t tmp_vec(Q);
  for (uint i=0; i<Q; i++)
  {  
    tmp_vec[i] = delta[T-1][i];
  }

  
  bestpath[T-1] = get_max_ind(tmp_vec);
 
  for (uint t=T-2; t>0 ;t--)
  {
    bestpath[t] = psi[t+1][bestpath[t+1]];
  }
  // very weird hack!
  bestpath[0] = psi[1][bestpath[1]];

//  for (uint i=0; i<bestpath.size(); i++)
//  {
//    cout << bestpath[i] << endl;
//  }


  uint lastind = 0;
  for (uint i=0; i<T; i++)
  {  
    uint step = 128;
 //   cout << bestpath[i] << " " << i << endl;
    for (uint j=0; j<step; j++)
    {
      lastind = i*step+j;
      beat_period[lastind] = bestpath[i];
      
    }
  }

  //fill in the last values...
  for (uint i=lastind; i<beat_period.size(); i++)
  {
    beat_period[i] = beat_period[lastind];
  }
 


}

double
TempoTrackV2::get_max_val(const d_vec_t &df)
{
  double maxval = 0.;
  for (uint i=0; i<df.size(); i++)
  {

    if (maxval < df[i])
    {
      maxval = df[i];
    }

  }

    
  return maxval;

}

int
TempoTrackV2::get_max_ind(const d_vec_t &df)
{

  double maxval = 0.;
  int ind = 0;
  for (uint i=0; i<df.size(); i++)
  {
    if (maxval < df[i])
    {
      maxval = df[i];
      ind = i;
    }

  }
  
  return ind;

}

void
TempoTrackV2::normalise_vec(d_vec_t &df)
{
  double sum = 0.;
  for (uint i=0; i<df.size(); i++)
  {
    sum += df[i];
  }

  for (uint i=0; i<df.size(); i++)
  {
    df[i]/= (sum + EPS);
  }


}

void
TempoTrackV2::calculateBeats(const d_vec_t &df, const d_vec_t &beat_period,
                             d_vec_t &beats)
{

  d_vec_t cumscore(df.size());
  i_vec_t backlink(df.size());
  d_vec_t localscore(df.size());

  // WHEN I FIGURE OUT HOW, I'LL WANT TO DO SOME FILTERING ON THIS... 
  for (uint i=0; i<df.size(); i++)
  {
    localscore[i] = df[i];
    backlink[i] = -1;
  }

  double tightness = 4.;
  double alpha = 0.9;

  // main loop
  for (uint i=3*beat_period[0]; i<localscore.size(); i++)
  {
    int prange_min = -2*beat_period[i];
    int prange_max = round(-0.5*beat_period[i]);

    d_vec_t txwt (prange_max - prange_min + 1);
    d_vec_t scorecands (txwt.size());

    for (uint j=0;j<txwt.size();j++)
    {
      double mu = static_cast<double> (beat_period[i]);
      txwt[j] = exp( -0.5*pow(tightness * log((round(2*mu)-j)/mu),2));

      scorecands[j] = txwt[j] * cumscore[i+prange_min+j];
    }

    double vv = get_max_val(scorecands);
    int xx = get_max_ind(scorecands);

    cumscore[i] = alpha*vv + (1.-alpha)*localscore[i];

    backlink[i] = i+prange_min+xx;

  }


  d_vec_t tmp_vec;
  for (uint i=cumscore.size() - beat_period[beat_period.size()-1] ; i<cumscore.size(); i++)
  {
    tmp_vec.push_back(cumscore[i]);
  }  

  int startpoint = get_max_ind(tmp_vec) + cumscore.size() - beat_period[beat_period.size()-1] ;

  i_vec_t ibeats;
  ibeats.push_back(startpoint);
  while (backlink[ibeats.back()] > 3*beat_period[0])
  {
    ibeats.push_back(backlink[ibeats.back()]);
  }
  

  for (uint i=0; i<ibeats.size(); i++)
  { 

    beats.push_back( static_cast<double>(ibeats[i]) );

 //   cout << ibeats[i] << " "  << beats[i] <<endl;
  }
}


