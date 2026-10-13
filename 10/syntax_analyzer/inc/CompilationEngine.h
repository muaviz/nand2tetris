#pragma once
#include "JackTokenizer.h"
#include <ostream>

class CompilationEngine {
public:
  CompilationEngine(JackTokenizer &tokenizer, std::ostream &output);

  void compileClass();

  void compileClassVarDec();
  void compileSubroutine();
  void compileParameterList();
  void compileSubroutineBody();
  void compileVarDec();

  void compileStatements();
  void compileLet();
  void compileIf();
  void compileWhile();
  void compileDo();
  void compileReturn();

  void compileExpression();
  void compileTerm();
  void compileExpressionList();

private:
  JackTokenizer &m_tokenizer;
  std::ostream &m_output;
  size_t m_indent{0};

  void writeOpenTag(const std::string &tag);
  void writeCloseTag(const std::string &tag);
  void writeCurrentToken();
};

std::string escapeXML(char c);
