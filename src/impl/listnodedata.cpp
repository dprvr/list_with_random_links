#include<stdexcept>
#include<string>

#include "../../src/heads/listnodedata.hpp"
#include "../../src/heads/utf8.h"


ListNodeData::ListNodeData(std::string data){
    if (!utf8::is_valid(data)){
        throw std::invalid_argument("The data must be string in UTF8 code.");
    }
    if (data.length() > 1000){
        throw std::invalid_argument("The data must be a string with length less or equal than 1000.");
    }
    data_ = std::string(data);
}

const std::string& ListNodeData::GetData() const{
    return data_;
}
