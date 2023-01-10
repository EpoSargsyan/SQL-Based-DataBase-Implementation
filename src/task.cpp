// #include "header/task.h"
#include "header/task.h"

std::ostream& operator<<(std::ostream& os, Taskinfo const& t_info) {  
    os  << "Name: "     << t_info.name      << std::endl
        << "User: "     << t_info.user_mail << std::endl
        << "Created: "  << t_info.time     << std::endl
        << "Deadline: " << t_info.deadline  << std::endl;
    return os;
}

Task::Task() {
    init();
}

Task::~Task() {}

void Task::init() {
    check_keywords["{"] = keywords::SKOB;
    check_keywords["id"] = keywords::ID;
    check_keywords["name"] = keywords::NAME;
    check_keywords["user"] = keywords::USER;
    check_keywords["created"] = keywords::CREATED;
    check_keywords["deadline"] = keywords::DEADLINE;

    m_check_command[keywords::ID] = "id";
    m_check_command[keywords::NAME] = "name";
    m_check_command[keywords::USER] = "user";
    m_check_command[keywords::CREATED] = "created";
    m_check_command[keywords::DEADLINE] = "deadline";
    
    flags_upd["id"] = false;
    flags_upd["name"] = false;
    flags_upd["user"] = false;
    flags_upd["deadline"] = false; 

    oriented_keys["where"] = keywords_to_upd::WHERE;
    oriented_keys["set"] = keywords_to_upd::SET;
}

void Task::fetch(const std::string& filename) {
    std::string str1;
    fs.open("/Users/epremsargsyan/Desktop/MY_SQL/MYSQL/DataBase/user.txt");
    
    if (!fs.is_open()) {
        throw std::runtime_error("Cannot open user.txt");
    }
    std::string spliter = " : ";
    std::vector<std::string> vec;
    while (!fs.eof()) {
        std::getline(fs, str1);
        if (str1.find("Email:") != std::string::npos) {
            m_help->line_cutter(str1);
            static int count = 1;
            m_user_data.insert(std::make_pair(count, str1));
            ++count;   
        }
    }
    fs.close();

    fs.open("/Users/epremsargsyan/Desktop/MY_SQL/MYSQL/DataBase/task.txt");
    if (!fs.is_open()) {
        std::runtime_error("Failed to open DataBase/task.txt file");
    }

    std::string str;
    spliter = " :}";
    std::string line;
    std::streampos m_value;
    static int user_id_tmp = 1;
    std::vector<std::string> m_vec;

    while (!fs.eof()) {    
        getline(fs, str);
        auto m_vec = m_help->parser(str, spliter);
        m_value = fs.tellg();
        Taskinfo* tmp_obj = new Taskinfo();
        
        if (static_cast<bool>(check_keywords[m_vec[0]])) { 
            m_fetch_info[tmp_obj] = m_value;

            m_value = fs.tellg();
            std::getline(fs.seekg(m_value), line);
            m_help->line_cutter(line);
            tmp_obj->ID = std::to_string(user_id_tmp++);

            m_value = fs.tellg();
            std::getline(fs.seekg(m_value), line);
            m_help->line_cutter(line);
            tmp_obj->name = line;
            
            m_value = fs.tellg();
            std::getline(fs.seekg(m_value), line);
            m_help->line_cutter(line);
            tmp_obj->status = std::stoi(line);

            m_value = fs.tellg();
            std::getline(fs.seekg(m_value), line);
            m_help->line_cutter(line);
            tmp_obj->time = line;

            m_value = fs.tellg();
            std::getline(fs.seekg(m_value), line);
            m_help->line_cutter(line);
            tmp_obj->user_mail = line;

            m_value = fs.tellg();
            std::getline(fs.seekg(m_value), line);
            m_help->line_cutter(line);
            tmp_obj->deadline = line;
        }   
    }
    fs.close();
}

void Task::flusher() {
    fs.open("/Users/epremsargsyan/Desktop/MY_SQL/MYSQL/DataBase/task.txt");
    
    if (!fs.is_open()) {
        throw std::runtime_error("Cannot open task.txt");
    }
    static int counter = 1;
    if (!m_fetch_info.empty()) {
        for (auto it : m_fetch_info) {
            fs  << "{" << std::endl 
                <<  "    " <<        "id:"           << counter++ << std::endl  
                <<  "    " <<        "Name: "        << it.first->name << std::endl 
                <<  "    " <<        "Status: "      << it.first->status << std::endl
                <<  "    " <<        "Created: "     << it.first->time << std::endl
                <<  "    " <<        "User email: "  << it.first->user_mail << std::endl
                <<  "    " <<        "Deadline: "    << it.first->deadline << std::endl 
                << "}" << std::endl;
        }
    }

    if (!m_task_info.empty()) {
        for (auto it : m_task_info) {
            fs  << "{" << std::endl 
                <<  "    " <<        "id:"           << counter++               << std::endl  
                <<  "    " <<        "Name: "        << it.second->name         << std::endl 
                <<  "    " <<        "Status: "      << it.second->status       << std::endl
                <<  "    " <<        "Created: "     << it.second->time         << std::endl
                <<  "    " <<        "User email: "  << it.second->user_mail    << std::endl
                <<  "    " <<        "Deadline: "    << it.second->deadline     << std::endl 
                << "}"                                                          << std::endl;
        }
    }
    fs.close();
}

void Task::_delete(const std::vector<std::string>& args) {
    auto throw_it = std::find(args.begin(), args.end(), "where");
    if (throw_it == args.end()) {
        throw std::invalid_argument("Please enter delete in task.txt  where...");
    }
    auto begin = std::find(args.begin(), args.end(), "task.txt");
    
    std::vector<std::string> delete_commands;
    while (*++begin != "where") {
        if (check_keywords.find(*begin) != check_keywords.end()) {
            delete_commands.push_back(*begin);
        }   
    }

    std::vector<std::string> keywords_info;
    while (++begin != args.end()) {
        keywords_info.push_back(*begin);   
    }
    
    std::string name;
    auto first_comand = std::find(keywords_info.begin(), keywords_info.end(), "name");
    if (first_comand != keywords_info.end()) {
        auto tmp_it = first_comand;
        ++tmp_it;
        auto second_comand = std::find_if (++first_comand, keywords_info.end(), [&] (std::string command) { return  check_keywords.find(command) != check_keywords.end(); }); 
        auto tmp_it2 = second_comand;
        
        while (first_comand != second_comand) {    
            name += *first_comand;
            name += ' ';
            ++first_comand;
        }
        keywords_info.erase(tmp_it, tmp_it2);
        name.erase(name.begin() + name.size() - 1);
    }

    std::vector<std::string> task_info;
    for (auto it : keywords_info) {   
        if (!static_cast<bool>(check_keywords[it])) {
            task_info.push_back(it);
            keywords_info.erase(std::remove(keywords_info.begin(), keywords_info.end(), it), keywords_info.end());  
        }
    }
    
    auto commands = delete_commands.begin();
    auto task = task_info.begin();
    auto keys = keywords_info.begin();
    bool flag = false;
    
    if (!m_fetch_info.empty()) {
        for (auto it : m_fetch_info) {
            if (it.first->status == -1) {
                continue;
            }
            auto commands = delete_commands.begin();
            auto task = task_info.begin();
            auto keys = keywords_info.begin();
            while (keys != keywords_info.end()) {
                switch (static_cast<int>(check_keywords[*keys])) {
                    case static_cast<int>(keywords::NAME) :     if (it.first->name == name) { flag = true; break; } 
                                                                else { flag = false; break; } 

                    case static_cast<int>(keywords::USER) :     if (it.first->user_mail == *task) { flag = true; break; }
                                                                else { flag = false; break; }

                    case static_cast<int>(keywords::DEADLINE) : if (it.first->deadline == *task) { flag = true; break; }
                                                                else { flag = false; break; }
                }
                if (!flag) { break; }
                ++keys;
                ++task;
            }
            if (flag) {
                if (delete_commands.empty()) {
                    it.first->status = -1;
                    continue;
                }
                while (commands != delete_commands.end()) {
                    switch (static_cast<int>(check_keywords[*commands])) {
                        case static_cast<int>(keywords::NAME) :         it.first->status = -1; break;
                        case static_cast<int>(keywords::USER) :         it.first->status = -1; break;
                        case static_cast<int>(keywords::DEADLINE) :     it.first->status = -1; break;
                    }
                    ++commands;
                }
            }
        }
    }
    if (!m_task_info.empty()) {
        for (auto it : m_task_info) {
            if (it.second->status == -1) {
                continue;
            }
            auto commands = delete_commands.begin();
            auto task = task_info.begin();
            auto keys = keywords_info.begin();
            while (keys != keywords_info.end()) {
                switch (static_cast<int>(check_keywords[*keys])) {
                    case static_cast<int>(keywords::NAME) :     if (it.second->name == name) { flag = true; break; } 
                                                                else { flag = false; break; } 

                    case static_cast<int>(keywords::USER) :     if (it.second->user_mail == *task) { flag = true; break; }
                                                                else { flag = false; break; }

                    case static_cast<int>(keywords::DEADLINE) : if (it.second->deadline == *task) { flag = true; break; }
                                                                else { flag = false; break; }
                }
                if (!flag) { break; }
                ++keys;
                ++task;
            }
            if (flag) {
                if (delete_commands.empty()) {
                    it.second->status = -1;
                    continue;
                }

                while (commands != delete_commands.end()) {
                    switch (static_cast<int>(check_keywords[*commands])) {
                        case static_cast<int>(keywords::NAME) :         it.second->status = -1; break;
                        case static_cast<int>(keywords::USER) :         it.second->status = -1; break;
                        case static_cast<int>(keywords::DEADLINE) :     it.second->status = -1; break;
                    }
                    ++commands;
                }
            }
        }
    }
}

void Task::select(const std::vector<std::string>& args) {
    auto throw_it = std::find(args.begin(), args.end(), "where");
    if (throw_it == args.end()) {
        throw std::invalid_argument("Please enter select in task.txt  ... where ...");
    }

    auto begin = std::find(args.begin(), args.end(), "task.txt");
    if (begin == args.end()) {
        throw std::invalid_argument("Please enter select in task.txt  ... where ...");
    }

    std::vector<std::string> select_commands;
    while (*++begin != "where") {
        if (check_keywords.find(*begin) != check_keywords.end()) {
            select_commands.push_back(*begin);
        }   
    }

    std::vector<std::string> keywords_info;
    while (++begin != args.end()) {
        keywords_info.push_back(*begin);   
    }
    
    std::string name;
    {
        auto first_comand = std::find(keywords_info.begin(), keywords_info.end(), "name");
        if (first_comand != keywords_info.end()) {
            auto tmp_it = first_comand;
            ++tmp_it;
            auto second_comand = std::find_if (++first_comand, keywords_info.end(), [&](std::string command) { return  check_keywords.find(command) != check_keywords.end(); }); 
            auto tmp_it2 = second_comand;
        
            while (first_comand != second_comand) {    
                name += *first_comand;
                name += ' ';
                ++first_comand;
            }
            keywords_info.erase(tmp_it, tmp_it2);
            name.erase(name.begin() + name.size() - 1);
        }
    }

    std::vector<std::string> task_info;
    for (auto it : keywords_info) {   
        if (!static_cast<bool>(check_keywords[it])) {
            task_info.push_back(it);
            keywords_info.erase(std::remove(keywords_info.begin(), keywords_info.end(), it), keywords_info.end());  
        }
    }
    
    auto commands = select_commands.begin();
    auto task = task_info.begin();
    auto keys = keywords_info.begin();
    bool flag = false;
    
    if (!m_fetch_info.empty()) {
        for (auto it : m_fetch_info) {
            if (it.first->status == -1) {
                continue;
            }
            auto commands = select_commands.begin();
            auto task = task_info.begin();
            auto keys = keywords_info.begin();

            while (keys != keywords_info.end()) {
                switch (static_cast<int>(check_keywords[*keys])) {
                    case static_cast<int>(keywords::NAME) :     if (it.first->name == name) { flag = true; break; } 
                                                                else { flag = false; break; } 

                    case static_cast<int>(keywords::USER) :     if (it.first->user_mail == *task) { flag = true; break; }
                                                                else { flag = false; break; }

                    case static_cast<int>(keywords::DEADLINE) : if (it.first->deadline == *task) { flag = true; break; }
                                                                else { flag = false; break; }
                }
                if (!flag) { break; }
                ++keys;
                ++task;
            }
            if (flag) {
                if (select_commands.empty()) {
                    std::cout << *it.first;
                    continue;
                }
                while (commands != select_commands.end()) {
                    switch (static_cast<int>(check_keywords[*commands])) {
                        case static_cast<int>(keywords::NAME) :     std::cout << "Name: " << it.first->name << std::endl; break;
                        case static_cast<int>(keywords::USER) :     std::cout << "User: " << it.first->user_mail << std::endl; break;
                        case static_cast<int>(keywords::DEADLINE) : std::cout << "Deadline: " << it.first->deadline << std::endl; break;
                    }
                    ++commands;
                }
            }
        }
    }
    if (!m_task_info.empty()) {
        for (auto it : m_task_info) {
            if (it.second->status == -1) {
                continue;
            }
            auto commands = select_commands.begin();
            auto task = task_info.begin();
            auto keys = keywords_info.begin();

            while (keys != keywords_info.end()) {
                switch (static_cast<int>(check_keywords[*keys])) {
                    case static_cast<int>(keywords::NAME) :     if (it.second->name == name) { flag = true; break; } 
                                                                else { flag = false; break; } 

                    case static_cast<int>(keywords::USER) :     if (it.second->user_mail == *task) { flag = true; break; }
                                                                else { flag = false; break; }

                    case static_cast<int>(keywords::DEADLINE) : if (it.second->deadline == *task) { flag = true; break; }
                                                                else { flag = false; break; }
                }
                if (!flag) { break; }
                ++keys;
                ++task;
            }

            if (flag) {
                if (select_commands.empty()) {
                    std::cout << *it.second;
                    continue;
                }
                while (commands != select_commands.end()) {
                    switch (static_cast<int>(check_keywords[*commands])) {
                        case static_cast<int>(keywords::NAME) :     std::cout << "Name: " << it.second->name << std::endl; break;
                        case static_cast<int>(keywords::USER) :     std::cout << "User: " << it.second->user_mail << std::endl; break;
                        case static_cast<int>(keywords::DEADLINE) : std::cout << "Deadline: " << it.second->deadline << std::endl; break;
                    }
                    ++commands;
                }
            }
        }
    }
}

void Task::input(const std::vector<std::string>& args) {
    m_help->check_task_errors(args);
    auto begin  = args.begin();
    Taskinfo* task_pr = new Taskinfo();
    std::string str;

    auto comand_part = std::find_if(begin, args.end(), [&](std::string str) {return   check_keywords.find(str) != check_keywords.end(); });
    time_t now  = std::time(0);
    while (comand_part != args.end()) {
        comand_part = std::find_if(begin, args.end(), [&](std::string str) {return check_keywords.find(str) != check_keywords.end(); });
        auto key = *comand_part;
        auto comand_part2 = std::find_if(++comand_part, args.end(), [&](std::string str) {return check_keywords.find(str) != check_keywords.end(); });
        
        while (comand_part != comand_part2) {
            if(check_keywords[key] == keywords::NAME) { str += *comand_part++; str+= ' ';continue; }
            str += *comand_part++;
        }
        if(check_keywords[key] == keywords::NAME) str.erase(str.begin() + str.size() -1);
        switch (static_cast<int>(check_keywords[key])) {
            case static_cast<int>(keywords::NAME):      task_pr->name = str; break;
                                                    
            case static_cast<int>(keywords::USER):      if (check_email(str))
                                                        task_pr->user_mail = str; break;

            case static_cast<int>(keywords::DEADLINE) : task_pr->deadline = str; break;
        }
        begin = comand_part;
        str.clear();
    }
    task_pr->time = ctime(&now);
    static int id = 1;
    task_pr->ID = std::to_string(id);
    m_task_info.insert(std::make_pair(id, *&task_pr));
    ++id;
}

void Task::update(const std::vector<std::string>& vec) {
    auto throw_it  = std::find(vec.begin(), vec.end(), "set");
    if (throw_it == vec.end()) {
        throw std::invalid_argument("Please enter task.txt  set ... where...");
    }

    auto throw_it2 = std::find(vec.begin(), vec.end(), "where");
    if (throw_it2 == vec.end()) {
        throw std::invalid_argument("Please enter task.txt  set ... where...");
    }

    auto begin = std::find(vec.begin(), vec.end(), "task.txt");
    ++begin;
    std::vector<std::string> update_keywords_before_where;
    while (*++begin != "where") {
        update_keywords_before_where.push_back(*begin);
    }

    std::vector<std::string> keywords_info_after_where;
    while (++begin != vec.end()) {
        keywords_info_after_where.push_back(*begin);
    }

    std::string name;
    {
        auto name_begin = std::find(update_keywords_before_where.begin(), update_keywords_before_where.end(), "name");
        if (name_begin != update_keywords_before_where.end()) {
            auto second_comand = std::find_if (++name_begin, update_keywords_before_where.end(), [&] (std::string command) { return check_keywords.find(command) != check_keywords.end(); });
            auto tmp1 = name_begin;
            auto tmp2 = second_comand;
            while (name_begin != second_comand) {
                name += *name_begin;
                name += ' ';
                ++name_begin;
            }
            update_keywords_before_where.erase(tmp1, tmp2);
            name.erase(name.begin() + name.size() - 1);
        }
    }

    std::string project;
    {
        auto begin = std::find(update_keywords_before_where.begin(), update_keywords_before_where.end(), "project");
        if (begin != update_keywords_before_where.end()) {
            auto second_comand = std::find_if (++begin, update_keywords_before_where.end(), [&] (std::string command) { return check_keywords.find(command) != check_keywords.end(); });
            auto tmp1 = begin;
            auto tmp2 = second_comand;
            while (begin != second_comand) {
                project += *begin;
                project += ' ';
                ++begin;
            }
            update_keywords_before_where.erase(tmp1, tmp2);
            project.erase(project.begin() + project.size() - 1);
        }
    }

    std::vector<std::string> update_info_before_where;
    for (auto it : update_keywords_before_where) {
        if (check_keywords.find(it) == check_keywords.end()) {
            update_info_before_where.push_back(it);
            update_keywords_before_where.erase(std::remove(update_keywords_before_where.begin(), update_keywords_before_where.end(), it),update_keywords_before_where.end());
        }
    }

    std::string find_name;
    {
        auto begin = std::find(keywords_info_after_where.begin(), keywords_info_after_where.end(),"name");
        if (begin != keywords_info_after_where.end()) {
            auto second_comand = std::find_if (++begin, keywords_info_after_where.end(), [&] (std::string command) { return  check_keywords.find(command) != check_keywords.end(); });
            auto tmp1 = begin;
            auto tmp2 = second_comand;
            while (begin != second_comand) {
                find_name += *begin;
                find_name += ' ';
                ++begin;
            }
            keywords_info_after_where.erase(tmp1, tmp2);
            find_name.erase(find_name.begin() + find_name.size() - 1);
        }
    }

    std::string find_project;
    {
        auto begin = std::find(keywords_info_after_where.begin(), keywords_info_after_where.end(), "project");
        if (begin != keywords_info_after_where.end()) {
            auto second_comand = std::find_if (++begin, keywords_info_after_where.end(), [&] (std::string command) { return  check_keywords.find(command) != check_keywords.end(); });
            auto tmp1 = begin;
            auto tmp2 = second_comand;
            while (begin != second_comand) {
                find_project += *begin;
                find_project += ' ';
                ++begin;
            }
            keywords_info_after_where.erase(tmp1, tmp2);
            find_project.erase(find_project.begin() + find_project.size() - 1);
        }
    }
    
    std::vector<std::string> find_task_after_where;
    for (auto& it : keywords_info_after_where) {
        if (check_keywords.find(it) == check_keywords.end()) {
            find_task_after_where.push_back(it);
            keywords_info_after_where.erase(std::remove(keywords_info_after_where.begin(), keywords_info_after_where.end(),it), keywords_info_after_where.end());
        }
    }

    auto upd_keywords_before_where = update_keywords_before_where.begin();
    auto info_before_where = update_info_before_where.begin();
    auto update_keywords_after_where = keywords_info_after_where.begin();
    auto task_after_where = find_task_after_where.begin();
    bool flag = false;
    if (!m_fetch_info.empty()) {
        for (auto& it : m_fetch_info) {
            if (it.first->status == -1) {
                continue;
            }
            auto upd_keywords_before_where = update_keywords_before_where.begin();
            auto info_before_where = update_info_before_where.begin();
            auto update_keywords_after_where = keywords_info_after_where.begin();
            auto task_after_where = find_task_after_where.begin();
            while (update_keywords_after_where != keywords_info_after_where.end()) {
                switch (static_cast<int>(check_keywords[*update_keywords_after_where])) {
                    case static_cast<int>(keywords::NAME) :     if (it.first->name == find_name) { flag = true; break; }
                                                                else { flag = false; break; }
                    case static_cast<int>(keywords::USER) :     if (it.first->user_mail == *task_after_where) { flag = true ; break; }
                                                                else { flag = false; break; }
                    case static_cast<int>(keywords::DEADLINE) : if (it.first->deadline == *task_after_where) { flag = true; break; }
                                                                else { flag = false; break; }
                    case static_cast<int>(keywords::PROJECT) :  if (it.first->project_name == find_project) { flag = true; break; }
                                                                else { flag = false; break; }
                }   
                if (!flag) { break; };
                ++update_keywords_after_where;
                ++task_after_where;
            }
            if (flag) {
                while (upd_keywords_before_where != update_keywords_before_where.end()) {
                    switch (static_cast<int>(check_keywords[*upd_keywords_before_where])) {
                        case static_cast<int>(keywords::NAME) : it.first->name = name; break;
                        case static_cast<int>(keywords::USER) : it.first->user_mail = *info_before_where; break;
                        case static_cast<int>(keywords::DEADLINE) : it.first->deadline = *info_before_where; break;
                        case static_cast<int>(keywords::PROJECT) : it.first->project_name = project; break;
                    }
                    ++upd_keywords_before_where;
                    ++info_before_where;
                }
            }
        }
    }
    if (!m_task_info.empty()) {
        for (auto& it : m_task_info) {
            if (it.second->status == -1) {
                continue;
            }
            auto upd_keywords_before_where = update_keywords_before_where.begin();
            auto info_before_where = update_info_before_where.begin();
            auto update_keywords_after_where = keywords_info_after_where.begin();
            auto task_after_where = find_task_after_where.begin();
            while (update_keywords_after_where != keywords_info_after_where.end()) {
                switch (static_cast<int>(check_keywords[*update_keywords_after_where])) {
                    case static_cast<int>(keywords::NAME) :     if (it.second->name == find_name) { flag = true; break;}
                                                                else { flag = false; break; }
                    case static_cast<int>(keywords::USER) :     if (it.second->user_mail == *task_after_where) { flag = true ; break; }
                                                                else { flag = false; break; }
                    case static_cast<int>(keywords::DEADLINE) : if (it.second->deadline == *task_after_where) { flag = true; break; }
                                                                else { flag = false; break; }
                    case static_cast<int>(keywords::PROJECT) :  if (it.second->project_name == find_project) { flag = true; break; }
                                                                else { flag = false; break; }
                }
                if (!flag) { break; };
                ++update_keywords_after_where;
                ++task_after_where;
            }
            if (flag) {
                while (upd_keywords_before_where != update_keywords_before_where.end()) {
                    switch (static_cast<int>(check_keywords[*upd_keywords_before_where])) {
                        case static_cast<int>(keywords::NAME) : it.second->name = name; break;
                        case static_cast<int>(keywords::USER) : it.second->user_mail = *info_before_where; break;
                        case static_cast<int>(keywords::DEADLINE) : it.second->deadline = *info_before_where; break;
                        case static_cast<int>(keywords::PROJECT) : it.second->project_name = project; break;
                    }
                    ++upd_keywords_before_where;
                    ++info_before_where;
                }
            }
        }
    }
}

bool Task::cmp_objs(const Taskinfo& lhs, Taskinfo* rhs) {
    if (flags_upd["id"]) {
        if (lhs.ID != rhs->name) { return false; }
    }
    if (flags_upd["name"]) {
        if (lhs.name != rhs->name) { return false; }
    }
    if (flags_upd["user"]) {
        if (lhs.user_mail != rhs->user_mail) { return false; }
    }
    if (flags_upd["deadline"]) {
        if (lhs.deadline != rhs->deadline) { return false; }
    }
    return true;
}

bool Task::check_email(const std::string& str) {
    bool flag = false;
    for (auto it : m_user_data) {
        if (it.second == str) {
            flag = true;
            break;
        }  
    }
    if (!flag) {
        throw std::invalid_argument("Invalid user email address");
    }
    return flag;
}

bool Task::check_task(const std::string& task_name) {
    std::string name = task_name;
    bool flag = false;
    for (auto it : m_fetch_info) {
        if (it.first->name == name) {
            flag = true;
            break;
        }
    }
    return flag;
}

bool Task::check_task(const std::string& project_name, const std::string& task_name) {
    bool flag = false;
    for (auto it : m_fetch_info) {
        if (it.first->name == task_name) {
            flag = true;
            break;
        }
    }
    return flag;
}

