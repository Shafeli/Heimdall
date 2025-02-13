#pragma once
#include "Circle.h"
#include "Rectangle.h"
#include "Vector2.h"

namespace Brokkr
{
    template<typename TypeName>
    class Ray
    {
        Vector2<TypeName> m_origin;  // Starting point of the ray
        Vector2<TypeName> m_direction;  // Normalized direction vector

    public:
        Ray() = default;

        Ray(const Vector2<TypeName>& origin, const Vector2<TypeName>& direction)
            : m_origin(origin), m_direction(direction.Normalized()) {}

        [[nodiscard]] Vector2<TypeName> GetOrigin() const { return m_origin; }
        [[nodiscard]] Vector2<TypeName> GetDirection() const { return m_direction; }

        void SetOrigin(const Vector2<TypeName>& origin) { m_origin = origin; }
        void SetDirection(const Vector2<TypeName>& direction) { m_direction = direction.Normalized(); }

        [[nodiscard]] Vector2<TypeName> GetPoint(TypeName distance) const
        {
            return m_origin + m_direction * distance;
        }

        // Ray -> Point Distance
        [[nodiscard]] TypeName DistanceToPoint(const Vector2<TypeName>& point) const
        {
            Vector2<TypeName> originToPoint = point - m_origin;
            Vector2<TypeName> projection = m_direction * originToPoint.Dot(m_direction);
            Vector2<TypeName> closestPoint = m_origin + projection;
            return (point - closestPoint).Length();
        }

        // Ray & Rect Intersection If an intersection occurs compute the distance from the ray's
        // origin to the first point of intersection.
        [[nodiscard]] bool Intersects(const Rectangle<TypeName>& rect, TypeName& outDistance) const
        {
            TypeName tmin = (rect.GetLeft() - m_origin.m_x) / m_direction.m_x;
            TypeName tmax = (rect.GetRight() - m_origin.m_x) / m_direction.m_x;

            if (tmin > tmax) std::swap(tmin, tmax);

            TypeName tymin = (rect.GetTop() - m_origin.m_y) / m_direction.m_y;
            TypeName tymax = (rect.GetBottom() - m_origin.m_y) / m_direction.m_y;

            if (tymin > tymax) std::swap(tymin, tymax);

            if ((tmin > tymax) || (tymin > tmax)) return false;

            tmin = std::max(tmin, tymin);
            tmax = std::min(tmax, tymax);

            if (tmax < 0) return false;

            outDistance = tmin;
            return true;
        }

        // Ray & Circle Intersection If an intersection occurs compute the distance from the ray's
        // origin to the first point of intersection.
        [[nodiscard]] bool Intersects(const Circle<TypeName>& circle, TypeName& outDistance) const
        {
            Vector2<TypeName> originToCircle = circle.GetCenter() - m_origin;
            TypeName projectionLength = originToCircle.Dot(m_direction);
            Vector2<TypeName> closestPoint = m_origin + m_direction * projectionLength;

            TypeName distanceSquared = (circle.GetCenter() - closestPoint).LengthSquared();
            TypeName radiusSquared = circle.GetRadius() * circle.GetRadius();

            if (distanceSquared > radiusSquared) return false;

            TypeName offset = std::sqrt(radiusSquared - distanceSquared);
            outDistance = projectionLength - offset;

            return outDistance >= 0;
        }

        // TODO: // Implement Möller Trumbore algorithm for Triangle Intersects
    };
}

