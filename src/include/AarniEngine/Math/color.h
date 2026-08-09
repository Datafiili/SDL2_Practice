#ifndef _COLORH_
#define _COLORH_

#include <cstdint>
#include <math.h>
#include <vector>

#include <AarniEngine/Math/curve.h>

struct Color
{
    uint8_t r = 0;
    uint8_t g = 0;
    uint8_t b = 0;
    uint8_t a = 255;
    public:
    Color(
        int r = 0,
        int g = 0,
        int b = 0, 
        int a = 255):
        r(static_cast<uint8_t>(std::clamp(r,0,255))),
        g(static_cast<uint8_t>(std::clamp(g,0,255))),
        b(static_cast<uint8_t>(std::clamp(b,0,255))),
        a(static_cast<uint8_t>(std::clamp(a,0,255))){};
    Color(char hex[9]):
        r(static_cast<uint8_t>(HexDigitToBase10(hex[1],1) + HexDigitToBase10(hex[2],0))),
        g(static_cast<uint8_t>(HexDigitToBase10(hex[3],1) + HexDigitToBase10(hex[4],0))),
        b(static_cast<uint8_t>(HexDigitToBase10(hex[5],1) + HexDigitToBase10(hex[6],0))),
        a(static_cast<uint8_t>(HexDigitToBase10(hex[7],1) + HexDigitToBase10(hex[8],0))){};
    
    static Color White(){ return {255,255,255,255};}
    static Color Black(){ return {0,0,0,255};}
    static Color Red(){ return {255,0,0,255};}
    static Color Green(){ return {0,255,0,255};}
    static Color Blue(){ return {0,0,255,255};}
    static Color Yellow(){ return {255,255,0,255};}
    static Color Purple(){ return {255,0,255,255};}
    static Color Cyan(){ return {0,255,255,255};}

    float GetValueProsentRed()
    {
        return r / 255.0f;
    }

    float GetValueProsentGreen()
    {
        return g / 255.0f;
    }

    float GetValueProsentBlue()
    {
        return b / 255.0f;
    }

    float GetValueProsentAlpha()
    {
        return a / 255.0f;
    }

    static Color SetValueProsent(float redProsent = 1.0f, float greenProsent = 1.0f, float blueProsent = 1.0, float alphaProsent = 1.0f)
    {
        return Color(
            std::clamp(255.0f * redProsent,0.0f,255.0f),
            std::clamp(255.0f * greenProsent,0.0f,255.0f),
            std::clamp(255.0f * blueProsent,0.0f,255.0f),
            std::clamp(255.0f * alphaProsent,0.0f,255.0f));
    }

    static float Average(Color c)
    {
        return (c.r + c.g + c.b) / 3;
    }

    static Color BlackAndWhite(Color c)
    {
        int a = (uint8_t)(Color::Average(c));
        return Color(a,a,a,c.a);
    }

    static uint8_t InvertValue(uint8_t value)
    {
        return 255 - value;
    }

    static Color Invert(Color c)
    {
        return Color(InvertValue(c.r),InvertValue(c.g),InvertValue(c.b), c.a);
    }

    static int Max(Color c)
    {
        return std::max(std::max(c.r,c.g),c.b);
    }
    
    static int Min(Color c)
    {
        return std::min(std::min(c.r,c.g),c.b);
    }

    static int HexDigitToBase10(char c, int digit = 0) //digit order from behind -> 0 = last, 1 = second last.
    {
        c = (char)toupper(c);
        char symbols[16] = {'0','1','2','3','4','5','6','7','8','9','A','B','C','D','E','F'};
        for(int i = 0; i < 16; i++)
        {
            if (symbols[i] == c)
            {
                return i * pow(16,digit);
            }
        }
        return -1;
    }

    // ----- Printing ----- //
    friend std::ostream& operator<<(std::ostream& os, const Color& pt); //overriding << operator
};

std::ostream& operator<<(std::ostream& os, const Color& c) // overriding << operator
{
    os << "(" << (int)c.r << ", " << (int)c.g << ", " << (int)c.b << ", " << (int)c.a << ")";
    return os;
}

struct ColorGradient
{
    public:
    Curve r,g,b,a;
    ColorGradient(
        Curve r = Curve::One(),
        Curve g = Curve::One(),
        Curve b = Curve::One(),
        Curve a = Curve::One()): r(r), g(g), b(b), a(a){};
    ColorGradient(Color c):
        r(Curve::ConstantValue((float)c.r / 255.0f)),
        g(Curve::ConstantValue((float)c.g / 255.0f)),
        b(Curve::ConstantValue((float)c.b / 255.0f)),
        a(Curve::ConstantValue((float)c.a / 255.0f)){};

    Color GetColor(float x)
    {
        return Color(
            (int)(r.GetValue(x) * 255.0f),
            (int)(g.GetValue(x) * 255.0f),
            (int)(b.GetValue(x) * 255.0f),
            (int)(a.GetValue(x) * 255.0f)
        );
    }

    // ----- Printing ----- //
    friend std::ostream& operator<<(std::ostream& os, const ColorGradient& pt); //overriding << operator
};

std::ostream& operator<<(std::ostream& os, const ColorGradient& c) // overriding << operator
{
    os << "(" << c.r << ", " << c.g << ", " << c.b << ", " << c.a << ")";
    return os;
}

#endif