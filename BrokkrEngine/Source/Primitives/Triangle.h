#pragma once
#include "Vector2.h"

namespace Brokkr
{
    template <typename TypeName>
    class Triangle
    {
        Vector2<TypeName> vertexA;
        Vector2<TypeName> vertexB;
        Vector2<TypeName> vertexC;

    public:
        // Constructors
        Triangle() = default;
        Triangle(const Vector2<TypeName>& a, const Vector2<TypeName>& b, const Vector2<TypeName>& c)
            : vertexA(a), vertexB(b), vertexC(c)
        {
            //
        }

        // Accessors
        [[nodiscard]] const Vector2<TypeName>& GetVertexA() const { return vertexA; }
        [[nodiscard]] const Vector2<TypeName>& GetVertexB() const { return vertexB; }
        [[nodiscard]] const Vector2<TypeName>& GetVertexC() const { return vertexC; }

        // Calculate area using the determinant formula
        [[nodiscard]] TypeName GetArea() const
        {
            return std::abs((vertexA.m_x * (vertexB.m_y - vertexC.m_y) +
                vertexB.m_x * (vertexC.m_y - vertexA.m_y) +
                vertexC.m_x * (vertexA.m_y - vertexB.m_y)) / 2.0);
        }

        // if a point lies inside the triangle : barycentric coordinates
        [[nodiscard]] bool Contains(const Vector2<TypeName>& point) const
        {
            TypeName area = GetArea();
            TypeName area1 = std::abs((point.m_x * (vertexB.m_y - vertexC.m_y) +
                vertexB.m_x * (vertexC.m_y - point.m_y) +
                vertexC.m_x * (point.m_y - vertexB.m_y)) / 2.0);
            TypeName area2 = std::abs((vertexA.m_x * (point.m_y - vertexC.m_y) +
                point.m_x * (vertexC.m_y - vertexA.m_y) +
                vertexC.m_x * (vertexA.m_y - point.m_y)) / 2.0);
            TypeName area3 = std::abs((vertexA.m_x * (vertexB.m_y - point.m_y) +
                vertexB.m_x * (point.m_y - vertexA.m_y) +
                point.m_x * (vertexA.m_y - vertexB.m_y)) / 2.0);

            return std::abs(area - (area1 + area2 + area3)) < std::numeric_limits<TypeName>::epsilon();
        }
    };
}
