#ifndef _LIB_MFCC_H
#define _LIB_MFCC_H

#define MFCC    6

typedef struct mfcc_t {
	
	/* Filter bank parameters */
	double  lowestFrequency; 
	int     linearFilters; 
	double  linearSpacing;
	int     logFilters;
	double  logSpacing;
	
	/* FFT length */
	int     fftSize;
	
	/* Analysis window length*/
	int     windowSize;
	
	int     totalFilters;
	
	/* Misc. */
	int     samplingRate;
	int     nceps;
	
	/* MFCC vector */
	double  *ceps;
	
	double  **mfccDCTMatrix;
	double  **mfccFilterWeights;
	
	/* The analysis window */
	double  *window;
	
	/* For the FFT */
	double* imagIn;		// always zero
	double* realOut;
	double* imagOut;
	
	/* Set if user want C0 */
	int WANT_C0;
	
} mfcc_t;

extern mfcc_t* init_mfcc(int fftSize, int nceps , int samplingRate, int WANT_C0);
extern int     do_mfcc(mfcc_t* mfcc_p, double* frame, int length);
extern void    close_mfcc(mfcc_t* mfcc_p);

#endif

