#include <gtest/gtest.h>
#include <string>

#include "../src/heads/listnodedata.hpp"


class NodeDataInitializationTest : public ::testing::TestWithParam<std::string>{
};

TEST_P(NodeDataInitializationTest, InitializeCorrectly) {
    std::string data = GetParam();

    ListNodeData nodeData = ListNodeData(data);

    EXPECT_TRUE(nodeData.GetData() == data);
}

INSTANTIATE_TEST_SUITE_P(
    CorrectInput,
    NodeDataInitializationTest,
    ::testing::Values(
        std::string(""),
        std::string("string"),
        std::string("1290_string***.&"),
        std::string(570, '^'),
        std::string(999, '-')
    )
);

class NodeDataInitializationFallTest : public ::testing::TestWithParam<std::string>{
};

TEST_P(NodeDataInitializationFallTest, InitializationThrowsException) {
    std::string data = GetParam();
    
    EXPECT_ANY_THROW({
        ListNodeData nodeData = ListNodeData(data);
    });
}

INSTANTIATE_TEST_SUITE_P(
    BadInput,
    NodeDataInitializationFallTest,
    ::testing::Values(
        std::string("\xCF\xF0\xE8\xE2\xE5\xF2 \xEC\xE8\xF0!"),
        std::string("Caf\xE9 \xA9 100\xAA"),
        std::string("Invalid \xFF\xFE\xFD bytes \x80\x81\x82"),
        std::string(1001, '*'),
        std::string(1200, '&')
    )
);