#include "header/mysql.h"

int main() {
    std::string str;
    MySQL sql;
    while(true) {
        try {
            std::cout << "Enter command:";
            std::getline(std::cin, str);
            std::cout << std::endl;
            if (str == "exit") {
                return 0;
            }
            sql.execute(str);
        } 
        catch (std::exception& e) {
            std::cout << e.what() << std::endl;
        }
        std::cout << std::endl;
    }
    return 0;
}
