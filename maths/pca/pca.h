#ifndef _PCA_H
#define _PCA_H

#ifdef __cplusplus
extern "C" {
#endif

/*
 *  pca.h
 *  soundbite
 *
 *  Created by Mark Levy on 08/02/2006.
 *  Copyright 2006 Centre for Digital Music, Queen Mary, University of London. All rights reserved.
 *
 */

void pca_project(double** data, int n, int m, int ncomponents);

#ifdef __cplusplus
}
#endif


#endif

