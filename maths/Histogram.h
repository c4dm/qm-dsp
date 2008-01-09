/* -*- c-basic-offset: 4 indent-tabs-mode: nil -*-  vi:set ts=8 sts=4 sw=4: */

// Histogram.h: interface for the THistogram class.
//
//////////////////////////////////////////////////////////////////////


#ifndef HISTOGRAM_H
#define HISTOGRAM_H


#include <valarray>

/*! \brief A histogram class

  This class computes the histogram of a vector.

\par Template parameters
	
	- T type of input data (can be any: float, double, int, UINT, etc...)
	- TOut type of output data: float or double. (default is double)

\par Moments:

	The moments (average, standard deviation, skewness, etc.) are computed using 
the algorithm of the Numerical recipies (see Numerical recipies in C, Chapter 14.1, pg 613).

\par Example:

	This example shows the typical use of the class:
\code
	// a vector containing the data
	vector<float> data;
	// Creating histogram using float data and with 101 containers,
	THistogram<float> histo(101);
	// computing the histogram
	histo.compute(data);
\endcode

Once this is done, you can get a vector with the histogram or the normalized histogram (such that it's area is 1):
\code
	// getting normalized histogram
	vector<float> v=histo.getNormalizedHistogram();
\endcode

\par Reference
	
	Equally spaced acsissa function integration (used in #GetArea): Numerical Recipies in C, Chapter 4.1, pg 130.

\author Jonathan de Halleux, dehalleux@auto.ucl.ac.be, 2002
*/

template<class T, class TOut = double>
class THistogram  
{
public:
    //! \name Constructors
    //@{
    /*! Default constructor
      \param counters the number of histogram containers (default value is 10)
    */
    THistogram(unsigned int counters = 10);
    virtual ~THistogram()				{ clear();};
    //@}

    //! \name Histogram computation, update
    //@{
    /*! Computes histogram of vector v
      \param v vector to compute histogram
      \param computeMinMax set to true if min/max of v have to be used to get the histogram limits	
	
      This function computes the histogram of v and stores it internally.
      \sa Update, GeTHistogram
    */
    void compute( const std::vector<T>& v, bool computeMinMax = true);
    //! Update histogram with the vector v
    void update( const std::vector<T>& v);
    //! Update histogram with t
    void update( const T& t);
    //@}

    //! \name Resetting functions
    //@{
    //! Resize the histogram. Warning this function clear the histogram.
    void resize( unsigned int counters );
    //! Clears the histogram
    void clear()						{ m_counters.clear();};
    //@}

    //! \name Setters
    //@{
    /*! This function sets the minimum of the histogram spectrum. 
      The spectrum is not recomputed, use it with care
    */
    void setMinSpectrum( const T& min )					{	m_min = min; computeStep();};
    /*! This function sets the minimum of the histogram spectrum. 
      The spectrum is not recomputed, use it with care
    */
    void setMaxSpectrum( const T& max )					{	m_max = max; computeStep();};
    //@}
    //! \name Getters
    //@{
    //! return minimum of histogram spectrum
    const T& getMinSpectrum() const							{	return m_min;};
    //! return maximum of histogram spectrum
    const T& getMaxSpectrum() const							{	return m_max;};
    //! return step size of histogram containers
    TOut getStep() const									{	return m_step;};
    //! return number of points in histogram
    unsigned int getSum() const;
    /*! \brief returns area under the histogram 

    The Simpson rule is used to integrate the histogram.
    */
    TOut getArea() const;

    /*! \brief Computes the moments of the histogram

    \param data dataset
    \param ave mean
    \f[ \bar x = \frac{1}{N} \sum_{j=1}^N x_j\f]
    \param adev mean absolute deviation
    \f[ adev(X) = \frac{1}{N} \sum_{j=1}^N | x_j - \bar x |\f]
    \param var average deviation:
    \f[ \mbox{Var}(X) = \frac{1}{N-1} \sum_{j=1}^N (x_j - \bar x)^2\f]
    \param sdev standard deviation:
    \f[ \sigma(X) = \sqrt{var(\bar x) }\f]
    \param skew skewness
    \f[ \mbox{Skew}(X) = \frac{1}{N}\sum_{j=1}^N \left[ \frac{x_j - \bar x}{\sigma}\right]^3\f]
    \param kurt kurtosis
    \f[ \mbox{Kurt}(X) = \left\{ \frac{1}{N}\sum_{j=1}^N \left[ \frac{x_j - \bar x}{\sigma}\right]^4 \right\} - 3\f]

    */
    static void getMoments(const std::vector<T>& data, TOut& ave, TOut& adev, TOut& sdev, TOut& var, TOut& skew, TOut& kurt);

    //! return number of containers
    unsigned int getSize() const							{	return m_counters.size();};
    //! returns i-th counter
    unsigned int operator [] (unsigned int i) const {
//	ASSERT( i < m_counters.size() );
        return m_counters[i];
    };
    //! return the computed histogram
    const std::vector<unsigned int>& geTHistogram() const	{	return m_counters;};
    //! return the computed histogram, in TOuts
    std::vector<TOut> geTHistogramD() const;
    /*! return the normalized computed histogram 

    \return the histogram such that the area is equal to 1
    */
    std::vector<TOut> getNormalizedHistogram() const;
    //! returns left containers position
    std::vector<TOut> getLeftContainers() const;
    //! returns center containers position
    std::vector<TOut> getCenterContainers() const;
    //@}
protected:
    //! Computes the step
    void computeStep()								{	m_step = (TOut)(((TOut)(m_max-m_min)) / (m_counters.size()-1));};
    //! Data accumulators
    std::vector<unsigned int> m_counters;
    //! minimum of dataset
    T m_min;
    //! maximum of dataset
    T m_max;
    //! width of container
    TOut m_step;
};

template<class T, class TOut>
THistogram<T,TOut>::THistogram(unsigned int counters)
    : m_counters(counters,0), m_min(0), m_max(0), m_step(0)
{

}

template<class T, class TOut>
void THistogram<T,TOut>::resize( unsigned int counters )
{
    clear();

    m_counters.resize(counters,0);

    computeStep();
}

template<class T, class TOut>
void THistogram<T,TOut>::compute( const std::vector<T>& v, bool computeMinMax)
{
    using namespace std;
    unsigned int i;
    int index;

    if (m_counters.empty())
	return;

    if (computeMinMax)
    {
	m_max = m_min = v[0];
	for (i=1;i<v.size();i++)
	{
	    m_max = std::max( m_max, v[i]);
	    m_min = std::min( m_min, v[i]);
	}
    }

    computeStep();

    for (i = 0;i < v.size() ; i++)
    {
	index=(int) floor( ((TOut)(v[i]-m_min))/m_step ) ;

	if (index >= m_counters.size() || index < 0)
	    return;

	m_counters[index]++;
    }
}

template<class T, class TOut>
void THistogram<T,TOut>::update( const std::vector<T>& v)
{
    if (m_counters.empty())
	return;

    computeStep();

    TOut size = m_counters.size();

    int index;
    for (unsigned int i = 0;i < size ; i++)
    {
	index = (int)floor(((TOut)(v[i]-m_min))/m_step);

	if (index >= m_counters.size() || index < 0)
	    return;

	m_counters[index]++;
    }
}

template<class T, class TOut>
void THistogram<T,TOut>::update( const T& t)
{	
    int index=(int) floor( ((TOut)(t-m_min))/m_step ) ;

    if (index >= m_counters.size() || index < 0)
	return;

    m_counters[index]++;
};

template<class T, class TOut>
std::vector<TOut> THistogram<T,TOut>::geTHistogramD() const
{
    std::vector<TOut> v(m_counters.size());
    for (unsigned int i = 0;i<m_counters.size(); i++)
	v[i]=(TOut)m_counters[i];

    return v;
}

template <class T, class TOut>
std::vector<TOut> THistogram<T,TOut>::getLeftContainers() const
{
    std::vector<TOut> x( m_counters.size());

    for (unsigned int i = 0;i<m_counters.size(); i++)
	x[i]= m_min + i*m_step;

    return x;
}

template <class T, class TOut>
std::vector<TOut> THistogram<T,TOut>::getCenterContainers() const
{
    std::vector<TOut> x( m_counters.size());

    for (unsigned int i = 0;i<m_counters.size(); i++)
	x[i]= m_min + (i+0.5)*m_step;

    return x;
}

template <class T, class TOut>
unsigned int THistogram<T,TOut>::getSum() const
{
    unsigned int sum = 0;
    for (unsigned int i = 0;i<m_counters.size(); i++)
	sum+=m_counters[i];

    return sum;
}

template <class T, class TOut>
TOut THistogram<T,TOut>::getArea() const
{
    const size_t n=m_counters.size();
    TOut area=0;

    if (n>6)
    {
	area=3.0/8.0*(m_counters[0]+m_counters[n-1])
	    +7.0/6.0*(m_counters[1]+m_counters[n-2])
	    +23.0/24.0*(m_counters[2]+m_counters[n-3]);
	for (unsigned int i=3;i<n-3;i++)
	{
	    area+=m_counters[i];
	}
    }
    else if (n>4)
    {
	area=5.0/12.0*(m_counters[0]+m_counters[n-1])
	    +13.0/12.0*(m_counters[1]+m_counters[n-2]);
	for (unsigned int i=2;i<n-2;i++)
	{
	    area+=m_counters[i];
	}
    }
    else if (n>1)
    {
	area=1/2.0*(m_counters[0]+m_counters[n-1]);
	for (unsigned int i=1;i<n-1;i++)
	{
	    area+=m_counters[i];
	}
    }
    else 
	area=0;

    return area*m_step;
}

template <class T, class TOut>
std::vector<TOut> THistogram<T,TOut>::getNormalizedHistogram() const
{
    std::vector<TOut> normCounters( m_counters.size());
    TOut area = (TOut)getArea();

    for (unsigned int i = 0;i<m_counters.size(); i++)
    {
	normCounters[i]= (TOut)m_counters[i]/area;
    }

    return normCounters;
};

template <class T, class TOut>
void THistogram<T,TOut>::getMoments(const std::vector<T>& data, TOut& ave, TOut& adev, TOut& sdev, TOut& var, TOut& skew, TOut& kurt)
{
    int j;
    double ep=0.0,s,p;
    const size_t n = data.size();

    if (n <= 1)
	// nrerror("n must be at least 2 in moment");
	return;

    s=0.0; // First pass to get the mean.
    for (j=0;j<n;j++)
	s += data[j];
	
    ave=s/(n);
    adev=var=skew=kurt=0.0; 
    /* Second pass to get the first (absolute), second,
       third, and fourth moments of the
       deviation from the mean. */

    for (j=0;j<n;j++) 
    {
	adev += fabs(s=data[j]-(ave));
	ep += s;
	var += (p=s*s);
	skew += (p *= s);
	kurt += (p *= s);
    }


    adev /= n;
    var=(var-ep*ep/n)/(n-1); // Corrected two-pass formula.
    sdev=sqrt(var); // Put the pieces together according to the conventional definitions. 
    if (var) 
    {
	skew /= (n*(var)*(sdev));
	kurt=(kurt)/(n*(var)*(var))-3.0;
    } 
    else
	//nrerror("No skew/kurtosis when variance = 0 (in moment)");
	return;
}

#endif

