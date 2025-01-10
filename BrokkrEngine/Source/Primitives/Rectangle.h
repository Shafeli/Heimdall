#pragma once
#include <algorithm>

#include "Triangle.h"
#include "Vector2.h"

namespace Brokkr
{
    template<typename TypeName>
    class Rectangle
    {
        Vector2<TypeName> m_size;
        Vector2<TypeName> m_position;

    public:
        Rectangle() = default;

        Rectangle(const Vector2<TypeName>& position, const Vector2<TypeName>& size)
            : m_size(size), m_position(position)
        {}

        [[nodiscard]] TypeName GetHeight() const { return m_size.y; }
        [[nodiscard]] TypeName GetWidth() const { return m_size.x; }
        [[nodiscard]] TypeName GetX() const { return m_position.x; }
        [[nodiscard]] TypeName GetY() const { return m_position.y; }

        void SetHeight(TypeName y) { m_size.y = y; }
        void SetWidth(TypeName x) { m_size.x = x; }
        void SetX(TypeName x) { m_position.x = x; }
        void SetY(TypeName y) { m_position.y = y; }

        void AdjustHeight(TypeName y) { m_size.y += y; }
        void AdjustWidth(TypeName x) { m_size.x += x; }
        void AdjustX(TypeName x) { m_position.x += x; }
        void AdjustY(TypeName y) { m_position.y += y; }

        [[nodiscard]] TypeName GetTop() const { return m_position.y; }
        [[nodiscard]] TypeName GetBottom() const { return m_position.y + m_size.y; }
        [[nodiscard]] TypeName GetLeft() const { return m_position.x; }
        [[nodiscard]] TypeName GetRight() const { return m_position.x + m_size.x; }

        [[nodiscard]] Vector2<TypeName> GetCenter() const
        {
            TypeName centerX = m_position.x + m_size.x / 2; 
            TypeName centerY = m_position.y + m_size.y / 2;
            return Vector2<TypeName>(centerX, centerY);
        }

        [[nodiscard]] TypeName GetAdjustedY(TypeName y) const { return m_position.y + y; }
        [[nodiscard]] TypeName GetAdjustedX(TypeName x) const { return m_position.x + x; }

        bool operator==(const Rectangle<TypeName>& other) const
        {
            return m_position == other.m_position && m_size == other.m_size;
        }

        void MoveTo(const Vector2<TypeName>& position)
        {
            m_position = position;
        }

        void Resize(TypeName width, TypeName height)
        {
            m_size = { width, height };
        }

        void Scale(TypeName factor)
        {
            m_size.x *= factor;
            m_size.y *= factor;
        }

        void Expand(TypeName factor)
        {
            m_position.x -= m_size.x * (factor - 1) / 2;
            m_position.y -= m_size.y * (factor - 1) / 2;
            m_size.x *= factor;
            m_size.y *= factor;
        }

        void ClampToBounds(const Rectangle<TypeName>& bounds)
        {
            m_position.x = std::max(bounds.GetLeft(), std::min(m_position.x, bounds.GetRight() - m_size.x));
            m_position.y = std::max(bounds.GetTop(), std::min(m_position.y, bounds.GetBottom() - m_size.y));
        }

        [[nodiscard]] bool Contains(const Vector2<TypeName>& point) const
        {
            return point.x >= m_position.x && point.x < m_position.x + m_size.x
                && point.y >= m_position.y && point.y < m_position.y + m_size.y;
        }

        [[nodiscard]] bool Intersects(const Rectangle<TypeName>& other) const
        {
            return m_position.x < other.m_position.x + other.m_size.x &&
                m_position.x + m_size.x > other.m_position.x &&
                m_position.y < other.m_position.y + other.m_size.y &&
                m_position.y + m_size.y > other.m_position.y;
        }

        [[nodiscard]] bool Intersects(const Triangle<TypeName>& triangle) const
        {
            // Check if any triangle vertex is inside the rectangle
            for (const auto& vertex : triangle.GetVertices())
            {
                if (Contains(vertex)) return true;
            }

            // Check if any rectangle edge intersects the triangle
            for (const auto& edge : GetEdges())
            {
                if (edge.Intersects(triangle)) return true;
            }

            return false;
        }


        // Returns a Rect of the two Rects intersection area
        [[nodiscard]] Rectangle<TypeName> Intersection(const Rectangle<TypeName>& other) const
        {
            // Coordinates of the top left corner of the intersection rect
            int x1 = std::max<TypeName>(m_position.x, other.m_position.x);
            int y1 = std::max<TypeName>(m_position.y, other.m_position.y);

            // Coordinates of the bottom right corner of the intersection rect
            const int x2 = std::min<TypeName>(m_position.x + m_size.x, other.m_position.x + other.m_size.x);
            const int y2 = std::min<TypeName>(m_position.y + m_size.y, other.m_position.y + other.m_size.y);

            // Check if the rects intersect
            if (x2 <= x1 || y2 <= y1) 
            {
                // The rects do not intersect
                return Rectangle({ 0, 0 }, { 0, 0 });
            }

            // return the intersection rects
            return Rectangle({ x1, y1 }, { x2 - x1, y2 - y1 });
        }

        void AddMargin(TypeName marginX, TypeName marginY)
        {
            m_position.x -= marginX;
            m_position.y -= marginY;
            m_size.x += marginX * 2;
            m_size.y += marginY * 2;
        }

        [[nodiscard]] TypeName DiagonalLength() const
        {
            return m_size.Length();
        }


        Vector2<TypeName>& GetPosition()
        {
            return m_position;
        }
    };
}
