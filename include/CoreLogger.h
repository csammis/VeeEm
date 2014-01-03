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
    static Log& Instance();
    static void Teardown();

    LogLevel Level() const { return m_Level; }

    bool IsStart() const { return m_IsStart; }
    void IsStart(bool val) { m_IsStart = val; }

    friend class Level;
    friend class End;

private:
    Log(enum LogLevel level);

    LogLevel m_Level;
    LogLevel m_Threshold;
    bool m_IsStart;

    static Log* s_Log;
};

class Level
{
public:
    Level(LogLevel level) : m_Level(level) { }

    Log& operator()(Log& logger) const;

private:
    LogLevel m_Level;
};

class End
{
public:
    End() { }

    Log& operator()(Log& logger) const;
};


template<typename T> Log& operator<<(Log& logger, const T& val)
{
    using namespace std;

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

    return logger;
}

inline Log& operator<<(Log& logger, Level manip)
{
    return manip(logger);
}

inline Log& operator<<(Log& logger, End manip)
{
    return manip(logger);
}

} } } // namespace VeeEm::Core::Logger

#endif //__CORELOGGER_H__
