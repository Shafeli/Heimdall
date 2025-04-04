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

        [[nodiscard]] TypeName GetHeight() const { return m_size.m_y; }
        [[nodiscard]] TypeName GetWidth() const { return m_size.m_x; }
        [[nodiscard]] TypeName GetX() const { return m_position.m_x; }
        [[nodiscard]] TypeName GetY() const { return m_position.m_y; }

        void SetHeight(TypeName y) { m_size.m_y = y; }
        void SetWidth(TypeName x) { m_size.m_x = x; }
        void SetX(TypeName x) { m_position.m_x = x; }
        void SetY(TypeName y) { m_position.m_y = y; }

        void AdjustHeight(TypeName y) { m_size.m_y += y; }
        void AdjustWidth(TypeName x) { m_size.m_x += x; }
        void AdjustX(TypeName x) { m_position.m_x += x; }
        void AdjustY(TypeName y) { m_position.m_y += y; }

        [[nodiscard]] TypeName GetTop() const { return m_position.m_y; }
        [[nodiscard]] TypeName GetBottom() const { return m_position.m_y + m_size.m_y; }
        [[nodiscard]] TypeName GetLeft() const { return m_position.m_x; }
        [[nodiscard]] TypeName GetRight() const { return m_position.m_x + m_size.m_x; }

        [[nodiscard]] Vector2<TypeName> GetCenter() const
        {
            TypeName centerX = m_position.m_x + m_size.m_x / 2; 
            TypeName centerY = m_position.m_y + m_size.m_y / 2;
            return Vector2<TypeName>(centerX, centerY);
        }

        [[nodiscard]] TypeName GetAdjustedY(TypeName y) const { return m_position.m_y + y; }
        [[nodiscard]] TypeName GetAdjustedX(TypeName x) const { return m_position.m_x + x; }

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
            m_size.m_x *= factor;
            m_size.m_y *= factor;
        }

        void Expand(TypeName factor)
        {
            m_position.m_x -= m_size.m_x * (factor - 1) / 2;
            m_position.m_y -= m_size.m_y * (factor - 1) / 2;
            m_size.m_x *= factor;
            m_size.m_y *= factor;
        }

        void ClampToBounds(const Rectangle<TypeName>& bounds)
        {
            m_position.m_x = std::max(bounds.GetLeft(), std::min(m_position.m_x, bounds.GetRight() - m_size.m_x));
            m_position.m_y = std::max(bounds.GetTop(), std::min(m_position.m_y, bounds.GetBottom() - m_size.m_y));
        }

        [[nodiscard]] bool Contains(const Vector2<TypeName>& point) const
        {
            return point.m_x >= m_position.m_x && point.m_x < m_position.m_x + m_size.m_x
                && point.m_y >= m_position.m_y && point.m_y < m_position.m_y + m_size.m_y;
        }

        [[nodiscard]] bool Intersects(const Rectangle<TypeName>& other) const
        {
            return m_position.m_x < other.m_position.m_x + other.m_size.m_x &&
                m_position.m_x + m_size.m_x > other.m_position.m_x &&
                m_position.m_y < other.m_position.m_y + other.m_size.m_y &&
                m_position.m_y + m_size.m_y > other.m_position.m_y;
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
            int x1 = std::max<TypeName>(m_position.m_x, other.m_position.m_x);
            int y1 = std::max<TypeName>(m_position.m_y, other.m_position.m_y);

            // Coordinates of the bottom right corner of the intersection rect
            const int x2 = std::min<TypeName>(m_position.m_x + m_size.m_x, other.m_position.m_x + other.m_size.m_x);
            const int y2 = std::min<TypeName>(m_position.m_y + m_size.m_y, other.m_position.m_y + other.m_size.m_y);

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
            m_position.m_x -= marginX;
            m_position.m_y -= marginY;
            m_size.m_x += marginX * 2;
            m_size.m_y += marginY * 2;
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
