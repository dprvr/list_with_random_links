#pragma once

#include <string>


class ListNodeData {
    public:
        ListNodeData(std::string data);

        const std::string& GetData() const;
        
    private:
        std::string data_;
};
