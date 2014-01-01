#ifndef __CORELOGGER_H__
#define __CORELOGGER_H__

#include <string>

namespace VeeEm { namespace Core {

enum class LogLevel
{
    LOG_ERROR = 0,
    LOG_WARNING = 1,
    LOG_INFO = 2,
    LOG_DEBUG = 3
};

class CoreLogger
{
public:

    static void Initialize(enum LogLevel level)
    {
        if (s_CoreLogger == nullptr)
        {
            s_CoreLogger = new CoreLogger(level);
        }
    }

    static void Write(enum LogLevel level, const std::string& message)
    {
        if (s_CoreLogger != nullptr)
        {
            s_CoreLogger->WriteInternal(level, message);
        }
    }

    static void Teardown()
    {
        if (s_CoreLogger != nullptr)
        {
            delete s_CoreLogger;
        }
    }

private:
    CoreLogger(enum LogLevel level);
    void WriteInternal(enum LogLevel level, const std::string& message) const;

    LogLevel m_level;

    static CoreLogger* s_CoreLogger;
};

} } // namespace VeeEm::Core

#endif //__CORELOGGER_H__
