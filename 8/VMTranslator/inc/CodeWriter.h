#include "Parser.h"
#include <fstream>
#include <string>

class CodeWriter {
public:
  CodeWriter(std::string fileName);
  void WriteArithmetic(std::string command);
  void WritePushPop(CMD_TYPE command, std::string segment, int i);
  void emitBinaryOperation(std::string op);
  void emitUnaryOperation(std::string op);
  void emitCompOperation(std::string op);
  void setFileName(const std::string &fileName);
  void writeLabel(const std::string label);
  void writeGoto(const std::string label);
  void writeIf(const std::string label);
  void writeFunction(const std::string functionName, int nVars);

private:
  std::ofstream file;
  const std::string outputFile;
  int labelCounter;
  std::string currentFileName;
};
