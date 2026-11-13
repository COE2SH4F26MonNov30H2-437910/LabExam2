#ifndef RAINFALLRECORD_HPP
#define RAINFALLRECORD_HPP

#include <iostream>
using namespace std;

class RainfallRecord {
protected:
    double* mmValues;
    int months;
    double evapLoss;

public:
    RainfallRecord();
    RainfallRecord(int count, double initVal, double extraVal);
    RainfallRecord(const RainfallRecord& other);
    virtual ~RainfallRecord();
    RainfallRecord& operator=(const RainfallRecord& other);

    double getMonthRain(int index) const;
    virtual double netRainfall() const;
};

#endif
