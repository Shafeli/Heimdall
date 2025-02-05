#pragma once

#include <algorithm>
#include "Vector2.h"

namespace Brokkr
{
    template<typename TypeName>
    class Line
    {
        Vector2<TypeName> m_start;
        Vector2<TypeName> m_end;

    public:
        Line() = default;

        Line(const Vector2<TypeName>& start, const Vector2<TypeName>& end)
            : m_start(start), m_end(end) {}

        [[nodiscard]] Vector2<TypeName> GetStart() const { return m_start; }
        [[nodiscard]] Vector2<TypeName> GetEnd() const { return m_end; }

        void SetStart(const Vector2<TypeName>& start) { m_start = start; }
        void SetEnd(const Vector2<TypeName>& end) { m_end = end; }

        [[nodiscard]] TypeName Length() const
        {
            return (m_end - m_start).Length();
        }

        [[nodiscard]] Vector2<TypeName> Midpoint() const
        {
            return (m_start + m_end) / 2;
        }

        [[nodiscard]] TypeName DistanceToPoint(const Vector2<TypeName>& point) const
        {
            Vector2<TypeName> ab = m_end - m_start;
            Vector2<TypeName> ac = point - m_start;
            TypeName projection = ac.Dot(ab) / ab.LengthSquared();

            // Clamp projection to line segment bounds [0, 1]
            projection = std::clamp(projection, TypeName(0), TypeName(1));

            // Find the closest point on the line segment
            Vector2<TypeName> closestPoint = m_start + ab * projection;

            // Return the distance between the point and the closest point
            return (closestPoint - point).Length();
        }

        [[nodiscard]] bool ContainsPoint(const Vector2<TypeName>& point) const
        {
            Vector2<TypeName> ab = m_end - m_start;
            Vector2<TypeName> ap = point - m_start;

            // Check if the point lies within the bounds of the line segment
            TypeName projection = ap.Dot(ab) / ab.LengthSquared();
            return projection >= 0 && projection <= 1 && (ap.Cross(ab) == 0);
        }

        // Line &Line Intersection
        [[nodiscard]] bool Intersects(const Line<TypeName>& other, Vector2<TypeName>& outIntersection) const
        {
            Vector2<TypeName> p = m_start;
            Vector2<TypeName> q = other.m_start;
            Vector2<TypeName> r = m_end - m_start;
            Vector2<TypeName> s = other.m_end - other.m_start;

            TypeName rCrossS = r.Cross(s);
            if (rCrossS == 0) return false; // Parallel or collinear lines

            TypeName t = (q - p).Cross(s) / rCrossS;
            TypeName u = (q - p).Cross(r) / rCrossS;

            if (t >= 0 && t <= 1 && u >= 0 && u <= 1)
            {
                outIntersection = p + r * t;
                return true;
            }

            return false;
        }
    };
}

