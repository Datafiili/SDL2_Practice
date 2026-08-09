#ifndef _CURVEH_
#define _CURVEH_

#include <vector>
#include <math.h>
#include <algorithm>

float CubicHermiteInterpolation(float t, int mode)
{
    float t2 = t * t;
    float t3 = t2 * t;
    switch (mode)
    {
    case 0: // h00 - start value
        return 2 * t3 - 3 * t2 + 1;
    case 1: // h10 - outgoing start tangent
        return t3 - 2 * t2 + t;
    case 2: // h01 - end value
        return -2 * t3 + 3 * t2;
    case 3: // h11 - incoming end tangent
        return t3 - t2;
    default:
        return 0.0f;
    }
}

struct Tangent
{
    enum Interpolation
    {
        Linear,
        Constant,
        Cubic
    };

    float slope, weight;
    Interpolation interpolationMode;
    Tangent(Interpolation i = Interpolation::Cubic, float slope = 0.0f, float weight = 1.0f): interpolationMode(i), slope(slope), weight(weight){};

    float GetWeightedSlope()
    {
        return slope * weight;
    }

    // ----- Printing ----- //
    friend std::ostream& operator<<(std::ostream& os, const Tangent& pt); //overriding << operator
};

std::ostream &operator<<(std::ostream &os, const Tangent &t) // overriding << operator
{
    if(t.interpolationMode == Tangent::Constant)
    {
        os << "(Interpolation: Constant)";
    }
    if(t.interpolationMode == Tangent::Cubic)
    {
        os << "(slope: " << t.slope << ", weight: " << t.weight << ", Interpolation: Cubic)";
    }
    if(t.interpolationMode == Tangent::Linear)
    {
        os << "(Interpolation: Linear)";
    }
    return os;
}

struct CurveKey
{
    float time, value;
    Tangent tangentIn, tangentOut;
    CurveKey(float time = 0.0f, float value = 0.0f, Tangent tin = Tangent(), Tangent tout = Tangent()): time(time), value(value), tangentIn(tin), tangentOut(tout){};

    // ----- Printing ----- //
    friend std::ostream& operator<<(std::ostream& os, const CurveKey& pt); //overriding << operator
};

std::ostream &operator<<(std::ostream &os, const CurveKey &k) // overriding << operator
{
    os << "(time: " << k.time << ", value: " << k.value << std::endl << "InTang: " << k.tangentIn << std::endl << "OutTang: " << k.tangentOut << ")";
    return os;
}

//Keys need to be sorted based on time.
class Curve
{
    private:
    static bool compareCurves(const CurveKey& a, const CurveKey& b) {
        return a.time < b.time;
    }

    void SortKeys()
    {
        std::sort(keys.begin(), keys.end(), compareCurves);
    }

    public:
    std::vector<CurveKey>keys;
    Curve(std::vector<CurveKey>keys = {}):keys(keys){ SortKeys(); };
    static Curve ConstantValue(float v){ return Curve({CurveKey(0.0f,v,Tangent(Tangent::Interpolation::Constant),Tangent(Tangent::Interpolation::Constant))}); } 
    static Curve One(){ return Curve(ConstantValue(1.0f)); } 
    static Curve Zero(){ return Curve(ConstantValue(0.0f)); } 
    static Curve AscendingLinear(){ return Curve({CurveKey(0.0f,0.0f,Tangent(Tangent::Interpolation::Linear)),CurveKey(1.0f,1.0f,Tangent(Tangent::Interpolation::Linear))}); } 
    static Curve DescesendingLinear(){ return Curve({CurveKey(0.0f,0.1f,Tangent(Tangent::Interpolation::Linear)),CurveKey(1.0f,0.0f,Tangent(Tangent::Interpolation::Linear))}); } 
    static Curve EasyInCubic(){ return Curve({CurveKey(0.0f,0.0f,Tangent(Tangent::Interpolation::Cubic)),CurveKey(1.0f,1.0f,Tangent(Tangent::Interpolation::Cubic, -M_PI / 2))}); } 
    static Curve EasyInCubicMirrored(){ return Curve({CurveKey(0.0f,1.0f,Tangent(Tangent::Interpolation::Cubic)),CurveKey(1.0f,0.0f,Tangent(Tangent::Interpolation::Cubic, M_PI / 2))}); } 
    static Curve EasyOutCubic(){ return Curve({CurveKey(0.0f,0.0f,Tangent(Tangent::Interpolation::Cubic, M_PI / 2)),CurveKey(1.0f,1.0f,Tangent(Tangent::Interpolation::Cubic))}); } 
    static Curve EasyOutCubicMirrored(){ return Curve({CurveKey(0.0f,1.0f,Tangent(Tangent::Interpolation::Cubic, -M_PI / 2)),CurveKey(1.0f,0.0f,Tangent(Tangent::Interpolation::Cubic))}); } 


    void AddKey(CurveKey key)
    {
        keys.push_back(key);
        SortKeys();
    }

    void Addkeys(std::vector<CurveKey> keyList)
    {
        keys.insert(keys.end(), keyList.begin(), keyList.end());
        SortKeys();
    }

    void SetConstantInterpolationAll()
    {
        SetInterpolationAll(Tangent::Interpolation::Constant);
    }

    void SetLinearInterpolationAll()
    {
        SetInterpolationAll(Tangent::Interpolation::Linear);
    }

    void SetInterpolationAll(Tangent::Interpolation interPol)
    {
        for(int i = 0; i < keys.size(); i++)
        {
            keys[i].tangentIn.interpolationMode = interPol;
            keys[i].tangentOut.interpolationMode = interPol;
        }
    }

    float GetValue(float time)
    {
        //Error prevention.
        if(keys.size() == 0) { return 0.0f; } // No keys, Assume zero.
        if(keys.size() == 1) { return keys[0].value; } // One key -> Return value of the key.

        //Looks for the two keys that are around the 
        int index = keys.size() - 1;
        for(int i = 1; i < keys.size(); i++)
        {
            if(keys[i].time == time) //If time matches perfectly with time, then value must equal to that.
            {
                return keys[i].value;
            }

            if(keys[i].time > time) //The key found.
            {
                index = i - 1;
                break;
            }
        }

        //In constant mode, left key's value.
        if(keys[index].tangentOut.interpolationMode == Tangent::Interpolation::Constant)
        {
            return keys[index].value;
        }

        //Both linear.
        if(keys[index].tangentOut.interpolationMode == Tangent::Interpolation::Linear
        && keys[index + 1].tangentIn.interpolationMode == Tangent::Interpolation::Linear)
        {
            float difference = keys[index + 1].value - keys[index].value;
            float timePros = (time - keys[index].time) / (keys[index + 1].time - keys[index].time);
            return keys[index].value + difference * timePros;
        }

        //Calculating steepness for linear values.
        if(keys[index].tangentOut.interpolationMode == Tangent::Interpolation::Linear)
        {
            keys[index].tangentOut.slope = 0.0f;
        }
        if(keys[index + 1].tangentIn.interpolationMode == Tangent::Interpolation::Linear)
        {
            keys[index + 1].tangentIn.slope = 0.0f;
        }

        //Normalizing time to be from 0 to 1 where left key is 0 and right 1.
        time = (time - keys[index].time) / (keys[index + 1].time - keys[index].time);

        //Calculating the cubic value.
        return
        CubicHermiteInterpolation(time,0) * keys[index].value +
        CubicHermiteInterpolation(time,1) * keys[index].tangentOut.GetWeightedSlope() +
        CubicHermiteInterpolation(time,2) * keys[index + 1].value +
        CubicHermiteInterpolation(time,3) * keys[index + 1].tangentIn.GetWeightedSlope();
    }

    private:
    // ----- Printing ----- //
    friend std::ostream& operator<<(std::ostream& os, const Curve& pt); //overriding << operator
};

std::ostream &operator<<(std::ostream &os, const Curve &k) // overriding << operator
{
    os << "Curve:" << std::endl;
    for(int i = 0; i < k.keys.size(); i++)
    {   
        os << "Key " << i << " ";
        os << k.keys[i];
        os << std::endl;
    }
    return os;
}

#endif