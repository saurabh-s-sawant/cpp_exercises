/* Example of Policy-based design pattern to create simple message logger.
 * Three policies: 
 * 1) StreamPolicy: option to write to console or a file.
 * 2) TimePolicy: option to show time stamp (e.g. in seconds or microseconds) during logging.
 * 3) CallablePolicy: option to invoke a callable for logging, 
 * e.g. user-defined lambda to log a matrix.
 *
 * Defaults are WriteToConsole, NoTimeStamp, NoCallable.
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


struct WithTimeStamp
{
    static std::string get_timestamp()
    {
        auto timestamp = std::chrono::system_clock::now();
        auto time_now = std::chrono::system_clock::to_time_t(timestamp);

        std::ostringstream oss;
        oss << std::put_time(std::localtime(&time_now), "%T");
        return "[" + oss.str() + "] ";
    }
};

struct WithMicroSecTimeStamp
{
    static std::string get_timestamp()
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

struct NoTimeStamp
{
    static std::string get_timestamp()
    {
        return std::string();
    }
};

struct WithCallable
{
    template<typename F=std::function<void()>, typename... Args>
    static void call(std::string& duration, F&& func={}, Args&&... args)
    {
        if (std::function<void(Args...)>(func)) 
        {
            // func can be invoked
            auto start_time = std::chrono::high_resolution_clock::now();

            std::invoke(std::forward<F>(func), std::forward<Args>(args)...);

            auto end_time = std::chrono::high_resolution_clock::now();
            auto time_elapsed = std::chrono::duration_cast<std::chrono::microseconds>
                                (end_time - start_time);

            std::ostringstream oss;
            oss << time_elapsed.count();
            duration = " Time taken: " + oss.str() + " micro-sec.\n";
        } else {
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
          typename TimePolicy=NoTimeStamp,
          typename CallablePolicy=NoCallable>
class MsgLogger : private StreamPolicy
{
public:
    /* Preference for r-value references.
     * l-value strings must be moved while passing as arguments.
     */
    explicit MsgLogger(StreamPolicy&& stream_policy = StreamPolicy(),
                       std::string&& init_msg = "") : 
    StreamPolicy(std::move(stream_policy))
    {
        StreamPolicy::operator()("\n" + TimePolicy::get_timestamp() + init_msg);
    }

    template <typename F=std::function<void()>, typename... Args>
    void operator() (std::string msg, F&& func={}, Args&&... args) 
    {
        std::string callable_duration= std::string{};
        
        CallablePolicy::call(callable_duration, std::forward<F>(func), 
                                                std::forward<Args>(args)...);

        StreamPolicy::operator()(TimePolicy::get_timestamp() + msg + callable_duration);
    }

    //non-copyable, but movable
   ~MsgLogger() = default;
    MsgLogger(const MsgLogger& that) = delete;
    MsgLogger& operator=(const MsgLogger& that) = delete;

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
    MsgLogger logger{WriteToConsole(), "Hello, this is logger!"};
    logger("logger is logging...");

    MsgLogger<WriteToConsole, WithMicroSecTimeStamp> micro_logger{WriteToConsole(), "Hello, this is micro_logger!"};
    micro_logger("micro_logger is logging...");

    MsgLogger<WriteToFile, WithMicroSecTimeStamp> file_logger{WriteToFile("file.dat"), "Hello, this is file_logger!"};
    file_logger("file_logger is logging...");

    MsgLogger<WriteToFile, WithMicroSecTimeStamp, WithCallable> callable_logger
    {WriteToFile("file.dat"), "Hello, I am callable_logger!"};

    std::vector<std::complex<int>> complex_vec = {{1, 2}, {3, 4}, {5, 6}};
    log_vector(callable_logger, complex_vec);

    std::vector<std::vector<int>> matrix = {{1, 2, 3}, {4, 5, 6}, {7, 8, 9}};
    log_matrix(callable_logger, matrix);

}
