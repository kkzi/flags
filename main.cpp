#include <iostream>

#include "flags.hpp"

using namespace std;

struct MyOpts
{
    string stringOpt{};
    int intOpt{ 100 };
};

int main(int argc, char** argv)
{
    auto flag = flags<MyOpts>("test", "1.0.0", { { "-a", &MyOpts::stringOpt, "message a" }, { "-b", &MyOpts::intOpt, "message b" } });
    auto myopts = flag.parse(argc, argv);
    if (argc == 1)
    {
        std::cout << flag.usage();
        return 0;
    }
    std::cout << "stringOpt = " << myopts.stringOpt << endl;
    std::cout << "intOpt = " << myopts.intOpt << endl;
}
