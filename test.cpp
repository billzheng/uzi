#include <iostream>
#include <cmath>
#include <vector>
#include <list>
#include <string>
#include <algorithm>
#include <cassert>
#include <tuple>
using namespace std;

struct CmdField
{
    const std::string key;
    const char* start {nullptr};
    size_t len{0};
};

struct CmdStream
{
    explicit CmdStream(const std::string& msg)
    : begin_(msg.data()),
      end_(msg.data() + msg.size())
    {
        curr_ = begin_;
    }

    bool hasNext() const
    {
        return curr_ < end_;
    }
    std::tuple<std::string, const char*, size_t> getCurrent()
    {
        auto* p = curr_;
        assert(curr_ < end_);
        while(*curr_ != '=')
        {
            ++curr_;
        }
        std::string key(p, curr_ - p);
        ++curr_; // move over '='
        auto expectedChar = '"';
        auto val_begin{curr_};
        ++curr_;
        while (*curr_ != expectedChar)
        {
            ++curr_;
        }
        ++curr_; // move over expectedChar
        ++curr_; // move field separator
        return std::make_tuple(key, val_begin, curr_ - val_begin - 1);
    }
private:
    const char* begin_{nullptr};
    const char* end_{nullptr};
    const char* curr_{nullptr};
};

int main(int argc, char* argv[])
{
    std::string s("MSGTYPE=\"CMD\" CMD=\"BRACKET_CREATE\"");
    CmdStream stream(s);

    while (stream.hasNext())
    {
       auto t = stream.getCurrent();
       std::cout << std::get<0>(t) << ' ' << std::get<1>(t) << ' ' << std::get<2>(t) << std::endl;
    }

    //std::cout << s << std::endl;
    return 0;
}
