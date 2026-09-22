#include "../../includes/Webserv.hpp"

void Msg::print(const std::string& mode, const std::string& msg, const std::string& color)
{
    if (mode == "[ERROR] ")
        std::cerr << mode << color << msg << RESET << std::endl;
    else 
        std::cout << color << mode << msg << RESET << std::endl;
}

void Msg::info(const std::string& msg) {
    print("[INFO] " , msg, LIGHT_BLUE);
}

void Msg::success(const std::string& msg) {
    print("[OK] " , msg, LIGHT_GREEN);
}

void Msg::warning(const std::string& msg) {
    print("[WARN] " , msg, YELLOW);
}

void Msg::error(const std::string& msg) {
    print("[ERROR] " , msg, RED);
}

void Msg::debug(const std::string& msg) {
    print("[DEBUG] " , msg, DARK_GREY);
}
