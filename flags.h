#pragma once

#include <functional>
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
    using property_t = std::variant<std::string T::*, int T::*, double T::*>;
    using callback_t = std::function<void(const std::string&)>;
    struct option_t
    {
        std::string name_;
        property_t prop_;
        std::string desc_;
        callback_t callback_;
    };

public:
    flags(std::string app, std::string version, const std::initializer_list<option_t>& opts)
        : app_(std::move(app))
        , ver_(std::move(version))
    {
        for (const auto& o : opts)
        {
            opts_[o.name_] = option_t{
                o.name_,
                o.prop_,
                o.desc_,
                [this, p = o.prop_](const std::string& val) {
                    std::visit([this, val](auto&& arg) {
                        std::stringstream ss;
                        ss << val;
                        ss >> this->*arg;
                    },
                    p);
                }
            };
        }

        if (opts_.count("-help") == 0)
        {
            option_t help_opt;
            help_opt.name_ = "-help";
            help_opt.desc_ = "show this help message";
            help_opt.callback_ = [this](auto&&) { std::cout << usage() << std::endl; };
            opts_["-help"] = help_opt;
        }

        if (!app_.empty() && !ver_.empty() && opts_.count("-version") == 0)
        {
            option_t ver_opt;
            ver_opt.name_ = "-version";
            ver_opt.desc_ = "show app version";
            ver_opt.callback_ = [this](auto&&) { std::cout << app_ << " " << ver_ << std::endl; };
            opts_["-version"] = ver_opt;
        }
    }

    flags(const std::initializer_list<option_t>& opts)
        : flags("", "", opts)
    {
    }

    ~flags() = default;

public:
    T parse(int argc, char** argv)
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

            if (opts_.count(key) > 0)
            {
                opts_.at(key).callback_(val);
            }
        }

        return static_cast<T>(*this);
    }

    std::string usage()
    {
        std::stringstream ss;
        if (!app_.empty())
        {
            ss << app_ << " " << ver_ << std::endl;
        }
        for (auto&& [name, opt] : opts_)
        {
            ss << "  " << name << ": " << opt.desc_ << std::endl;
        }
        ss << std::endl;
        return ss.str();
    }

private:
    flags(const flags&) = delete;
    flags(flags&&) = delete;
    flags& operator=(const flags&) = delete;
    flags& operator=(flags&&) = delete;

private:
    std::string app_;
    std::string ver_;
    std::map<std::string, option_t> opts_;
};
