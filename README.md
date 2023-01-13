# flags

A very simple command line arguments parser with single header file. 

C++ 17 REQUIRED.



## Quick Start

### Example

```cpp
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

```

### Usage


```batch
:: help
> example.exe --help
flags example v1.0
  --help      show this help message
  --version   show app version
  -ip         ip address, like 192.168.1.14
  -port       port, default is 6014
  -keepalive  keep alive, default is false 


:: version
> example.exe --version
flags example v1.0

:: use =
> example.exe -ip=192.168.1.14
options: .ip=192.168.1.14, .port=6014, .keep_alive=false

:: use space
> example.exe -ip 192.168.1.14
options: .ip=192.168.1.14, .port=6014, .keep_alive=false 

```


## License
The project is available under the MIT license.

