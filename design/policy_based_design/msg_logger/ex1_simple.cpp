/** Policy-based design to create message logger
 *
 * see: https://stackoverflow.com/questions/20276659/policy-based-approach-with-logger
 */
#include <cstdlib>
#include <utility>
#include <iostream>


struct WriteToConsole 
{
    void operator() (const std::string& msg) const {
        std::cout << msg << "\n";
    }
};


template <typename StreamPolicy=WriteToConsole>
class MsgLogger : private StreamPolicy
{
    std::string _msg;
public:
    /*pass-by-value and then move: 
     * best case: copy elision (compiler will elid the copy) and members are move constructed
     * worst case: when passing l-value, copy and then move is performed
     */
    //explicit MsgLogger(std::string&& msg = "*****",
    //                   StreamPolicy stream_policy = StreamPolicy()) : 
    //_msg(std::move(msg)),
    //StreamPolicy(std::move(stream_policy))
    //{
    //    StreamPolicy::operator()(_msg);
    //}

    /*pass-by-r-value reference: 
     * l-values cannot be passed, but to be moved while passing as arguments
     */
    explicit MsgLogger(std::string&& msg = "*****",
                       StreamPolicy&& stream_policy = StreamPolicy()) : 
    _msg(std::move(msg)),
    StreamPolicy(std::move(stream_policy))
    {
        StreamPolicy::operator()(_msg);
    }

    /*copy constructor*/
    MsgLogger(const MsgLogger& that) :
        _msg(that._msg),
        StreamPolicy(that)
    {
        StreamPolicy::operator()(_msg);
    }

    /*copy assignment operator*/
    MsgLogger& operator=(const MsgLogger& that) 
    {
        if(this == &that) return *this;
        _msg = that._msg;
        StreamPolicy::operator=(that);
        StreamPolicy::operator()(_msg);
        return *this;
    }

    /*move constructor*/
    MsgLogger(MsgLogger&& that) : 
        _msg(std::move(that._msg)), 
        StreamPolicy(std::move(that))
    {
        StreamPolicy::operator()(_msg);
    }

    /*move assignment operator*/
    MsgLogger& operator=(MsgLogger&& that) 
    {
        if(this == &that) return *this;
        _msg = std::move(that._msg);
        StreamPolicy::operator=(std::move(that));
        StreamPolicy::operator()(_msg);
        return *this;
    }
};

int main() 
{
    MsgLogger clog{}; /*default constructor*/
    MsgLogger clog2{"Hello from clog2"};
    MsgLogger clog3{clog2}; /*copy constructor*/
    clog = clog2; /*copy assignment operator*/
    MsgLogger clog4{std::move(clog2)}; /*move constructor*/
    clog = std::move(clog4); /*move assignment operator*/
}
