#pragma once
#include "../comand.h"
#include "header/mysql.h"

struct Projectinfo {
    std::string id = "NIL";
    std::string name = "NIL";
    std::string description = "NIL";
    std::string deadline = "NIL";
    std::string time = "NIL";
    int status = 0;
    std::vector<std::string> user;
    std::string task = "NIL";
};

class Project : public Comand {
public:
    Project(User*, Task*);
    virtual void select(const std::vector<std::string>&) override;
    virtual void _delete(const std::vector<std::string>&) override;
    virtual void input(const std::vector<std::string>&) override;
    virtual void update(const std::vector<std::string>&) override;
    void fetch(const std::string&) override;
    void flusher() override;
    ~Project() {};

private:
    void init();
    auto check_project_errors(const std::vector<std::string>&);
    bool check_vec_equality(const std::vector<std::string>&, const std::vector<std::string>&);
    std::string spliter_for_commands(std::vector<std::string>&, const std::string&);
    std::string spliter_for_find(std::vector<std::string>&, const std::string&);
    std::vector<std::string> vec_update(std::vector<std::string>&, const std::vector<std::string>&);

private:
    enum class keywords { ID = 1, NAME, DESCRIPTION, DEADLINE, USER, TASK, SKOB };
    std::map<Projectinfo*, std::streampos> m_fetch_project;
    std::map<std::string, keywords> check_keywords;
    std::map<size_t, Projectinfo*> m_project_data;

private:
    Projectinfo projinf;
    std::fstream fs;
    User* m_user;
    Task* m_task;
    Helper* m_help;
};