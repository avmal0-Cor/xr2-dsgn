/* SCE CONFIDENTIAL
 * PlayStation(R)3 Programmer Tool Runtime Library 310.001
 *                Copyright (C) 2009 Sony Computer Entertainment Inc.
 *                                               All Rights Reserved.
 */


#ifndef H_SCE_PEER_RBTREE
#define H_SCE_PEER_RBTREE

#include <PSGL/Base.h>
#include <stdint.h>

// To avoid win32 macro name collision.
#ifdef min
#undef min
#endif
#ifdef max
#undef max
#endif

//////////////////////////////////////////////////////////////////

namespace PSGL
{

/// Red/Black tree class.
/// Used internally by Map and Set.
class RBTree:public Base
{
    public:

        /// Red/Black tree node.
        struct RBNode:public Base
        {
                RBNode();  ///< Node constructor.
                void       *mValue; ///< Node value pointer.
                const void *mKey; ///< Node key pointer.
            private:
                friend class RBTree;
                struct RBNode *mLeft;
                struct RBNode *mRight;
                struct RBNode *mUp;
								/// Red/black sense of node.
                enum
                {
                    Red,			///< Red node.
                    Black			///< Black node.
                };
                uint8_t mColor;
        };

        /// Node comparison function type.
        typedef bool (*Cmpfunc)(const void *, const void *);

        /// Constructor.
        /// @param cmpfunc The comparison function to use for this tree.
        explicit RBTree(Cmpfunc cmpfunc);
        /// Destructor.
        ~RBTree();

        /// Search the tree.
        /// @param key The key to search for.
        /// @return
        ///             - 0 if key not found.
        ///             - A pointer to the RBNode containing key,
        ///               if found.
        RBNode *search(const void *key) const;

        /// Insert a key/value pair into the tree.
        /// Multiple values may be inserted for a given key.
        /// @param key A pointer to the key to use for this value.
        /// @param value A pointer to the value object referenced by key.
        void insert(const void *key, void *value);

        /// Erase a key/value pair from the tree.
        /// This call will erase only the first instance of key
        /// found in the tree.
        /// @param key The key to remove from the tree.
        void erase(const void *key);

        /// Erase a specific key/value pair from the tree.
        /// This call will erase only exact key/value pair
        /// passed in.
        /// @param cur The node to remove from the tree.
        void erase(RBNode *cur);

        /// Iterate to the next logical value in the tree.
        /// Given a pointer to an RBNode, iterate to the
        /// next logically ordered node in the tree.
        /// @param cur The current node.
        /// @return
        ///             - 0 if at the last node in the tree.
        ///             - A pointer to the next logical node.
        RBNode *next(RBNode *cur) const;
        /// Reverse-iterate to the previous logical value
        /// in the tree.
        /// Given a pointer to an RBNode, iterate to the
        /// previous logically ordered node in the tree.
        /// @param cur The current node.
        /// @return
        ///             - 0 if at the first node in the tree.
        ///             - A pointer to the previous logical node.
        RBNode *prev(RBNode *cur) const;
        /// Return the tree node with the least (logical) value.
        /// @return The "first" node in the tree, or 0 if empty.
        RBNode *min() const;
        /// Return the tree node with the greatest (logical) value.
        /// @return The "last" node in the tree, or 0 if empty.
        RBNode *max() const;
        /// Clear the tree of all key/value pairs.
        /// Does not delete the actual data pointers.
        void clear();
        /// Return the number of elements in the tree.
        /// @return The number of key/value pairs in the tree.
        uint32_t size() const;

        /// Dump a subtree to a file.
        /// This is for debugging only and will be hidden.
        /// @param topnode The top of the subtree to dump.
        void dumpSubtree(RBTree::RBNode *topnode);

    private:
        RBTree &operator=(const RBTree &rhs);
        RBTree(RBTree const &rhs);
        void leftRotate(RBNode *rot);
        void rightRotate(RBNode *rot);
        void insertFix(RBNode *rot);
        void deleteFix(RBNode *rot);
        uint32_t           mNelem;
        Cmpfunc            mCompare;
        RBNode             *mRoot;
        RBNode             mSentinel;
};

//////////////////////////////////////////////////////////////////

} // namespace PSGL

#endif // H_SCE_PEER_RBTREE
