#include <iostream>

#include "flags.hpp"

using namespace std;

struct MyOpts
{
    string stringOpt{};
};

int main(int argc, char** argv)
{
    auto flag = flags<MyOpts>("test", "1.0.0", { { "-a", "message a", &MyOpts::stringOpt } });
    auto myopts = flag.parse(argc, argv);
    if (argc == 1)
    {
        std::cout << flag.usage();
        return 0;
    }
    std::cout << "stringOpt = " << myopts.stringOpt << endl;
    //std::cout << "intOpt = " << myopts.intOpt << endl;
}
