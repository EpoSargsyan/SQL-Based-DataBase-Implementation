#include "header/user.h" 
#include "comand.h"

bool operator==(const std::string& rhs, Userinf* user) {
    if (user->name == rhs) return true;
    if (user->surname == rhs) return true;
    if (user->email == rhs) return true;
    if (user->position == rhs) return true;
    if (user->phone_number == rhs) return true;
    if (user->age == rhs) return true;
    return false;
}

std::ostream& operator<< (std::ostream& os, Userinf& obj) {    
    os  << "Name: "    <<       obj.name            << std::endl
        << "Surname: " <<       obj.surname         << std::endl
        << "Email: " <<         obj.email           << std::endl
        << "Status: " <<        obj.status          << std::endl
        << "Position: " <<      obj.position        << std::endl
        << "Phone Number: " <<  obj.phone_number    << std::endl
        << "Age: " <<           obj.age             << std::endl;
    return os;
}

User::User() { init(); }

User::~User() {
    for (auto it : m_userdata) {
        delete it.second;
        it.second = nullptr;
    }
}

void User::init() {
    m_keywords["id"] = keywords::ID;
    m_keywords["{"] = keywords::SKOB;
    m_keywords["name"] = keywords::NAME;
    m_keywords["surname"] = keywords::SURNAME;
    m_keywords["email"] = keywords::EMAIL;
    m_keywords["position"] = keywords::POSITION;
    m_keywords["number"] = keywords::PHONE_NUMBER;
    m_keywords["age"] = keywords::AGE;

    m_check_command[keywords::NAME] = "name";
    m_check_command[keywords::SURNAME] = "surname";
    m_check_command[keywords::EMAIL] = "email";
    m_check_command[keywords::POSITION] = "position";
    m_check_command[keywords::PHONE_NUMBER] = "number";
    m_check_command[keywords::AGE] = "age";

    flags_upd["name"] = false;
    flags_upd["surname"] = false;
    flags_upd["age"] = false;                       
    flags_upd["email"] = false;
    flags_upd["position"] = false;
    flags_upd["phone_number"] = false;
    flags_upd["age"] = false;
}

std::string User::check_email(const std::string& email) {
    if(!valid_email(email)) { throw std::invalid_argument("Invalid email address"); };
    
    if (!m_read_pos.empty()) {
        for (auto it : m_read_pos) {
            if(it.first->status == -1 && it.first->email == email) { return email; }
        
            if (it.first->email == email) {
                throw std::invalid_argument("Email was already used");
            }
        }
    }
    if (!m_userdata.empty()) {
        for (auto it : m_userdata) {
            if(it.second->status == -1 && it.second->email == email) { return email; }

            if (it.second->email == email) {
                throw std::invalid_argument("Email was already used");
            }
        }
    }
    return email;
}

bool User::valid_email(const std::string& rhs) {
    if (rhs.empty()) { return false; }

    std::size_t at_signe_ind = rhs.find('@');
    if (at_signe_ind == std::string::npos) { return false; }

    std::string to_check = rhs.substr(at_signe_ind, rhs.size() - at_signe_ind);
    for (int i = 0; i < at_signe_ind; ++i) {
        if (!((rhs[i] >= 'a' && rhs[i] <= 'z') || (rhs[i] >= 'A' && rhs[i] <= 'Z') || (rhs[i] == 46) || (rhs[i] == 95) || (rhs[i] >= '0' && rhs[i] <= '9'))) {
            return false;
        }
    }

    if ((to_check == "@gmail.com") || (to_check == "@mail.ru" || to_check == "@inbox.ru")) {
        return true;
    }
    return false;
}
     
void User::flusher() {
    flush();
}

void User::input(const std::vector<std::string>& vec) {   
    std::vector<std::string> input_vec;
    auto begin = vec.begin();

    while (*begin != "user.txt") {
        ++begin;
    }
    while (++begin != vec.end()) {
        input_vec.push_back(*begin);    
    }
    
    m_help->check_errors(input_vec);
    auto input_vec_begin = input_vec.begin();
    auto target = input_vec.begin();
    Userinf* ptr = new Userinf();

    for (auto it : input_vec) {
        target = input_vec_begin; 
        switch (static_cast<int>(m_keywords[*input_vec_begin])) {
            case static_cast<int>(keywords::NAME):          ptr->name = *++target; break;
            case static_cast<int>(keywords::SURNAME):       ptr->surname = *++target; break;
            case static_cast<int>(keywords::EMAIL):         ptr->email = check_email(*++target); break; 
            case static_cast<int>(keywords::POSITION):      ptr->position = *++target; break;
            case static_cast<int>(keywords::PHONE_NUMBER):  ptr->phone_number = *++target; break;
            case static_cast<int>(keywords::AGE):           ptr->age = *++target; break;
        }
        ++input_vec_begin;
    }

    static int  id = 1;
    m_userdata.insert(std::make_pair(id, *&ptr));
    ++id;
    flusher();
}

void User::flush() {
    static int counter = 1;
    fs.open("/Users/epremsargsyan/Desktop/MY_SQL/MYSQL/DataBase/user.txt");
    if (!fs.is_open()) {
        std::runtime_error("Failed to open DataBase/user.txt file");
    }
    if (!m_read_pos.empty()) {
        for (auto it : m_read_pos) {
            fs << "{" << std::endl  
            << "    " <<         "id:" << counter++ << std::endl  
            << "    " <<         "Name: " << it.first->name << std::endl 
            << "    " <<         "Surname: " << it.first->surname << std::endl
            << "    " <<         "Email: " << it.first->email << std::endl
            << "    " <<         "Status: " << it.first->status << std::endl
            << "    " <<         "Position: " << it.first->position << std::endl
            << "    " <<         "Phone Number: " << it.first->phone_number << std::endl
            << "    " <<         "Age: " << it.first->age << std::endl
            << "}" << std::endl;       
        }
    }
    if (!m_userdata.empty()) {
        for (auto it : m_userdata) {
            fs << "{" << std::endl 
            << "    " <<         "id:" << counter++ << std::endl  
            << "    " <<         "Name: " << it.second->name << std::endl 
            << "    " <<         "Surname: " << it.second->surname << std::endl
            << "    " <<         "Email: " << it.second->email << std::endl
            << "    " <<         "Status: " << it.second->status << std::endl
            << "    " <<         "Position: " << it.second->position << std::endl
            << "    " <<         "Phone Number: " << it.second->phone_number << std::endl
            << "    " <<         "Age: " << it.second->age << std::endl
            << "}" << std::endl;       
        }
    }
    fs.close();
}

void User::_delete(const std::vector<std::string>& vec) {
    auto throw_it = std::find(vec.begin(), vec.end(), "where");
    if (throw_it == vec.end()) {
        throw std::invalid_argument("Please enter delete user.txt  where...");
    }
    std::vector<std::string> half_vec;
    auto tmp_it = vec.begin();

    while (*tmp_it != "where") {
        ++tmp_it;
    }
    while (++tmp_it != vec.end()) {
        half_vec.push_back(*tmp_it);
    }

    std::vector<std::string> second_half_vec;
    for (auto it : half_vec) {   
        if (!static_cast<bool>(m_keywords[it])) {
            second_half_vec.push_back(it);
            half_vec.erase(std::remove(half_vec.begin(), half_vec.end(), it), half_vec.end());   
        }
    }    
    
    auto status = half_vec.begin();
    auto command_begin = half_vec.begin();
    auto begin  = second_half_vec.begin();
    bool flag = false;

    if (!m_read_pos.empty())  {
        for (auto it : m_read_pos) {
            status = half_vec.begin();
            command_begin = half_vec.begin();
            begin = second_half_vec.begin();
                while (begin != second_half_vec.end()) {
                    switch (static_cast<int>(m_keywords[*status])) {
                        case static_cast<int>(keywords::NAME):          if (it.first->name == *begin) { flag = true; break; }
                                                                        else { flag = false; break; }

                        case static_cast<int>(keywords::SURNAME):       if (it.first->surname == *begin) { flag = true; break; }
                                                                        else { flag = false; break; }
                                                                    
                        case static_cast<int>(keywords::EMAIL):         if (it.first->email == *begin) { flag = true; break; }
                                                                        else { flag = false; break; }
                                                                
                        case static_cast<int>(keywords::POSITION):      if (it.first->position == *begin) { flag = true; break; }
                                                                        else { flag = false; break; }
                                                                                                                                    
                        case static_cast<int>(keywords::PHONE_NUMBER):  if (it.first->phone_number == *begin) { flag = true; break; }
                                                                        else { flag = false; break; }
                                                                
                        case static_cast<int>(keywords::AGE):           if (it.first->age == *begin) { flag = true; break; }
                                                                        else { flag = false; break; }
                    }
                    if (!flag) { break; }
                    ++begin;
                    ++status;
                }
     
            if (flag) {      
                if (half_vec.empty()) {
                    it.first->status = -1;
                    break;
            }
                while (command_begin != half_vec.end()) {
                    switch (static_cast<int>(m_keywords[*command_begin])) {
                        case static_cast<int>(keywords::NAME):          it.first->status = -1; break;
                        case static_cast<int>(keywords::SURNAME):       it.first->status = -1; break;
                        case static_cast<int>(keywords::EMAIL):         it.first->status = -1; break;
                        case static_cast<int>(keywords::POSITION):      it.first->status = -1; break;
                        case static_cast<int>(keywords::PHONE_NUMBER):  it.first->status = -1; break;
                        case static_cast<int>(keywords::AGE):           it.first->status = -1; break;
                    }
                    ++command_begin;  
                }
            }
        }
    }

    if (!m_userdata.empty()) {
        for (auto it : m_userdata) {
            status = half_vec.begin();
            command_begin = half_vec.begin();
            begin = second_half_vec.begin();

            while (begin != second_half_vec.end()) {
                switch (static_cast<int>(m_keywords[*status])) {
                    case static_cast<int>(keywords::NAME):          if (it.second->name == *begin) { flag = true; break; }
                                                                    else { flag = false; break; }

                    case static_cast<int>(keywords::SURNAME):       if (it.second->surname == *begin) {  flag = true;break; }
                                                                    else { flag = false; break; }
                                                                    
                    case static_cast<int>(keywords::EMAIL):         if (it.second->email == *begin) { flag = true; break; }
                                                                    else { flag = false; break; }
                                                                    
                    case static_cast<int>(keywords::POSITION):      if (it.second->position == *begin) { flag = true;break; }
                                                                    else { flag = false; break; }
                                                                    
                    case static_cast<int>(keywords::PHONE_NUMBER):  if (it.second->phone_number == *begin) { flag = true;break; }
                                                                    else { flag = false; break;}

                    case static_cast<int>(keywords::AGE):           if (it.second->age == *begin) { flag = true;break; }
                                                                    else { flag = false; break; }
                }
                if (!flag) { break; }
                ++begin;
                ++status;
            }
     
            if (flag) {
                if (half_vec.empty()) {
                    it.second->status = -1;
                    break;
                }
                while (command_begin != half_vec.end()) {
                    switch (static_cast<int>(m_keywords[*command_begin])) {
                        case static_cast<int>(keywords::NAME):          it.second->status = -1; break;
                        case static_cast<int>(keywords::SURNAME):       it.second->status = -1; break;
                        case static_cast<int>(keywords::EMAIL):         it.second->status = -1; break;
                        case static_cast<int>(keywords::POSITION):      it.second->status = -1; break;
                        case static_cast<int>(keywords::PHONE_NUMBER):  it.second->status = -1; break;
                        case static_cast<int>(keywords::AGE):           it.second->status = -1; break;
                    }
                    ++command_begin;  
                }
            }
        }
    }
}

void User::fetch(const std::string&) {
    fs.open("/Users/epremsargsyan/Desktop/MY_SQL/MYSQL/DataBase/user.txt");
    if (!fs.is_open()) {
        std::runtime_error("Failed to open DataBase/user.txt file");
    }

    std::string str;
    std::string spliter = " :}";
    std::string line;
    std::streampos m_value;
    static int user_id_tmp = 1;
    std::vector<std::string> m_vec;
    //std::cout << "a" << std::endl;
    while (!fs.eof()) {
        getline(fs, str);
        auto m_vec = m_help->parser(str, spliter);
        m_value = fs.tellg();
        Userinf* tmp_obj = new Userinf();
        if (static_cast<bool>(m_keywords[m_vec[0]])) {
            m_read_pos[tmp_obj] = m_value;
            
            m_value = fs.tellg();
            std::getline(fs.seekg(m_value), line);
            m_help->line_cutter(line);
            tmp_obj->id = line;
            
            m_value = fs.tellg();
            std::getline(fs.seekg(m_value), line);
            m_help->line_cutter(line);
            tmp_obj->name = line;

            m_value = fs.tellg();
            std::getline(fs.seekg(m_value), line);
            m_help->line_cutter(line);
            tmp_obj->surname = line;

            m_value = fs.tellg();
            std::getline(fs.seekg(m_value), line);
            m_help->line_cutter(line);
            tmp_obj->email = line;

            m_value = fs.tellg();
            std::getline(fs.seekg(m_value), line);
            m_help->line_cutter(line);
            tmp_obj->status = std::stoi(line);

            m_value = fs.tellg();
            std::getline(fs.seekg(m_value), line);
            m_help->line_cutter(line);
            tmp_obj->position = line;

            m_value = fs.tellg();
            std::getline(fs.seekg(m_value), line);
            m_help->line_cutter(line);
            tmp_obj->phone_number = line;

            m_value = fs.tellg();
            std::getline(fs.seekg(m_value), line);
            m_help->line_cutter(line);
            tmp_obj->age = line;
        }
    }
    fs.close();
}

void User::select(const std::vector<std::string>& vec) {
    auto throw_it = std::find(vec.begin(), vec.end(), "where");
    if (throw_it == vec.end()) {
        throw std::invalid_argument("Please enter select in user.txt  ... where ...");
    }
    auto it = vec.begin();
    std::vector<std::string> half_vec;

    while (*it != "where") {
        if (m_keywords.find(*it) != m_keywords.end()) {
            half_vec.push_back(*it);
        }
        ++it;
    }
    
    std::vector<std::string> second_half_vec;
    while (++it != vec.end()) {
        second_half_vec.push_back(*it);
    }

    std::vector<std::string> third_half_vec;
    for (auto it : second_half_vec) {   
        if (!static_cast<bool>(m_keywords[it])) {
            third_half_vec.push_back(it);
            second_half_vec.erase(std::remove(second_half_vec.begin(), second_half_vec.end(), it), second_half_vec.end());  
        }
    }    
    
    auto status = second_half_vec.begin();
    auto begin = third_half_vec.begin();
    auto command_begin = half_vec.begin();
    bool flag = false;

    if (!m_read_pos.empty()) {
        for (auto it : m_read_pos) {
            if (it.first->status == -1) {
                continue;
            }

            begin = third_half_vec.begin();
            status = second_half_vec.begin();
            command_begin = half_vec.begin();

            while (begin != third_half_vec.end()) {
                switch (static_cast<int>(m_keywords[*status])) {
                    case static_cast<int>(keywords::NAME):          if (it.first->name == *begin) { flag = true; break; }
                                                                    else { flag = false; break; }
                                                                        
                    case static_cast<int>(keywords::SURNAME):       if (it.first->surname == *begin) { flag = true; break; }
                                                                    else { flag = false; break; }
                                                                        
                    case static_cast<int>(keywords::EMAIL):         if (it.first->email == *begin) { flag = true; break; }
                                                                    else { flag = false; break; }
                                                                        
                    case static_cast<int>(keywords::POSITION):      if (it.first->position == *begin) { flag = true; break; }
                                                                    else { flag = false; break; }  

                    case static_cast<int>(keywords::PHONE_NUMBER):  if (it.first->phone_number == *begin) { flag = true; break; }
                                                                    else { flag = false; break; }
                                                                
                    case static_cast<int>(keywords::AGE):           if (it.first->age == *begin) { flag = true; break; }
                                                                    else { flag = false; break; }
                                                                        
                }
                if (!flag) { break; }
                ++status;
                ++begin;
            }   
     
            if (flag) {
                if (half_vec.empty()) {
                std::cout << *it.first << std::endl;
                continue;
                }

                while (command_begin != half_vec.end()) {
                    switch (static_cast<int>(m_keywords[*command_begin])) {
                        case static_cast<int>(keywords::NAME):              std::cout << "Name:" <<  it.first->name << std::endl; break;
                        case static_cast<int>(keywords::SURNAME):           std::cout << "Surname: " <<  it.first->surname << std::endl; break;
                        case static_cast<int>(keywords::EMAIL):             std::cout << "Email: " << it.first->email << std::endl; break;
                        case static_cast<int>(keywords::POSITION):          std::cout << "Position: " << it.first->position << std::endl; break;
                        case static_cast<int>(keywords::PHONE_NUMBER):      std::cout << "Phone number: " <<  it.first->phone_number << std::endl; break;
                        case static_cast<int>(keywords::AGE):               std::cout << "Age: " << it.first->age << std::endl; break;
                    }
                    ++command_begin;
                }
            }      
        }         
    }

    if (!m_userdata.empty()) {
        for (auto it : m_userdata) {
            if (it.second->status == -1) {
                continue;
            }

            begin = third_half_vec.begin();
            status = second_half_vec.begin();
            command_begin = half_vec.begin();

            while (begin != third_half_vec.end()) {
                switch (static_cast<int>(m_keywords[*status])) {
                    case static_cast<int>(keywords::NAME):          if (it.second->name == *begin) {flag = true; break; }
                                                                    else { flag = false; break; }

                    case static_cast<int>(keywords::SURNAME):       if (it.second->surname == *begin) {  flag = true; break; }
                                                                    else { flag = false; break; }
                                                            
                    case static_cast<int>(keywords::EMAIL):         if (it.second->email == *begin) { flag = true; break; }
                                                                    else { flag = false; break; }
                                                                  
                    case static_cast<int>(keywords::POSITION):      if (it.second->position == *begin) { flag = true; break; }
                                                                    else { flag = false; break; }
                                                                  
                    case static_cast<int>(keywords::PHONE_NUMBER):  if (it.second->phone_number == *begin) {  flag = true; break; }
                                                                    else { flag = false; break; }
                                                            
                    case static_cast<int>(keywords::AGE):           if (it.second->age == *begin) {flag = true; break; }
                                                                    else { flag = false; break; }
                }
                if (!flag) { break;}
                ++status;
                ++begin;
            }
     
            if (flag) {
                if (half_vec.empty()) {
                    std::cout << *it.second << std::endl;
                    continue;
                }

                while (command_begin != half_vec.end()) {    
                    switch (static_cast<int>(m_keywords[*command_begin])) {
                        case static_cast<int>(keywords::NAME):              std::cout << "Name:" <<  it.second->name << std::endl; break;
                        case static_cast<int>(keywords::SURNAME):           std::cout << "Surname: " <<  it.second->surname << std::endl; break;
                        case static_cast<int>(keywords::EMAIL):             std::cout << "Email: " << it.second->email << std::endl; break;
                        case static_cast<int>(keywords::POSITION):          std::cout << "Position: " << it.second->position << std::endl; break;
                        case static_cast<int>(keywords::PHONE_NUMBER):      std::cout << "Phone number: " <<  it.second->phone_number << std::endl; break;
                        case static_cast<int>(keywords::AGE):               std::cout << "Age: " << it.second->age << std::endl; break;
                    }
                    ++command_begin;
                }
            }
        }        
    }
}

void User::update(const std::vector<std::string>& vec) {
    auto throw_it = std::find(vec.begin(), vec.end(), "set");
    if (throw_it == vec.end()) {
        throw std::invalid_argument("Please enter user.txt  set ... where...");
    }
    auto throw_it2 = std::find(vec.begin(), vec.end(), "where");
    if (throw_it2 == vec.end()) {
        throw std::invalid_argument("Please enter  user.txt set ... where...");
    }
    for (auto x : m_userdata) { if (x.second->status == -1) { return; } }
    
    std::vector<std::string> update_vec_key;
    std::vector<std::string> update_vec_val;

    for (auto i = ++std::find(vec.begin(), vec.end(), "set"); i != vec.end(); ++i) {
        if (*i == "where") {
            break;
        }
        update_vec_key.push_back(*i);
    }

    for (auto i = ++std::find(vec.begin(), vec.end(), "where"); i != vec.end(); ++i) {
        update_vec_val.push_back(*i);
    }

    for (auto y = update_vec_val.begin(); y !=update_vec_val.end(); y++) {
        switch (static_cast<int>(m_keywords[*y])) {
            case static_cast<int>(keywords::NAME) :            flags_upd[*y] = true; break;
            case static_cast<int>(keywords::SURNAME) :         flags_upd[*y] = true; break;
            case static_cast<int>(keywords::AGE) :             flags_upd[*y] = true; break;
            case static_cast<int>(keywords::POSITION) :        flags_upd[*y] = true; break;
            case static_cast<int>(keywords::EMAIL) :           flags_upd[*y] = true; break;
            case static_cast<int>(keywords::PHONE_NUMBER) :    flags_upd[*y] = true; break;
        }
    }

    Userinf* tmp_obj = new Userinf();
    int count = 0;
   
    for(auto x : update_vec_val) {
        auto begin2 = update_vec_val.begin();
        auto tmp2 = std::find_if(begin2, update_vec_val.end(), [&](std::string command) { return  m_keywords.find(command) != m_keywords.end(); });
            
        switch (static_cast<int>(m_keywords[*tmp2])) {
            case static_cast<int>(keywords::NAME) :         if (flags_upd[m_check_command[keywords::NAME]])  tmp_obj->name = *++tmp2; break;
            case static_cast<int>(keywords::SURNAME) :      if( flags_upd[m_check_command[keywords::SURNAME]]) tmp_obj->surname = *++tmp2; break;
            case static_cast<int>(keywords::AGE) :          if (flags_upd[m_check_command[keywords::AGE]]) tmp_obj->age = *++tmp2; break;
            case static_cast<int>(keywords::POSITION) :     if (flags_upd[m_check_command[keywords::POSITION]]) tmp_obj->position = *++tmp2; break;
            case static_cast<int>(keywords::EMAIL) :        if (flags_upd[m_check_command[keywords::EMAIL]]) tmp_obj->email = *++tmp2; break;
            case static_cast<int>(keywords::PHONE_NUMBER) : if (flags_upd[m_check_command[keywords::PHONE_NUMBER]]) tmp_obj->phone_number = *++tmp2; break;
        }
        begin2 = ++tmp2;
    }
        
    auto begin_keys = update_vec_key.begin();
    auto tmp2 = std::find_if(begin_keys, update_vec_key.end(), [&](std::string command) { return  m_keywords.find(command) != m_keywords.end(); });
    
    if (m_userdata.empty()) {
        for (auto x : m_read_pos) {
            if (x.first->status == -1) { continue; }

            if (cmp_objs(*tmp_obj, x.first)) {
                while(begin_keys != update_vec_key.end()) {
                    tmp2 = std::find_if(begin_keys, update_vec_key.end(), [&](std::string command) { return  m_keywords.find(command) != m_keywords.end(); });
                    switch (static_cast<int>(m_keywords[*tmp2])) {
                        case static_cast<int>(keywords::NAME) :         x.first->name = *++tmp2; break;
                        case static_cast<int>(keywords::SURNAME) :      x.first->surname = *++tmp2; break;
                        case static_cast<int>(keywords::AGE) :          x.first->age = *++tmp2; break;
                        case static_cast<int>(keywords::PHONE_NUMBER) : x.first->phone_number = *++tmp2; break;
                        case static_cast<int>(keywords::POSITION) :     x.first->position = *++tmp2; break;
                        case static_cast<int>(keywords::EMAIL) :        x.first->email =check_email (*++tmp2); break;
                    }
                    begin_keys = ++tmp2;
                }
            }
            begin_keys = update_vec_key.begin();
        }
    } else {
        for (auto x : m_userdata) {
            if (x.second->status == -1) { continue; }

            if (cmp_objs(*tmp_obj, x.second)) {
                while(begin_keys != update_vec_key.end()) {
                    tmp2 = std::find_if(begin_keys, update_vec_key.end(), [&](std::string command) { return  m_keywords.find(command) != m_keywords.end(); });
        
                    switch (static_cast<int>(m_keywords[*tmp2])) {
                        case static_cast<int>(keywords::NAME) :         x.second->name = *++tmp2; break;
                        case static_cast<int>(keywords::SURNAME) :      x.second->surname = *++tmp2; break;
                        case static_cast<int>(keywords::AGE) :          x.second->age = *++tmp2; break;
                        case static_cast<int>(keywords::PHONE_NUMBER) : x.second->phone_number = *++tmp2; break;
                        case static_cast<int>(keywords::POSITION) :     x.second->position = *++tmp2; break;
                        case static_cast<int>(keywords::EMAIL) :        x.second->email = check_email(*++tmp2); break;
                    }
                    begin_keys = ++tmp2;
                }
            }
            begin_keys = update_vec_key.begin();
        }
    }
    delete tmp_obj;
}

bool User::cmp_objs(const Userinf& lhs, Userinf* rhs) {
    if (flags_upd["name"]) {
        if (lhs.name != rhs->name) { return false; }
    }
    if (flags_upd["surname"]) {
        if (lhs.surname != rhs->surname) { return false; }
    }
    if (flags_upd["position"]) {
        if (lhs.position != rhs->position) { return false; }
    }
    if (flags_upd["email"]) {
        if (lhs.email != rhs->email) { return false; }
    }
    if (flags_upd["age"]) {
        if (lhs.age != rhs->age) { return false; }
    }
    return true;
}

bool User::check_user(const std::string& email) {
    bool flag = false;
    std::string mail = email;
    for (auto it : m_read_pos) {
        if (it.first->email == mail) {
            flag = true;
            break;
        }
    }
    return flag;
}