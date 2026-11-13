#include "RainfallRecord.hpp"

RainfallRecord::RainfallRecord() : mmValues(nullptr), months(0), evapLoss(3.5) {}

RainfallRecord::RainfallRecord(int count, double initVal, double extraVal) : mmValues(nullptr), months(count), evapLoss(extraVal) {
    if (months > 0) {
        mmValues = new double[months];
        for (int i = 0; i < months; ++i) {
            mmValues[i] = initVal + static_cast<double>(i);
        }
    } else {
        months = 0;
    }
}

// TODO 1: Complete the Copy Constructor (Deep Copy)
RainfallRecord::RainfallRecord(const RainfallRecord& other) {
    (void)other;
}

// TODO 2: Complete the Destructor (Release DMA)
RainfallRecord::~RainfallRecord() {
}

// TODO 3: Complete the Copy Assignment Operator (Deep Copy + Self-Assignment Guard)
RainfallRecord& RainfallRecord::operator=(const RainfallRecord& other) {
    (void)other;
    return *this;
}

double RainfallRecord::getMonthRain(int index) const {
    if (index >= 0 && index < months && mmValues != nullptr) {
        return mmValues[index];
    }
    return static_cast<double>(0);
}

// TODO 4: Implement netRainfall() (Maximum 8 lines of implementation)
double RainfallRecord::netRainfall() const {
    return static_cast<double>(0);
}
