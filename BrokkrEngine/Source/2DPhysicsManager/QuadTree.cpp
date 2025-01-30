#include "QuadTree.h"
#include "PhysicsManager.h"


Brokkr::QuadTree::QuadTree(ObjectID data, const Rectangle<float>& rect, const size_t maxObjectPerNode)
    : m_objectID(data)
    , m_rect(rect)
    , m_MaxObjectPerNode(maxObjectPerNode)
{
    //
}

void Brokkr::QuadTree::Init(const Rectangle<float>& rect)
{
    m_rect = rect;
    m_isLeaf = true;
    m_colliderNodes.clear();
    m_leafs.clear();
}

void Brokkr::QuadTree::Insert(const ObjectID& data, const Rectangle<float>& rect)
{
    // Check if rect is completely outside of the current node
    if (!m_rect.Intersects(rect))
    {
        return;
    }

    // Check if the current is full
    if (m_colliderNodes.size() >= m_MaxObjectPerNode)
    {
        // Divide the current node if it is a leaf
        if (m_isLeaf)
        {
            Divide();
        }

        // Insert the object into child node
        for (QuadTree& leaf : m_leafs)
        {
            leaf.Insert(data, rect);
        }

        return;
    }

    // Add to the current node
    m_colliderNodes.emplace_back(data, rect);

    // If the current node is not a leaf, add to appropriate child nodes
    if (!m_isLeaf)
    {
        for (QuadTree& leaf : m_leafs)
        {
            if (leaf.m_rect.Intersects(rect))  // Check for intersection
            {
                leaf.Insert(data, rect);
            }
        }
    }
}

std::vector<Brokkr::QuadTree::ObjectID> Brokkr::QuadTree::Query(const Rectangle<float>& rect) const
{
    std::vector<ObjectID> result;

    // If the query rect does not intersect return an empty result
    if (!m_rect.Intersects(rect))
    {
        return result;
    }

    // If current is a leaf return all that intersect with the query rect
    if (m_isLeaf)
    {
        for (const auto& node : m_colliderNodes)
        {
            if (node.second.Intersects(rect))
            {
                result.push_back(node.first);
            }
        }
    }
    else // Otherwise query all leafs recursively
    {
        for (const QuadTree& leaf : m_leafs)
        {
            auto childResult = leaf.Query(rect);
            result.insert(result.end(), childResult.begin(), childResult.end());
        }
    }

    return result;
}

void Brokkr::QuadTree::Remove(const ObjectID& data, const Rectangle<float>& rect)
{
    // If not intersect return
    if (!m_rect.Intersects(rect)) 
    {
        return;
    }

    // Remove the object if it exists
    if (m_isLeaf) 
    {
        for (auto iterator = m_colliderNodes.begin(); iterator != m_colliderNodes.end(); ++iterator) 
        {
            if (iterator->first == data && iterator->second == rect) 
            {
                m_colliderNodes.erase(iterator);
                break;
            }
        }
    }
    else 
    {
        // Otherwise remove from the leaf nodes
        for (QuadTree& leaf : m_leafs) 
        {
            leaf.Remove(data, rect);
        }


        RemoveEmptyNodes();
    }
}

void Brokkr::QuadTree::Destroy()
{
    m_colliderNodes.clear();

    for (QuadTree& leaf : m_leafs)
    {
        leaf.Destroy();
    }

    m_leafs.clear();
}

void Brokkr::QuadTree::Divide()
{
    CreateLeafNodes();
    MoveObjectsToLeafs();
    ClearObjects();
    m_isLeaf = false;
}

void Brokkr::QuadTree::CreateLeafNodes()
{
    // Create four leaf nodes
    const float halfWidth = m_rect.GetWidth() / 2.f;
    const float halfHeight = m_rect.GetHeight() / 2.f;

    Rectangle nwRect(m_rect.GetPosition(), Vector2(halfWidth, halfHeight));
    Rectangle neRect(Vector2(m_rect.GetX() + halfWidth, m_rect.GetY()), Vector2(halfWidth, halfHeight));
    Rectangle swRect(Vector2(m_rect.GetX(), m_rect.GetY() + halfHeight), Vector2(halfWidth, halfHeight));
    Rectangle seRect(Vector2(m_rect.GetX() + halfWidth, m_rect.GetY() + halfHeight), Vector2(halfWidth, halfHeight));

    // Create four leaf
    m_leafs.emplace_back(m_objectID, nwRect, m_MaxObjectPerNode);
    m_leafs.emplace_back(m_objectID, neRect, m_MaxObjectPerNode);
    m_leafs.emplace_back(m_objectID, swRect, m_MaxObjectPerNode);
    m_leafs.emplace_back(m_objectID, seRect, m_MaxObjectPerNode);
}

void Brokkr::QuadTree::MoveObjectsToLeafs()
{
    // Move the objects from the current
    for (const auto& node : m_colliderNodes)
    {
        for (QuadTree& leaf : m_leafs)
        {
            if (leaf.m_rect.Intersects(node.second))
            {
                leaf.Insert(node.first, node.second);
            }
        }
    }
}

void Brokkr::QuadTree::ClearObjects()
{
    // Clear the objects in the current node
    m_colliderNodes.clear();
}

void Brokkr::QuadTree::RemoveEmptyNodes()
{
    // If the node is not a leaf remove empty nodes from it
    if (!m_isLeaf) 
    {
        for (auto iterator = m_leafs.begin(); iterator != m_leafs.end();) 
        {
            iterator->RemoveEmptyNodes();
            if (iterator->m_colliderNodes.empty() && iterator->m_isLeaf)
            {
                iterator = m_leafs.erase(iterator);
            }
            else 
            {
                ++iterator;
            }
        }
    }

    // If the node is a leaf and has no objects it can be removed
    if (m_isLeaf && m_colliderNodes.empty()) 
    {
        m_leafs.clear();
        m_isLeaf = true;
    }
}