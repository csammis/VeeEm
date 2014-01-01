#include "CoreLogger.h"

#include <iostream>

using namespace VeeEm::Core;

CoreLogger* CoreLogger::s_CoreLogger;

CoreLogger::CoreLogger(enum LogLevel level)
    : m_level(level)
{
}

void CoreLogger::WriteInternal(enum LogLevel level, const std::string& message) const
{
    /*if (level > m_level)
    {
        return;
    }*/

    using namespace std;
    switch (level)
    {
    case LogLevel::LOG_ERROR:
        cout << "!! ERROR: ";
        break;
    case LogLevel::LOG_WARNING:
        cout << "~~ WARN:  ";
        break;
    case LogLevel::LOG_INFO:
        cout << "## INFO:  ";
        break;
    case LogLevel::LOG_DEBUG:
        cout << "** DEBUG: ";
        break;
    }

    cout << message << endl;
}

