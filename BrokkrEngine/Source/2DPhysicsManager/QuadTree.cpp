#include "QuadTree.h"
#include "PhysicsManager.h"

Brokkr::QuadTree::QuadTree(ObjectID data, const Rectangle<float>& rect, const size_t maxObjectPerNode)
    : m_objectID(data)
    , m_rect(rect)
    , m_MaxObjectPerNode(maxObjectPerNode)
    , m_isLeaf(true) // Ensure new nodes are initialized as leaf nodes
{
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
    // If the object is completely outside this node, do nothing.
    if (!m_rect.Intersects(rect)) return;

    // If this node is a leaf and has space, add the object.
    if (m_isLeaf)
    {
        m_colliderNodes.emplace_back(data, rect);

        // If it exceeds capacity, split the node.
        if (m_colliderNodes.size() > m_MaxObjectPerNode)
        {
            Divide();
        }
        return;
    }

    // Try inserting into child nodes
    bool insertedIntoLeaf = false;
    for (QuadTree& leaf : m_leafs)
    {
        if (leaf.m_rect.Intersects(rect))  // Ensure it fully fits in one leaf
        {
            leaf.Insert(data, rect);
            insertedIntoLeaf = true;
            break;
        }
    }

    // If the object didn't fit in any child node, keep it in the parent
    if (!insertedIntoLeaf)
    {
        m_colliderNodes.emplace_back(data, rect);
    }
}

std::vector<Brokkr::QuadTree::ObjectID> Brokkr::QuadTree::Query(const Rectangle<float>& rect) const
{
    std::vector<ObjectID> result;

    // If the query rect does not intersect, return an empty result
    if (!m_rect.Intersects(rect)) return result;

    // If this is a leaf node, check stored objects
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
    else // Otherwise, query child nodes
    {
        for (const QuadTree& leaf : m_leafs)
        {
            auto childResult = leaf.Query(rect);
            result.insert(result.end(), std::make_move_iterator(childResult.begin()), std::make_move_iterator(childResult.end()));
        }
    }

    return result;
}

void Brokkr::QuadTree::Remove(const ObjectID& data, const Rectangle<float>& rect)
{
    // If it does not intersect, ignore
    if (!m_rect.Intersects(rect)) return;

    // If this is a leaf, remove the object
    if (m_isLeaf)
    {
        m_colliderNodes.erase(std::remove_if(m_colliderNodes.begin(), m_colliderNodes.end(),
            [&](const auto& node) { return node.first == data && node.second == rect; }),
            m_colliderNodes.end());
    }
    else // Otherwise, remove from children
    {
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
    m_isLeaf = true;
}

void Brokkr::QuadTree::Divide()
{
    if (!m_isLeaf) return; // Prevent redundant division

    CreateLeafNodes();

    // Move objects into appropriate leafs, ensuring they fit
    for (const auto& node : m_colliderNodes)
    {
        bool inserted = false;
        for (QuadTree& leaf : m_leafs)
        {
            if (leaf.m_rect.Intersects(node.second))
            {
                leaf.Insert(node.first, node.second);
                inserted = true;
                break;
            }
        }

        // If object is too big for any leaf, keep it in the parent node
        if (!inserted)
        {
            m_tempNodes.emplace_back(node.first, node.second);
        }
    }

    m_colliderNodes = std::move(m_tempNodes);
    m_isLeaf = false;
}

void Brokkr::QuadTree::CreateLeafNodes()
{
    const float halfWidth = m_rect.GetWidth() / 2.f;
    const float halfHeight = m_rect.GetHeight() / 2.f;

    Rectangle nwRect(m_rect.GetPosition(), Vector2(halfWidth, halfHeight));
    Rectangle neRect(Vector2(m_rect.GetX() + halfWidth, m_rect.GetY()), Vector2(halfWidth, halfHeight));
    Rectangle swRect(Vector2(m_rect.GetX(), m_rect.GetY() + halfHeight), Vector2(halfWidth, halfHeight));
    Rectangle seRect(Vector2(m_rect.GetX() + halfWidth, m_rect.GetY() + halfHeight), Vector2(halfWidth, halfHeight));

    m_leafs.emplace_back(m_objectID, nwRect, m_MaxObjectPerNode);
    m_leafs.emplace_back(m_objectID, neRect, m_MaxObjectPerNode);
    m_leafs.emplace_back(m_objectID, swRect, m_MaxObjectPerNode);
    m_leafs.emplace_back(m_objectID, seRect, m_MaxObjectPerNode);
}

void Brokkr::QuadTree::RemoveEmptyNodes()
{
    // Remove completely empty child nodes
    for (auto it = m_leafs.begin(); it != m_leafs.end();)
    {
        it->RemoveEmptyNodes();
        if (it->m_isLeaf && it->m_colliderNodes.empty())
        {
            it = m_leafs.erase(it);
        }
        else
        {
            ++it;
        }
    }

    // If all child nodes are gone, revert to a leaf node
    if (m_leafs.empty())
    {
        m_isLeaf = true;
    }
}
