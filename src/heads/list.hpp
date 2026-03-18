#pragma once

#include <string>
#include <unordered_set>

class ListNodeData;
class ListNode;

class List{
    public:
        List();

        size_t GetCapacityLimit() const;
        size_t GetNodesCount() const;
        bool IsEmpty() const;

        ListNode* GetHeadNode() const;
        ListNode* GetTailNode() const;
        ListNode* GetNodeAt(int index) const;

        bool ContainsNode(ListNode* node) const;
        
        bool TryGetPreviousNode(ListNode* node, ListNode*& outPreviousNode) const;
        bool TryGetNextNode(ListNode* node, ListNode*& outNextNode) const;
        bool TryGetRandomNode(ListNode* node, ListNode*& outRandomNode) const;
        
        ListNodeData* GetNodeData(ListNode* node) const;

        void SetRandomNodeForNode(ListNode* node, ListNode* randomNode) const;
        void SetDataForNode(ListNode* node, ListNodeData* data) const;

        ListNode* AddNodeToTheLeft(ListNodeData* data);
        ListNode* AddNodeToTheRight(ListNodeData* data);
        ListNode* AddNodeAfter(ListNode* node, ListNodeData* data);
        ListNode* AddNodeBefore(ListNode* node, ListNodeData* data);

        void RemoveHeadNode();
        void RemoveTailNode();
        void RemoveNode(ListNode* node);

        static List* CreateFromFile(const std::string& filepath);
        static void SerializeListToFile(List* list, const std::string& filepath);
        static List* DeserializeFromFile(const std::string& filepath);

    private:
        void AfterNodeInsert(ListNode* node);
        void BeforeNodeDelete(ListNode* node);

        static const size_t capacityLimit_ = 1000000;

        ListNode* headNode_;
        ListNode* tailNode_;
        std::unordered_set<ListNode*> nodes_;
};