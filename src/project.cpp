// #include "header/project.h"
#include "header/project.h"

std::ostream& operator<<(std::ostream& os, const Projectinfo& info) {
    if (!info.user.empty()) {
        os << "User/s: ";
        for (auto it : info.user) {
            os  << it << " ";
        }
    }
    os  << std::endl
        << "Task: " << info.task << std::endl
        << "Name: " << info.name << std::endl
        << "Description: " << info.description << std::endl
        << "Created: " << info.time << std::endl
        << "Deadline: " << info.deadline << std::endl;
    return os;
}

void Project::init() {
    check_keywords["{"] = keywords::SKOB;
    check_keywords["id"] = keywords::ID;
    check_keywords["name"] = keywords::NAME;
    check_keywords["description"] = keywords::DESCRIPTION;
    check_keywords["deadline"] = keywords::DEADLINE;
    check_keywords["user"] = keywords::USER;
    check_keywords["task"] = keywords::TASK;
}

Project::Project(User* user, Task* task) {
    m_user = user;
    m_task = task;
    init();
}

void Project::fetch(const std::string& filename) {
    fs.open("/Users/epremsargsyan/Desktop/MY_SQL/MYSQL/DataBase/project.txt");  
    if (!fs.is_open()) {
        fs.close();
        throw std::runtime_error("Cannot open project.txt");
    }
    std::string str = "";
    std::string spliter = " ";
    std::streampos m_value;
    std::string line = "";
    while (!fs.eof()) {
        getline(fs, str);
        auto vec = m_help->parser(str, spliter);
        m_value = fs.tellg();
        Projectinfo* tmp_obj = new Projectinfo();
        if (static_cast<bool>(check_keywords[vec[0]])) {
            m_fetch_project[tmp_obj] = m_value;
            m_value = fs.tellg();

            std::getline(fs.seekg(m_value), line);
            m_help->line_cutter(line);
            tmp_obj->id = line;
            m_value = fs.tellg();

            std::getline(fs.seekg(m_value), line);
            m_help->line_cutter(line);
            tmp_obj->user = m_help->parser(line, spliter);
            m_value = fs.tellg();

            std::getline(fs.seekg(m_value), line);
            m_help->line_cutter(line);
            tmp_obj->name = line;
            m_value = fs.tellg();

            std::getline(fs.seekg(m_value), line);
            m_help->line_cutter(line);
            tmp_obj->time = line;
            m_value = fs.tellg();

            std::getline(fs.seekg(m_value), line);
            m_help->line_cutter(line);
            tmp_obj->status = std::stoi(line);
            m_value = fs.tellg();

            std::getline(fs.seekg(m_value), line);
            m_help->line_cutter(line);
            tmp_obj->task = line;
            m_value = fs.tellg();

            std::getline(fs.seekg(m_value), line);
            m_help->line_cutter(line);
            tmp_obj->description = line;
            m_value = fs.tellg();

            std::getline(fs.seekg(m_value), line);
            m_help->line_cutter(line);
            tmp_obj->deadline = line;
        }
    }
    fs.close();
}

auto Project::check_project_errors(const std::vector<std::string>& args) {
    auto begin = std::find(args.begin(), args.end(), "name");
    if (begin == args.end()) {
        throw std::invalid_argument("Please enter a project name");
    }
    return begin;
}

void Project::input(const std::vector<std::string>& args) {
    check_project_errors(args);
    auto tmp = std::find(args.begin(), args.end(), "project.txt");
    std::vector<std::string> keywords_info;
    while (++tmp != args.end()) {
        keywords_info.push_back(*tmp);
    }
    std::string name =          spliter_for_commands(keywords_info, "name");
    std::string task_pos =      spliter_for_commands(keywords_info, "task");
    std::string description =   spliter_for_commands(keywords_info, "description");
    Projectinfo* project_obj = new Projectinfo();
    std::vector<std::string> users_vec;
    {
        auto user_pos = std::find(keywords_info.begin(), keywords_info.end(), "user");
        if (user_pos != keywords_info.end()) {
            auto second_command = std::find_if(++user_pos, keywords_info.end(), [&](std::string command) { return  check_keywords.find(command) != check_keywords.end(); });
            auto tmp1 = user_pos;
            auto tmp2 = second_command;
            while (user_pos != second_command) {
               if(m_user->check_user(*user_pos)) {
                    project_obj->user.push_back(*user_pos);
               } else {
                    std::cout << *user_pos << std::endl;
                    throw std::invalid_argument("User is undifinded");
               }
               ++user_pos;
            }
            keywords_info.erase(tmp1, tmp2);
        }
    }
    std::vector<std::string> project_info;
    for (auto it : keywords_info) {
        if (!static_cast<bool>(check_keywords[it])) {
            project_info.push_back(it);
            keywords_info.erase(std::remove(keywords_info.begin(), keywords_info.end(), it), keywords_info.end());
        }
    }
    auto key = keywords_info.begin();
    auto info = project_info.begin();
    while (key != keywords_info.end()) {
        switch (static_cast<int>(check_keywords[*key])) {
            case static_cast<int>(keywords::NAME) :         project_obj->name = name;               break;
            case static_cast<int>(keywords::DESCRIPTION) :  project_obj->description = description; break;
            case static_cast<int>(keywords::DEADLINE) :     project_obj->deadline = *info;          break;
            case static_cast<int>(keywords::TASK)   :       project_obj->task = task_pos;           break;
        }
        ++key;
    }
    time_t now  = std::time(0);
    project_obj->time = ctime(&now);
    static size_t id = 1;
    m_project_data.insert(std::make_pair(id, *&project_obj));
    ++id;
}

void Project::select(const std::vector<std::string>& args) {
    auto throw_it = std::find(args.begin(), args.end(), "where");
    if (throw_it == args.end()) {
        throw std::invalid_argument("Please enter select in task.txt  ... where ...");
    }

    auto begin = std::find(args.begin(), args.end(), "project.txt"); 
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

    std::string name = spliter_for_commands(keywords_info, "name");
    std::string description = spliter_for_commands(keywords_info, "description");
    std::string task = spliter_for_commands(keywords_info, "task");

    if(!m_task->check_task(name, task)) {
        std::cout << "Task is undefined" << std::endl;
        std::cout << "Do you want to add a task with this project name?" << std::endl;
        std::cout << "y/n" << std::endl;
        char question;
        do {
            std::cin >> question;
            std::cin.ignore(1);
        }
        while(question != 'y' && question != 'n');
        if (question == 'y') {
            std::string spliter = " ";
            std::vector<std::string> task_input = m_help->parser("please input in task.txt name  " + task + " project  " + name, spliter);
            m_task->input(task_input);
        } else {
            std::cout << std::endl;
            throw std::invalid_argument("OK");
        }
    }
    std::string user = spliter_for_commands(keywords_info, "user");
    std::string spliter = " ";
    std::vector<std::string> vec_for_usr = m_help->parser(user, spliter);

    std::vector<std::string> project_info;
    for (auto it : keywords_info) {
        if (!static_cast<bool>(check_keywords[it])) {
            project_info.push_back(it);
            keywords_info.erase(std::remove(keywords_info.begin(), keywords_info.end(), it), keywords_info.end());
        }
    }

    auto commands = select_commands.begin();
    auto proj = project_info.begin();
    auto keys = keywords_info.begin();
    bool flag = false;

    if (!m_fetch_project.empty()) {
        for (auto it : m_fetch_project) {
            if (it.first->status == -1) {
                continue;
            }
            auto commands = select_commands.begin();
            auto proj = project_info.begin();
            auto keys = keywords_info.begin();

            while (keys != keywords_info.end()) {
                switch (static_cast<int>(check_keywords[*keys])) {
                    case static_cast<int>(keywords::NAME) :         if (it.first->name == name) { flag = true; break; }
                                                                    else { flag = false; break; }
                    case static_cast<int>(keywords::DEADLINE) :     if (it.first->deadline == *proj) { flag = true; break; }
                                                                    else { flag = false; break; }
                    case static_cast<int>(keywords::DESCRIPTION) :  if (it.first->description == description) { flag = true; break; }
                                                                    else { flag = false; break; }
                    case static_cast<int>(keywords::USER) :         if (check_vec_equality(it.first->user, vec_for_usr)) { flag = true; break; }
                                                                    else { flag = false; break; }
                    case static_cast<int>(keywords::TASK) :         if (it.first->task == task) { flag = true; break; }    
                                                                    else { flag = false; break; }                                                               
                }
                if (!flag) { break; }
                ++keys;
                ++proj;
            }
            if (flag) {
                if (select_commands.empty()) {
                    std::cout << *it.first << std::endl;
                    continue;
                }

                while (commands != select_commands.end()) {
                    switch (static_cast<int>(check_keywords[*commands])) {
                        case static_cast<int>(keywords::NAME) : std::cout << "Name: " << it.first->name << std::endl; break;
                        case static_cast<int>(keywords::DESCRIPTION) : std::cout << "Description: " << it.first->description << std::endl; break;
                        case static_cast<int>(keywords::USER) : std::cout << "user: " << user << std::endl; break;
                        case static_cast<int>(keywords::TASK) : std::cout << "task: " << it.first->task << std::endl; break;
                        case static_cast<int>(keywords::DEADLINE) : std::cout << "deadline: " << it.first->deadline << std::endl; break;
                    }
                    ++commands;
                }
            }
        }
    }

    if (!m_project_data.empty()) {
        for (auto it : m_project_data) {
            if (it.second->status == -1) {
                continue;
            }
            auto commands = select_commands.begin();
            auto proj = project_info.begin();
            auto keys = keywords_info.begin();
            
            while (keys != keywords_info.end()) {
                switch (static_cast<int>(check_keywords[*keys])) {
                    case static_cast<int>(keywords::NAME) :         if (it.second->name == name) { flag = true; break; }
                                                                    else { flag = false; break; }
                    case static_cast<int>(keywords::DEADLINE) :     if (it.second->deadline == *proj) { flag = true; break; }
                                                                    else { flag = false; break; }
                    case static_cast<int>(keywords::DESCRIPTION) :  if (it.second->description == description) { flag = true; break; }
                                                                    else { flag = false; break; }
                    case static_cast<int>(keywords::USER) :         if (check_vec_equality(it.second->user, vec_for_usr)) { flag = true; break; }
                                                                    else { flag = false; break; }
                    case static_cast<int>(keywords::TASK) :         if (it.second->task == task) { flag = true; break; }    
                                                                    else { flag = false; break; }  
                }
                if (!flag) { break; }
                ++keys;
                ++proj;
            }
            if (flag) {
                if (select_commands.empty()) {
                    std::cout << *it.second << std::endl;
                    continue;
                }
                while (commands != select_commands.end()) {
                    switch (static_cast<int>(check_keywords[*commands])) {
                        case static_cast<int>(keywords::NAME) : std::cout << "Name: " << it.second->name << std::endl; break;
                        case static_cast<int>(keywords::DESCRIPTION) : std::cout << "Description: " << it.second->description << std::endl; break;
                        case static_cast<int>(keywords::USER) : std::cout << "user: " << user << std::endl; break;
                        case static_cast<int>(keywords::TASK) : std::cout << "task: " << it.second->task << std::endl; break;
                        case static_cast<int>(keywords::DEADLINE) : std::cout << "deadline: " << it.second->deadline << std::endl; break;
                    }
                    ++commands;
                }
            }
        }
    } 
}

void Project::update(const std::vector<std::string>& vec) {
    auto throw_it  = std::find(vec.begin(), vec.end(), "set");
    if (throw_it == vec.end()) {
        throw std::invalid_argument("Please enter project.txt  set ... where ...");
    }

    auto throw_it2 = std::find(vec.begin(), vec.end(), "where");
    if (throw_it2 == vec.end()) {
        throw std::invalid_argument("Please enter project.txt  set ... where ...");
    }

    auto begin = std::find(vec.begin(), vec.end(), "project.txt");
    ++begin;
    std::vector<std::string> update_keywords_before_where;
    while (*++begin != "where") {
        update_keywords_before_where.push_back(*begin);
    }

    std::vector<std::string> keywords_info_after_where;
    while (++begin != vec.end()) {
        keywords_info_after_where.push_back(*begin);
    }

    std::string name = spliter_for_commands(update_keywords_before_where, "name");
    std::string description = spliter_for_commands(update_keywords_before_where, "description");
    std::string user = spliter_for_commands(update_keywords_before_where, "user");
    std::string task = spliter_for_commands(update_keywords_before_where, "task");
    std::string spliter = " ";
    std::vector<std::string> vec_for_usr = m_help->parser(user, spliter);

    std::vector<std::string> update_info_before_where;
    for (auto it : update_keywords_before_where) {
        if (check_keywords.find(it) == check_keywords.end()) {
            update_info_before_where.push_back(it);
            update_keywords_before_where.erase(std::remove(update_keywords_before_where.begin(), update_keywords_before_where.end(), it),update_keywords_before_where.end());
        }
    }

    std::string find_name =  spliter_for_commands(keywords_info_after_where, "name");
    std::string find_description = spliter_for_commands(keywords_info_after_where, "description");
    std::string find_task = spliter_for_commands(keywords_info_after_where, "task");
    std::string find_user = spliter_for_commands(keywords_info_after_where, "user");
    std::vector<std::string> vec_for_find_usr = m_help->parser(find_user, spliter);

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

    if (!m_fetch_project.empty()) {
        for (auto& it : m_fetch_project) {
            if (it.first->status == -1) {
                continue;
            }
            auto upd_keywords_before_where = update_keywords_before_where.begin();
            auto info_before_where = update_info_before_where.begin();
            auto update_keywords_after_where = keywords_info_after_where.begin();
            auto task_after_where = find_task_after_where.begin();
            while (update_keywords_after_where != keywords_info_after_where.end()) {
                switch (static_cast<int>(check_keywords[*update_keywords_after_where])) {
                    case static_cast<int>(keywords::NAME) :         if (it.first->name == find_name) { flag = true; break; }
                                                                    else { flag = false; break; }
                    case static_cast<int>(keywords::DESCRIPTION) :  if (it.first->description == find_description) { flag = true ; break; }
                                                                    else { flag = false; break; }
                    case static_cast<int>(keywords::DEADLINE) :     if (it.first->deadline == *task_after_where) { flag = true; break; }
                                                                    else { flag = false; break; }
                    case static_cast<int>(keywords::TASK) :         if (it.first->task == find_task) { flag = true; break; }
                                                                    else { flag = false; break; }
                    case static_cast<int>(keywords::USER) :         if (check_vec_equality(it.first->user, vec_for_find_usr)) { flag = true; break; }
                                                                    else { flag = false; break; }
                }
                if (!flag) { break; }
                ++update_keywords_after_where;
                ++task_after_where;
            }
            if (flag) {
                while (upd_keywords_before_where != update_keywords_before_where.end()) {
                    switch (static_cast<int>(check_keywords[*upd_keywords_before_where])) {
                        case static_cast<int>(keywords::NAME) : it.first->name = name; break;
                        case static_cast<int>(keywords::USER) : vec_update(it.first->user, vec_for_usr); break;
                        case static_cast<int>(keywords::DEADLINE) : it.first->deadline = *info_before_where; break;
                        case static_cast<int>(keywords::DESCRIPTION) : it.first->description = description; break;
                        case static_cast<int>(keywords::TASK) : it.first->task = task;
                    }
                    ++upd_keywords_before_where;
                    ++info_before_where;
                }
            }
        }
    }
    if (!m_project_data.empty()) {
        for (auto& it : m_project_data) {
            if (it.second->status == -1) {
                continue;
            }
            auto upd_keywords_before_where = update_keywords_before_where.begin();
            auto info_before_where = update_info_before_where.begin();
            auto update_keywords_after_where = keywords_info_after_where.begin();
            auto task_after_where = find_task_after_where.begin();
            while (update_keywords_after_where != keywords_info_after_where.end()) {
                switch (static_cast<int>(check_keywords[*update_keywords_after_where])) {
                    case static_cast<int>(keywords::NAME) :         if (it.second->name == find_name) { flag = true; break;}
                                                                    else { flag = false; break; }
                    case static_cast<int>(keywords::USER) :         if (check_vec_equality(it.second->user, vec_for_find_usr)) { flag = true ; break; }
                                                                    else { flag = false; break; }
                    case static_cast<int>(keywords::DEADLINE) :     if (it.second->deadline == *task_after_where) { flag = true; break; }
                                                                    else { flag = false; break; }
                    case static_cast<int>(keywords::DESCRIPTION) :  if (it.second->deadline == find_description) { flag = true; break; }
                                                                    else { flag = false; break; }
                    case static_cast<int>(keywords::TASK) :         if (it.second->task == find_task) { flag = true; break; }
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
                        case static_cast<int>(keywords::USER) : vec_update(it.second->user, vec_for_usr); break;
                        case static_cast<int>(keywords::DEADLINE) : it.second->deadline = *info_before_where; break;
                        case static_cast<int>(keywords::DESCRIPTION) : it.second->description = description; break;
                        case static_cast<int>(keywords::TASK) : it.second->task = task; break; 
                    }
                    ++upd_keywords_before_where;
                    ++info_before_where;
                }   
            }
        }
    }
}

std::vector<std::string> Project::vec_update(std::vector<std::string>& vec_for_upd, const std::vector<std::string>& vec) {
    vec_for_upd.clear();
    for (auto it : vec) {
        vec_for_upd.push_back(it);
    }
    return vec_for_upd;
}

std::string Project::spliter_for_commands(std::vector<std::string>& vec,const std::string& str) {
    std::string str2 = "";
    auto first_command = std::find(vec.begin(), vec.end(), str);
    if (first_command != vec.end()) {
        auto tmp_it = first_command;
        ++tmp_it;
        auto second_command = std::find_if(++first_command, vec.end(),  [&](std::string command) { return  check_keywords.find(command) != check_keywords.end(); });
        auto tmp_it2 = second_command;

        while (first_command != second_command) {
            str2 += *first_command;
            str2 += ' ';
            ++first_command;
        }
        vec.erase(tmp_it, tmp_it2);
        str2.erase(str2.begin() + str2.size() - 1);
    }
    return str2;
}

void Project::_delete(const std::vector<std::string>& args) {
    auto throw_it = std::find(args.begin(), args.end(), "where");
    if (throw_it == args.end()) {
        throw std::invalid_argument("Please enter delete in task.txt  where ...");
    }
    std::vector<std::string> keywords_info;
    while (++throw_it != args.end()) {
        keywords_info.push_back(*throw_it);
    }
    std::string project_name =   spliter_for_commands(keywords_info, "name");
    std::string description  =   spliter_for_commands(keywords_info, "description");
    std::string task_name    =   spliter_for_commands(keywords_info, "task");
    std::string spliter = " ";
    auto users_vec = m_help->parser(spliter_for_commands(keywords_info, "user"), spliter);
    std::vector<std::string> project_info;
    for (auto it : keywords_info) {
        if (!static_cast<bool>(check_keywords[it])) {
            project_info.push_back(it);
            keywords_info.erase(std::remove (keywords_info.begin(), keywords_info.end(), it), keywords_info.end());
        }
    }

    auto commands = keywords_info.begin();
    auto info = project_info.begin();
    bool flag = false;

    if (!m_fetch_project.empty()) {
        for (auto it : m_fetch_project) {
            if (it.first->status == -1) {
                continue;
            }
            auto commands = keywords_info.begin();
            auto info = project_info.begin();
            while (commands != keywords_info.end()) {
                switch (static_cast<int>(check_keywords[*commands])) {
                    case static_cast<int>(keywords::NAME) :         if (it.first->name == project_name) {flag = true; break; }
                                                                    else { flag = false; break; }
                    case static_cast<int>(keywords::DESCRIPTION) :  if (it.first->description == description) { flag = true; break; }
                                                                    else { flag = false; break; }
                    case static_cast<int>(keywords::TASK) :         if (it.first->task == task_name) { flag = true; break; }
                                                                    else { flag = false; break; }
                    case static_cast<int>(keywords::DEADLINE) :     if (it.first->deadline == *info) { flag = true; break; }
                                                                    else { flag = false; break; }
                    case static_cast<int>(keywords::USER) :         if (check_vec_equality(it.first->user, users_vec)) { flag = true; break; }
                                                                    else { flag = false; break; }
                }
                if (!flag) { break; }
                ++info;
                ++commands;
            }
            if (flag) {
                it.first->status = -1;
                continue;
            }
        }
    }
    if (!m_project_data.empty()) {
        for (auto it : m_project_data) {
            if (it.second->status == -1) {
                continue;
            }
            auto commands = keywords_info.begin();
            auto info = project_info.begin();
            while (commands != keywords_info.end()) {
                switch (static_cast<int>(check_keywords[*commands])) {
                    case static_cast<int>(keywords::NAME) :         if (it.second->name == project_name) { flag = true; break; }
                                                                    else { flag = false; break; }
                    case static_cast<int>(keywords::DESCRIPTION) :  if (it.second->description == description) { flag = true; break; }
                                                                    else { flag = false; break; }
                    case static_cast<int>(keywords::TASK) :         if (it.second->task == task_name) { flag = true; break; }
                                                                    else { flag = false; break; }
                    case static_cast<int>(keywords::DEADLINE) :     if (it.second->deadline == *info) { flag = true; break; }
                                                                    else { flag = false; break; }
                    case static_cast<int>(keywords::USER) :         if (check_vec_equality(it.second->user, users_vec)) { flag = true; break; }
                                                                    else { flag = false; break; }
                }
                if (!flag) { break; }
                ++info;
                ++commands;
            }
            if (flag) {
                it.second->status = -1;
                continue;
            }
        }
    }
}

void Project::flusher() {
    fs.open("/Users/epremsargsyan/Desktop/MY_SQL/MYSQL/DataBase/project.txt");
    if (!fs.is_open()) {
        throw std::runtime_error("File doesn't open");
    }
    static int counter = 1;
    if (!m_fetch_project.empty()) {
        for (auto it : m_fetch_project) {
            fs << "{" << std::endl
            << "   " << "id: " << counter++ << std::endl
            << "   " << "User/s: ";
            for (auto it2 : it.first->user) {
            fs   <<  it2 << " ";
            }
            fs  << std::endl
                << "   " <<        "Name: "        << it.first->name << std::endl
                << "   " <<        "Created: "     << it.first->time
                << "   " <<        "Status: "      << it.first->status << std::endl
                << "   "  <<       "Task: "        << it.first->task << std::endl
                << "   " <<        "Description: " << it.first->description << std::endl
                << "   " <<        "Deadline: "    << it.first->deadline << std::endl
                << "}" << std::endl;
        }
    }
    if (!m_project_data.empty()) {
        for (auto it : m_project_data) {
            fs << "{" << std::endl
            << "   " << "id: " << counter++ << std::endl
            << "   " << "User/s: ";
            for (auto it2 : it.second->user) {
                fs   <<  it2 << " ";
            }
            fs  << std::endl
                <<  "   " <<        "Name: "        << it.second->name << std::endl
                <<  "   " <<        "Created: "     << it.second->time 
                <<  "   " <<        "Status: "      << it.second->status << std::endl
                <<  "   "  <<        "Task: "        << it.second->task << std::endl
                <<  "   " <<        "Description: " << it.second->description << std::endl
                <<  "   " <<        "Deadline: "    << it.second->deadline << std::endl
                << "}" << std::endl;
        }
    }
    fs.close();
}

bool Project::check_vec_equality(const std::vector<std::string>& vec1, const std::vector<std::string>& vec2) {
    int count = 0;
    if (vec1.size() != vec2.size()) {
        return false;
    }

    for (int i = 0; i < vec1.size(); ++i) {
        for (int j = 0; j < vec1.size(); ++j) {
            if (vec1[i] == vec2[j]) {
                ++count;
                break;
            }
        }
    }
    return (count == vec1.size()) ? true : false;
}