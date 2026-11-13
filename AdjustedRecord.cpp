#include "AdjustedRecord.hpp"

AdjustedRecord::AdjustedRecord() : RainfallRecord() {}

AdjustedRecord::AdjustedRecord(int count, double initVal, double extraVal) : RainfallRecord(count, initVal, extraVal) {}

AdjustedRecord::AdjustedRecord(const AdjustedRecord& other) : RainfallRecord(other) {}

AdjustedRecord::~AdjustedRecord() {}

AdjustedRecord& AdjustedRecord::operator=(const AdjustedRecord& other) {
    if (this != &other) {
        RainfallRecord::operator=(other);
    }
    return *this;
}

// TODO 5: Override netRainfall() (Maximum 8 lines of implementation)
double AdjustedRecord::netRainfall() const {
    return static_cast<double>(0);
}
