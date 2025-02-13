#pragma once
#include "Rectangle.h"
#include "Triangle.h"
#include "Vector2.h"

namespace Brokkr
{
    template<typename TypeName>
    class Circle
    {
        Vector2<TypeName> m_center;
        TypeName m_radius;

    public:
        Circle() = default;

        Circle(const Vector2<TypeName>& center, TypeName radius)
            : m_center(center), m_radius(radius) {}

        [[nodiscard]] TypeName GetRadius() const { return m_radius; }
        [[nodiscard]] Vector2<TypeName> GetCenter() const { return m_center; }

        void SetRadius(TypeName radius) { m_radius = radius; }
        void SetCenter(const Vector2<TypeName>& center) { m_center = center; }

        [[nodiscard]] bool Contains(const Vector2<TypeName>& point) const
        {
            return (point - m_center).LengthSquared() <= m_radius * m_radius;
        }

        [[nodiscard]] bool Intersects(const Circle<TypeName>& other) const
        {
            TypeName distanceSquared = (m_center - other.m_center).LengthSquared();
            TypeName radiusSum = m_radius + other.m_radius;
            return distanceSquared <= radiusSum * radiusSum;
        }

        [[nodiscard]] bool Intersects(const Rectangle<TypeName>& rect) const
        {
            TypeName closestX = std::max(rect.GetLeft(), std::min(m_center.m_x, rect.GetRight()));
            TypeName closestY = std::max(rect.GetTop(), std::min(m_center.m_y, rect.GetBottom()));
            TypeName distanceSquared = (m_center.m_x - closestX) * (m_center.m_x - closestX) +
                (m_center.m_y - closestY) * (m_center.m_y - closestY);
            return distanceSquared <= m_radius * m_radius;
        }

        [[nodiscard]] bool Intersects(const Triangle<TypeName>& triangle) const
        {
            // Check if the circle center is inside the triangle
            if (triangle.Contains(m_center)) return true;

            // Check distance from circle to each edge of the triangle
            for (const auto& edge : triangle.GetEdges())
            {
                if (edge.DistanceToPoint(m_center) <= m_radius) return true;
            }

            return false;
        }

    };
}
