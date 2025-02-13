#pragma once
#include <cmath>
#include <random>
#include <sstream>
#include <vector>
#include "Core/EngineDefinitions.h"

// Sources:
// https://www.haroldserrano.com/blog/developing-a-math-engine-in-c-implementing-vectors - Developing a Math Engine
namespace Brokkr
{
    template <typename TypeName>
    class Vector2
    {
        // constant small epsilon value to avoid division by zero
        static constexpr TypeName kEPSILON = static_cast<TypeName>(1e-6);

    public:
        TypeName m_x, m_y;

        // construction
        Vector2() : m_x(TypeName(0)), m_y(TypeName(0)) {}
        Vector2(const Vector2<TypeName>& v2) = default;
        Vector2(const TypeName x, const TypeName y) : m_x(x), m_y(y) {}

        // prime operators
        Vector2<TypeName> operator+(const Vector2<TypeName>& other) const { return Vector2<TypeName>(m_x + other.m_x, m_y + other.m_y); }
        Vector2<TypeName> operator-(const Vector2<TypeName>& other) const { return Vector2<TypeName>(m_x - other.m_x, m_y - other.m_y); }
        Vector2<TypeName> operator*(TypeName scalar) const { return Vector2<TypeName>(m_x * scalar, m_y * scalar); }
        Vector2<TypeName> operator*(const TypeName matrix[2][2]) const
        {
            return ApplyRotationMatrix(matrix);
        }

        Vector2<TypeName> operator/(TypeName scalar) const
        {
            assert(scalar != TypeName(0)); // division by zero
            return Vector2<TypeName>(m_x / scalar, m_y / scalar);
        }

        Vector2<TypeName>& operator/=(TypeName scalar)
        {
            assert(scalar != TypeName(0));
            m_x /= scalar;
            m_y /= scalar;
            return *this;
        }
        Vector2<TypeName>& operator=(const Vector2<TypeName>& other) = default;
        Vector2<TypeName>& operator*=(TypeName scalar) { m_x *= scalar; m_y *= scalar; return (*this); }
        bool operator==(const Vector2<TypeName>& other) const { return (m_x == other.m_x && m_y == other.m_y); }
        bool operator!=(const Vector2<TypeName>& other) const { return (m_x != other.m_x || m_y != other.m_y); }
        Vector2<TypeName>& operator+=(const Vector2<TypeName>& other) { m_x += other.m_x; m_y += other.m_y; return (*this); }
        Vector2<TypeName>& operator-=(const Vector2<TypeName>& other) { m_x -= other.m_x; m_y -= other.m_y; return (*this); }
        Vector2<TypeName> operator-() const { return Vector2<TypeName>(-m_x, -m_y); }  // inverse of the vector 

        ///////////////////////////////////////////////////////////////////////////////////////////////////////////////
        // Math
        ///////////////////////////////////////////////////////////////////////////////////////////////////////////////

        // Convert from polar to Cartesian and return a Vector2
        static Vector2<TypeName> FromPolar(TypeName radius, TypeName angleRadians)
        {
            TypeName x = radius * std::cos(angleRadians);
            TypeName y = radius * std::sin(angleRadians);
            return Vector2<TypeName>(x, y);
        }

        // TODO: Vector2 to polar
        // Convert this Vector2 from Cartesian to Polar and return as (radius, angleRadians)
        //[[nodiscard]] Vector2<TypeName> ToPolar() const
        //{
        //    TypeName radius = Length();
        //    TypeName angleRadians = std::atan2(m_y, m_x);
        //    return  Vector2<TypeName>(radius, angleRadians);
        //}

        [[nodiscard]] TypeName Length() const
        {
            return { std::sqrt((m_x * m_x) + (m_y * m_y)) };
        }

        [[nodiscard]] TypeName LengthSquared() const { return ((m_x * m_x) + (m_y * m_y)); }

        [[nodiscard]] Vector2<TypeName> Normalize()
        {
            TypeName x = m_x;
            TypeName y = m_y;
            const TypeName len = Length();

            if (len > kEPSILON)
            {
                x /= len;
                y /= len;
            }
            else
            {
                return Vector2(); // default is a zero vector
            }

            return Vector2<TypeName>(x, y);
        }

        // Updated IsExtremelyCloseTo adjustable tolerance
        [[nodiscard]] bool ApproximatelyEquals(const Vector2<TypeName>& other, TypeName tolerance = kEPSILON) const
        {
            return std::abs(m_x - other.m_x) < tolerance && std::abs(m_y - other.m_y) < tolerance;
        }

        // Pretty sure this is from GAP295 
        [[nodiscard]] bool IsExtremelyCloseTo(const Vector2<TypeName>& right) const
        {
            if (const Vector2<TypeName> diff = right - (*this); diff.Length() < 0.01f)
                return true;
            return false;
        }

        [[nodiscard]] TypeName Dot(const Vector2<TypeName>& other) const
        {
            return (m_x * other.m_x) + (m_y * other.m_y);
        }

        [[nodiscard]] TypeName Cross(const Vector2<TypeName>& other) const
        {
            return (m_x * other.m_y) - (m_y * other.m_x);
        }

        // Calculated between two vectors in radians
        [[nodiscard]] TypeName AngleBetween(const Vector2<TypeName>& other) const
        {
            const TypeName dot = Dot(other);
            const TypeName l1 = Length();
            const TypeName l2 = other.Length();
            if (l1 > kEPSILON && l2 > kEPSILON) 
            {
                return std::acos(dot / (l1 * l2)); // inverse cosine
            }
            return TypeName(0);
        }

        [[nodiscard]] Vector2<TypeName> ProjectOnto(const Vector2<TypeName>& other) const
        {
            const TypeName scalar = Dot(other) / other.LengthSquared();
            return other * scalar;
        }

        [[nodiscard]] Vector2<TypeName> OrthogonalProjectOnto(const Vector2<TypeName>& other) const
        {
            return (*this) - ProjectOnto(other);
        }

        // Clamp the projection result to a range
        [[nodiscard]] Vector2<TypeName> ProjectOntoClamped(const Vector2<TypeName>& other, TypeName minLength, TypeName maxLength) const
        {
            //Vector2<TypeName> projection = ProjectOnto(other);
            //Vector2<TypeName> normProjection = projection.Normalize();
            //TypeName length = projection.Length();
            //if (length < minLength) 
            //{
            //    normProjection *= minLength;
            //}
            //else if (length > maxLength) 
            //{
            //    normProjection *= maxLength;
            //}
            //return normProjection;

            Vector2<TypeName> projection = ProjectOnto(other);
            TypeName length = projection.Length();

            if (length < minLength)
            {
                projection = projection.Normalize() * minLength;
            }
            else if (length > maxLength)
            {
                projection = projection.Normalize() * maxLength;
            }

            return projection;
        }

        // Get a perpendicular vector
        [[nodiscard]] Vector2<TypeName> Perpendicular() const
        {
            return Vector2<TypeName>(-m_y, m_x);
        }

        //TODO: Vector2 Unit Tests needed for everything after this
        // Reflect this vector across a normal
        [[nodiscard]] Vector2<TypeName> Reflect(const Vector2<TypeName>& normal) const
        {
            // Lerp(v1,v2,t)=v1?(1?t)+v2?t
            return (*this) - 2 * Dot(normal) * normal;
        }

        // reflect that clamps the reflected vector's length to a maximum
        [[nodiscard]] Vector2<TypeName> ReflectClamped(const Vector2<TypeName>& normal, TypeName maxLength) const
        {
            Vector2<TypeName> reflected = Reflect(normal);
            reflected.ClampMagnitude(maxLength);
            return reflected;
        }

        // Vector2<float>v1(0.0f, 0.0f);
        // Vector2<float>v2(10.0f, 10.0f);
        // float t = 0.25f 25% of the way from v1 to v2
        // Vector2<float> result = v1.Lerp(v2, t);
        // Result will be (2.5, 2.5), 25% of the way from (0, 0) to (10, 10)
        // Interpolate between this vector and another vector 
        [[nodiscard]] Vector2<TypeName> Lerp(const Vector2<TypeName>& target, TypeName t) const
        {
            return (*this) * (1 - t) + target * t;
        }

        // Interpolate between this vector and another vector 
        [[nodiscard]] Vector2<TypeName> ClampLerp(const Vector2<TypeName>& target, TypeName t, TypeName minT = 0.0f, TypeName maxT = 1.0f) const
        {
            t = std::clamp(t, minT, maxT);
            return (*this) * (1 - t) + target * t;
        }

        // Rotate the vector by an angle in radians
        [[nodiscard]] Vector2<TypeName> Rotate(TypeName angleRadians) const
        {
            const TypeName cosAngle = std::cos(angleRadians);
            const TypeName sinAngle = std::sin(angleRadians);
            return Vector2<TypeName>(
                m_x * cosAngle - m_y * sinAngle,
                m_x * sinAngle + m_y * cosAngle
                );
        }

        // rotate the vector around a point
        [[nodiscard]] Vector2<TypeName> RotateAround(const Vector2<TypeName>& point, TypeName angleRadians) const
        {
            Vector2<TypeName> translated = (*this) - point;
            translated = translated.Rotate(angleRadians);
            return translated + point;
        }

        [[nodiscard]] Vector2<TypeName> RotateDegrees(TypeName angleDegrees) const
        {
            return Rotate(angleDegrees * static_cast<TypeName>(PI / 180.0));
        }

        // Transform Vector by a rotation matrix
        [[nodiscard]] Vector2<TypeName> ApplyRotationMatrix(const TypeName matrix[2][2]) const
        {
            return Vector2<TypeName>(
                m_x * matrix[0][0] + m_y * matrix[0][1],
                m_x * matrix[1][0] + m_y * matrix[1][1]
                );
        }

        // apply both rotation and translation
        [[nodiscard]] Vector2<TypeName> Transform(const Vector2<TypeName>& translation, TypeName angleRadians) const
        {
            return Rotate(angleRadians) + translation;
        }

        // Generate a array of vectors equally spaced between two vectors
        static std::vector<Vector2<TypeName>> LinearSpace(const Vector2<TypeName>& start, const Vector2<TypeName>& end, int numSteps)
        {
            std::vector<Vector2<TypeName>> result;
            if (numSteps <= 1) return { start };

            Vector2<TypeName> step = (end - start) * (1.0 / (numSteps - 1));
            for (int i = 0; i < numSteps; ++i)
            {
                result.push_back(start + step * static_cast<TypeName>(i));
            }
            return result;
        }

        // Generate a random Vector2 with in range
        static Vector2<TypeName> Random(TypeName minX, TypeName maxX, TypeName minY, TypeName maxY)
        {
            static std::random_device rd;
            static std::mt19937 gen(rd());
            std::uniform_real_distribution<TypeName> disX(minX, maxX);
            std::uniform_real_distribution<TypeName> disY(minY, maxY);
            return Vector2<TypeName>(disX(gen), disY(gen));
        }

        // if the vector is a zero vector
        [[nodiscard]] bool IsZero(TypeName tolerance = 1e-6) const
        {
            return std::abs(m_x) < tolerance && std::abs(m_y) < tolerance;
        }

        [[nodiscard]] Vector2<TypeName> MinComponents(const Vector2<TypeName>& other) const
        {
            return Vector2<TypeName>(std::min(m_x, other.m_x), std::min(m_y, other.m_y));
        }

        [[nodiscard]] Vector2<TypeName> MaxComponents(const Vector2<TypeName>& other) const
        {
            return Vector2<TypeName>(std::max(m_x, other.m_x), std::max(m_y, other.m_y));
        }

        [[nodiscard]] TypeName DistanceTo(const Vector2<TypeName>& other) const
        {
            return (*this - other).Length();
        }

        /*
        What is Manhattan Distance?
        Manhattan distance measures the total distance between two points when only
        horizontal and vertical movements are allowed.
        */
        [[nodiscard]] TypeName ManhattanDistance(const Vector2<TypeName>& other) const
        {
            return std::abs(m_x - other.m_x) + std::abs(m_y - other.m_y);
        }

        // convert the vector to a string format Vector2(x,y)
        [[nodiscard]] std::string ToString() const
        {
            return "Vector2(" + std::to_string(m_x) + ", " + std::to_string(m_y) + ")";
        }

        // To CSV format x , y
        std::string Serialize() const
        {
            return std::to_string(m_x) + "," + std::to_string(m_y);
        }

        // string data to a Vector2 "number,number"
        static Vector2<TypeName> Deserialize(const std::string& data)
        {
            //
        }

    };

    template <typename TypeName>
    Vector2<TypeName> operator+(const Vector2<TypeName>& left, const Vector2<TypeName>& right)
    {
        return Vector2<TypeName>(left.m_x + right.m_x, left.m_y + right.m_y);
    }

    template <typename TypeName>
    Vector2<TypeName> operator-(const Vector2<TypeName>& left, const Vector2<TypeName>& right)
    {
        return Vector2<TypeName>(left.m_x - right.m_x, left.m_y - right.m_y);
    }

    template <typename TypeName>
    Vector2<TypeName> operator*(const Vector2<TypeName>& left, const Vector2<TypeName>& right)
    {
        return Vector2<TypeName>(left.m_x * right.m_x, left.m_y * right.m_y);
    }

    template <typename TypeName>
    Vector2<TypeName> operator*(const Vector2<TypeName>& left, const TypeName right)
    {
        return Vector2<TypeName>(left.m_x * right, left.m_y * right);
    }

    template <typename TypeName>
    Vector2<TypeName> operator*(const TypeName left, const Vector2<TypeName>& right)
    {
        return Vector2<TypeName>(left * right.m_x, left * right.m_y);
    }

    template <typename TypeName>
    Vector2<TypeName> operator/(const Vector2<TypeName>& left, const TypeName right)
    {
        return Vector2<TypeName>(left.m_x / right, left.m_y / right);
    }

}
