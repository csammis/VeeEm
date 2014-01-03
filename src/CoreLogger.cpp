#include "CoreLogger.h"

#include <iostream>

using namespace VeeEm::Core;

CoreLogger* CoreLogger::s_CoreLogger;

void CoreLogger::Initialize(enum LogLevel level)
{
    if (s_CoreLogger == nullptr)
    {
        s_CoreLogger = new CoreLogger(level);
    }
}

CoreLogger& CoreLogger::Instance()
{
    return (*s_CoreLogger);
}

void CoreLogger::Teardown()
{
    if (s_CoreLogger != nullptr)
    {
        delete s_CoreLogger;
    }
}

CoreLogger::CoreLogger(enum LogLevel level)
    : m_Level(level), m_IsStart(true)
{
}

CoreLogger& Level::operator()(CoreLogger& logger) const
{
    logger.m_Level = m_Level;
    return logger;
}

CoreLogger& End::operator()(CoreLogger& logger) const
{
    std::cout << std::endl;
    logger.m_IsStart = true;
    return logger;
}

