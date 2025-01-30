#pragma once
#include <vector>
#include "Rectangle.h"

namespace Brokkr
{
    class QuadTree
    {
        inline static constexpr size_t kMaxObjectPerNode = 25;
        inline static constexpr size_t kMaxDepth = 25;

        using ObjectID = int;
        using NodeDataContainer = std::vector<std::pair<ObjectID, Rectangle<float>>>;
        
        ObjectID m_objectID; // Objects ID
        Rectangle<float> m_rect;

        size_t m_MaxObjectPerNode = kMaxObjectPerNode;

        bool m_isLeaf = true;

        NodeDataContainer m_colliderNodes;
        std::vector<QuadTree> m_leafs;

    public:
        QuadTree() = default;
        QuadTree(ObjectID data, const Rectangle<float>& rect, const size_t maxObjectPerNode);

        void Init(const Rectangle<float>& rect);
        void Insert(const ObjectID& data, const Rectangle<float>& rect);

        [[nodiscard]] std::vector<ObjectID> Query(const Rectangle<float>& rect) const;

        //TODO: Revisit removing from tree this is leaving empty nodes and over time will cause a overflow 
        void Remove(const  ObjectID& data, const Rectangle<float>& rect); // - temp fix destroy and rebuild after a move 
        void Destroy();

    private:
        void Divide();
        void CreateLeafNodes();
        void MoveObjectsToLeafs();
        void ClearObjects();
        void RemoveEmptyNodes();
    };
}
