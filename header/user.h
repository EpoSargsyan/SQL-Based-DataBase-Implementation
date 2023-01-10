#pragma once
#include "helper/helper.h"
#include "../comand.h"
#include <iterator>

struct Userinf {
    std::string id = "NILL";
    std::string name = "NILL";
    std::string surname = "NILL";
    std::string email = "NILL";
    int status = 0;
    std::string position = "GUEST";
    std::string phone_number = "NILL";
    std::string age = "NILL";
};

class User : public Comand {
public:
    User();
    ~User() ;
    virtual void select (const std::vector<std::string>&)  override ;
    virtual void _delete (const std::vector<std::string>&) override;
    virtual void input (const std::vector<std::string>&)   override;
    virtual void update (const std::vector<std::string>& ) override ;
    void flusher() override;
    bool check_user(const std::string&);

private:
    Userinf ptr;
    void flush();
    void init();
    void fetch (const std::string&) override;
    bool cmp_objs (const Userinf& , Userinf*);
    std::string check_email (const std::string&);
    bool valid_email (const std::string&);

private:
    enum class keywords { ID = 100, NAME = 1, SURNAME, EMAIL, POSITION, PHONE_NUMBER, AGE, SKOB};  
    std::unordered_map<keywords, std::string> m_check_command;
    std::unordered_map<std::string, keywords> m_keywords;
    std::map<std::string, bool> flags_upd;
    std::map<Userinf,size_t> mp_userinf;
    std::map<size_t,Userinf*> m_userdata;
    std::map<Userinf*, std::streampos> m_read_pos;

private:   
    Userinf* user_ptr;
    std::fstream fs;
    Helper* m_help;
};