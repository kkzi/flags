#pragma once

#include <functional>
#include <iostream>
#include <map>
#include <memory>
#include <sstream>
#include <string>
#include <string_view>
#include <variant>
#include <vector>

template <class T>
class flags : T
{
private:
    // using property = std::variant<std::string T::*, int T::*, double T::*, bool T::*>;
    using property = std::variant<std::string T::*, int T::*, double T::*>;
    using option = std::pair<std::string, property>;

public:
    flags(const std::initializer_list<option> &opts)
    {
        for (const auto &o : opts)
        {
            callbacks_[o.first] = [this, p = o.second](const std::string &val) {
                std::visit(
                    [this, val](auto &&arg) {
                        std::stringstream ss;
                        ss << val;
                        ss >> this->*arg;
                    },
                    p);
            };
        }
    }

    ~flags() = default;

public:
    T parse(int argc, char **argv)
    {
        std::vector<std::string_view> vargv(argv, argv + argc);
        for (auto idx = 1; idx < argc; ++idx)
        {
            std::string key, val;
            std::string line = argv[idx];
            auto equal_pos = line.find('=');
            if (equal_pos == std::string::npos)
            {
                key = line;
                val = argc > (++idx) ? argv[idx] : "";
            }
            else
            {
                key = line.substr(0, equal_pos);
                val = line.substr(equal_pos + 1);
            }

            if (callbacks_.count(key) > 0)
            {
                callbacks_.at(key)(val);
            }
        }

        return static_cast<T>(*this);
    }

private:
    flags(const flags &) = delete;
    flags(flags &&) = delete;
    flags &operator=(const flags &) = delete;
    flags &operator=(flags &&) = delete;

private:
    std::map<std::string, std::function<void(const std::string &)>> callbacks_;
};
