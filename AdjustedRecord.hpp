#ifndef ADJUSTEDRECORD_HPP
#define ADJUSTEDRECORD_HPP

#include "RainfallRecord.hpp"

class AdjustedRecord : public RainfallRecord {
public:
    AdjustedRecord();
    AdjustedRecord(int count, double initVal, double extraVal);
    AdjustedRecord(const AdjustedRecord& other);
    virtual ~AdjustedRecord();
    AdjustedRecord& operator=(const AdjustedRecord& other);

    virtual double netRainfall() const override;
};

#endif
