//
// C++ implementation of GNU getopt with a twist
// Created by Nolan Woods on 10/28/2018.
//

#ifndef BINDOPT_H
#define BINDOPT_H

#include <string>
#include <vector>
#include <list>
#include <set>
#include <iterator>
#include <iostream>
#include <ostream>
#include <functional>
#include <utility>
#include <memory>

namespace bindopt {

    //String conversion operators
    //Generic for basic assignment
    template <typename T>
    static T operator<<(T &val, const T &val2) {
        return val = val2;
    }

    static bool operator<<(bool& val, const std::string& str) {
        return val = !(str.empty() or str[0] == 'f' or str[0] == 'F' or str[0] == '0');
    }

    static int operator<<(int &val, const std::string &str) {
        return val = std::stoi(str);
    }

    static unsigned int operator<<(unsigned int &val, const std::string &str) {
        return val = std::stoul(str);
    }

    static long operator<<(long &val, const std::string &str) {
        return val = std::stol(str);
    }

    static unsigned long operator<<(unsigned long &val, const std::string &str) {
        return val = std::stoul(str);
    }

    static long long operator<<(long long &val, const std::string &str) {
        return val = std::stoll(str);
    }

    static unsigned long long operator<<(unsigned long long &val, const std::string &str) {
        return val = std::stoull(str);
    }

    static float operator<<(float &val, const std::string &str) {
        return val = std::stof(str);
    }

    static double operator<<(double &val, const std::string &str) {
        return val = std::stod(str);
    }

    static long double operator<<(long double &val, const std::string &str) {
        return val = std::stold(str);
    }

    struct Option {
        const std::string name;
        const char short_name;
        const bool required;
        const std::string desc;
        const std::string err;

        //Virtual class does not support aggregate initialization, use variadic constructor with std::forward<>()
        Option(std::string name,
        const char short_name,
        const bool required,
        std::string desc,
        std::string err) : name{std::move(name)}, short_name{short_name}, required{required}, desc{std::move(desc)}, err{std::move(err)} {};

        virtual unsigned short parse(const std::string &str1, const std::string &str2) = 0;
        virtual bool is_flag() {return false;}
        virtual bool is_positional() {return false;}

        bool operator==(const std::string& str) {
            return !str.empty() and ( //Ignore empty inputs
                    (str[0] == '-' and str[1] == short_name) //short name match
                    or (!str.compare(0, 2, "--") and !str.compare(2, name.find("=", 2), name)) //long name match
                    or ((str.size() == 1 xor str[0] != '-') and is_positional())); // positional option match
        }

    protected:
        std::string extract(const std::string &str1, const std::string &str2, unsigned short& consumed) {
            unsigned long start{0};
            if (is_positional()) {
                consumed = 1;
                return str1;
            } else if (str1[0] == '-' and str1[1] == short_name) {
                consumed = 1;
                return str1.substr(2);
            } else if (!str1.compare(0, 2, "--") and (start = str1.find('=', 2)) != str1.npos) {
                consumed = 1;
                return str1.substr(start+1);
            } else {
                consumed = 2;
                return str2;
            }
        }
    };

    template <typename T>
    struct BoundOption : Option {
        T& value;
        static void default_validator(T& val) {};
        std::function<void(T&)> validator;

        template <typename... Args>
        BoundOption(T& value, Args&&... args) : Option{std::forward<Args>(args)...}, value{value}, validator{default_validator} {};
        template <typename... Args>
        BoundOption(T& value, const std::function<void(T&)> validator, Args&&... args) : Option{std::forward<Args>(args)...}, value{value}, validator{validator} {};

        unsigned short parse(const std::string &str1, const std::string &str2) override {
            unsigned short consumed;
            value << extract(str1, str2, consumed);
            validator(value);
            return consumed;
        }
    };

    struct FlagOption : BoundOption<bool> {
        using BoundOption<bool>::BoundOption;
        unsigned short parse(const std::string &str1, const std::string &str2) override {
            value = true;
            return 1;
        }

        bool is_flag() override { return true; }
    };

    template <typename T>
    struct PositionalOption : BoundOption<T> {
        using BoundOption<T>::BoundOption;
        using BoundOption<T>::parse;

        bool is_positional() override { return true; }
    };

    template <typename T>
    struct RepeatingOption : BoundOption<std::vector<T>> {
        using BoundOption<std::vector<T>>::BoundOption;
        using BoundOption<std::vector<T>>::value;
        using Option::extract;
        unsigned short parse(const std::string &str1, const std::string &str2) override {
            T val;
            unsigned short consumed;
            validator(val << extract(str1, str2, consumed));
            value.push_back(std::move(val));
            return consumed;
        }
    };

    using Options = std::vector<std::unique_ptr<Option>>;

    static std::list<std::string> bindopt(int argc, char **argv, Options& options) {
        std::list<std::string> args;
        //Convert argv to std::strings
        for (int i = 0; i < argc; ++i) {
            args.emplace_back(argv[i]);
        }

        //Split compound short args and count positionals
        unsigned int required_positionals{0};
        std::set<char> flag_opts;
        std::set<char> val_opts;
        for (auto& option : options) {
            if (option->is_positional() and option->required) ++required_positionals;
            if (option->short_name) {
                if (option->is_flag()) {
                    flag_opts.insert(option->short_name);
                } else {
                    val_opts.insert(option->short_name);
                }
            }
        }

        for (auto it{args.begin()}; it != args.end(); ++it) {
            if (it->length() > 2 and (*it)[0] == '-' and (*it)[1] != '-' and flag_opts.count((*it)[1])) {
                args.insert(std::next(it), (val_opts.count((*it)[2]) ? "-" : "") + it->substr(2));
                it->resize(2);
            }
        }

        //Parse options
        std::string err{};
        std::vector<std::pair<Option*, std::string>> pending_optional_positionals;
        for (auto& option : options) {
            bool found{false};
            for (auto it{args.begin()}; it != args.end(); ++it)
                if (*option == *it)
                    try {
                        found = true;
                        if (option->is_positional()) {
                            if (option->required) {
                                --required_positionals;
                            } else {
                                //Save parsing for after required positionals are fulfilled
                                pending_optional_positionals.emplace_back(option.get(), *it);
                                it = args.erase(it);
                                continue;
                            }
                        }
                        switch(option->parse(*it, *std::next(it))) {
                            case 2:
                                it = args.erase(it);
                            case 1:
                                it = args.erase(it);
                            default:
                                break;
                        }
                    } catch (std::logic_error &e) {
                        err += option->err + " \"" + option->name + "\":\n\t" + e.what() + '\n';
                    }

            if (option->required and not found) {
                if (option->is_positional() and not pending_optional_positionals.empty()) {
                    //Rob optional of argument
                    auto pending = (pending_optional_positionals.size() > required_positionals ? pending_optional_positionals.end()-required_positionals : pending_optional_positionals.begin());
                    option->parse(pending->second, "");
                    --required_positionals;
                    pending_optional_positionals.erase(pending);
                } else {
                    err += option->err + " \"" + option->name + "\"\n";
                }
            }
        }

        //Finalize pending positionals
        for (auto& pending : pending_optional_positionals) pending.first->parse(pending.second, "");

        if (not err.empty())
            throw std::invalid_argument{err};

        return args;
    }
}
#endif //BINDOPT_H
