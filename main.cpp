#include <iostream>
#include "RainfallRecord.hpp"
#include "AdjustedRecord.hpp"

using namespace std;

// Lightweight heap allocation tracker using only <iostream> and C++11 primitives (no size_t, no extra headers)
static char g_heapPool[65536];
static int g_poolOffset = 0;
static int g_activeAllocs = 0;
static int g_totalAllocs = 0;

void* operator new(decltype(sizeof(0)) bytes) {
    int req = static_cast<int>(bytes);
    if (req <= 0) req = 1;
    // 8-byte alignment
    int rem = req % 8;
    if (rem != 0) req += (8 - rem);
    if (g_poolOffset + req > 65536) return nullptr;
    void* ptr = &g_heapPool[g_poolOffset];
    g_poolOffset += req;
    ++g_activeAllocs;
    ++g_totalAllocs;
    return ptr;
}

void* operator new[](decltype(sizeof(0)) bytes) {
    int req = static_cast<int>(bytes);
    if (req <= 0) req = 1;
    int rem = req % 8;
    if (rem != 0) req += (8 - rem);
    if (g_poolOffset + req > 65536) return nullptr;
    void* ptr = &g_heapPool[g_poolOffset];
    g_poolOffset += req;
    ++g_activeAllocs;
    ++g_totalAllocs;
    return ptr;
}

void operator delete(void* ptr) noexcept {
    if (ptr != nullptr) {
        --g_activeAllocs;
    }
}

void operator delete[](void* ptr) noexcept {
    if (ptr != nullptr) {
        --g_activeAllocs;
    }
}

void operator delete(void* ptr, decltype(sizeof(0))) noexcept {
    if (ptr != nullptr) {
        --g_activeAllocs;
    }
}

void operator delete[](void* ptr, decltype(sizeof(0))) noexcept {
    if (ptr != nullptr) {
        --g_activeAllocs;
    }
}

bool approxEqual(double a, double b) {
    double diff = a - b;
    if (diff < 0.0) diff = -diff;
    return diff < 0.001;
}

int main() {
    int passed = 0;
    int failed = 0;

    // Expected reference values computed dynamically
    // Array of 3 elements initialized as: initVal + 0, initVal + 1, initVal + 2
    double initV = static_cast<double>(30.0);
    double extV = static_cast<double>(3.5);

    // --- PARENT CLASS TEST CASES (1 to 5) ---
    // TC 1: Default constructor & empty compute
    {
        RainfallRecord pDef;
        if (approxEqual(static_cast<double>(pDef.netRainfall()), 0.0) &&
            approxEqual(static_cast<double>(pDef.getMonthRain(0)), 0.0)) {
            cout << "[PASS] TC 01: RainfallRecord default constructor" << endl;
            ++passed;
        } else {
            cout << "[FAIL] TC 01: RainfallRecord default constructor" << endl;
            ++failed;
        }
    }

    // TC 2: Parameterized constructor & base compute()
    {
        RainfallRecord p1(3, initV, extV);
        bool ok = approxEqual(static_cast<double>(p1.getMonthRain(0)), static_cast<double>(initV)) &&
                  approxEqual(static_cast<double>(p1.getMonthRain(2)), static_cast<double>(initV + static_cast<double>(2)));
        // Verify compute() is non-trivial when initV > 0
        if (ok && p1.getMonthRain(1) == static_cast<double>(initV + static_cast<double>(1))) {
            // Compare against a second identical instance
            RainfallRecord pCheck(3, initV, extV);
            if (approxEqual(static_cast<double>(p1.netRainfall()), static_cast<double>(pCheck.netRainfall())) &&
                !approxEqual(static_cast<double>(p1.netRainfall()), 0.0)) {
                cout << "[PASS] TC 02: RainfallRecord parameterized constructor & netRainfall()" << endl;
                ++passed;
            } else {
                cout << "[FAIL] TC 02: RainfallRecord netRainfall() returned unexpected value" << endl;
                ++failed;
            }
        } else {
            cout << "[FAIL] TC 02: RainfallRecord element initialization mismatch" << endl;
            ++failed;
        }
    }

    // TC 3: Parent Copy Constructor (Deep Copy verification)
    {
        RainfallRecord orig(3, initV, extV);
        RainfallRecord copyObj(orig);
        if (approxEqual(static_cast<double>(copyObj.netRainfall()), static_cast<double>(orig.netRainfall())) &&
            approxEqual(static_cast<double>(copyObj.getMonthRain(1)), static_cast<double>(orig.getMonthRain(1))) &&
            !approxEqual(static_cast<double>(copyObj.netRainfall()), 0.0)) {
            cout << "[PASS] TC 03: RainfallRecord copy constructor deep copy" << endl;
            ++passed;
        } else {
            cout << "[FAIL] TC 03: RainfallRecord copy constructor failed" << endl;
            ++failed;
        }
    }

    // TC 4: Parent Copy Assignment Operator & Self-Assignment
    {
        RainfallRecord a(3, initV, extV);
        RainfallRecord b(2, initV, extV);
        b = a;
        RainfallRecord* selfPtr = &b;
        b = *selfPtr; // Self-assignment check
        if (approxEqual(static_cast<double>(b.netRainfall()), static_cast<double>(a.netRainfall())) &&
            approxEqual(static_cast<double>(b.getMonthRain(2)), static_cast<double>(a.getMonthRain(2))) &&
            !approxEqual(static_cast<double>(b.netRainfall()), 0.0)) {
            cout << "[PASS] TC 04: RainfallRecord copy assignment & self-assignment" << endl;
            ++passed;
        } else {
            cout << "[FAIL] TC 04: RainfallRecord copy assignment failed" << endl;
            ++failed;
        }
    }

    // TC 5: Parent Destructor Heap Release Check
    {
        int before = g_activeAllocs;
        {
            RainfallRecord temp(4, initV, extV);
        }
        int after = g_activeAllocs;
        if (before == after && g_totalAllocs > 0) {
            cout << "[PASS] TC 05: RainfallRecord destructor released heap memory" << endl;
            ++passed;
        } else {
            cout << "[FAIL] TC 05: RainfallRecord destructor leaked heap memory" << endl;
            ++failed;
        }
    }

    // --- CHILD CLASS TEST CASES (6 to 10) ---
    // TC 6: Child Parameterized Construction & Overridden compute()
    {
        RainfallRecord baseObj(3, initV, extV);
        AdjustedRecord childObj(3, initV, extV);
        if (!approxEqual(static_cast<double>(childObj.netRainfall()), 0.0) &&
            !approxEqual(static_cast<double>(childObj.netRainfall()), static_cast<double>(baseObj.netRainfall()))) {
            cout << "[PASS] TC 06: AdjustedRecord overridden netRainfall()" << endl;
            ++passed;
        } else {
            cout << "[FAIL] TC 06: AdjustedRecord overridden netRainfall() failed" << endl;
            ++failed;
        }
    }

    // TC 7: Polymorphic Dispatch via Base Pointer
    {
        AdjustedRecord directChild(3, initV, extV);
        RainfallRecord* polyPtr = &directChild;
        if (!approxEqual(static_cast<double>(polyPtr->netRainfall()), 0.0) &&
            approxEqual(static_cast<double>(polyPtr->netRainfall()), static_cast<double>(directChild.netRainfall()))) {
            cout << "[PASS] TC 07: AdjustedRecord polymorphic dispatch via RainfallRecord*" << endl;
            ++passed;
        } else {
            cout << "[FAIL] TC 07: AdjustedRecord polymorphic dispatch failed" << endl;
            ++failed;
        }
    }

    // TC 8: Child Copy Constructor
    {
        AdjustedRecord c1(3, initV, extV);
        AdjustedRecord c2(c1);
        if (approxEqual(static_cast<double>(c1.netRainfall()), static_cast<double>(c2.netRainfall())) &&
            !approxEqual(static_cast<double>(c2.netRainfall()), 0.0)) {
            cout << "[PASS] TC 08: AdjustedRecord copy constructor" << endl;
            ++passed;
        } else {
            cout << "[FAIL] TC 08: AdjustedRecord copy constructor failed" << endl;
            ++failed;
        }
    }

    // TC 9: Child Copy Assignment Operator
    {
        AdjustedRecord c1(3, initV, extV);
        AdjustedRecord c2;
        c2 = c1;
        if (approxEqual(static_cast<double>(c1.netRainfall()), static_cast<double>(c2.netRainfall())) &&
            !approxEqual(static_cast<double>(c2.netRainfall()), 0.0)) {
            cout << "[PASS] TC 09: AdjustedRecord copy assignment operator" << endl;
            ++passed;
        } else {
            cout << "[FAIL] TC 09: AdjustedRecord copy assignment operator failed" << endl;
            ++failed;
        }
    }

    // TC 10: Virtual Destructor via Base Pointer Deletion
    {
        int before = g_activeAllocs;
        RainfallRecord* heapChild = new AdjustedRecord(4, initV, extV);
        delete heapChild;
        int after = g_activeAllocs;
        if (before == after) {
            cout << "[PASS] TC 10: AdjustedRecord virtual destructor via RainfallRecord*" << endl;
            ++passed;
        } else {
            cout << "[FAIL] TC 10: AdjustedRecord virtual destructor leaked memory" << endl;
            ++failed;
        }
    }

    cout << "========================================" << endl;
    cout << "FINAL SCORE: " << passed << " / 10 PASSED (" << failed << " FAILED)" << endl;
    if (g_activeAllocs == 0) {
        cout << "MEMORY LEAK REPORT: NO MEMORY LEAKS DETECTED (0 active allocations)." << endl;
    } else {
        cout << "MEMORY LEAK REPORT: WARNING! " << g_activeAllocs << " active heap allocation(s) leaked!" << endl;
    }
    cout << "========================================" << endl;

    return (failed == 0 && g_activeAllocs == 0) ? 0 : 1;
}
