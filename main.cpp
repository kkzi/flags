#include <iostream>

#include "flags.h"

using namespace std;

struct MyOpts
{
    string stringOpt{};
    int intOpt{ 100 };
};

int main(int argc, char **argv)
{
    auto flag = flags<MyOpts>({ { "-a", &MyOpts::stringOpt }, { "-b", &MyOpts::intOpt } });
    auto myopts = flag.parse(argc, argv);
    std::cout << "stringOpt = " << myopts.stringOpt << endl;
    std::cout << "intOpt = " << myopts.intOpt << endl;
}
