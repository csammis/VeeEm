#ifndef __PARSER_H__
#define __PARSER_H__

#include <string>
#include <vector>
#include <map>

class Instruction;
enum class Opcode;

namespace VeeEm { namespace Core { namespace Parser {

struct Line
{
    std::string m_content;
    int m_lineNumber;

    explicit Line(const std::string& content, int lineNumber)
        : m_content(content), m_lineNumber(lineNumber) { }
};

typedef std::map<std::string, int> LabelInstructionMap;

class Tokenizer;

class Parser
{
public:
    Parser();
    ~Parser();

    bool ParseLine(const Line& line, std::vector<Instruction>& instructions, LabelInstructionMap& labels);

private:
    typedef std::map<std::string, std::pair<Opcode, int> > OpcodeParseMap;
    OpcodeParseMap m_opcodes;

    bool ParseInstruction(const std::string& operation, int lineNumber, Tokenizer& parameters, std::vector<Instruction>& instructions);
};

} } } // namespace VeeEm::Core::Parser

#endif //__PARSER_H__

