#pragma once
#include <cmath>
#include <random>
#include <sstream>
#include <vector>
#define PI    3.14159265358979323846264338327950288   /**< pi */

namespace Brokkr
{
    template <typename TypeName>
    class Vector2
    {
        // constant small epsilon value to avoid division by zero
        static constexpr TypeName kEPSILON = static_cast<TypeName>(1e-6);

    public:
        TypeName x, y;

        // construction
        Vector2() : x(TypeName(0)), y(TypeName(0)) {}
        Vector2(const Vector2<TypeName>& v2) = default;
        Vector2(const TypeName x, const TypeName y) : x(x), y(y) {}

        // prime operators
        Vector2<TypeName> operator+(const Vector2<TypeName>& other) const { return Vector2<TypeName>(x + other.x, y + other.y); }
        Vector2<TypeName> operator-(const Vector2<TypeName>& other) const { return Vector2<TypeName>(x - other.x, y - other.y); }
        Vector2<TypeName> operator*(TypeName scalar) const { return Vector2<TypeName>(x * scalar, y * scalar); }
        Vector2<TypeName> operator*(const TypeName matrix[2][2]) const {
            return ApplyRotationMatrix(matrix);
        }

        Vector2<TypeName> operator/(TypeName scalar) const {
            assert(scalar != TypeName(0)); // division by zero
            return Vector2<TypeName>(x / scalar, y / scalar);
        }
        Vector2<TypeName>& operator/=(TypeName scalar) {
            assert(scalar != TypeName(0));
            x /= scalar;
            y /= scalar;
            return *this;
        }
        Vector2<TypeName>& operator=(const Vector2<TypeName>& other) = default;
        Vector2<TypeName>& operator*=(TypeName scalar) { x *= scalar; y *= scalar; return (*this); }
        bool operator==(const Vector2<TypeName>& other) const { return (x == other.x && y == other.y); }
        bool operator!=(const Vector2<TypeName>& other) const { return (x != other.x || y != other.y); }
        Vector2<TypeName>& operator+=(const Vector2<TypeName>& other) { x += other.x; y += other.y; return (*this); }
        Vector2<TypeName>& operator-=(const Vector2<TypeName>& other) { x -= other.x; y -= other.y; return (*this); }
        Vector2<TypeName> operator-() const { return Vector2<TypeName>(-x, -y); }  // inverse of the vector 

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

        // Convert this Vector2 from Cartesian to Polar and return as a pair (radius, angleRadians)
        [[nodiscard]] std::pair<TypeName, TypeName> ToPolar() const
        {
            TypeName radius = Length();
            TypeName angleRadians = std::atan2(y, x);
            return { radius, angleRadians };
        }

        [[nodiscard]] TypeName Length() const
        {
            return std::sqrt((x * x) + (y * y));
        }

        [[nodiscard]] TypeName LengthSquared() const { return ((x * x) + (y * y)); }

        void Normalize()
        {
            const TypeName len = Length();
            if (len > kEPSILON)
            {
                x /= len;
                y /= len;
            }
        }

        // For operations that require an approximate equality checks
        [[nodiscard]] bool ApproximatelyEquals(const Vector2<TypeName>& other, TypeName tolerance = kEPSILON) const
        {
            return std::abs(x - other.x) < tolerance && std::abs(y - other.y) < tolerance;
        }

        [[nodiscard]] bool IsExtremelyCloseTo(const Vector2<TypeName>& right) const
        {
            if (const Vector2<TypeName> diff = right - (*this); diff.Length() < 0.01f)
                return true;
            return false;
        }

        [[nodiscard]] TypeName Dot(const Vector2<TypeName>& other) const
        {
            return (x * other.x) + (y * other.y);
        }

        [[nodiscard]] TypeName Cross(const Vector2<TypeName>& other) const
        {
            return (x * other.y) - (y * other.x);
        }

        // Calculated between two vectors in radians
        [[nodiscard]] TypeName AngleBetween(const Vector2<TypeName>& other) const
        {
            const TypeName dot = Dot(other);
            const TypeName l1 = Length();
            const TypeName l2 = other.Length();
            if (l1 > kEPSILON && l2 > kEPSILON) {
                return std::acos(dot / (l1 * l2));
            }
            return TypeName(0);
        }

        [[nodiscard]] Vector2<TypeName> OrthogonalProjectOnto(const Vector2<TypeName>& other) const
        {
            return (*this) - ProjectOnto(other);
        }

        [[nodiscard]] Vector2<TypeName> ProjectOnto(const Vector2<TypeName>& other) const
        {
            const TypeName scalar = Dot(other) / other.LengthSquared();
            return other * scalar;
        }

        // Clamp the projection result to a range
        [[nodiscard]] Vector2<TypeName> ProjectOntoClamped(const Vector2<TypeName>& other, TypeName minLength, TypeName maxLength) const
        {
            Vector2<TypeName> projection = ProjectOnto(other);
            TypeName length = projection.Length();
            if (length < minLength) {
                projection.Normalize();
                projection *= minLength;
            }
            else if (length > maxLength) {
                projection.Normalize();
                projection *= maxLength;
            }
            return projection;
        }

        // Get a perpendicular vector
        [[nodiscard]] Vector2<TypeName> Perpendicular() const
        {
            return Vector2<TypeName>(-y, x);
        }

        // Generate a array of vectors linearly spaced between two vectors
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

        TypeName EaseIn(TypeName t) const { return t * t; }
        TypeName EaseOut(TypeName t) const { return t * (2 - t); }
        TypeName EaseInOut(TypeName t) const { return (t < 0.5) ? (2 * t * t) : (-1 + (4 - 2 * t) * t); }

        [[nodiscard]] Vector2<TypeName> LerpEaseIn(const Vector2<TypeName>& target, TypeName t) const
        {
            t = EaseIn(t);
            return Lerp(target, t);
        }

        // Rotate the vector by an angle in radians
        [[nodiscard]] Vector2<TypeName> Rotate(TypeName angleRadians) const
        {
            const TypeName cosAngle = std::cos(angleRadians);
            const TypeName sinAngle = std::sin(angleRadians);
            return Vector2<TypeName>(
                x * cosAngle - y * sinAngle,
                x * sinAngle + y * cosAngle
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
                x * matrix[0][0] + y * matrix[0][1],
                x * matrix[1][0] + y * matrix[1][1]
                );
        }

        // apply both rotation and translation
        Vector2<TypeName> Transform(const Vector2<TypeName>& translation, TypeName angleRadians) const {
            return Rotate(angleRadians) + translation;
        }

        // Clamp the vector's magnitude to a maximum value
        void ClampMagnitude(TypeName maxMagnitude)
        {
            if (Length() > maxMagnitude)
            {
                Normalize();
                (*this) *= maxMagnitude;
            }
        }

        // return "swizzled" or swapped x and y versions of the vector
        [[nodiscard]] Vector2<TypeName> YX() const { return Vector2<TypeName>(y, x); }

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
            return std::abs(x) < tolerance && std::abs(y) < tolerance;
        }

        [[nodiscard]] Vector2<TypeName> MinComponents(const Vector2<TypeName>& other) const
        {
            return Vector2<TypeName>(std::min(x, other.x), std::min(y, other.y));
        }

        [[nodiscard]] Vector2<TypeName> MaxComponents(const Vector2<TypeName>& other) const
        {
            return Vector2<TypeName>(std::max(x, other.x), std::max(y, other.y));
        }

        [[nodiscard]] TypeName DistanceTo(const Vector2<TypeName>& other) const
        {
            return (*this - other).Length();
        }

        /*
        What is Manhattan Distance?
        Manhattan distance measures the total distance between two points when only
        horizontal and vertical movements are allowed. It's named after the
        Manhattan borough of New York City sense it has a grid-like street layout.
        */
        [[nodiscard]] TypeName ManhattanDistance(const Vector2<TypeName>& other) const
        {
            return std::abs(x - other.x) + std::abs(y - other.y);
        }

        // convert the vector to a string for easy debugging
        [[nodiscard]] std::string ToString() const
        {
            return "Vector2(" + std::to_string(x) + ", " + std::to_string(y) + ")";
        }

        std::string Serialize() const
        {
            return std::to_string(x) + "," + std::to_string(y);
        }

        // string data to a Vector2 "number,number"
        static Vector2<TypeName> Deserialize(const std::string& data)
        {
            const size_t commaPos = data.find(',');
            TypeName x = static_cast<TypeName>(std::stod(data.substr(0, commaPos)));
            TypeName y = static_cast<TypeName>(std::stod(data.substr(commaPos + 1)));
            return Vector2<TypeName>(x, y);
        }

    };

    template <typename TypeName>
    Vector2<TypeName> operator+(const Vector2<TypeName>& left, const Vector2<TypeName>& right)
    {
        return Vector2<TypeName>(left.x + right.x, left.y + right.y);
    }

    template <typename TypeName>
    Vector2<TypeName> operator-(const Vector2<TypeName>& left, const Vector2<TypeName>& right)
    {
        return Vector2<TypeName>(left.x - right.x, left.y - right.y);
    }

    template <typename TypeName>
    Vector2<TypeName> operator*(const Vector2<TypeName>& left, const Vector2<TypeName>& right)
    {
        return Vector2<TypeName>(left.x * right.x, left.y * right.y);
    }

    template <typename TypeName>
    Vector2<TypeName> operator*(const Vector2<TypeName>& left, const TypeName right)
    {
        return Vector2<TypeName>(left.x * right, left.y * right);
    }

    template <typename TypeName>
    Vector2<TypeName> operator*(const TypeName left, const Vector2<TypeName>& right)
    {
        return Vector2<TypeName>(left * right.x, left * right.y);
    }

    template <typename TypeName>
    Vector2<TypeName> operator/(const Vector2<TypeName>& left, const TypeName right)
    {
        return Vector2<TypeName>(left.x / right, left.y / right);
    }

}
