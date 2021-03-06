/*
Revision of last commit: $Rev: 245 $
Author of last commit: $Author: werner $
Date of last commit: $Date: 2007-10-23 09:44:59 +0900 (Tue, 23 Oct 2007) $
*/

#include "../DataTrace.h"

DataTrace::DataTrace() 
	:  points(vector< pair< double, bool > >(0,pair< double, bool >(0,false))), weight(-1), startTime(-1), stopTime(-1), lag(-1), lagWeight(-1), validLength(0) {}

DataTrace::DataTrace(int size)
	:  points(vector< pair< double, bool > >(size, pair< double, bool >(0,false))), weight(-1), startTime(-1), stopTime(-1), lag(-1), lagWeight(-1), validLength(0) {}

int DataTrace::getLength() const {
	return points.size();
}

int DataTrace::getValidLength() const {
	return validLength;
}

///
/// This will remove all existing data !!!
///
void DataTrace::resetAndSetLength(const int size) {
	validLength = 0;
	points.clear();
	points.resize(size,pair< double, bool >(0,false));
}

double DataTrace::getWeight() const {	
	if (weight == -1) crash("DataTrace","Weight not set");
	return weight;
}


double DataTrace::getStartTime() const {
	if (startTime == -1) crash("DataTrace","StartTime not set");
	return startTime;
}

double DataTrace::getStopTime() const {
	if (stopTime == -1) crash("DataTrace","StopTime not set");
	return stopTime;
}

double DataTrace::getSamplingFrequency() const{
	if (samplingFrequency == -1) crash("DataTrace","SamplingFrequency not set");
	return samplingFrequency;
}

string DataTrace::getName() const {
	return name;
}

int DataTrace::getLag() const {
	if (lag == -1) crash("DataTrace","Lag not set");
	return lag;
}

double DataTrace::getLagWeight() const {
	if (lagWeight == -1) crash("DataTrace", "Lag weight not set");
	return lagWeight;
}

void DataTrace::setStartTime(const double newStartTime) {
	startTime = newStartTime;
}
    
void DataTrace::setStopTime(const double newStopTime) {
	stopTime = newStopTime;	
}
    
void DataTrace::setSamplingFrequency(const double newFrequency) {
	samplingFrequency = newFrequency;
}


void DataTrace::setWeight(const double newWeight) {
	weight = newWeight;
}

void DataTrace::setName(const string newName) {
	name = newName;
}

void DataTrace::setLag(int newLag, double newLagWeight) {
	if (newLag < 1) crash("DataTrace","Lags smaller than 1 or not allowed");
	lag = newLag;
	lagWeight = newLagWeight;
}

void DataTrace::printOn(OutputChannel & output) const {
	int length = points.size();
	output << length;
	for (int i = 0; i < length; i++) {
		output << points[i].first;
		output << points[i].second;
	}
	output << weight;
	output << startTime;
	output << stopTime;
	output << samplingFrequency;
	output << name;
	output << lag;
	output << lagWeight;
	output << validLength;
}

void DataTrace::readFrom(InputChannel & input) {
	unsigned length;
	input >> length;
	points = vector< pair< double, bool > >(length, pair< double, bool >(0, false));
	for (unsigned i = 0; i < points.size(); i++) {
		input >> points[i].first;
		input >> points[i].second;
	}
	input >> weight;
	input >> startTime;
	input >> stopTime;
	input >> samplingFrequency;
	input >> name;
	input >> lag;
	input >> lagWeight;
	input >> validLength;
}
    
bool DataTrace::pointIsValid(const int subscript) const {
	if (subscript < 0 || subscript >= (int)points.size()) {
        crash("DataTrace","Invalid subscript: "+str(subscript));
    }
    return points[subscript].second;
} 

double DataTrace::get(const int subscript) const {
    if (subscript < 0 || subscript >= (int)points.size()) {
        crash("DataTrace","Invalid subscript: "+str(subscript));
    }
	if (!points[subscript].second) crash("DataTrace","Retrieving data point that is not valid: "+str(subscript));
	return points[subscript].first;
}

void DataTrace::set (const int subscript, const double newValue) {
    if (subscript < 0 || subscript >= (int)points.size()) {
        crash("DataTrace","Invalid subscript: "+str(subscript));
    }
	if (!points[subscript].second) validLength++;
	points[subscript].second = true;
	points[subscript].first = newValue;
}

