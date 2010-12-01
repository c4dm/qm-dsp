/*
 *  Segmenter.cpp
 *
 *  Created by Mark Levy on 04/04/2006.
 *  Copyright 2006 Centre for Digital Music, Queen Mary, University of London. All rights reserved.
 *
 */

#include <iomanip>

#include "Segmenter.h"

ostream& operator<<(ostream& os, const Segmentation& s)
{
	os << "structure_name : begin_time end_time\n";
	
	for (int i = 0; i < s.segments.size(); i++)
	{
		Segment seg = s.segments[i];
		os << std::fixed << seg.type << ':' << '\t' << std::setprecision(6) << seg.start / static_cast<double>(s.samplerate) 
			<< '\t' << std::setprecision(6) << seg.end / static_cast<double>(s.samplerate) << "\n";
	}
	
	return os;
}
