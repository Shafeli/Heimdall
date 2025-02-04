#pragma once

#include "Vector2.h"
#include "UnitTests/UnitTestSystem.h"


namespace Brokkr
{
    class UnitTest
    {

        static bool TestVector2Addition()
        {
            Vector2<float> a(1.0f, 2.0f);
            Vector2<float> b(3.0f, 4.0f);
            Vector2<float> result = a + b;
            return (result.x == 4.0f && result.y == 6.0f);
        }

        static bool TestVector2Normalization()
        {
            Vector2<float> v(3.0f, 4.0f);
            v.Normalize();
            return (std::abs(v.Length() - 1.0f) < 0.0001f);
        }

    public:

        static void RegisterEngineTests(UnitTestSystem* pTestSystem)
        {
            pTestSystem->AddTest("Vector2 Addition", TestVector2Addition);
            pTestSystem->AddTest("Vector2 Normalization", TestVector2Normalization);
        }
    };

}



