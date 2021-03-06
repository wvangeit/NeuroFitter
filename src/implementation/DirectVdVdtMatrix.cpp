/*
Revision of last commit: $Rev: 222 $
Author of last commit: $Author: werner $
Date of last commit: $Date: 2007-10-04 14:03:41 +0900 (Thu, 04 Oct 2007) $
*/

#include "../DirectVdVdtMatrix.h"

DirectVdVdtMatrix::DirectVdVdtMatrix() 
	: MapVdVdtMatrix() {}

DirectVdVdtMatrix::DirectVdVdtMatrix(FixedParameters params) 
	: MapVdVdtMatrix(params) {}

DirectVdVdtMatrix::DirectVdVdtMatrix(const DataTrace& trace, FixedParameters params) 
	:	MapVdVdtMatrix(trace, params) {}

double DirectVdVdtMatrix::compare(const VdVdtMatrix & o) const {

	const DirectVdVdtMatrix & other = dynamic_cast<const DirectVdVdtMatrix &>(o);

    double errorValue = 0;

    double diff = 0;

    const double precision = toDouble(fixedParams["comparePrecision"]);

    if (other.getVLength() != vLength) crash("DirectVdVdtMatrix","V dimensions don't match");
    if (other.getdVdtLength() != dVdtLength) crash("DirectVdVdtMatrix","dVdt dimensions don't match");


	if (toInt(fixedParams["SumOfSquareRoots"]) == 1) {
    	////////////////////////////////////////////////////////////
    	/// Calculate the square of the sum of the squares roots ///
    	////////////////////////////////////////////////////////////

    	for (int vIndex=0;vIndex<vLength;vIndex++) {
        	for (int dVdtIndex=0;dVdtIndex<dVdtLength;dVdtIndex++) {
            	diff=fabs(get(vIndex,dVdtIndex)-other.get(vIndex,dVdtIndex));
            	if (diff > precision) {
                	errorValue += pow(diff,0.5);
            	}
        	}
    	}

		errorValue = pow(errorValue,2);
	}
	else {
    	///////////////////////////////////////////////////////////
    	/// Calculate the square root of the sum of the squares ///
    	///////////////////////////////////////////////////////////

    	for (int vIndex=0;vIndex<vLength;vIndex++) {
        	for (int dVdtIndex=0;dVdtIndex<dVdtLength;dVdtIndex++) {
            	diff=fabs(get(vIndex,dVdtIndex)-other.get(vIndex,dVdtIndex));
            	if (diff > precision) {
                	errorValue += pow(diff,2);
            	}
        	}
    	}

		errorValue = pow(errorValue,0.5);
    }

	                                                                                                    
    return errorValue;

}
