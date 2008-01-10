/* -*- c-basic-offset: 4 indent-tabs-mode: nil -*-  vi:set ts=8 sts=4 sw=4: */
/*
    QM DSP Library

    Centre for Digital Music, Queen Mary, University of London.
    This file copyright 2005-2006 Christian Landone.
    All rights reserved.
*/

#ifndef DECIMATOR_H
#define DECIMATOR_H

class Decimator  
{
public:
    void process( const double* src, double* dst );
    void doAntiAlias( const double* src, double* dst, unsigned int length );

    Decimator( unsigned int inLength, unsigned int decFactor );
    virtual ~Decimator();

    int getFactor() const { return m_decFactor; }
    static int getHighestSupportedFactor() { return 8; }

private:
    void resetFilter();
    void deInitialise();
    void initialise( unsigned int inLength, unsigned int decFactor );

    unsigned int m_inputLength;
    unsigned int m_outputLength;
    unsigned int m_decFactor;

    double Input;
    double Output ;

    double o1,o2,o3,o4,o5,o6,o7;

    double a[ 9 ];
    double b[ 9 ];
	
    double* decBuffer;
};

#endif // 
