#include <stdexcept>
#include <unordered_set>
#include <filesystem>
#include <fstream>
#include <vector>
#include <tuple>
#include <unordered_map>

#include "../../src/heads/listnodedata.hpp"
#include "../../src/heads/listnode.hpp"
#include "../../src/heads/list.hpp"


List::List(){
    headNode_ = nullptr;
    tailNode_ = nullptr;
    nodesCount_ = 0;
    nodes_ = std::unordered_set<ListNode*>();
}

int List::GetNodesCount() const{
    return nodesCount_;
}

bool List::IsEmpty() const{
    return nodesCount_ == 0;
}

ListNode* List::GetHeadNode() const{
    if (nodesCount_ == 0){
        throw std::logic_error("Trying to get the head node of an empty list.");
    }
    return headNode_;
}

ListNode* List::GetTailNode() const{
    if (nodesCount_ == 0){
        throw std::logic_error("Trying to get the tail node of an empty list.");
    }
    return tailNode_;
}

ListNode* List::GetNodeAt(int index) const{
    if (index < 0 || index >= nodesCount_){
        throw std::invalid_argument("The node index was bigger or equal to the nodes count.");
    }
    ListNode* node = headNode_;
    for (int i = 1; i <= index; i++){
        node = node->GetNextNode();
    }
    return node;
}

bool List::ContainsNode(ListNode* node) const{
    if (node == nullptr){
        throw std::invalid_argument("The given node was null.");
    }
    bool contains = nodes_.find(node) != nullptr;
    return contains;
}

bool List::TryGetPreviousNode(ListNode* node, ListNode*& outPreviousNode) const{
    if (node == nullptr){
        throw std::invalid_argument("The given node was null.");
    }
    if (!ContainsNode(node)){
        throw std::invalid_argument("The given node doesn't belong to the list.");
    }
    bool has = node->HasPreviousNode();
    outPreviousNode = has ? node->GetPreviousNode() : nullptr;
    return has;
}

bool List::TryGetNextNode(ListNode* node, ListNode*& outNextNode) const{
    if (node == nullptr){
        throw std::invalid_argument("The given node was null.");
    }
    if (!ContainsNode(node)){
        throw std::invalid_argument("The given node doesn't belong to the list.");
    }
    bool has = node->HasNextNode();
    outNextNode = has ? node->GetNextNode() : nullptr;
    return has;
}

bool List::TryGetRandomNode(ListNode* node, ListNode*& outRandomNode) const{
    if (node == nullptr){
        throw std::invalid_argument("The given node was null.");
    }
    if (!ContainsNode(node)){
        throw std::invalid_argument("The given node doesn't belong to the list.");
    }
    if (!node->HasRandomNode()){
        outRandomNode = nullptr;
        return false;
    }
    ListNode* randomNode = node->GetRandomNode();
    bool randomNodeBelongToList = ContainsNode(randomNode);
    if (!randomNodeBelongToList){
        node->SetRandomNode(nullptr);
        outRandomNode = nullptr;
        return false;
    }
    outRandomNode = randomNode;
    return true;
}

ListNodeData* List::GetNodeData(ListNode* node) const{
    if (node == nullptr){
        throw std::invalid_argument("The given node was null.");
    }
    if (!ContainsNode(node)){
        throw std::invalid_argument("The given node doesn't belong to the list.");
    }
    if (!node->HasData()){
        throw std::logic_error("The list node state was invalid, looks like internal program logic error.");
    }
    ListNodeData* data = node->GetData();
    return data;
}

void List::SetRandomNodeForNode(ListNode* node, ListNode* randomNode) const{
    if (node == nullptr){
        throw std::invalid_argument("The given node was null.");
    }
    if (!ContainsNode(node)){
        throw std::invalid_argument("The given node doesn't belong to the list.");
    }
    if (randomNode == nullptr){
        throw std::invalid_argument("The given random node was null.");
    }
    if (!ContainsNode(randomNode)){
        throw std::invalid_argument("The given random node doesn't belongs to the list.");
    }
    node->SetRandomNode(randomNode);
}

void List::SetDataForNode(ListNode* node, ListNodeData* data) const{
    if (node == nullptr){
        throw std::invalid_argument("The given node was null.");
    }
    if (!ContainsNode(node)){
        throw std::invalid_argument("The given node doesn't belong to the list.");
    }
    if (data == nullptr){
        throw std::invalid_argument("The given data was null.");
    }
    node->SetData(data);
}

void List::AfterNodeInsert(ListNode* newNode){
    nodes_.insert(newNode);
    nodesCount_++;
}

void List::BeforeNodeDelete(ListNode* nodeToRemove){
    nodes_.erase(nodeToRemove);
    nodesCount_--;
    nodeToRemove->Clear();
}

ListNode* List::AddNodeToTheLeft(ListNodeData* data){
    if (data == nullptr){
        throw std::invalid_argument("The data was null.");
    }
    ListNode* newNode = new ListNode(data);
    if (nodesCount_ == 0) {
        headNode_ = newNode;
        tailNode_ = newNode;
    }
    else {
        headNode_->SetPreviousNode(newNode);
        newNode->SetNextNode(headNode_);
        headNode_ = newNode;
    }
    AfterNodeInsert(newNode);
    return newNode;
}

ListNode* List::AddNodeToTheRight(ListNodeData* data){
    ListNode* newNode = new ListNode(data);
    if (nodesCount_ == 0) {
        headNode_ = newNode;
        tailNode_ = newNode;
    }
    else{
        tailNode_->SetNextNode(newNode);
        newNode->SetPreviousNode(tailNode_);
        tailNode_ = newNode;
    }
    AfterNodeInsert(newNode);
    return newNode;
}

ListNode* List::AddNodeAfter(ListNode* node, ListNodeData* data){
    if (!ContainsNode(node)){
        throw std::invalid_argument("The given node doesn't belongs to the list.");
    }
    if (node == tailNode_){
        return AddNodeToTheRight(data);
    }
    ListNode* nextNode = node->GetNextNode();
    ListNode* newNode = new ListNode(data);
    newNode->ConnectToLeft(node);
    newNode->ConnectToRight(nextNode);
    AfterNodeInsert(newNode);
    return newNode;
}

ListNode* List::AddNodeBefore(ListNode* node, ListNodeData* data){
    if (!ContainsNode(node)){
        throw std::invalid_argument("The given node doesn't belongs to the list.");
    }
    if (node == headNode_){
        return AddNodeToTheLeft(data);
    }
    ListNode* previousNode = node->GetPreviousNode();
    ListNode* newNode = new ListNode(data);
    newNode->ConnectToLeft(previousNode);
    newNode->ConnectToRight(node);
    AfterNodeInsert(newNode);
    return newNode;
}

void List::RemoveHeadNode(){
    if (nodesCount_ == 0){
        throw std::logic_error("The list is empty, nothing to delete.");
    }
    ListNode* nodeToRemove = headNode_;
    if (nodesCount_ == 1){
        headNode_ = nullptr;
        tailNode_ = nullptr;    
    }
    else{
        ListNode* temp = headNode_;
        headNode_ = nodeToRemove->GetNextNode();
        headNode_->SetPreviousNode(nullptr);
    }
    BeforeNodeDelete(nodeToRemove);
    delete nodeToRemove;
    int i = 0;
}

void List::RemoveTailNode(){
    if (nodesCount_ == 0){
        throw std::logic_error("The list is empty, nothing to delete.");
    }
    ListNode* nodeToRemove = tailNode_;
    if (nodesCount_ == 1){
        headNode_ = nullptr;
        tailNode_ = nullptr;
    }
    else{
        tailNode_ = nodeToRemove->GetPreviousNode();
        tailNode_->SetNextNode(nullptr);
    }
    BeforeNodeDelete(nodeToRemove);
    delete nodeToRemove;
}

void List::RemoveNode(ListNode* node){
    if (!ContainsNode(node)){
        throw std::invalid_argument("The given node doesn't belong to the list.");
    }
    if (nodesCount_ == 0){
        throw std::logic_error("The list is empty, nothing to delete.");
    }
    if (node == headNode_){
        RemoveHeadNode();
        return;
    }
    if (node == tailNode_){
        RemoveTailNode();
        return;
    }
    ListNode* previousNode = node->GetPreviousNode();
    ListNode* nextNode = node->GetNextNode();
    previousNode->ConnectToRight(nextNode);
    BeforeNodeDelete(node);
    delete node;
}

static bool TryParseInFileLine(std::string line, ListNodeData*& outData, int& outRandomIndex){
    size_t delimiterIndex = line.find(';');
    if (delimiterIndex == std::string::npos || delimiterIndex == line.length() - 1){
        return false;
    }
    std::string dataStr = line.substr(0, delimiterIndex);
    ListNodeData* data;
    try{
        data = new ListNodeData(dataStr);
    }
    catch(...){
        return false;
    }
    std::string randomIndexStr = line.substr(delimiterIndex + 1, line.length() - delimiterIndex - 1);
    int randomIndex;
    try{
        randomIndex = std::stoi(randomIndexStr);
    }
    catch(...){
        return false;
    }
    if (randomIndex < -1){
        return false;
    }
    outData = data;
    outRandomIndex = randomIndex;
    return true;
}

static bool TryParseInFile(const std::string& filepath, std::vector<std::tuple<ListNodeData*, int>>& dataRandomNodeIndexSequence){
    bool success = true;
    std::ifstream fileStream;
    try{
        fileStream = std::ifstream(filepath);
        std::string currentLine;
        while(std::getline(fileStream, currentLine)){
            ListNodeData* data;
            int randomIndex;
            if(!TryParseInFileLine(currentLine, data, randomIndex)){
                success = false;
                break;
            }
            dataRandomNodeIndexSequence.push_back(std::make_tuple(data, randomIndex));
        }
    }
    catch(...){
        fileStream.close();
        return false;
    }
    fileStream.close();
    return success;
}

static bool TryCreateListFromParsedData(const std::vector<std::tuple<ListNodeData*, int>>& dataRandomSequence, List*& outList){
    bool success = true;
    std::vector<ListNode*> createdNodes = std::vector<ListNode*>();
    List* list = new List();
    int sequenceLength = dataRandomSequence.size();
    for (size_t i = 0; i < dataRandomSequence.size(); i++){
        int currentRandom = std::get<1>(dataRandomSequence[i]);
        if (currentRandom >= sequenceLength){
            success = false;
            break;
        }
        ListNode* addedNode = list->AddNodeToTheRight(std::get<0>(dataRandomSequence[i]));
        createdNodes.push_back(addedNode);
    }
    if (!success){
        return false;
    }
    for (size_t i = 0; i < createdNodes.size(); i++){
        int randomIndex = std::get<1>(dataRandomSequence[i]);
        if (randomIndex != -1){
            ListNode* node = createdNodes[i];
            ListNode* randomNode = createdNodes[randomIndex];
            list->SetRandomNodeForNode(node, randomNode);
        }
    }
    outList = list;  
    return success;
}

List* List::CreateFromFile(const std::string& filepath){
    if(!std::filesystem::exists(filepath)){
        throw std::invalid_argument("The given file doesn't exists.");
    }
    if (!std::filesystem::is_regular_file(filepath)){
        throw std::invalid_argument("The given path reference to the directory.");
    }
    if (std::filesystem::path(filepath).extension() != ".in"){
        throw std::invalid_argument("Only files with '.in' extension accepted.");
    }

    std::vector<std::tuple<ListNodeData*, int>> dataRandomSequence = std::vector<std::tuple<ListNodeData*, int>>();
    if (!TryParseInFile(filepath, dataRandomSequence)){
        throw std::invalid_argument("File parse failed, looks like the file format invalid.");
    };
    
    List* list;
    if (!TryCreateListFromParsedData(dataRandomSequence, list)){
        throw std::invalid_argument("File handle failed, looks like the file content invalid.");
    }
    
    return list;
}

static std::unordered_map<ListNode*, size_t> GetListNodeToIndexMap(List* list){
    std::unordered_map<ListNode*, size_t> nodeToIndex = std::unordered_map<ListNode*, size_t>();
    ListNode* currentNode = list->GetHeadNode();    
    for (size_t i = 0; i < list->GetNodesCount(); i++){
        nodeToIndex.insert(std::pair(currentNode, i));
        list->TryGetNextNode(currentNode, currentNode);
    }
    return nodeToIndex;
}

static std::vector<std::tuple<std::string, int>> MapListToDataRandomSequence(List* list){
    std::vector<std::tuple<std::string, int>> sequence = std::vector<std::tuple<std::string, int>>();
    if (list->IsEmpty()){
        return sequence;
    }
    std::unordered_map<ListNode*, size_t> nodeToIndex = GetListNodeToIndexMap(list);
    ListNode* currentNode = list->GetHeadNode();    
    for (size_t i = 0; i < list->GetNodesCount(); i++){
        std::string currentData = list->GetNodeData(currentNode)->GetData();
        ListNode* randomNode;
        int currentRandomIndex = list->TryGetRandomNode(currentNode, randomNode) ? nodeToIndex[randomNode] : -1;
        sequence.push_back(std::make_tuple(currentData, currentRandomIndex));
        list->TryGetNextNode(currentNode, currentNode);
    }
    return sequence;
}

static bool TrySerializeStringIntSequenceToBinaryFile(std::string filepath, std::vector<std::tuple<std::string, int>> sequence){
    std::ofstream fileStream;
    try{
        fileStream = std::ofstream(filepath, std::ios::binary | std::ios::trunc);

        size_t sequenceLength = sequence.size();
        const char* sequenceLengthBytes = reinterpret_cast<const char*>(&sequenceLength);
        fileStream.write(sequenceLengthBytes, sizeof(sequenceLength));

        for (const auto pair: sequence){
            std::string str = std::get<0>(pair);
            size_t strLength = str.length();
            int intValue = std::get<1>(pair);

            const char* strLengthBytes = reinterpret_cast<const char*>(&strLength);
            const char* intBytes = reinterpret_cast<const char*>(&intValue);
            
            fileStream.write(strLengthBytes, sizeof(strLength));
            fileStream.write(str.c_str(), strLength * sizeof(char));
            fileStream.write(intBytes, sizeof(int));
        }

    }
    catch(...){
        fileStream.close();
        return false;
    }
    fileStream.close();
    return true;
}


void List::SerializeListToFile(List* list, const std::string& filepath){
    if (std::filesystem::path(filepath).extension() != ".out"){
        throw std::invalid_argument("The extension of output file must be '.out'.");
    }
    auto sequence = MapListToDataRandomSequence(list);
    if (!TrySerializeStringIntSequenceToBinaryFile(filepath, sequence)){
        throw std::logic_error("Something went wrong while process list serialization.");
    };
}

bool TryCreateListFromStringIntSequence(const std::vector<std::tuple<std::string, int>>& sequence, List*& list){
    list = new List();
    std::vector<ListNode*> createdNodes = std::vector<ListNode*>(); 
    try{
        for (const auto pair: sequence){
        std::string data = std::get<0>(pair);
        ListNodeData* nodeData = new ListNodeData(data);
        createdNodes.push_back(list->AddNodeToTheRight(nodeData));
        }
        for (int i = 0; i < sequence.size(); i++){
            int randomIndex = std::get<1>(sequence[i]);
            if (randomIndex != -1){
                list->SetRandomNodeForNode(createdNodes[i], createdNodes[randomIndex]);
            }
        }
    }
    catch(...){
        return false;
    }
    return true;
}

static bool TryDeserializeStringIntSequenceFromFile(std::string filepath, std::vector<std::tuple<std::string, int>>& sequence){
    std::ifstream fileStream;
    try{
        fileStream = std::ifstream(filepath);
        sequence = std::vector<std::tuple<std::string, int>>();
        
        size_t sequenceLength;
        char* sequenceLengthBytes =reinterpret_cast<char*>(&sequenceLength); 
        fileStream.read(sequenceLengthBytes, sizeof(sequenceLength));

        for (size_t i = 0; i < sequenceLength; i++){
            size_t strLength;
            char* strLengthBytes = reinterpret_cast<char*>(&strLength);
            fileStream.read(strLengthBytes, sizeof(strLength));
            
            std::string str = std::string(strLength, '\0');
            fileStream.read(&str[0], strLength * sizeof(char));

            int intValue;
            char* intValueBytes = reinterpret_cast<char*>(&intValue);
            fileStream.read(intValueBytes, sizeof(intValue));

            sequence.push_back(std::make_tuple(str, intValue));
        }
    }
    catch(...){
        fileStream.close();
        return false;
    }
    fileStream.close();
    return true;
}

List* List::DeserializeFromFile(const std::string& filepath){
    if(!std::filesystem::exists(filepath)){
        throw std::invalid_argument("The given file doesn't exists.");
    }
    if (!std::filesystem::is_regular_file(filepath)){
        throw std::invalid_argument("The given path reference to the directory.");
    }
    if (std::filesystem::path(filepath).extension() != ".out"){
        throw std::invalid_argument("Only files with '.out' extension accepted.");
    }
    std::vector<std::tuple<std::string, int>> sequence;
    if (!TryDeserializeStringIntSequenceFromFile(filepath, sequence)){
        throw std::logic_error("Something went wrong while process file deserialization.");
    }
    List* list;
    if(!TryCreateListFromStringIntSequence(sequence, list)){
        throw std::logic_error("Something went wrong while process file deserialization.");
    };   
    return list;
}