#include <algorithm>
#include "comand.h"
#include "header/mysql.h"
#include "header/user.h" 
#include "header/task.h"
#include "header/project.h"
#include <locale>

void MySQL::execute(const std::string& rhs) {   
    static int a = 0;
    std::string spliter = " =";  
    std::vector<std::string> vec = m_help->parser(rhs, spliter);
    m_help->check_command_name(vec);
    static int b = 0;
    static int c = 0;
    auto file_name = std::find_if(vec.begin(), vec.end(),[&](std::string file_name){ return m_filename.find(file_name) != m_filename.end(); });
    auto comand_name = std::find_if(vec.begin(), vec.end(),[&](std::string command){ return m_map.find(command) != m_map.end(); });
    switch (static_cast<int>(m_filename[*file_name])) {
        case static_cast<int>(File_name::USER) :    if (b == 0) { ++b;m_command_par[m_filename[*file_name]]->fetch(*file_name); break; } break;
        case static_cast<int>(File_name::TASK) :    if (a == 0) { ++a; m_command_par[m_filename[*file_name]]->fetch(*file_name); break; } break;
        case static_cast<int>(File_name::PROJECT) : if (c == 0) { 
                                                                    ++c;
                                                                    m_command_par[m_filename["task.txt"]]->fetch(*file_name); 
                                                                    m_command_par[m_filename["user.txt"]]->fetch(*file_name);
                                                                    m_command_par[m_filename[*file_name]]->fetch(*file_name); break; 
                                                                }
    }
    switch (static_cast<int>(m_map[*comand_name])) {
        case static_cast<int>(Commands::INPUT)  : m_command_par[m_filename[*file_name]]->input(vec); break;
        case static_cast<int>(Commands::DELETE) : m_command_par[m_filename[*file_name]]->_delete(vec); break;
        case static_cast<int>(Commands::UPDATE) : m_command_par[m_filename[*file_name]]->update(vec); break;
        case static_cast<int>(Commands::SELECT) : m_command_par[m_filename[*file_name]]->select(vec); break;
    }   
}

void MySQL::init() {
    m_map["input"] = Commands::INPUT;
    m_map["update"] = Commands::UPDATE;
    m_map["delete"] = Commands::DELETE;
    m_map["select"] = Commands::SELECT;
    m_map["exit"] = Commands::EXIT;

    m_filename["user.txt"] = File_name::USER;
    m_filename["project.txt"] = File_name::PROJECT;
    m_filename["task.txt"] = File_name::TASK;
}

Comand::~Comand() {}

MySQL::MySQL() {
    User* user = new User();
    Task* task = new Task();
    m_command_par[File_name::USER] = user;
    m_command_par[File_name::TASK] = task;
    m_command_par[File_name::PROJECT] = new Project(user, task);
    init();
}

MySQL::~MySQL() {
    m_command_par[File_name::USER]->flusher();
    m_command_par[File_name::TASK]->flusher();
    m_command_par[File_name::PROJECT]->flusher();
    
    for (auto it : m_command_par) {
        delete it.second;
        it.second = nullptr;
    }
}

bool MySQL::check_command(const std::string& command) {
    if(m_map.find(command) != m_map.end()) {
        return true;
    }
    throw std::invalid_argument("invalid operation");
}
