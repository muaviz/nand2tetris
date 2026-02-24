#include <fstream>
#include <string>
#include <vector>

enum class CMD_TYPE {
  C_ARITHMETIC,
  C_PUSH,
  C_POP,
  C_LABEL,
  C_GOTO,
  C_IF,
  C_FUNCTION,
  C_RETURN,
  C_CALL
};

class Parser {
public:
  Parser(std::string fileName);
  bool hasMoreCommands();
  void advance();
  CMD_TYPE commandType();
  std::string arg1();
  int arg2();

private:
  std::string currentLine;
  std::string nextLine;
  bool moreCommands;
  const std::string inputFile;
  std::ifstream file;
  std::string currentCommand = "";
  std::vector<std::string> tokens;
  std::vector<std::string> tokenise(std::string cmd);
  void loadNextCommand();
  bool onlyws(std::string line);
  void removeInline(std::string &line);
};
