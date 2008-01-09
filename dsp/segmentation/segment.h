#ifndef _SEGMENT_H
#define _SEGMENT_H

/*
 *  segment.h
 *  soundbite
 *
 *  Created by Mark Levy on 06/04/2006.
 *  Copyright 2006 Centre for Digital Music, Queen Mary, University of London. All rights reserved.
 *
 */

typedef struct segment_t
{
	long start;			/* in samples */
	long end;
	int type;
} segment_t;

typedef struct segmentation_t
{
	int nsegs;			/* number of segments */
	int nsegtypes;		/* number of segment types, so possible types are {0,1,...,nsegtypes-1} */
	int samplerate;
	segment_t* segments;
} segmentation_t;

typedef enum 
{ 
	FEATURE_TYPE_UNKNOWN = 0, 
	FEATURE_TYPE_CONSTQ = 1, 
	FEATURE_TYPE_CHROMA 
} feature_types;

#endif

