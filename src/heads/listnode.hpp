#pragma once

class ListNodeData;
class List;

class ListNode{
    
    friend class List;

    #ifdef LISTNODE_UNIT_TESTS
    public:
    #else
    private:
    #endif

        ListNode(ListNodeData* data);
        
        ListNode* GetPreviousNode() const;
        ListNode* GetNextNode() const;
        ListNode* GetRandomNode() const;
        ListNodeData* GetData() const;

        bool HasNextNode() const;
        bool HasPreviousNode() const;
        bool HasRandomNode() const;
        bool HasData() const;

        void SetNextNode(ListNode* nextNode);
        void SetPreviousNode(ListNode* newPreviousNode);
        void SetRandomNode(ListNode* randomNode);
        void SetData(ListNodeData* data);

        void ConnectToLeft(ListNode* node);
        void ConnectToRight(ListNode* node);

        void Clear();
    
    private:
        ListNode* previousNode_;
        ListNode* randomNode_;
        ListNode* nextNode_;
        ListNodeData* data_;
};
