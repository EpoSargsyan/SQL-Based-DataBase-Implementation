#pragma once
#include "../helper/helper.h"
#include "../comand.h"
#include "user.h"
#include "task.h"

class MySQL {
public:
    MySQL();
    ~MySQL();
    void execute(const std::string&);

private:
    Helper* m_help;
    enum Commands { INPUT = 1, UPDATE, DELETE, SELECT, EXIT };
    enum File_name { USER = 1, PROJECT, TASK };
    std::unordered_map<std::string, Commands> m_map;
    std::unordered_map <std::string, File_name> m_filename;
    std::unordered_map <File_name, Comand*> m_command_par;

private:
    auto get_info(std::vector<std::string>, const std::string&);
    bool check_command(const std::string&);
    void init();
};