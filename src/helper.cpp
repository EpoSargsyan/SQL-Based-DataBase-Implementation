
#include "helper/helper.h"

bool Helper::check_opeation(const std::string& op) {
    return ((op == "name") || (op == "surname") || (op == "email"));
}

void Helper::line_cutter(std::string& line) { 
   auto li = line.find(':');
   auto beg = line.begin();
   line.erase(beg + line.find(' '));
   line.erase(beg + line.find(' '));
   line.erase (beg, beg + li);  
   
}

std::string Helper::toLowerCase(std::string  s)  {
    std::string str;
    for (int i = 0; i < s.size(); ++i) {
        if (s[i] >= 'A' && s[i] <= 'Z') {
            s[i] += 32;
        }
    }
    str = s;
    return str;
}

std::vector<std::string> Helper::parser(const std::string& str, std::string& spliter) {
    std::vector<std::string> nstr = {};
    std::string tstr = {};
    for (int i = 0; i < str.size(); ++i) {
        if (spliter.find(str[i]) != std::string::npos) {
            if (tstr != "") {
                nstr.push_back(tstr);
                tstr.clear();
                continue;
            }
            continue;
        }
        tstr.push_back(str[i]);
    }
    nstr.push_back(tstr);
    return nstr;
}

void Helper::check_errors(const std::vector<std::string>& vec) {
    int count = 0;
    for (int i = 0; i < vec.size(); ++i) {
        std::string str =  toLowerCase(vec[i]);
        if (str == "name" || str == "surname" || str == "email") {
            ++count;
        }    
    }
    if (count != 3) {
        throw std::invalid_argument("Please enter a Name, Surname and Email");
    }
}

void Helper::check_task_errors(const std::vector<std::string>& vec) {
    int count = 0;
    for (int i = 0; i < vec.size(); ++i) {
        std::string str = toLowerCase(vec[i]);
        if (str == "name" || str == "user") {
            ++count;
        }  
    }
    if (count != 2) {
        throw std::invalid_argument("Please enter a Task name, User email");
    }
}

void Helper::check_command_name(const std::vector<std::string>& vec) {
    int count_for_command = 0;
    std::string str = "";
    for (int i = 0; i < vec.size(); ++i) {
        str = toLowerCase(vec[i]);
        if (str == "input" || str == "select" || str == "update" || str == "delete") {
            ++count_for_command;
        }
    }
    if (count_for_command <= 0) {
        throw std::invalid_argument("Please check the command name");
    }
}
