#ifndef __VEEEMPROGRAM_H__
#define __VEEEMPROGRAM_H__

#include <vector>
#include <string>
#include <map>

class Instruction;

class VeeEmProgram
{
public:
    VeeEmProgram();
    ~VeeEmProgram();

    void SetInstructions(const std::vector<Instruction>& instructions);
    void SetLabels(const std::map<std::string, int>& labels);

    void Execute() const;

private:
    std::map<std::string, int> m_Labels;
    std::vector<Instruction> m_Instructions;
};

#endif //__VEEEMPROGRAM_H__
