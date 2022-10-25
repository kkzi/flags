#include <iostream>
#include "flags.hpp"

struct example_option
{
    std::string ip{};
    uint16_t port{ 6014 };
    bool keep_alive{ false };
};

int main(int argc, char** argv)
{
    auto opts = flags<example_option>("flags example v1.0", {
        { "-ip", "ip address, like 192.168.1.14", &example_option::ip },
        { "-port", "port, default is 6014", &example_option::port },
        { "-keepalive", "keep alive, default is false", &example_option::keep_alive },
    })(argc, argv);

    std::cout << "options: " << ".ip=" << opts.ip << ", " 
        << ".port=" << opts.port 
        << ", " << ".keep_alive=" << std::boolalpha << opts.keep_alive 
        << std::endl;
}
