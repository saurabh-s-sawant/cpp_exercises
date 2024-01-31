/** Policy-based design to create message logger
 *
 * see: https://stackoverflow.com/questions/20276659/policy-based-approach-with-logger
 */
#include <iostream>
#include <fstream>
#include <string>
#include <chrono>
#include <iomanip>
#include <filesystem>
#include <mutex>

struct WriteToConsole 
{
    void operator() (const std::string& msg) {
        std::cout << msg << "\n";
    }
};

struct WriteToFile 
{
    explicit WriteToFile(std::string filename) 
    {
        try 
        {
            /* lock the mutex for thread-safety.
             * if filenames are guaranteed to be unique to each thread, 
             * then _mutex is not necessary.
             */
            std::lock_guard<std::mutex> lock(_mutex);

            if (std::filesystem::exists(filename)) {
                std::ofstream clearFile(filename.c_str(), std::ios::trunc);        
                if (!clearFile.is_open()) {
                    throw std::runtime_error("Error clearing file: " + filename);
                }
            }

            _file.open(filename.c_str(), std::ios::app);

            if(!_file.is_open()) {
                throw std::runtime_error("Error opening file: " + filename);
            }
        }
        catch (...) {
            // rethrow exception after unlocking mutex
            _mutex.unlock();
            throw; 
        }
    }
    void operator() (const std::string& msg) 
    {
        try {
            std::lock_guard<std::mutex> lock(_mutex);

            _file << msg << "\n";
        }
        catch (...) {
            _mutex.unlock();
            throw; 
        }
    }

    WriteToFile(const WriteToFile&) = delete;
    WriteToFile& operator=(const WriteToFile&) = delete;

    WriteToFile(WriteToFile&& that) : 
        _file(std::move(that._file))
    {}

    WriteToFile& operator=(WriteToFile&& that) noexcept 
    {
        if(this == &that) return *this;

        _file = std::move(that._file);
        return *this;
    }

    ~WriteToFile() noexcept 
    {
        std::lock_guard<std::mutex> lock(_mutex);
        if(_file.is_open()) _file.close();
    }  

private:
    std::ofstream _file;
    std::mutex _mutex;
};


struct WithTimeStamp
{
    static std::string get_timestamp()
    {
        auto timestamp = std::chrono::system_clock::now();
        std::time_t now_c = std::chrono::system_clock::to_time_t(timestamp);

        std::ostringstream oss;
        oss << std::put_time(std::localtime(&now_c), "%T");
        return "[" + oss.str() + "]: ";
    }
};

struct NoTimeStamp
{
    static std::string get_timestamp()
    {
        return std::string();
    }
};

template <typename StreamPolicy=WriteToConsole, typename TimePolicy=NoTimeStamp>
class MsgLogger : private StreamPolicy
{
public:
    /* Preference for r-value references.
     * l-value strings must be moved while passing as arguments.
     */
    explicit MsgLogger(StreamPolicy&& stream_policy = StreamPolicy(),
                       std::string&& init_msg = "###\n") : 
    StreamPolicy(std::move(stream_policy))
    {
        StreamPolicy::operator()("\n" + TimePolicy::get_timestamp() + init_msg);
    }

    void operator() (std::string msg) 
    {
        StreamPolicy::operator()(TimePolicy::get_timestamp() + msg);
    }

   ~MsgLogger() = default;
    MsgLogger(const MsgLogger& that) = delete;
    MsgLogger& operator=(const MsgLogger& that) = delete;

    MsgLogger(MsgLogger&& that) : 
        StreamPolicy(std::move(that))
    {}

    MsgLogger& operator=(MsgLogger&& that) noexcept
    {
        if(this == &that) return *this;

        StreamPolicy::operator=(std::move(that));
        return *this;
    }

};

int main() 
{
    MsgLogger clogger{WriteToConsole(), "Hello, this is clogger!"};
    clogger("clogger is logging...");

    MsgLogger<WriteToConsole, WithTimeStamp> ctlogger{WriteToConsole(), "Hello from ctlogger!"};
    ctlogger("ctlogger is logging...");

    MsgLogger<WriteToFile, WithTimeStamp> ftlogger{WriteToFile("file1.txt"), "Hello from ftlogger!"};
    ftlogger("ftlogger is logging...");

    std::string header = "Hello from ftlogger2!";
    MsgLogger<WriteToFile, WithTimeStamp> ftlogger2{WriteToFile("file1.txt"), std::move(header)};
    ftlogger2("ftlogger2 is logging...");
    ftlogger("ftlogger is still logging...");
    ftlogger2("ftlogger2 is also still logging...");

    MsgLogger<WriteToFile, WithTimeStamp> ftlogger3{std::move(ftlogger2)};
    ftlogger3("ftlogger3 started logging...");

}
