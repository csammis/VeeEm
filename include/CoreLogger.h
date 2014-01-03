#ifndef __CORELOGGER_H__
#define __CORELOGGER_H__

#include <string>
#include <iostream> // cstodo ugh

namespace VeeEm { namespace Core {

enum class LogLevel
{
    ERROR = 0,
    WARNING = 1,
    INFO = 2,
    DEBUG = 3
};

class CoreLogger
{
public:

    static void Initialize(enum LogLevel level);
    static CoreLogger& Instance();
    static void Teardown();

    LogLevel Level() const { return m_Level; }

    bool IsStart() const { return m_IsStart; }
    void IsStart(bool val) { m_IsStart = val; }

    friend class Level;
    friend class End;

private:
    CoreLogger(enum LogLevel level);

    LogLevel m_Level;
    bool m_IsStart;

    static CoreLogger* s_CoreLogger;
};

class Level
{
public:
    Level(LogLevel level) : m_Level(level) { }

    CoreLogger& operator()(CoreLogger& logger) const;

private:
    LogLevel m_Level;
};

class End
{
public:
    End() { }

    CoreLogger& operator()(CoreLogger& logger) const;
};


template<typename T> CoreLogger& operator<<(CoreLogger& logger, const T& val)
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

inline CoreLogger& operator<<(CoreLogger& logger, Level manip)
{
    return manip(logger);
}

inline CoreLogger& operator<<(CoreLogger& logger, End manip)
{
    return manip(logger);
}

} } // namespace VeeEm::Core

#endif //__CORELOGGER_H__
