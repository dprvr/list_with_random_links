#include <gtest/gtest.h>
#include <tuple>
#include <string>

#define LISTNODE_UNIT_TESTS

#include "../src/heads/listnodedata.hpp"
#include "../src/heads/listnode.hpp"


ListNodeData* createListNodeData(const char* data){
    return new ListNodeData(std::string(data));
}

ListNode* createListNode(const char* data){
    return new ListNode(createListNodeData(data));
}


class NodeInitializationTest : public ::testing::TestWithParam<ListNodeData*>{
};

TEST_P(NodeInitializationTest, InitializeCorrectly){
    ListNodeData* data = GetParam();
    
    ListNode* listNode = new ListNode(data);

    EXPECT_EQ(listNode->GetData(), data);
    EXPECT_EQ(listNode->GetNextNode(), nullptr);
    EXPECT_EQ(listNode->GetPreviousNode(), nullptr);
    EXPECT_EQ(listNode->GetRandomNode(), nullptr);
}

INSTANTIATE_TEST_SUITE_P(
    ValidInput,
    NodeInitializationTest,
    ::testing::Values(
        nullptr,
        createListNodeData(""),
        createListNodeData("data"),
        createListNodeData("6728"),
        createListNodeData("JO__**__JO")
    )
);

using NodeGettersAndSettersTestParam = std::tuple<ListNode*, ListNode*, ListNode*, ListNodeData*>;

class NodeGettersAndSettersTest: public ::testing::TestWithParam<NodeGettersAndSettersTestParam>{ 
};

TEST_P(NodeGettersAndSettersTest, NodeGettersAndSettersLogicCorrect){
    auto [previous, random, next, data] = GetParam();

    ListNode* listNode = new ListNode(new ListNodeData(std::string("some")));

    listNode->SetPreviousNode(previous);
    listNode->SetRandomNode(random);
    listNode->SetNextNode(next);
    listNode->SetData(data);

    EXPECT_EQ(listNode->GetPreviousNode(), previous);
    EXPECT_EQ(listNode->GetRandomNode(), random);
    EXPECT_EQ(listNode->GetNextNode(), next);
    EXPECT_EQ(listNode->GetData(), data);
}

INSTANTIATE_TEST_SUITE_P(
    CorrectInput,
    NodeGettersAndSettersTest,
    ::testing::Combine(
        ::testing::Values(nullptr, createListNode("1")),
        ::testing::Values(nullptr, createListNode("2")),
        ::testing::Values(nullptr, createListNode("3")),
        ::testing::Values(nullptr, createListNodeData("data"))
    )   
);

using NodeHasMethodsTestParam = std::tuple<ListNode*, ListNode*, ListNode*, ListNodeData*>;

class NodeHasMethodsTest : public ::testing::TestWithParam<NodeHasMethodsTestParam>{
};

TEST_P(NodeHasMethodsTest, NodeHasMethodsLogicCorrect){
    auto [previous, random, next, data] = GetParam();

    ListNode* listNode = new ListNode(new ListNodeData(std::string("some")));

    listNode->SetPreviousNode(previous);
    listNode->SetRandomNode(random);
    listNode->SetNextNode(next);
    listNode->SetData(data);

    EXPECT_EQ(listNode->HasPreviousNode(), previous != nullptr);
    EXPECT_EQ(listNode->HasRandomNode(), random != nullptr);
    EXPECT_EQ(listNode->HasNextNode(), next != nullptr);
    EXPECT_EQ(listNode->HasData(), data != nullptr);
}

INSTANTIATE_TEST_SUITE_P(
    CorrectInput,
    NodeHasMethodsTest,
    ::testing::Combine(
        ::testing::Values(nullptr, createListNode("1")),
        ::testing::Values(nullptr, createListNode("2")),
        ::testing::Values(nullptr, createListNode("3")),
        ::testing::Values(nullptr, createListNodeData("data"))
    )   
);

using NodeClearTestParam = std::tuple<ListNode*, ListNode*, ListNode*, ListNodeData*>;

class NodeClearTest : public ::testing::TestWithParam<NodeClearTestParam>{
};

TEST_P(NodeClearTest, NodeClearLogicCorrect){
    auto [previous, random, next, data] = GetParam();
    ListNode* listNode = new ListNode(new ListNodeData(std::string("some")));
    listNode->SetPreviousNode(previous);
    listNode->SetRandomNode(random);
    listNode->SetNextNode(next);
    listNode->SetData(data);

    listNode->Clear();

    EXPECT_FALSE(listNode->HasPreviousNode());
    EXPECT_FALSE(listNode->HasRandomNode());
    EXPECT_FALSE(listNode->HasNextNode());
    EXPECT_FALSE(listNode->HasData());
}

INSTANTIATE_TEST_SUITE_P(
    CorrectInput,
    NodeClearTest,
    ::testing::Combine(
        ::testing::Values(nullptr, createListNode("1")),
        ::testing::Values(nullptr, createListNode("2")),
        ::testing::Values(nullptr, createListNode("3")),
        ::testing::Values(nullptr, createListNodeData("data"))
    )   
);

using NodeConnectionsSetMethodsTestParam = std::tuple<ListNode*, ListNode*, ListNode*>;

class NodeConnectionsSetMethodsTest : public ::testing::TestWithParam<NodeConnectionsSetMethodsTestParam>{
};

TEST_P(NodeConnectionsSetMethodsTest, SetConnectionsLogicCorrect){
    auto [left, middle, right] = GetParam();

    EXPECT_NE(left->GetNextNode(), middle);
    EXPECT_NE(middle->GetPreviousNode(), left);
    EXPECT_NE(middle->GetNextNode(), right);
    EXPECT_NE(right->GetPreviousNode(), middle);

    middle->ConnectToLeft(left);
    middle->ConnectToRight(right);

    EXPECT_EQ(left->GetNextNode(), middle);
    EXPECT_EQ(middle->GetPreviousNode(), left);
    EXPECT_EQ(middle->GetNextNode(), right);
    EXPECT_EQ(right->GetPreviousNode(), middle);
}

INSTANTIATE_TEST_SUITE_P(
    CorrectInput,
    NodeConnectionsSetMethodsTest,
    ::testing::Values(
        std::tuple{createListNode("1"), createListNode("2"), createListNode("3")}
    )
);

using NodeConnectionsSetMethodsFallTestParam = std::tuple<ListNode*, ListNode*>;

class NodeConnectionsSetMethodsFallTest : public ::testing::TestWithParam<NodeConnectionsSetMethodsFallTestParam>{
};

TEST_P(NodeConnectionsSetMethodsFallTest, FallCorrectly){
    auto [left, right] = GetParam();
    ListNode* node = createListNode("data");

    EXPECT_ANY_THROW({
        node->ConnectToLeft(left);
        node->ConnectToRight(right);
    });
}

INSTANTIATE_TEST_SUITE_P(
    BadInput,
    NodeConnectionsSetMethodsFallTest,
    ::testing::Values(
        std::tuple{nullptr, createListNode("1")},
        std::tuple{createListNode("1"), nullptr},
        std::tuple{nullptr, nullptr}
    )
);