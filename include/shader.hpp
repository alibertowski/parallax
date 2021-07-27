#pragma once

#include <string>

class Shader {
public:
    Shader(std::string fileName);

    inline const std::string& get_source_code() const { return source_code; };
private:
    std::string source_code;
};
