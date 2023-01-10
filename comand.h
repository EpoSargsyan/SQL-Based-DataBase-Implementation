#pragma once
#include <string>
#include <vector>
#include <map>
#include <unordered_map>
#include <iostream>
#include <iomanip>
#include "helper/helper.h"

class Comand {
public:
    virtual void select(const std::vector<std::string>&) = 0;
    virtual void _delete(const std::vector<std::string>&) = 0;
    virtual void input(const std::vector<std::string>&) = 0;
    virtual void update(const std::vector<std::string>&) = 0;
    virtual void fetch(const std::string& ) = 0;
    virtual void flusher() = 0;
    virtual ~Comand();
};