#pragma once

#include "Vector2.h"
#include "UnitTests/UnitTestSystem.h"

// Sources:
// https://cplusplus.com/forum/general/271350/ - Vector test direction
// https://www.omnicalculator.com/math/vector - Vector Calculator 01
// https://www.emathhelp.net/calculators/linear-algebra/vector-calculator/ - Vector Calculator 02 cool things like Orthogonal Complement Calculator
namespace Brokkr
{
    class UnitTest
    {

        // Adding Test
        static bool TestVector2Addition()
        {
            Vector2<float> a(1.0f, 2.0f);
            Vector2<float> b(3.0f, 4.0f);
            Vector2<float> result = a + b;
            return (result.m_x == 4.0f && result.m_y == 6.0f);
        }

        // Normalization Test
        static bool TestVector2Normalization()
        {
            Vector2<float> v(3.0f, 4.0f);
            auto nV = v.Normalize();
            return (std::abs(nV.Length() - 1.0f) < 0.0001f);
        }

        static bool TestFromPolar()
        {

            // Test 45 degrees or pi/4 radians
            constexpr float radius = 5.0f;
            constexpr float angle = PI / 4.0;  // pi/4 45 degrees

            const Vector2<float> result = Vector2<float>::FromPolar(radius, angle);

            // Convert polar to Cartesian
            const float expectedX = radius * std::cos(angle);
            const float expectedY = radius * std::sin(angle);

            return std::abs(result.m_x - expectedX) < 0.0001f && std::abs(result.m_y - expectedY) < 0.0001f;
        }

        //static bool TestToPolar() comve back to this later...
        //{
        //    Vector2<float> cartesianVector(0.0f, 5.0f);
        //    Vector2<float> polarVector = cartesianVector.ToPolar();
        //    Vector2<float> expectedResult(5.0f, 0.0001f);
        //    return expectedResult == polarVector;
        //}

        static bool TestApproximatelyEquals()
        {
            Vector2<float> v1(500.0f, 500.f);
            return v1.ApproximatelyEquals({ 500.1f, 500.1f }, 1.0f);
        }

        static bool TestDot()
        {
            Vector2<float> v1(1.0f, 0.0f);
            Vector2<float> v2(0.0f, 1.0f);

            // Perpendicular check If two vectors are perpendicular their dot product is always 0
            return v1.Dot(v2) == 0.0f;  
        }

        static bool TestCross()
        {
            Vector2<float> v1(1.0f, 0.0f);
            Vector2<float> v2(0.0f, 1.0f);

            // The cross product of perpendicular vectors should be 1
            float result = v1.Cross(v2);

            return result == 1.0f;  // 1 * 1 - 0 * 0 = 1
        }

        static bool TestAngleBetween()
        {
            Vector2<float> v1(1.0f, 0.0f);
            Vector2<float> v2(0.0f, 1.0f); 

            // Angle between v1 and v2 should be 90 degrees pi/2 radians
            float angle = v1.AngleBetween(v2);  // Should be pi/2

            return std::abs( angle - PI / 2.0f) < 0.0001;
        }

        static bool TestProjection()
        {
            Vector2<float> v1(3.0f, 4.0f);
            Vector2<float> v2(1.0f, 0.0f); 

            // Project v1 onto v2
            Vector2<float> projection = v1.ProjectOnto(v2);
            Vector2<float> expected(3.0f, 0.0f);

            // Check result TODO: std::abs wrap
            return projection == expected;
        }

        static bool TestOrthogonalProjection()
        {
            Vector2<float> v1(3.0f, 4.0f);
            Vector2<float> v2(1.0f, 0.0f);

            //the projection should be v1 - the projection of v1 onto v2
            Vector2<float> orthogonalProjection = v1.OrthogonalProjectOnto(v2);

            // The expected orthogonal projection should be the Y component of v1
            Vector2<float> expected(0.0f, 4.0f);  // Perpendicular to v2 x

            // Check result TODO: std::abs wrap
            return orthogonalProjection == expected;
        }

        static bool TestProjectOntoClamped()
        {
            Vector2<float> v1(3.0f, 4.0f);  // Vector to project
            Vector2<float> v2(1.0f, 0.0f);  // Projection target x
            constexpr float kMinLength = 2.0f;
            constexpr float kMaxLength = 4.0f;

            Vector2<float> result = v1.ProjectOntoClamped(v2, kMinLength, kMaxLength);
            const float resultLength = result.Length();

            // Check if the length is with in the range
            constexpr float kTolerance = 0.0001f;
            const bool isLengthValid = resultLength >= kMinLength - kTolerance && resultLength <= kMaxLength + kTolerance;

            // Check if the direction is correct,
            // if two vectors have the same direction normalized versions be equal
            const bool isDirectionValid = (result.Normalize() == v2.Normalize());

            return isLengthValid && isDirectionValid;
        }

        static bool TestPerpendicular()
        {
            Vector2<float> v1(3.0f, 4.0f);
            Vector2<float> expected(-4.0f, 3.0f);

            // Get the perpendicular vector
            const Vector2<float> result = v1.Perpendicular();

            // Check if the result matches the expected perpendicular vector
            constexpr float kTolerance = 0.0001f;
            return result.m_x - expected.m_x < kTolerance && result.m_y - expected.m_y < kTolerance;
        }

        static bool TestReflect()
        {
            const Vector2<float> v1(1.0f, -1.0f);           // Incoming vector
            const Vector2<float> normal(0.0f, 1.0f);        // Reflect Y
            const Vector2<float> expected(1.0f, 1.0f);      // Expected reflection

            const Vector2<float> reflected = v1.Reflect(normal);

            return reflected == expected;
        }

        static bool TestReflectClamped()
        {
            const Vector2<float> v1(3.0f, -4.0f);           // Incoming vector
            const Vector2<float> normal(0.0f, 1.0f);        // Reflect Y
            Vector2<float> expected(3.0f, 4.0f);            // Expected reflection without clamping
            constexpr float kMaxLength = 5.0f;


            Vector2<float> reflected = v1.ReflectClamped(normal, kMaxLength);
            const float reflectedLength = reflected.Length();

            // Check if the length is with in the range
            constexpr float kTolerance = 0.0001f;
            const bool isLengthValid = reflectedLength <= kMaxLength + kTolerance;

            // Check direction should not change direction
            const bool isDirectionValid = reflected.Normalize() == expected.Normalize();

            return isLengthValid && isDirectionValid;
        }

        static bool TestLerp()
        {
            const Vector2<float> start(0.0f, 0.0f);
            const Vector2<float> end(10.0f, 10.0f);
            const Vector2<float> half(5.0f, 5.0f);

            // Test t = start
            const Vector2<float> resultStart = Vector2<float>::Lerp(start, end, 0.0f);
            const bool testStart = resultStart == start;

            // Test t = end
            const Vector2<float> resultEnd = Vector2<float>::Lerp(start, end, 1.0f);
            const bool testEnd = resultEnd == end;

            // Test t = 0.5 halfway between start end end
            const Vector2<float> resultHalf = Vector2<float>::Lerp(start, end, 0.5f);
            const bool testHalf = resultHalf == half;

            return testStart && testEnd && testHalf;
        }

        static bool TestRotate()
        {
            const Vector2<float> v(1.0f, 0.0f);              // Start
            const Vector2<float> expected(0.0f, 1.0f);       // Expected result
            constexpr float kTolerance = 0.0001f;

            const Vector2<float> rotated = v.Rotate(static_cast<float>(PI / 2.0));  // 90degrees pi/2
            return rotated.ApproximatelyEquals(expected, kTolerance);
        }

        static bool TestRotateAround()
        {
            const Vector2<float> v(3.0f, 3.0f);         // Start
            const Vector2<float> pivot(1.0f, 1.0f);     // Rotation pivot/ anchor point
            const Vector2<float> expected(-1.0f, 3.0f); // Expected result after 90-degree rotation
            constexpr float kTolerance = 0.0001f;

            // Rotate v around pivot by 90 degree
            const Vector2<float> rotated = v.RotateAround(pivot, static_cast<float>(PI / 2.0));
            return rotated.ApproximatelyEquals(expected, kTolerance);
        }

        static bool TestRotateDegrees()
        {
            const Vector2<float> v(1.0f, 0.0f);                 // Start
            const Vector2<float> expected(0.0f, 1.0f);          // Expected result
            constexpr float kTolerance = 0.0001f;

            const Vector2<float> rotated = v.RotateDegrees(90.0f);  // 90degrees pi/2
            return rotated.ApproximatelyEquals(expected, kTolerance);
        }

        static bool TestApplyRotationMatrix()
        {
            const Vector2<float> v(1.0f, 0.0f);         // Original vector
            const Vector2<float> expected(0.0f, 1.0f);  // 90degree
            constexpr float kTolerance = 0.0001f;

            // 90degree rotation matrix
            constexpr float kRotationMatrix[2][2] = 
            {
                {0.0f, -1.0f},
                {1.0f,  0.0f}
            };

            // Apply rotation
            const Vector2<float> rotated = v.ApplyRotationMatrix(kRotationMatrix);

            // Check if result is approximately the expected value
            return rotated.ApproximatelyEquals(expected, kTolerance);
        }

    public:

        static void RegisterEngineTests(UnitTestSystem* pTestSystem)
        {
            pTestSystem->AddTest("Vector2 Addition", TestVector2Addition);
            pTestSystem->AddTest("Vector2 Normalization", TestVector2Normalization);
            pTestSystem->AddTest("Vector2 FromPolar", TestFromPolar);
            // pTestSystem->AddTest("Vector2 ToPolar", TestToPolar);
            pTestSystem->AddTest("Vector2 ApproximatelyEquals", TestApproximatelyEquals);
            pTestSystem->AddTest("Vector2 Dot", TestDot);
            pTestSystem->AddTest("Vector2 Cross", TestCross);
            pTestSystem->AddTest("Vector2 AngleBetween", TestAngleBetween);
            pTestSystem->AddTest("Vector2 TestProjection", TestProjection);
            pTestSystem->AddTest("Vector2 TestOrthogonalProjection", TestOrthogonalProjection);
            pTestSystem->AddTest("Vector2 ProjectionOntoClamped", TestProjectOntoClamped);
            pTestSystem->AddTest("Vector2 TestPerpendicular", TestPerpendicular);
            pTestSystem->AddTest("Vector2 TestReflect", TestReflect);
            pTestSystem->AddTest("Vector2 TestReflectClamped", TestReflectClamped);
            pTestSystem->AddTest("Vector2 TestLerp", TestLerp);
            pTestSystem->AddTest("Vector2 TestRotate", TestRotate);
            pTestSystem->AddTest("Vector2 TestRotateAround", TestRotateAround);
            pTestSystem->AddTest("Vector2 TestRotateDegrees", TestRotateDegrees);
            pTestSystem->AddTest("Vector2 TestApplyRotationMatrix", TestApplyRotationMatrix);
        }
    };

}



