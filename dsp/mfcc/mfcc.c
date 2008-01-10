#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#include "mfcc.h"
#include "SBFFT.h"
#include "windows.h"

/*
 * 
 *  Initialise the MFCC structure and return a pointer to a 
 *  feature vector 
 * 
 */

extern mfcc_t* init_mfcc(int fftSize, int nceps , int samplingRate, int WANT_C0){

  int i,j;
  /* Calculate at startup */
  double *freqs, *lower, *center, *upper, *triangleHeight, *fftFreqs;
  
  /* Allocate space for the structure */
  mfcc_t* mfcc_p = (mfcc_t*)malloc(sizeof(mfcc_t));
  
  mfcc_p->lowestFrequency   = 66.6666666;
  mfcc_p->linearFilters     = 13;
  mfcc_p->linearSpacing     = 66.66666666;
  mfcc_p->logFilters        = 27;
  mfcc_p->logSpacing        = 1.0711703;
  
  /* FFT and analysis window sizes */
  mfcc_p->fftSize           = fftSize;

  mfcc_p->totalFilters      = mfcc_p->linearFilters + mfcc_p->logFilters;
  
  mfcc_p->samplingRate      = samplingRate;
  
  /* The number of cepstral componenents */
  mfcc_p->nceps             = nceps;

  /* Set if user want C0 */
  mfcc_p->WANT_C0           = WANT_C0;
  
  /* Allocate space for feature vector */
  if (mfcc_p->WANT_C0==1) {
    mfcc_p->ceps              = (double*)calloc(nceps+1,sizeof(double));
  } else {
    mfcc_p->ceps              = (double*)calloc(nceps,sizeof(double));
  }
 
  /* Allocate space for local vectors */
  mfcc_p->mfccDCTMatrix     = (double**)calloc(mfcc_p->nceps+1, sizeof(double*));
  for (i=0;i<mfcc_p->nceps+1; i++) {
    mfcc_p->mfccDCTMatrix[i]= (double*)calloc(mfcc_p->totalFilters, sizeof(double)); 
  }

  mfcc_p->mfccFilterWeights = (double**)calloc(mfcc_p->totalFilters, sizeof(double*));
  for (i=0;i<mfcc_p->totalFilters; i++) {
    mfcc_p->mfccFilterWeights[i] = (double*)calloc(mfcc_p->fftSize, sizeof(double)); 
  }

  freqs  = (double*)calloc(mfcc_p->totalFilters+2,sizeof(double));
  
  lower  = (double*)calloc(mfcc_p->totalFilters,sizeof(double));
  center = (double*)calloc(mfcc_p->totalFilters,sizeof(double));
  upper  = (double*)calloc(mfcc_p->totalFilters,sizeof(double));

  triangleHeight = (double*)calloc(mfcc_p->totalFilters,sizeof(double));
  fftFreqs       = (double*)calloc(mfcc_p->fftSize,sizeof(double));
  
  for (i=0;i<mfcc_p->linearFilters;i++) {
    freqs[i] = mfcc_p->lowestFrequency + ((double)i) * mfcc_p->linearSpacing;
  }
  
  for (i=mfcc_p->linearFilters; i<mfcc_p->totalFilters+2; i++) {
    freqs[i] = freqs[mfcc_p->linearFilters-1] * 
               pow(mfcc_p->logSpacing, (double)(i-mfcc_p->linearFilters+1));
  }
  
  /* Define lower, center and upper */
  memcpy(lower,  freqs,mfcc_p->totalFilters*sizeof(double));
  memcpy(center, &freqs[1],mfcc_p->totalFilters*sizeof(double));
  memcpy(upper,  &freqs[2],mfcc_p->totalFilters*sizeof(double));
    
  for (i=0;i<mfcc_p->totalFilters;i++){
      triangleHeight[i] = 2./(upper[i]-lower[i]);
  }
  
  for (i=0;i<mfcc_p->fftSize;i++){
      fftFreqs[i] = ((double) i / ((double) mfcc_p->fftSize ) * 
                     (double) mfcc_p->samplingRate);
  }

  /* Build now the mccFilterWeight matrix */
  for (i=0;i<mfcc_p->totalFilters;i++){

    for (j=0;j<mfcc_p->fftSize;j++) {
      
        if ((fftFreqs[j] > lower[i]) && (fftFreqs[j] <= center[i])) {
          
          mfcc_p->mfccFilterWeights[i][j] = triangleHeight[i] * 
                                            (fftFreqs[j]-lower[i]) / (center[i]-lower[i]); 
          
        }
        else
        {
          
          mfcc_p->mfccFilterWeights[i][j] = 0.0;
        
        }

        if ((fftFreqs[j]>center[i]) && (fftFreqs[j]<upper[i])) {

          mfcc_p->mfccFilterWeights[i][j] = mfcc_p->mfccFilterWeights[i][j] + triangleHeight[i] * (upper[i]-fftFreqs[j]) 
                                            / (upper[i]-center[i]);
        }
        else
        {

          mfcc_p->mfccFilterWeights[i][j] = mfcc_p->mfccFilterWeights[i][j] + 0.0;
            
        }
    }

  }

#ifndef PI    
#define PI 3.14159265358979323846264338327950288
#endif 

  /*
   * 
   * We calculate now mfccDCT matrix 
   * NB: +1 because of the DC component
   * 
   */
  
  for (i=0; i<nceps+1; i++) {
    for (j=0; j<mfcc_p->totalFilters; j++) {
        mfcc_p->mfccDCTMatrix[i][j] = (1./sqrt((double) mfcc_p->totalFilters / 2.))  
                                      * cos((double) i * ((double) j + 0.5) / (double) mfcc_p->totalFilters * PI);
    }
  }

  for (j=0;j<mfcc_p->totalFilters;j++){
    mfcc_p->mfccDCTMatrix[0][j]     =  (sqrt(2.)/2.) * mfcc_p->mfccDCTMatrix[0][j];
  }
   
  /* The analysis window */
  mfcc_p->window = hamming(mfcc_p->fftSize);

  /* Allocate memory for the FFT */
  mfcc_p->imagIn      = (double*)calloc(mfcc_p->fftSize,sizeof(double));
  mfcc_p->realOut     = (double*)calloc(mfcc_p->fftSize,sizeof(double));
  mfcc_p->imagOut     = (double*)calloc(mfcc_p->fftSize,sizeof(double));
  
  free(freqs);
  free(lower);
  free(center);
  free(upper);
  free(triangleHeight);
  free(fftFreqs);

  return mfcc_p;

}

/*
 *
 *  Free the memory that has been allocated 
 *
 */ 

extern void close_mfcc(mfcc_t* mfcc_p) {

  int i;
  
  /* Free the structure */
  for (i=0;i<mfcc_p->nceps+1;i++) {
    free(mfcc_p->mfccDCTMatrix[i]);
  }
  free(mfcc_p->mfccDCTMatrix);

  for (i=0;i<mfcc_p->totalFilters; i++) {
    free(mfcc_p->mfccFilterWeights[i]);
  }
  free(mfcc_p->mfccFilterWeights);

  /* Free the feature vector */
  free(mfcc_p->ceps);

  /* The analysis window */
  free(mfcc_p->window);

  /* Free the FFT */
  free(mfcc_p->imagIn);
  free(mfcc_p->realOut);
  free(mfcc_p->imagOut);

  /* Free the structure itself */
  free(mfcc_p);
  mfcc_p = NULL;

}

/*
 * 
 * Extract the MFCC on the input frame 
 * 
 */ 


// looks like we have to have length = mfcc_p->fftSize ??????

extern int do_mfcc(mfcc_t* mfcc_p, double* frame, int length){

  int i,j;
  
  double *fftMag;
  double *earMag;

  double *inputData;
  
  double tmp;

  earMag    = (double*)calloc(mfcc_p->totalFilters, sizeof(double));
  inputData = (double*)calloc(mfcc_p->fftSize, sizeof(double)); 
  
  /* Zero-pad if needed */
  memcpy(inputData, frame, length*sizeof(double));
  
  /* Calculate the fft on the input frame */
  fft_process(mfcc_p->fftSize, 0, inputData, mfcc_p->imagIn, mfcc_p->realOut, mfcc_p->imagOut);

  /* Get the magnitude */
  fftMag = abs_fft(mfcc_p->realOut, mfcc_p->imagOut, mfcc_p->fftSize);

  /* Multiply by mfccFilterWeights */
  for (i=0;i<mfcc_p->totalFilters;i++) {
    tmp = 0.;
    for(j=0;j<mfcc_p->fftSize/2; j++) {
      tmp = tmp + (mfcc_p->mfccFilterWeights[i][j]*fftMag[j]);
    }
    if (tmp>0)
		earMag[i] = log10(tmp);
	else
		earMag[i] = 0.0;
  }

  /*
   * 
   * Calculate now the ceptral coefficients 
   * with or without the DC component
   *
   */
  
   if (mfcc_p->WANT_C0==1) {
     
    for (i=0;i<mfcc_p->nceps+1;i++) {
      tmp = 0.;
      for (j=0;j<mfcc_p->totalFilters;j++){
        tmp = tmp + mfcc_p->mfccDCTMatrix[i][j]*earMag[j];
      }
      /* Send to workspace */
      mfcc_p->ceps[i] = tmp;
    }

   }
    else 
   {  
      for (i=1;i<mfcc_p->nceps+1;i++) {
        tmp = 0.;
        for (j=0;j<mfcc_p->totalFilters;j++){
          tmp = tmp + mfcc_p->mfccDCTMatrix[i][j]*earMag[j];
        }
        /* Send to workspace */
        mfcc_p->ceps[i-1] = tmp;
      }
  }
    
  free(fftMag);
  free(earMag);
  free(inputData);

  return mfcc_p->nceps;

}




