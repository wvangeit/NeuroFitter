/*
Revision of last commit: $Rev: 184 $
Author of last commit: $Author: werner $
Date of last commit: $Date: 2007-07-12 15:27:13 +0900 (Thu, 12 Jul 2007) $
*/

#ifndef NEUROFITTER_MAPVDVDTMATRIX_H
#define NEUROFITTER_MAPVDVDTMATRIX_H

#include <iostream>
#include <cmath>

using namespace std;

#include "VdVdtMatrix.h"

class MapVdVdtMatrix : public VdVdtMatrix {

public:
	MapVdVdtMatrix();
	virtual ~MapVdVdtMatrix();

	MapVdVdtMatrix(const FixedParameters);
	MapVdVdtMatrix(const DataTrace&, const FixedParameters);

	double get(const int v, const int dVdt) const;	
	void set(const int v, const int dVdt, double value);	

	void makeEmpty();

	virtual double compare(const VdVdtMatrix&) const = 0;
	
protected:


	map< int, double > matrix;

	int mIndex(const int v, const int dVdt) const;
	int vIndex(const int mIndex) const;
	int dVdtIndex(const int mIndex) const;	

};
	
#endif
