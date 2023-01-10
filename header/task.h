#pragma once
#include "../comand.h"

struct Taskinfo {
    std::string ID = "NILL";
    std::string name = "NILL";
    std::string user_mail = "NILL";
    std::string project_name ="NIL";
    int status = 0;
    std::string time  = "NILL";
    std::string deadline = "NILL/NILL/NILL";
};

class Task : public Comand {
public: 
    Task() ;
    ~Task();
    virtual void select(const std::vector<std::string>&) override;
    virtual void _delete(const std::vector<std::string>&) override;
    virtual void input(const std::vector<std::string>&) override;
    virtual void update(const std::vector<std::string>&) override;
    void fetch(const std::string&) override;
    void flusher() override;
    bool check_task(const std::string&);
    bool check_task(const std::string&, const std::string&);

private:
    bool check_email(const std::string&);
    void init();
    bool cmp_objs(const Taskinfo&, Taskinfo*);

private:
    enum class keywords { ID = 1, NAME , CREATED, USER, DEADLINE, SKOB, PROJECT };
    enum class keywords_to_upd { WHERE = 1, SET };
    std::map<std::string, keywords_to_upd> oriented_keys;
    std::map <keywords, std::string> m_check_command;
    std::map<std::string, bool> flags_upd;
    std::map<size_t, std::string> m_user_data;
    std::map<std::string, keywords> check_keywords;
    std::map<size_t, Taskinfo*> m_task_info;
    std::map<Taskinfo*, std::streampos> m_fetch_info;

private:
    Taskinfo* Taskinf;
    std::fstream fs;
    Helper* m_help;
};