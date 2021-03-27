/**
 * @file        QuadTree.h
 * @author      Argishti Ayvazyan (ayvazyan.argishti@gmail.com)
 * @brief       Declaring the QuadTree class.
 * @date        25-03-2021
 * @copyright   Copyright (c) 2021
 */

#pragma once

#include <array>
#include <memory>
#include <stack>
#include <algorithm>

#include <boost/container/flat_set.hpp>

#include <Square.h>
#include <Utility.h>

namespace space
{

/**
 * @brief   Implementation of quadtree.
 *
 * @tparam  TKey The type of values.
 */
template <typename TKey>
class QuadTree
{
private:

    enum class ZOrderPos : size_t
    {
        LeftTop = 0
        , LeftBottom = 1
        , RightTop = 2
        , RightBottom = 3
    };

    class Node
    {
    public:
        using TValue = TKey;
        using TRegion = space::Square<typename TKey::TCoordinate>;
        using TChildContainer = std::array<std::unique_ptr<Node>, 4>;
        using TValueContainer = boost::container::flat_set<TValue>;

        Node() = delete;

        explicit Node(TRegion region)
            : m_region(region)
        {
        }

        void addValue(const TValue& box)
        {
            m_values.insert(box);
        }

        void eraseValue(const TValue& box)
        {
            m_values.erase(box);
        }

        void setChild(ZOrderPos pos, std::unique_ptr<Node>&& child)
        {
            m_child[static_cast<std::size_t>(pos)] = std::move(child);
        }

        [[nodiscard]]
        std::unique_ptr<Node>& getChild(ZOrderPos pos) noexcept
        {
            return m_child[static_cast<std::size_t>(pos)];
        }

        [[nodiscard]]
        TChildContainer& getChildren() noexcept
        {
            return m_child;
        }

        [[nodiscard]]
        const TChildContainer& getChildren() const noexcept
        {
            return m_child;
        }

        [[nodiscard]]
        const TValueContainer& getValues() const noexcept
        {
            return m_values;
        }

        [[nodiscard]]
        TValueContainer& getValues() noexcept
        {
            return m_values;
        }

        [[nodiscard]]
        TRegion& region() noexcept
        {
            return m_region;
        }

        [[nodiscard]]
        const TRegion& region() const noexcept
        {
            return m_region;
        }

        [[nodiscard]]
        bool empty() const noexcept
        {
            (void)getValues().size();
            if (!getValues().empty())
            {
                return false;
            }
            return std::ranges::all_of(getChildren(), [](const auto& child)
            {
               return nullptr == child;
            });
        }

    private:
        TRegion m_region;
        TChildContainer m_child;
        TValueContainer m_values;
    };


    using TRegion = typename Node::TRegion;
private:

    using TNodePtr = std::unique_ptr<Node>;
public:

    QuadTree()
        : m_root(nullptr)
    {
    }

    /**
     * @brief   Inserts a value to the quad tree.
     *
     * @param   key The new value.
     */
    void insert(const TKey& key)
    {
        if (nullptr == m_root)
        {
            creatRoot(key);
        }

        growUpIfNeeds(key);

        auto *node = growDownIfNeedsAndReturnLastNode(key);
        node->addValue(key);
    }

    /**
     * @brief   Finds values intersecting a given rectangle.
     *
     * @tparam  TOutIt The type of output iterator.
     * @param   key The rectangle for query.
     * @param   outIt The output iterator.
     */
    template <typename TOutIt>
    void query(const TKey& key, TOutIt outIt) const
    {
        std::stack<const Node *> nodeStack;
        auto pushNodeIfNotNull = [&nodeStack](const Node *node)
        {
            if (nullptr == node)
            {
                return;
            }
            nodeStack.push(node);
        };
        auto popNode = [&nodeStack]()
        {
            auto *node = nodeStack.top();
            nodeStack.pop();
            return node;
        };
        pushNodeIfNotNull(m_root.get());

        while (!nodeStack.empty())
        {
            const Node *currentNode = popNode();
            if (!space::util::hesIntersect(key, currentNode->region()))
            {
                continue;
            }
            for (auto& child : currentNode->getChildren())
            {
                pushNodeIfNotNull(child.get());
            }
            for (const auto& value : currentNode->getValues())
            {
                if (!space::util::hesIntersect(key, value))
                {
                    continue;
                }
                outIt = value;
            }
        }
    }

    /**
     * @brief   Removes given rectangle form quadtree.
     *
     * @param   key the rectangle.
     */
    void remove(const TKey& key)
    {
        auto* node = findNode(key);
        if (nullptr == node)
        {
            return;
        }
        (*node)->eraseValue(key);

        // remove node if empty.
        if ((*node)->empty())
        {
            node->reset(nullptr);
        }
    }

    /**
     * @brief   Determines whether the quadtree contains the specified key.
     *
     * @param   key The key to locate in the quadtree.
     * @return  true if the quadtree contains an element with the specified key; otherwise, false.
     */
    bool contains(const TKey& key) const
    {
        const auto pNode = findNode(key);
        if (nullptr == pNode)
        {
            return false;
        }
        const auto& values = (*pNode)->getValues();
        return values.end() != values.find(key);
    }

private:

    /**
     * @internal
     * @brief   Returns node for the given key.
     *
     * @param   key The key.
     * @return  The pointer to node unique_ptr if that exists, otherwise null.
     */
    const TNodePtr* findNode(const TKey& key) const
    {
        if (nullptr == m_root)
        {
            return nullptr;
        }
        auto* currentNode = std::addressof(m_root);
        while (!hasIntersectionWithRegionSplitLines(key, (*currentNode)->region()))
        {
            const auto zOrderPos = getZOrderPos((*currentNode)->region(), key);
            auto& child = (*currentNode)->getChild(zOrderPos);

            if (nullptr == child)
            {
                return nullptr;
            }
            currentNode = std::addressof(child);
        }
        return currentNode;
    }

    TNodePtr* findNode(const TKey& key)
    {
        return const_cast<TNodePtr*>(std::as_const(*this).findNode(key));
    }

    /**
     * @internal
     * @brief       Creates new root.
     *
     * @details     The new root region can contain the given key.
     * @param key   The key for computing region size.
     */
    void creatRoot(const TKey& key)
    {
        const auto[x, y] = space::util::topRight(key);
        auto regionSize = static_cast<int32_t>(std::pow(2, static_cast<int32_t>(std::log2(std::max(x, y))) + 1));
        if (0 == regionSize)
        {
            regionSize = 1;
        }

        TRegion regionForNewNode {{0, 0}, regionSize};
        m_root = std::make_unique<Node>(regionForNewNode);
    }

    /**
     * @internal
     * @brief   Grow up the tree if the given key cannot contain in the root.
     *
     * @details Creates new root and set old root the child for it.
     *
     * @param   key The rectangle.
     */
    void growUpIfNeeds(const TKey& key)
    {
        while (!space::util::contains(m_root->region(), key))
        {
            const auto regionSize = m_root->region().size() << 1;
            TRegion regionSizeForNewRoot {{0, 0}, regionSize};
            auto newRoot = std::make_unique<Node>(regionSizeForNewRoot);
            newRoot->setChild(ZOrderPos::LeftBottom, std::move(m_root));
            m_root = std::move(newRoot);
        }
    }

    /**
     * @internal
     * @brief       Grow down the tree if the associated node for key not exists.
     *              Returns associated node for the key.
     *
     * @param key   The rectangle.
     * @return      The associated node pointer for the key.
     */
    Node* growDownIfNeedsAndReturnLastNode(const TKey& key)
    {
        auto *currentNode = m_root.get();
        while (!(hasIntersectionWithRegionSplitLines(key, currentNode->region())
                 || 1 == currentNode->region().size()))
        {
            const auto childPosition = getZOrderPos(currentNode->region(), key);
            auto& child = currentNode->getChild(childPosition);
            if (nullptr == child)
            {
                auto newChildRegion = makeChildRegion(currentNode->region(), childPosition);
                child = std::make_unique<Node>(newChildRegion);
            }
            currentNode = child.get();
        }

        return currentNode;
    }


private:

    /**
     * @internal
     * @brief       Return the middle x-axis coordinate for the given region.
     *
     * @param rect  The region.
     * @return      The middle x-axis coordinate
     */
    static auto getRectMiddleX(const TRegion& rect)
    {
        return (rect.pos().x() + (rect.size() / 2));
    }

    /**
     * @internal
     * @brief       Return the middle y-axis coordinate for the given region.
     *
     * @param rect  The region.
     * @return      The middle y-axis coordinate
     */
    static auto getRectMiddleY(const TRegion& rect)
    {
        return (rect.pos().y() + (rect.size() / 2));
    }

    /**
     * @internal
     * @brief           Checks the given rectangle has an intersection with region split lines.
     *
     * @param rect      The rectangle.
     * @param region    The region.
     * @return          true if has intersection, otherwise false.
     */
    static bool hasIntersectionWithRegionSplitLines(const TKey& rect, const TRegion& region)
    {
        const auto middleX = getRectMiddleX(region);
        const auto middleY = getRectMiddleY(region);
        return ((rect.pos().x() <= middleX) && (middleX <= rect.pos().x() + rect.width()))
               || ((rect.pos().y() <= middleY) && (middleY <= rect.pos().y() + rect.height()));
    }


    /**
     * @internal
     * @brief           Returns the z-order position for the given rectangle.
     *
     * @param region    The region.
     * @param key       The key.
     * @return          The z-order position.
     */
    static ZOrderPos getZOrderPos(const TRegion& region, const TKey& key)
    {
        const auto middleX = getRectMiddleX(region);
        const auto middleY = getRectMiddleY(region);
        const auto[x, y] = key.pos();
        if (x < middleX)
        {
            if (y > middleY)
            {
                return ZOrderPos::LeftTop;
            }
            return ZOrderPos::LeftBottom;
        }
        if (y > middleY)
        {
            return ZOrderPos::RightTop;
        }
        return ZOrderPos::RightBottom;
    }


    /**
     * @internal
     * @brief           Makes this region for the child.
     *
     * @param region    The parent region.
     * @param zOrderPos The z-order position.
     * @return          The new region for child.
     */
    static TRegion makeChildRegion(const TRegion& region, ZOrderPos zOrderPos)
    {
        const auto middleX = getRectMiddleX(region);
        const auto middleY = getRectMiddleY(region);
        const auto size = static_cast<typename TRegion::TCoordinate>(std::round(region.size() / 2.0));

        switch (zOrderPos)
        {
            case ZOrderPos::LeftTop:
                return TRegion {{region.pos().x(), middleY}, size};
            case ZOrderPos::LeftBottom:
                return TRegion {region.pos(), size};
            case ZOrderPos::RightTop:
                return TRegion {{middleX, middleY}, size};
            case ZOrderPos::RightBottom:
                return TRegion {{middleX, region.pos().y()}, size};
            default:
                assert(false && "404");
        }
    }

private:

    /**
     * @brief The root for quadtree.
     */
    TNodePtr m_root;
};

} // namespace space

