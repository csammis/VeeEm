#ifndef __PARSER_H__
#define __PARSER_H__

#include <string>
#include <vector>
#include <map>

class Instruction;
enum class Opcode;

namespace VeeEm { namespace Core { namespace Parser {

typedef std::map<std::string, int> LabelInstructionMap;

class Tokenizer;

class Parser
{
public:
    explicit Parser(const std::string& filename);
    ~Parser();

    bool Parse();

    const std::vector<Instruction>& Instructions() const;
    const LabelInstructionMap& Labels() const;
    const LabelInstructionMap& Sections() const;

private:
    typedef std::map<std::string, std::pair<Opcode, int> > OpcodeParseMap;
    OpcodeParseMap m_opcodes;

    std::string m_filename;
    std::vector<Instruction> m_instructions;
    LabelInstructionMap m_labels;
    LabelInstructionMap m_sections;

    bool ParseLine(const std::string& line, int lineNumber);
    bool ParseInstruction(const std::string& operation, int lineNumber, Tokenizer& parameters);
};

} } } // namespace VeeEm::Core::Parser

#endif //__PARSER_H__

