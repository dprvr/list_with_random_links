#include <stdexcept>

#include "../../src/heads/listnodedata.hpp"
#include "../../src/heads/listnode.hpp"

ListNode::ListNode(ListNodeData* nodeData){
    previousNode_ = nullptr;
    randomNode_ = nullptr;
    nextNode_ = nullptr;
    data_ = nodeData;
}

ListNode* ListNode::GetPreviousNode() const{
    return previousNode_;
}

ListNode* ListNode::GetNextNode() const{
    return nextNode_;
}

ListNode* ListNode::GetRandomNode() const{
    return randomNode_;
}

ListNodeData* ListNode::GetData() const{
    return data_;
}

bool ListNode::HasNextNode() const{
    bool has = nextNode_ != nullptr;
    return has;
}

bool ListNode::HasPreviousNode() const{
    bool has = previousNode_ != nullptr;
    return has;
}

bool ListNode::HasRandomNode() const{
    bool has = randomNode_ != nullptr;
    return has;
}

bool ListNode::HasData() const{
    bool has = data_ != nullptr;
    return has;
}

void ListNode::SetNextNode(ListNode* newNextNode){
    nextNode_ = newNextNode;
}

void ListNode::SetPreviousNode(ListNode* newPreviousNode){
    previousNode_ = newPreviousNode;
}

void ListNode::SetRandomNode(ListNode* randomNode){
    randomNode_ = randomNode;
}

void ListNode::SetData(ListNodeData* data){
    data_ = data;
}

void ListNode::ConnectToLeft(ListNode* node){
    if (node == nullptr){
        throw std::invalid_argument("The given node was null.");
    }
    this->previousNode_ = node;
    node->nextNode_ = this;
}

void ListNode::ConnectToRight(ListNode* node){
    if (node == nullptr){
        throw std::invalid_argument("The given node was null.");
    }
    nextNode_ = node;
    node->previousNode_ = this;
}

void ListNode::Clear(){
    previousNode_ = nullptr;
    randomNode_ = nullptr;
    nextNode_ = nullptr;
    data_ = nullptr;
}