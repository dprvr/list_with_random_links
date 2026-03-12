#include <gtest/gtest.h>
#include <tuple>
#include <vector>
#include <string>
#include <filesystem>
#include <fstream>

#include "../src/heads/listnodedata.hpp"
#include "../src/heads/listnode.hpp"
#include "../src/heads/list.hpp"


TEST(ListTests, InitializeCorrectly){
    
    List* list = new List();

    EXPECT_TRUE(list->IsEmpty());
    EXPECT_EQ(list->GetNodesCount(), 0);
}

TEST(ListTests, ListHeadTailMethodsFallCorrectlyWhenItEmpty){
    List* list = new List();

    EXPECT_ANY_THROW(list->GetHeadNode());
    EXPECT_ANY_THROW(list->GetTailNode());
    EXPECT_ANY_THROW(list->RemoveHeadNode());
    EXPECT_ANY_THROW(list->RemoveTailNode());
}

TEST(ListTests, ListMethodsFallCorrectlyWhenInputIsNull){
    List* list = new List();
    ListNodeData* data = new ListNodeData(std::string("data"));
    ListNode* node;

    EXPECT_ANY_THROW(list->AddNodeAfter(nullptr, data));
    EXPECT_ANY_THROW(list->AddNodeAfter(node, nullptr));
    EXPECT_ANY_THROW(list->AddNodeAfter(nullptr, nullptr));

    EXPECT_ANY_THROW(list->AddNodeBefore(nullptr, data));
    EXPECT_ANY_THROW(list->AddNodeBefore(node, nullptr));
    EXPECT_ANY_THROW(list->AddNodeBefore(nullptr, nullptr));

    EXPECT_ANY_THROW(list->ContainsNode(nullptr));
    EXPECT_ANY_THROW(list->GetNodeData(nullptr));
    EXPECT_ANY_THROW(list->RemoveNode(nullptr));

    EXPECT_ANY_THROW(list->SetDataForNode(nullptr, data));
    EXPECT_ANY_THROW(list->SetDataForNode(node, nullptr));
    EXPECT_ANY_THROW(list->SetDataForNode(nullptr, nullptr));

    EXPECT_ANY_THROW(list->SetRandomNodeForNode(nullptr, node));
    EXPECT_ANY_THROW(list->SetRandomNodeForNode(node, nullptr));
    EXPECT_ANY_THROW(list->SetRandomNodeForNode(nullptr, nullptr));

    EXPECT_ANY_THROW(list->TryGetNextNode(nullptr, node));
    EXPECT_ANY_THROW(list->TryGetPreviousNode(nullptr, node));
    EXPECT_ANY_THROW(list->TryGetRandomNode(nullptr, node));
}

TEST(ListTests, ListMethodsFallCorrectlyWhenInputNodeNotBelongToList){
    ListNodeData* data = new ListNodeData(std::string("data"));
    List* list = new List();
    List* secondList = new List();
    ListNode* listNode = list->AddNodeToTheRight(data);
    ListNode* otherListNode = secondList->AddNodeToTheRight(data);
    ListNode* node;

    EXPECT_ANY_THROW(list->AddNodeAfter(otherListNode, data));
    EXPECT_ANY_THROW(list->AddNodeBefore(otherListNode, data));
    EXPECT_ANY_THROW(list->GetNodeData(otherListNode));
    EXPECT_ANY_THROW(list->RemoveNode(otherListNode));
    EXPECT_ANY_THROW(list->SetDataForNode(otherListNode, data));

    EXPECT_ANY_THROW(list->SetRandomNodeForNode(otherListNode, listNode));
    EXPECT_ANY_THROW(list->SetRandomNodeForNode(listNode, otherListNode));
    EXPECT_ANY_THROW(list->SetRandomNodeForNode(otherListNode, otherListNode));

    EXPECT_ANY_THROW(list->TryGetNextNode(otherListNode, node));
    EXPECT_ANY_THROW(list->TryGetPreviousNode(otherListNode, node));
    EXPECT_ANY_THROW(list->TryGetRandomNode(otherListNode, node));
}

TEST(ListTests, ListContainsLogicCorrect){
    List* list = new List();
    List* secondList = new List();    
    ListNodeData* data = new ListNodeData(std::string("data"));
    ListNode* unknownNode;

    int checkCount = 50;
    for (int i = 0; i < checkCount; i++){
        ListNode* firstListNode = list->AddNodeToTheRight(data);
        ListNode* secondListNode = secondList->AddNodeToTheRight(data);
        
        EXPECT_TRUE(list->ContainsNode(firstListNode));
        EXPECT_TRUE(secondList->ContainsNode(secondListNode));
        
        EXPECT_FALSE(list->ContainsNode(secondListNode));
        EXPECT_FALSE(secondList->ContainsNode(firstListNode));
        
        EXPECT_FALSE(list->ContainsNode(unknownNode));
        EXPECT_FALSE(secondList->ContainsNode(unknownNode));
    }
}

TEST(ListTests, ListHeadTailGettersLogicCorrect){
    List* list = new List();
    ListNodeData* data = new ListNodeData(std::string("data"));

    ListNode* firstNode = list->AddNodeToTheRight(new ListNodeData(std::string("data")));
    EXPECT_EQ(list->GetHeadNode(), firstNode);
    EXPECT_EQ(list->GetTailNode(), firstNode);

    ListNode* secondNode = list->AddNodeToTheRight(new ListNodeData(std::string("data")));
    EXPECT_EQ(list->GetHeadNode(), firstNode);
    EXPECT_EQ(list->GetTailNode(), secondNode);

    ListNode* zeroNode = list->AddNodeToTheLeft(new ListNodeData(std::string("data")));
    EXPECT_EQ(list->GetHeadNode(), zeroNode);
    EXPECT_EQ(list->GetTailNode(), secondNode);

    list->RemoveHeadNode();
    EXPECT_EQ(list->GetHeadNode(), firstNode);
    EXPECT_EQ(list->GetTailNode(), secondNode);

    list->RemoveTailNode();
    EXPECT_EQ(list->GetHeadNode(), firstNode);
    EXPECT_EQ(list->GetTailNode(), firstNode);
}

TEST(ListTests, ListTryGetPreviousNextNodesLogicCorrect){
    List* list = new List();
    ListNode* zeroNode = list->AddNodeToTheRight(new ListNodeData(std::string("data")));
    ListNode* firstNode = list->AddNodeToTheRight(new ListNodeData(std::string("data")));
    ListNode* secondNode = list->AddNodeToTheRight(new ListNodeData(std::string("data")));
    ListNode* tempNode = nullptr;

    ListNode* secondNodePrevious = nullptr;
    EXPECT_FALSE(list->TryGetNextNode(secondNode, tempNode));
    EXPECT_FALSE(list->TryGetRandomNode(secondNode, tempNode));
    EXPECT_TRUE(list->TryGetPreviousNode(secondNode, secondNodePrevious));
    EXPECT_EQ(secondNodePrevious, firstNode);
    
    ListNode* firstNodePrevious = nullptr;
    ListNode* firstNodeNext = nullptr;
    EXPECT_TRUE(list->TryGetPreviousNode(firstNode, firstNodePrevious));
    EXPECT_TRUE(list->TryGetNextNode(firstNode, firstNodeNext));
    EXPECT_FALSE(list->TryGetRandomNode(firstNode, tempNode));
    EXPECT_EQ(firstNodePrevious, zeroNode);
    EXPECT_EQ(firstNodeNext, secondNode);

    ListNode* zeroNodeNext = nullptr;
    EXPECT_FALSE(list->TryGetPreviousNode(zeroNode, tempNode));
    EXPECT_TRUE(list->TryGetNextNode(zeroNode, zeroNodeNext));
    EXPECT_FALSE(list->TryGetRandomNode(zeroNode, tempNode));
    EXPECT_EQ(zeroNodeNext, firstNode);

    list->SetRandomNodeForNode(zeroNode, zeroNode);
    list->SetRandomNodeForNode(firstNode, firstNode);
    list->SetRandomNodeForNode(secondNode, secondNode);
    
    ListNode* zeroNodeRandom = nullptr;
    ListNode* firstNodeRandom = nullptr;
    ListNode* secondNodeRandom = nullptr;

    EXPECT_TRUE(list->TryGetRandomNode(zeroNode, zeroNodeRandom));
    EXPECT_TRUE(list->TryGetRandomNode(firstNode, firstNodeRandom));
    EXPECT_TRUE(list->TryGetRandomNode(secondNode, secondNodeRandom));
}

TEST(ListTests, ListNodeDataGetSetLogicCorrect){
    List* list = new List();
    ListNodeData* sourceData = new ListNodeData(std::string("source_data"));
    ListNode* node = list->AddNodeToTheRight(sourceData);
    ListNodeData* outNode = nullptr;

    EXPECT_EQ(list->GetNodeData(node), sourceData);

    ListNodeData* newData = new ListNodeData(std::string("new_data"));
    list->SetDataForNode(node, newData);

    EXPECT_EQ(list->GetNodeData(node), newData);
}

TEST(ListTests, ListAddNodeToTheLeftLogicCorrect){
    List* list = new List();

    ListNode* zeroNode = list->AddNodeToTheLeft(new ListNodeData(std::string("0")));
    ListNode* firstNode = list->AddNodeToTheLeft(new ListNodeData(std::string("1")));
    ListNode* secondNode = list->AddNodeToTheLeft(new ListNodeData(std::string("2")));

    EXPECT_TRUE(list->ContainsNode(zeroNode));
    EXPECT_TRUE(list->ContainsNode(firstNode));
    EXPECT_TRUE(list->ContainsNode(secondNode));

    EXPECT_EQ(list->GetNodeAt(0), secondNode);
    EXPECT_EQ(list->GetNodeAt(1), firstNode);
    EXPECT_EQ(list->GetNodeAt(2), zeroNode);

    ListNode* tempNode;
    ListNode* factPreviousNode;
    ListNode* factNextNode;

    EXPECT_TRUE(list->TryGetPreviousNode(zeroNode, factPreviousNode));
    EXPECT_FALSE(list->TryGetNextNode(zeroNode, factNextNode));
    EXPECT_EQ(factPreviousNode, firstNode);

    EXPECT_TRUE(list->TryGetPreviousNode(firstNode, factPreviousNode));
    EXPECT_TRUE(list->TryGetNextNode(firstNode, factNextNode));
    EXPECT_EQ(factPreviousNode, secondNode);
    EXPECT_EQ(factNextNode, zeroNode);

    EXPECT_FALSE(list->TryGetPreviousNode(secondNode, factPreviousNode));
    EXPECT_TRUE(list->TryGetNextNode(secondNode, factNextNode));
    EXPECT_EQ(factNextNode, firstNode);

    EXPECT_EQ(list->GetNodesCount(), 3);
    EXPECT_EQ(list->GetHeadNode(), secondNode);
    EXPECT_EQ(list->GetTailNode(), zeroNode);
}

TEST(ListTests, ListAddNodeToTheRightLogicCorrect){
    List* list = new List();

    ListNode* zeroNode = list->AddNodeToTheRight(new ListNodeData(std::string("0")));
    ListNode* firstNode = list->AddNodeToTheRight(new ListNodeData(std::string("1")));
    ListNode* secondNode = list->AddNodeToTheRight(new ListNodeData(std::string("2")));

    EXPECT_TRUE(list->ContainsNode(zeroNode));
    EXPECT_TRUE(list->ContainsNode(firstNode));
    EXPECT_TRUE(list->ContainsNode(secondNode));

    EXPECT_EQ(list->GetNodeAt(0), zeroNode);
    EXPECT_EQ(list->GetNodeAt(1), firstNode);
    EXPECT_EQ(list->GetNodeAt(2), secondNode);

    ListNode* tempNode;
    ListNode* factPreviousNode;
    ListNode* factNextNode;

    EXPECT_FALSE(list->TryGetPreviousNode(zeroNode, factPreviousNode));
    EXPECT_TRUE(list->TryGetNextNode(zeroNode, factNextNode));
    EXPECT_EQ(factNextNode, firstNode);

    EXPECT_TRUE(list->TryGetPreviousNode(firstNode, factPreviousNode));
    EXPECT_TRUE(list->TryGetNextNode(firstNode, factNextNode));
    EXPECT_EQ(factPreviousNode, zeroNode);
    EXPECT_EQ(factNextNode, secondNode);

    EXPECT_TRUE(list->TryGetPreviousNode(secondNode, factPreviousNode));
    EXPECT_FALSE(list->TryGetNextNode(secondNode, factNextNode));
    EXPECT_EQ(factPreviousNode, firstNode);

    EXPECT_EQ(list->GetNodesCount(), 3);
    EXPECT_EQ(list->GetHeadNode(), zeroNode);
    EXPECT_EQ(list->GetTailNode(), secondNode);
}

TEST(ListTests, ListAddNodeBeforeLogicCorrect){
    List* list = new List();
    ListNode* zeroNode = list->AddNodeToTheRight(new ListNodeData(std::string("0")));

    ListNode* firstNode = list->AddNodeBefore(zeroNode, new ListNodeData(std::string("1")));
    ListNode* secondNode = list->AddNodeBefore(zeroNode, new ListNodeData(std::string("2")));

    EXPECT_TRUE(list->ContainsNode(zeroNode));
    EXPECT_TRUE(list->ContainsNode(firstNode));
    EXPECT_TRUE(list->ContainsNode(secondNode));

    EXPECT_EQ(list->GetNodeAt(0), firstNode);
    EXPECT_EQ(list->GetNodeAt(1), secondNode);
    EXPECT_EQ(list->GetNodeAt(2), zeroNode);

    ListNode* factPreviousNode;
    ListNode* factNextNode;

    EXPECT_FALSE(list->TryGetPreviousNode(firstNode, factPreviousNode));
    EXPECT_TRUE(list->TryGetNextNode(firstNode, factNextNode));
    EXPECT_EQ(factNextNode, secondNode);

    EXPECT_TRUE(list->TryGetPreviousNode(secondNode, factPreviousNode));
    EXPECT_TRUE(list->TryGetNextNode(secondNode, factNextNode));
    EXPECT_EQ(factPreviousNode, firstNode);
    EXPECT_EQ(factNextNode, zeroNode);

    EXPECT_TRUE(list->TryGetPreviousNode(zeroNode, factPreviousNode));
    EXPECT_FALSE(list->TryGetNextNode(zeroNode, factNextNode));
    EXPECT_EQ(factPreviousNode, secondNode);

    EXPECT_EQ(list->GetNodesCount(), 3);
    EXPECT_EQ(list->GetHeadNode(), firstNode);
    EXPECT_EQ(list->GetTailNode(), zeroNode);
}

TEST(ListTests, ListAddNodeAfterLogicCorrect){
    List* list = new List();
    ListNode* zeroNode = list->AddNodeToTheRight(new ListNodeData(std::string("0")));

    ListNode* firstNode = list->AddNodeAfter(zeroNode, new ListNodeData(std::string("1")));
    ListNode* secondNode = list->AddNodeAfter(zeroNode, new ListNodeData(std::string("2")));

    EXPECT_TRUE(list->ContainsNode(zeroNode));
    EXPECT_TRUE(list->ContainsNode(firstNode));
    EXPECT_TRUE(list->ContainsNode(secondNode));

    EXPECT_EQ(list->GetNodeAt(0), zeroNode);
    EXPECT_EQ(list->GetNodeAt(1), secondNode);
    EXPECT_EQ(list->GetNodeAt(2), firstNode);

    ListNode* factPreviousNode;
    ListNode* factNextNode;

    EXPECT_FALSE(list->TryGetPreviousNode(zeroNode, factPreviousNode));
    EXPECT_TRUE(list->TryGetNextNode(zeroNode, factNextNode));
    EXPECT_EQ(factNextNode, secondNode);

    EXPECT_TRUE(list->TryGetPreviousNode(secondNode, factPreviousNode));
    EXPECT_TRUE(list->TryGetNextNode(secondNode, factNextNode));
    EXPECT_EQ(factPreviousNode, zeroNode);
    EXPECT_EQ(factNextNode, firstNode);

    EXPECT_TRUE(list->TryGetPreviousNode(firstNode, factPreviousNode));
    EXPECT_FALSE(list->TryGetNextNode(firstNode, factNextNode));
    EXPECT_EQ(factPreviousNode, secondNode);

    EXPECT_EQ(list->GetNodesCount(), 3);
    EXPECT_EQ(list->GetHeadNode(), zeroNode);
    EXPECT_EQ(list->GetTailNode(), firstNode);
}

TEST(ListTests, ListRemoveHeadNodeLogicCorrect){
    List* list = new List();
    ListNode* zeroNode = list->AddNodeToTheRight(new ListNodeData(std::string("0")));
    ListNode* firstNode = list->AddNodeToTheRight(new ListNodeData(std::string("1")));
    ListNode* secondNode = list->AddNodeToTheRight(new ListNodeData(std::string("2")));
    ListNode* factPreviousNode;
    ListNode* factNextNode;

    list->RemoveHeadNode();
    EXPECT_FALSE(list->ContainsNode(zeroNode));
    EXPECT_EQ(list->GetNodesCount(), 2);
    EXPECT_EQ(list->GetHeadNode(), firstNode);
    EXPECT_EQ(list->GetTailNode(), secondNode);
    EXPECT_EQ(list->GetNodeAt(0), firstNode);
    EXPECT_EQ(list->GetNodeAt(1), secondNode);
    EXPECT_FALSE(list->TryGetPreviousNode(firstNode, factPreviousNode));
    EXPECT_TRUE(list->TryGetNextNode(firstNode, factNextNode));
    EXPECT_EQ(factNextNode, secondNode);
    EXPECT_TRUE(list->TryGetPreviousNode(secondNode, factPreviousNode));
    EXPECT_FALSE(list->TryGetNextNode(secondNode, factNextNode));
    EXPECT_EQ(factPreviousNode, firstNode);

    list->RemoveHeadNode();
    EXPECT_FALSE(list->ContainsNode(firstNode));
    EXPECT_EQ(list->GetNodesCount(), 1);
    EXPECT_EQ(list->GetHeadNode(), secondNode);
    EXPECT_EQ(list->GetTailNode(), secondNode);
    EXPECT_EQ(list->GetNodeAt(0), secondNode);
    EXPECT_FALSE(list->TryGetPreviousNode(secondNode, factPreviousNode));
    EXPECT_FALSE(list->TryGetNextNode(secondNode, factNextNode));

    list->RemoveHeadNode();
    EXPECT_FALSE(list->ContainsNode(secondNode));
    EXPECT_EQ(list->GetNodesCount(), 0);
    EXPECT_TRUE(list->IsEmpty());
}

TEST(ListTests, ListRemoveTailNodeLogicCorrect){
    List* list = new List();
    ListNode* zeroNode = list->AddNodeToTheRight(new ListNodeData(std::string("0")));
    ListNode* firstNode = list->AddNodeToTheRight(new ListNodeData(std::string("1")));
    ListNode* secondNode = list->AddNodeToTheRight(new ListNodeData(std::string("2")));
    ListNode* factPreviousNode;
    ListNode* factNextNode;

    list->RemoveTailNode();
    EXPECT_FALSE(list->ContainsNode(secondNode));
    EXPECT_EQ(list->GetNodesCount(), 2);
    EXPECT_EQ(list->GetHeadNode(), zeroNode);
    EXPECT_EQ(list->GetTailNode(), firstNode);
    EXPECT_EQ(list->GetNodeAt(0), zeroNode);
    EXPECT_EQ(list->GetNodeAt(1), firstNode);
    EXPECT_FALSE(list->TryGetPreviousNode(zeroNode, factPreviousNode));
    EXPECT_TRUE(list->TryGetNextNode(zeroNode, factNextNode));
    EXPECT_EQ(factNextNode, firstNode);
    EXPECT_TRUE(list->TryGetPreviousNode(firstNode, factPreviousNode));
    EXPECT_FALSE(list->TryGetNextNode(firstNode, factNextNode));
    EXPECT_EQ(factPreviousNode, zeroNode);

    list->RemoveTailNode();
    EXPECT_FALSE(list->ContainsNode(firstNode));
    EXPECT_EQ(list->GetNodesCount(), 1);
    EXPECT_EQ(list->GetHeadNode(), zeroNode);
    EXPECT_EQ(list->GetTailNode(), zeroNode);
    EXPECT_EQ(list->GetNodeAt(0), zeroNode);
    EXPECT_FALSE(list->TryGetPreviousNode(zeroNode, factPreviousNode));
    EXPECT_FALSE(list->TryGetNextNode(zeroNode, factNextNode));

    list->RemoveTailNode();
    EXPECT_FALSE(list->ContainsNode(zeroNode));
    EXPECT_EQ(list->GetNodesCount(), 0);
    EXPECT_TRUE(list->IsEmpty());
}

TEST(ListTests, ListRemoveNodeLogicCorrect){
    List* list = new List();
    ListNode* zeroNode = list->AddNodeToTheRight(new ListNodeData(std::string("0")));
    ListNode* firstNode = list->AddNodeToTheRight(new ListNodeData(std::string("1")));
    ListNode* secondNode = list->AddNodeToTheRight(new ListNodeData(std::string("2")));
    ListNode* thirdNode = list->AddNodeToTheRight(new ListNodeData(std::string("3")));
    ListNode* fourthNode = list->AddNodeToTheRight(new ListNodeData(std::string("4")));
    ListNode* factPreviousNode;
    ListNode* factNextNode;

    list->RemoveNode(zeroNode);
    EXPECT_FALSE(list->ContainsNode(zeroNode));
    EXPECT_EQ(list->GetNodesCount(), 4);
    EXPECT_EQ(list->GetHeadNode(), firstNode);
    EXPECT_EQ(list->GetTailNode(), fourthNode);
    EXPECT_EQ(list->GetNodeAt(0), firstNode);
    EXPECT_EQ(list->GetNodeAt(1), secondNode);
    EXPECT_EQ(list->GetNodeAt(2), thirdNode);
    EXPECT_EQ(list->GetNodeAt(3), fourthNode);
    EXPECT_FALSE(list->TryGetPreviousNode(firstNode, factPreviousNode));
    EXPECT_TRUE(list->TryGetNextNode(firstNode, factNextNode));
    EXPECT_EQ(factNextNode, secondNode);

    list->RemoveNode(fourthNode);
    EXPECT_FALSE(list->ContainsNode(fourthNode));
    EXPECT_EQ(list->GetNodesCount(), 3);
    EXPECT_EQ(list->GetHeadNode(), firstNode);
    EXPECT_EQ(list->GetTailNode(), thirdNode);
    EXPECT_EQ(list->GetNodeAt(0), firstNode);
    EXPECT_EQ(list->GetNodeAt(1), secondNode);
    EXPECT_EQ(list->GetNodeAt(2), thirdNode);
    EXPECT_TRUE(list->TryGetPreviousNode(thirdNode, factPreviousNode));
    EXPECT_FALSE(list->TryGetNextNode(thirdNode, factNextNode));
    EXPECT_EQ(factPreviousNode, secondNode);

    list->RemoveNode(secondNode);
    EXPECT_FALSE(list->ContainsNode(secondNode));
    EXPECT_EQ(list->GetNodesCount(), 2);
    EXPECT_EQ(list->GetHeadNode(), firstNode);
    EXPECT_EQ(list->GetTailNode(), thirdNode);
    EXPECT_EQ(list->GetNodeAt(0), firstNode);
    EXPECT_EQ(list->GetNodeAt(1), thirdNode);
    EXPECT_FALSE(list->TryGetPreviousNode(firstNode, factPreviousNode));
    EXPECT_TRUE(list->TryGetNextNode(firstNode, factNextNode));
    EXPECT_EQ(factNextNode, thirdNode);
    EXPECT_TRUE(list->TryGetPreviousNode(thirdNode, factPreviousNode));
    EXPECT_FALSE(list->TryGetNextNode(thirdNode, factNextNode));
    EXPECT_EQ(factPreviousNode, firstNode);
}

static std::string GetPathToTempTestFile(std::string filename){
    std::filesystem::path path = std::filesystem::current_path().parent_path().parent_path() / "tests" / "test_data" / filename;
    return path.string();
}

using ListCreateFromFileLogicTestParam = std::tuple<std::string, std::vector<std::string>, std::vector<int>>;

class ListCreateFromFileLogicTest : public ::testing::TestWithParam<ListCreateFromFileLogicTestParam>{
};

TEST_P(ListCreateFromFileLogicTest, CreateCorrect){
    auto [filename, expectedData, expectedRandom] = GetParam();
    std::string filepath = GetPathToTempTestFile(filename);

    List* list = list->CreateFromFile(filepath);

    EXPECT_EQ(list->GetNodesCount(), expectedData.size());

    ListNode* currentNode = list->GetHeadNode();
    
    for (size_t i = 0; i < expectedData.size(); i++){
        EXPECT_EQ(list->GetNodeData(currentNode)->GetData(), expectedData[i]);
        ListNode* tempNode;
        if (expectedRandom[i] == -1){
            EXPECT_FALSE(list->TryGetRandomNode(currentNode, tempNode));
        }
        else{
            EXPECT_TRUE(list->TryGetRandomNode(currentNode, tempNode));
            EXPECT_EQ(list->GetNodeData(tempNode)->GetData(), expectedData[expectedRandom[i]]);
        }
        list->TryGetNextNode(currentNode, currentNode);
    }
}

INSTANTIATE_TEST_SUITE_P(
    CorrectData,
    ListCreateFromFileLogicTest,
    ::testing::Values(
        std::make_tuple(std::string("valid0.in"), std::vector<std::string>{"apple", "banana", "carrot"}, std::vector<int>{2, -1, 1}),
        std::make_tuple(std::string("valid1.in"), std::vector<std::string>{"0", "1", "2", "3", "4", "5", "6", "7", "8", "9"}, std::vector<int>{1, 0, 3, 3, 4, 1, 1, 9, 7, 8})        
    )
);

using ListCreateFromFileWhenFileInvalidTestParam = std::string;

class ListCreateFromFileWhenFileInvalidTest : public ::testing::TestWithParam<ListCreateFromFileWhenFileInvalidTestParam>{
};

TEST_P(ListCreateFromFileWhenFileInvalidTest, MethodFallCorrect){
    std::string filename = GetParam();
    std::string filepath = GetPathToTempTestFile(filename);

    EXPECT_ANY_THROW(List* list = List::CreateFromFile(filepath));
}

INSTANTIATE_TEST_SUITE_P(
    InvalidFiles,
    ListCreateFromFileWhenFileInvalidTest,
    ::testing::Values(
        std::string("not_exist.in"),
        std::string("invalid.extension"),        
        std::string("tdir"),
        std::string("non_utf8.in"),
        std::string("content_format_invalid.in")    
    )
);

TEST(ListTests, ListSerializeDeserializeLogicCorrect){
    std::string filepath = GetPathToTempTestFile("list.out");
    List* list = new List();
    size_t nodesCount = 50;
    std::vector<ListNode*> createdNodes = std::vector<ListNode*>();
    for (size_t i = 0; i < nodesCount; i++){
        createdNodes.push_back(list->AddNodeToTheRight(new ListNodeData(std::to_string(i))));
    }
    for (size_t i = 1; i < nodesCount - 1; i++){
        list->SetRandomNodeForNode(createdNodes[i], createdNodes[i - 1]);
    }

    List::SerializeListToFile(list, filepath);
    List* factList = List::DeserializeFromFile(filepath);

    ListNode* factCurrentNode = factList->GetHeadNode();
    ListNode* expectedCurrentNode = list->GetHeadNode();
    for (size_t i = 1; i < nodesCount - 1; i++){
        list->TryGetNextNode(expectedCurrentNode, expectedCurrentNode);
        factList->TryGetNextNode(factCurrentNode, factCurrentNode);
        EXPECT_EQ(factList->GetNodeData(factCurrentNode)->GetData(), list->GetNodeData(expectedCurrentNode)->GetData());
        
        ListNode* factRandom;
        EXPECT_TRUE(factList->TryGetRandomNode(factCurrentNode, factRandom));
        ListNode* expectedRandom;
        EXPECT_TRUE(list->TryGetRandomNode(expectedCurrentNode, expectedRandom));
        EXPECT_EQ(factList->GetNodeData(factRandom)->GetData(), list->GetNodeData(expectedRandom)->GetData());
    }
}