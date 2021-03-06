#ifndef __CORELOGGER_H__
#define __CORELOGGER_H__

#include <string>
#include <iostream> // cstodo ugh

namespace VeeEm { namespace Core { namespace Logger {

enum class LogLevel
{
    ERROR = 0,
    WARNING = 1,
    INFO = 2,
    DEBUG = 3
};

class Log
{
public:

    static void Initialize(enum LogLevel threshold);
    static Log& Instance(enum LogLevel level);
    static void Teardown();

    LogLevel Level() const { return m_Level; }
    LogLevel Threshold() const { return m_Threshold; }
    bool ShouldOutput() const { return m_Level <= m_Threshold; }

    bool IsStart() const { return m_IsStart; }
    void IsStart(bool val) { m_IsStart = val; }

    friend class End;

private:
    Log(enum LogLevel level);

    LogLevel m_Level;
    LogLevel m_Threshold;
    bool m_IsStart;

    static Log* s_Log;
};

template<typename T> Log& operator<<(Log& logger, const T& val)
{
    using namespace std;

    if (logger.ShouldOutput())
    {
        if (logger.IsStart())
        {
            switch (logger.Level())
            {
            case LogLevel::ERROR:
                cout << "!! ERROR: ";
                break;
            case LogLevel::WARNING:
                cout << "~~ WARN:  ";
                break;
            case LogLevel::INFO:
                cout << "## INFO:  ";
                break;
            case LogLevel::DEBUG:
                cout << "** DEBUG: ";
                break;
            }
            logger.IsStart(false);
        }

        cout << val;
    }

    return logger;
}

// Output manipulator for writing a log line and resetting to print a new header
//
class End
{
public:
    End() { }

    Log& operator()(Log& logger) const;
};

inline Log& operator<<(Log& logger, End manip)
{
    return manip(logger);
}

} } } // namespace VeeEm::Core::Logger

#endif //__CORELOGGER_H__
