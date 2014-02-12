#include "Parser.h"
#include "Instruction.h"
#include "CoreLogger.h"
#include "Opcode.h"
#include <fstream>

using namespace std;
using namespace VeeEm::Core::Parser;
using namespace VeeEm::Core::Logger;

namespace VeeEm { namespace Core { namespace Parser {

struct Line
{
    string m_content;
    int m_lineNumber;

    explicit Line(const string& content, int lineNumber)
        : m_content(content), m_lineNumber(lineNumber) { }
};

class Tokenizer
{
public:
    Tokenizer(const Line& l, const string& delimiters = " \t\n") : m_Line(l), m_Position(0), m_Delimiters(delimiters) { }
    
    int LineNumber() const
    {
        return m_Line.m_lineNumber;
    }

    std::string NextToken()
    {
        // 0123456789
        //   hello
        string::size_type start = m_Line.m_content.find_first_not_of(m_Delimiters, m_Position);
        if (start == string::npos)
        {
            return "";
        }
        m_Position = m_Line.m_content.find_first_of(m_Delimiters, start);
        if (m_Position == string::npos)
        {
            m_Position = m_Line.m_content.size();
        }
        return Trim(m_Line.m_content.substr(start, m_Position - start));
    }

    Tokenizer Remainder(string delimiters = " \t\n") const
    {
        Line line(m_Line.m_content.substr(m_Position), m_Line.m_lineNumber);
        return { line, delimiters };
    }

private:
    Line m_Line;
    string::size_type m_Position;
    string m_Delimiters;

    static string Trim(const string& input)
    {
        string::size_type start = input.find_first_not_of(" \t\n");
        string::size_type end = input.find_last_not_of(" \t\n");
        if (start == string::npos)
        {
            return "";
        }
        return input.substr(start, end - start + 1);
    }
};

} } } // namespace VeeEm::Core::Parser


Parser::Parser(const std::string& filename)
    : m_filename(filename)
{
    m_opcodes["load"] = std::make_pair(Opcode::LOAD, 2);
    m_opcodes["incr"] = std::make_pair(Opcode::INCREMENT, 1);
    m_opcodes["decr"] = std::make_pair(Opcode::DECREMENT, 1);
    m_opcodes["add"] = std::make_pair(Opcode::ADD, 3);
    m_opcodes["sub"] = std::make_pair(Opcode::SUBTRACT, 3);
    m_opcodes["comp"] = std::make_pair(Opcode::COMPARE, 2);
    m_opcodes["je"] = std::make_pair(Opcode::JUMP_EQUAL, 1);
    m_opcodes["jne"] = std::make_pair(Opcode::JUMP_NOT_EQUAL, 1);
    m_opcodes["jlt"] = std::make_pair(Opcode::JUMP_LESS_THAN, 1);
    m_opcodes["jlte"] = std::make_pair(Opcode::JUMP_LESS_THAN_EQUAL_TO, 1);
    m_opcodes["jgt"] = std::make_pair(Opcode::JUMP_GREATER_THAN, 1);
    m_opcodes["jgte"] = std::make_pair(Opcode::JUMP_GREATER_THAN_EQUAL_TO, 1);
    m_opcodes["push"] = std::make_pair(Opcode::PUSH, 1);
    m_opcodes["pop"] = std::make_pair(Opcode::POP, 1);
    m_opcodes["call"] = std::make_pair(Opcode::CALL, 1);
    m_opcodes["return"] = std::make_pair(Opcode::RETURN, 0);
    m_opcodes["syscall"] = std::make_pair(Opcode::SYSCALL, 1);
}

Parser::~Parser()
{
}

const std::vector<Instruction>& Parser::Instructions() const
{
    return m_instructions;
}

const LabelInstructionMap& Parser::Labels() const
{
    return m_labels;
}

const LabelInstructionMap& Parser::Sections() const
{
    return m_sections;
}

bool Parser::Parse()
{
    ifstream fromFile;
    fromFile.open(m_filename.c_str());
    if (!fromFile.good())
    {
        Log::Instance(LogLevel::ERROR) << "Unable to load " << m_filename << End();
        return false;
    }

    bool successfulParse = true;
    try
    {
        for (int linenumber = 1; fromFile.good(); linenumber++)
        {
            string raw_instruction;
            getline(fromFile, raw_instruction);

            if (!ParseLine(raw_instruction, linenumber))
            {
                successfulParse = false;
                continue;
            }
        }
    }
    catch (...)
    {
        Log::Instance(LogLevel::ERROR) << "Caught exception loading from " << m_filename << End();
        successfulParse = false;
    }

    fromFile.close();
    return successfulParse;
}

bool AddLabelToMap(const string& label, int lineNumber, int location, LabelInstructionMap& labelmap)
{
    if (labelmap.find(label) != labelmap.end())
    {
        Log::Instance(LogLevel::ERROR) << "Found duplicate label \"" << label << "\" at " << lineNumber << End();
        return false;
    }
    labelmap[label] = location;
    return true;
}

bool Parser::ParseLine(const string& line, int lineNumber)
{
    Tokenizer parse(Line(line, lineNumber));
    string token = parse.NextToken();
    if (token.empty())
    {
        return true;
    }

    if (token[0] == '#')
    {
        return true;
    }

    if (token.front() == '.')
    {
        string label = token.substr(1, token.size() - 1);
        if (!AddLabelToMap(label, parse.LineNumber(), m_instructions.size(), m_sections))
        {
            return false;
        }
    }
    else if (token.back() == ':')
    {
        string label = token.substr(0, token.size() - 1);
        if (!AddLabelToMap(label, parse.LineNumber(), m_instructions.size(), m_labels))
        {
            return false;
        }
    }
    else
    {
        Tokenizer parameters = parse.Remainder(",");
        if (!ParseInstruction(token, parse.LineNumber(), parameters))
        {
            return false;
        }
    }

    return true;
}

bool Parser::ParseInstruction(const string& operation, int lineNumber, Tokenizer& parameters)
{
    auto lookup = m_opcodes.find(operation);
    if (lookup == m_opcodes.end())
    {
        Log::Instance(LogLevel::ERROR) << "Unknown instuction found during parse: " << operation << " (line " << lineNumber << ")" << End();
        return false;
    }

    int parameterCount = lookup->second.second;
    Instruction inst(lookup->second.first, parameterCount);
    for (int i = 0; i < parameterCount; i++)
    {
        string parameter = parameters.NextToken();
        if (parameter.empty())
        {
            Log::Instance(LogLevel::ERROR) << "Found too few parameters for instruction " << operation 
                << " (expected " << parameterCount << " read " << i << ")" << End();
            return false;
        }
        inst.SetParameter(i, parameter);
    }
    m_instructions.push_back(inst);

    return true;
}



