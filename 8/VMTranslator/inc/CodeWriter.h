#include "Parser.h"
#include <fstream>
#include <string>

class CodeWriter {
public:
  CodeWriter(const std::string &fileName);
  void WriteArithmetic(const std::string &command);
  void WritePushPop(CMD_TYPE command, const std::string &segment, int i);
  void emitBinaryOperation(const std::string &op);
  void emitUnaryOperation(const std::string &op);
  void emitCompOperation(const std::string &op);
  void setFileName(const std::string &fileName);
  void writeLabel(const std::string &label);
  void writeGoto(const std::string &label);
  void writeIf(const std::string &label);
  void writeFunction(const std::string &functionName, int nVars);
  void writeCall(const std::string &functionName, int nArgs);
  void writeReturn();
  void writeInit();

private:
  std::ofstream file;
  const std::string outputFile;
  int labelCounter;
  int returnCounter;
  std::string currentFileName;
  std::string currentFunction;
};
