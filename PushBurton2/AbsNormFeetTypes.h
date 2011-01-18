#ifndef ABSNORMFEETTYPES_H
#define ABSNORMFEETTYPES_H

class PressurePointData
{
public:
    int minVal;
    int maxVal;

    int rawVal;
    float normVal;

    void update(int inVal)
    {
        rawVal = inVal;
        maxVal = Max(inVal, maxVal);
        minVal = Min(inVal, minVal);
        normVal = (inVal - minVal)*(1.0f/(maxVal-minVal));
    }

    void reset()
    {
        minVal = 1024;
        maxVal = 0;

        normVal = -1.0f;
        rawVal = -1;
    }
};

class SnowboradPressureData
{
public:
    PressurePointData LToe;
    PressurePointData RToe;

    PressurePointData LHeel;
    PressurePointData RHeel;

    void reset()
    {
        LToe.reset();
        RToe.reset();

        LHeel.reset();
        RHeel.reset();

        totalRawSum = 0;
    }

    int computeSum()
    {
        totalRawSum = LHeel.rawVal+LToe.rawVal+RHeel.rawVal+RToe.rawVal;
        return totalRawSum;
    }

    int totalRawSum;
};

#endif // ABSNORMFEETTYPES_H
