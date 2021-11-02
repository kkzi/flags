//  (C) Copyright 2021 kizi@qq.com
//  https://github.com/kkzi/flags.git
//  Distributed under the Boost Software License, Version 1.0. (See accompanying
//  file LICENSE or copy at http://www.boost.org/LICENSE_1_0.txt)

#pragma once

#include <functional>
#include <iostream>
#include <map>
#include <memory>
#include <sstream>
#include <string>
#include <string_view>
#include <tuple>
#include <variant>
#include <vector>

template <class T>
class flags : T
{
private:
    using property_t = std::variant<std::string T::*, int T::*, double T::*>;
    using callback_t = std::function<void(const std::string &)>;
    using option_t = std::tuple<std::string, std::string, property_t>;
    using option_parser_t = std::tuple<std::string, callback_t>;

public:
    flags(std::string app, std::string version, const std::initializer_list<option_t> &opts)
        : app_(std::move(app))
        , ver_(std::move(version))
    {
        auto parse = [this](const property_t &p, const std::string &val) {
            std::visit(
                [this, val](auto &&arg) {
                    std::stringstream ss;
                    ss << val;
                    ss >> this->*arg;
                },
                p);
        };

        for (const auto &[name, desc, prop] : opts)
        {
            register_option_parser(name, desc, [parse, prop](auto &&val) {
                parse(prop, val);
            });
        }

        register_option_parser("--help", "show this help message", [this](auto &&) {
            std::cout << usage();
            exit(0);
        });

        if (!app_.empty() && !ver_.empty())
        {
            register_option_parser("--version", "show app version", [this](auto &&) {
                std::cout << app_ << " " << ver_ << std::endl;
                exit(0);
            });
        }
    }

    flags(const std::initializer_list<option_t> &opts)
        : flags("", "", opts)
    {
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

            if (parsers_.count(key) > 0)
            {
                std::get<1>(parsers_.at(key))(val);
            }
            else
            {
                std::cout << "invalid argument " << key << std::endl;
                exit(0);
            }
        }

        return *this;
    }

    std::string usage()
    {
        std::stringstream ss;
        if (!app_.empty())
        {
            ss << app_ << " " << ver_ << std::endl;
        }
        for (auto &&[name, tp] : parsers_)
        {
            ss << "  " << name << ": " << std::get<0>(tp) << std::endl;
        }
        return ss.str();
    }

private:
    void register_option_parser(const std::string &name, const std::string &desc, const callback_t &func)
    {
        if (parsers_.count(name) == 0 && func != nullptr)
        {
            parsers_[name] = option_parser_t{ desc, func };
        }
    }

private:
    flags(const flags &) = delete;
    flags(flags &&) = delete;
    flags &operator=(const flags &) = delete;
    flags &operator=(flags &&) = delete;

private:
    std::string app_;
    std::string ver_;
    std::map<std::string, option_parser_t> parsers_;
};
