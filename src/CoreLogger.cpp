#include "CoreLogger.h"

#include <iostream>

using namespace VeeEm::Core::Logger;

Log* Log::s_Log;

void Log::Initialize(enum LogLevel threshold)
{
    if (s_Log == nullptr)
    {
        s_Log = new Log(threshold);
    }
}

Log& Log::Instance()
{
    return (*s_Log);
}

void Log::Teardown()
{
    if (s_Log != nullptr)
    {
        delete s_Log;
    }
}

Log::Log(enum LogLevel level)
    : m_Level(level), m_Threshold(level), m_IsStart(true)
{
}

Log& Level::operator()(Log& logger) const
{
    logger.m_Level = m_Level;
    return logger;
}

Log& End::operator()(Log& logger) const
{
    std::cout << std::endl;
    logger.m_IsStart = true;
    return logger;
}

