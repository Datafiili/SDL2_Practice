#ifndef _MAT4X4H_
#define _MAT4X4H_

#include <AarniEngine/Math/vector.h>

struct Mat4x4{
    float m[4][4] = {0}; //row by column

    Mat4x4& operator+=(const Mat4x4& other)
    {
        for(int i = 0; i < 4; i++)
        {
            for(int k = 0; k < 4; k++)
            {
                m[i][k] += other.m[i][k];
            }
        }
        return *this;
    }

    friend Mat4x4 operator+(Mat4x4 m1, const Mat4x4& m2)
    {
        return m1 += m2;
    }

    static Mat4x4 AddMatricies(Mat4x4 &a, Mat4x4 &b)
    {
        Mat4x4 result = {};
        for(int i = 0; i < 4; i++)
        {
            for(int k = 0; k < 4; k++)
            {
                result.m[i][k] = a.m[i][k] + b.m[i][k];
            }
        }
        return result;
    }

    static void MultiplyMatrixVector(Vector3 &inputVector, Vector3 &outputVector, Mat4x4 &matrix)
    {
        
        outputVector.x = inputVector.x * matrix.m[0][0] + inputVector.y * matrix.m[1][0] + inputVector.z * matrix.m[2][0] + matrix.m[3][0];
        outputVector.y = inputVector.x * matrix.m[0][1] + inputVector.y * matrix.m[1][1] + inputVector.z * matrix.m[2][1] + matrix.m[3][1];
        outputVector.z = inputVector.x * matrix.m[0][2] + inputVector.y * matrix.m[1][2] + inputVector.z * matrix.m[2][2] + matrix.m[3][2];
        float w = inputVector.x * matrix.m[0][3] + inputVector.y * matrix.m[1][3] +  inputVector.z * matrix.m[2][3] + matrix.m[3][3];
        if(w != 0.0f)
        {
            outputVector /= w;
        }
    }

     // ----- Printing ----- //
    friend std::ostream& operator<<(std::ostream& os, const Mat4x4& pt); //overriding << operator
};

std::ostream& operator<<(std::ostream& os, const Mat4x4& m) // overriding << operator
{
    os << "Matrix 4x4:\n";
    for(int i = 0; i < 4; i++)
    {
        os << "[";
        for(int k = 0; k < 4; k++)
        {
            os << m.m[i][k];
            if(k < 3){ os << ", "; }
        }
        os << "]";
        if(i < 3){ os << "\n";}
    }
    return os;
}

#endif