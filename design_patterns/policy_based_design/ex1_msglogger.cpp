/* Author: Saurabh S. Sawant
 * Description: Example of policy-based design. 
 *
 * A message logger with three policies:
 *   1) StreamPolicy: dictates where the log messages will be directed. Implementations:
 *       - WriteToConsole: sends messages to the console. (default)
 *       - WriteToFile: writes messages to a file (with support for thread safety).
 *
 *   2) StampPolicy: controls the formatting of prepended message stamps.
 *       - NoStamp: opts for no stamp inclusion. (default)
 *       - WithStamp_TimeSecPrecis: prepends a timestamp to each log message.
 *       - WithStamp_TimeMicroSecPrecis: prepends a timestamp with microsecond precision.
 *
 *   3) CallablePolicy: determines whether a callable can be passed to the logger object.
 *       - NoCallable: opts not to support a callable. (default)
 *       - WithCallable: allows users to pass a user-defined callable such as a lambda function.
 * See blog: https://saurabh-s-sawant.github.io/blog/2024/policyBasedDesign/
 */

#include <iostream>
#include <fstream>
#include <string>
#include <chrono>
#include <iomanip>
#include <filesystem>
#include <mutex>
#include <functional>
#include <complex>

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


struct WithStamp_TimeSecPrecis
{
    static std::string get_stamp()
    {
        auto timestamp = std::chrono::system_clock::now();
        auto time_now = std::chrono::system_clock::to_time_t(timestamp);

        std::ostringstream oss;
        oss << std::put_time(std::localtime(&time_now), "%T");
        return "[" + oss.str() + "] ";
    }
};

struct WithStamp_TimeMicroSecPrecis
{
    static std::string get_stamp()
    {
        auto timestamp = std::chrono::high_resolution_clock::now();
        uint64_t micros_since_epoch = std::chrono::duration_cast<std::chrono::microseconds>(
                timestamp.time_since_epoch()).count();

        auto microseconds = micros_since_epoch % 1000000;
        auto time_now = std::chrono::system_clock::to_time_t(timestamp);

        std::ostringstream oss;
        oss << std::put_time(std::localtime(&time_now), "%T")
            << "." << std::setw(6) << std::setfill('0') << microseconds;
        return "[" + oss.str() + "] ";
    }
};

struct NoStamp
{
    static std::string get_stamp()
    {
        return std::string{};
    }
};

struct WithCallable
{
    template<typename F=std::function<void()>, typename... Args>
    static void call(std::string& duration, F&& func={}, Args&&... args)
    {
        //convert func to std::function object and test
        if (std::function<void(Args...)>(func)) 
        {
            /* func can be invoked.
             * measure duration, output as string.
             */
            auto start_time = std::chrono::high_resolution_clock::now();

            std::invoke(std::forward<F>(func), std::forward<Args>(args)...);

            auto end_time = std::chrono::high_resolution_clock::now();
            auto time_elapsed = std::chrono::duration_cast<std::chrono::microseconds>
                                (end_time - start_time);

            std::ostringstream oss;
            oss << time_elapsed.count();
            duration = " Time taken: " + oss.str() + " micro-sec.\n";
        } 
        else {
            /* func is empty
             * attempting to invoke func will result in std::bad_function_call
             */
        }
    }
};

struct NoCallable
{
    template<typename F, typename... Args>
    static void call(std::string& duration, F&& func, Args&&... args)
    {
    }
};


template <typename StreamPolicy=WriteToConsole, 
          typename StampPolicy=NoStamp,
          typename CallablePolicy=NoCallable>
class MsgLogger : private StreamPolicy
{
public:
    /* Preference for r-value references.
     * l-value strings must be moved while passing as arguments.
     */
    explicit MsgLogger(std::string&& init_msg = "",
                       StreamPolicy&& stream_policy = StreamPolicy()) :
    StreamPolicy(std::move(stream_policy))
    {
        StreamPolicy::operator()("\n" + StampPolicy::get_stamp() + init_msg);
    }

    /*default F is an empty function */
    template <typename F=std::function<void()>, typename... Args>
    void operator() (std::string msg, F&& func={}, Args&&... args) 
    {
        std::string callable_duration= std::string{}; 
        
        //forward callable signature and arguments
        CallablePolicy::call(callable_duration, std::forward<F>(func), 
                                                std::forward<Args>(args)...);

        StreamPolicy::operator()(StampPolicy::get_stamp() + msg + callable_duration);
    }

   ~MsgLogger() = default;

    //non-copyable
    MsgLogger(const MsgLogger& that) = delete;
    MsgLogger& operator=(const MsgLogger& that) = delete;

    //movable, let's say
    MsgLogger(MsgLogger&& that) : 
        StreamPolicy(std::move(that)) {}

    MsgLogger& operator=(MsgLogger&& that) noexcept
    {
        if(this == &that) return *this;
        StreamPolicy::operator=(std::move(that));
        return *this;
    }
};


template<typename LoggerType, typename VecType>
void log_vector(LoggerType& logger, const std::vector<VecType>& vec) 
{
    logger("Vector is printed! ", [&]() {
        int i=0;    
        for (auto& v: vec)
        {
            std::ostringstream oss;
            oss << " vec[" << i << "]: " << v.real() << " + " << v.imag() << "i";
            logger(oss.str());
            i++;
        }
    });
}

template<typename LoggerType, typename MatrixType>
void log_matrix(LoggerType& logger, const std::vector<std::vector<MatrixType>>& matrix)
{
    logger("Matrix is printed! ", [&]() {
        for (size_t i = 0; i < matrix.size(); ++i)
        {
            std::ostringstream oss;
            oss << "row " << i << ": ";
            for (size_t j = 0; j < matrix[i].size(); ++j)
            {
                oss << matrix[i][j] << " ";
            }
            logger(oss.str());
        }
    });
}


int main() 
{
    //default logger
    MsgLogger logger("Hello, this is default  logger!");
    logger("default logger is logging...");

    //file logger without callable
    MsgLogger<WriteToFile, WithStamp_TimeMicroSecPrecis> 
        file_logger{"Hello, this is file_logger!", WriteToFile("file.dat")};
    file_logger("file_logger is logging...");

    //file logger with callable
    MsgLogger<WriteToFile, WithStamp_TimeMicroSecPrecis, WithCallable> callable_logger
    {"Hello, I am callable_logger!", WriteToFile("file.dat")};

    std::vector<std::complex<int>> complex_vec = {{1, 2}, {3, 4}, {5, 6}};
    log_vector(callable_logger, complex_vec);

    //std::vector<std::vector<int>> matrix = {{1, 2, 3}, {4, 5, 6}, {7, 8, 9}};
    //log_matrix(callable_logger, matrix);

}
