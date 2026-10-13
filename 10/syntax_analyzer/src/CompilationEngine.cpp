#include "CompilationEngine.h"
#include "JackTokenizer.h"
#include <ostream>
#include <string>

CompilationEngine::CompilationEngine(JackTokenizer &tokenizer,
                                     std::ostream &output)
    : m_tokenizer(tokenizer), m_output(output) {}

void CompilationEngine::compileClass() {
  writeOpenTag("class");

  writeCurrentToken();
  writeCurrentToken();
  writeCurrentToken();

  while (m_tokenizer.tokenType() == TOKEN_TYPE::KEYWORD &&
         (m_tokenizer.keyWord() == KEYWORD_TYPE::STATIC ||
          m_tokenizer.keyWord() == KEYWORD_TYPE::FIELD)) {
    compileClassVarDec();
  }
  while (m_tokenizer.tokenType() == TOKEN_TYPE::KEYWORD &&
         (m_tokenizer.keyWord() == KEYWORD_TYPE::CONSTRUCTOR ||
          m_tokenizer.keyWord() == KEYWORD_TYPE::FUNCTION ||
          m_tokenizer.keyWord() == KEYWORD_TYPE::METHOD)) {
    compileSubroutine();
  }

  writeCurrentToken();

  writeCloseTag("class");
}

void CompilationEngine::compileClassVarDec() {
  writeOpenTag("classVarDec");

  writeCurrentToken();
  writeCurrentToken();
  writeCurrentToken();

  while (m_tokenizer.tokenString() == ",") {
    writeCurrentToken();
    writeCurrentToken();
  }

  writeCurrentToken();
  writeCloseTag("classVarDec");
}

void CompilationEngine::compileSubroutine() {
  writeOpenTag("subroutineDec");

  writeCurrentToken();
  writeCurrentToken();
  writeCurrentToken();
  writeCurrentToken();

  compileParameterList();

  writeCurrentToken();

  compileSubroutineBody();

  writeCloseTag("subroutineDec");
}

void CompilationEngine::compileSubroutineBody() {
  writeOpenTag("subroutineBody");

  writeCurrentToken();

  while (m_tokenizer.tokenType() == TOKEN_TYPE::KEYWORD &&
         m_tokenizer.keyWord() == KEYWORD_TYPE::VAR) {
    compileVarDec();
  }

  compileStatements();

  writeCurrentToken();

  writeCloseTag("subroutineBody");
}

void CompilationEngine::compileVarDec() {
  writeOpenTag("varDec");

  writeCurrentToken();
  writeCurrentToken();
  writeCurrentToken();

  while (m_tokenizer.tokenString() == ",") {
    writeCurrentToken();
    writeCurrentToken();
  }

  writeCurrentToken();

  writeCloseTag("varDec");
}

void CompilationEngine::compileParameterList() {
  writeOpenTag("parameterList");

  if (m_tokenizer.tokenString() != ")") {
    writeCurrentToken();
    writeCurrentToken();

    while (m_tokenizer.tokenString() == ",") {
      writeCurrentToken();
      writeCurrentToken();
      writeCurrentToken();
    }
  }

  writeCloseTag("parameterList");
}

void CompilationEngine::compileStatements() {
  writeOpenTag("statements");

  while (m_tokenizer.tokenString() == "let" ||
         m_tokenizer.tokenString() == "if" ||
         m_tokenizer.tokenString() == "while" ||
         m_tokenizer.tokenString() == "do" ||
         m_tokenizer.tokenString() == "return") {
    if (m_tokenizer.tokenString() == "let") {
      compileLet();
    } else if (m_tokenizer.tokenString() == "if") {
      compileIf();
    } else if (m_tokenizer.tokenString() == "while") {
      compileWhile();
    } else if (m_tokenizer.tokenString() == "do") {
      compileDo();
    } else if (m_tokenizer.tokenString() == "return") {
      compileReturn();
    }
  }
  writeCloseTag("statements");
}

void CompilationEngine::compileLet() {
  writeOpenTag("letStatement");

  writeCurrentToken();
  writeCurrentToken();

  if (m_tokenizer.tokenString() == "[") {
    writeCurrentToken();
    compileExpression();
    writeCurrentToken();
  }

  writeCurrentToken();

  compileExpression();

  writeCurrentToken();

  writeCloseTag("letStatement");
}

void CompilationEngine::compileIf() {
  writeOpenTag("ifStatement");

  writeCurrentToken();
  writeCurrentToken();

  compileExpression();

  writeCurrentToken();
  writeCurrentToken();

  compileStatements();

  writeCurrentToken();

  if (m_tokenizer.tokenString() == "else") {
    writeCurrentToken();
    writeCurrentToken();

    compileStatements();

    writeCurrentToken();
  }

  writeCloseTag("ifStatement");
}

void CompilationEngine::compileWhile() {
  writeOpenTag("whileStatement");

  writeCurrentToken();
  writeCurrentToken();

  compileExpression();

  writeCurrentToken();
  writeCurrentToken();

  compileStatements();

  writeCurrentToken();

  writeCloseTag("whileStatement");
}

void CompilationEngine::compileDo() {
  writeOpenTag("doStatement");

  writeCurrentToken();
  writeCurrentToken();

  if (m_tokenizer.tokenString() == ".") {
    writeCurrentToken();
    writeCurrentToken();
  }
  writeCurrentToken();

  compileExpressionList();

  writeCurrentToken();
  writeCurrentToken();

  writeCloseTag("doStatement");
}

void CompilationEngine::compileReturn() {
  writeOpenTag("returnStatement");

  writeCurrentToken();

  if (m_tokenizer.tokenString() != ";") {
    compileExpression();
  }

  writeCurrentToken();

  writeCloseTag("returnStatement");
}

void CompilationEngine::compileExpression() {
  writeOpenTag("expression");
  compileTerm();

  while (m_tokenizer.tokenString() == "+" || m_tokenizer.tokenString() == "-" ||
         m_tokenizer.tokenString() == "=" || m_tokenizer.tokenString() == ">" ||
         m_tokenizer.tokenString() == "<" || m_tokenizer.tokenString() == "*" ||
         m_tokenizer.tokenString() == "/" || m_tokenizer.tokenString() == "&" ||
         m_tokenizer.tokenString() == "|") {
    writeCurrentToken();
    compileTerm();
  }

  writeCloseTag("expression");
}

void CompilationEngine::compileTerm() {
  writeOpenTag("term");

  switch (m_tokenizer.tokenType()) {
  case TOKEN_TYPE::INT_CONST: {
    writeCurrentToken();
    break;
  }
  case TOKEN_TYPE::STRING_CONST: {
    writeCurrentToken();
    break;
  }
  case TOKEN_TYPE::KEYWORD: {
    KEYWORD_TYPE kw = m_tokenizer.keyWord();
    if (kw == KEYWORD_TYPE::TRUE || kw == KEYWORD_TYPE::FALSE ||
        kw == KEYWORD_TYPE::NULL_VAL || kw == KEYWORD_TYPE::THIS) {
      writeCurrentToken();
    }
    break;
  }
  case TOKEN_TYPE::SYMBOL: {
    std::string symb = m_tokenizer.tokenString();
    if (symb == "(") {
      writeCurrentToken();
      compileExpression();
      writeCurrentToken();
    } else if (symb == "-" || symb == "~") {
      writeCurrentToken();
      compileTerm();
    }
    break;
  }
  case TOKEN_TYPE::IDENTIFIER: {
    std::string nextToken = m_tokenizer.peekNextToken();
    if (nextToken == "[") {
      writeCurrentToken();
      writeCurrentToken();
      compileExpression();
      writeCurrentToken();
    } else if (nextToken == "(" || nextToken == ".") {
      writeCurrentToken();
      if (m_tokenizer.tokenString() == ".") {
        writeCurrentToken();
        writeCurrentToken();
      }
      writeCurrentToken();
      compileExpressionList();
      writeCurrentToken();
    } else {
      writeCurrentToken();
    }
    break;
  }
  }

  writeCloseTag("term");
}

void CompilationEngine::compileExpressionList() {
  writeOpenTag("expressionList");
  if (m_tokenizer.tokenString() != ")") {
    compileExpression();
    while (m_tokenizer.tokenString() == ",") {
      writeCurrentToken();
      compileExpression();
    }
  }
  writeCloseTag("expressionList");
}

void CompilationEngine::writeOpenTag(const std::string &tag) {
  m_output << std::string(m_indent, ' ');
  m_output << "<" << tag << ">\n";
  m_indent += 2;
}

void CompilationEngine::writeCloseTag(const std::string &tag) {
  m_indent -= 2;
  m_output << std::string(m_indent, ' ');
  m_output << "</" << tag << ">\n";
}

void CompilationEngine::writeCurrentToken() {
  m_output << std::string(m_indent, ' ');

  TOKEN_TYPE type = m_tokenizer.tokenType();
  switch (type) {
  case TOKEN_TYPE::KEYWORD:
    m_output << "<keyword> " << m_tokenizer.tokenString() << " </keyword>\n";
    break;
  case TOKEN_TYPE::SYMBOL:

    m_output << "<symbol> " << escapeXML(m_tokenizer.symbol())
             << " </symbol>\n";
    break;
  case TOKEN_TYPE::IDENTIFIER:
    m_output << "<identifier> " << m_tokenizer.identifier()
             << " </identifier>\n";
    break;
  case TOKEN_TYPE::INT_CONST:
    m_output << "<integerConstant> " << m_tokenizer.tokenString()
             << " </integerConstant>\n";
    break;
  case TOKEN_TYPE::STRING_CONST:
    m_output << "<stringConstant> " << m_tokenizer.tokenString()
             << " </stringConstant>\n";
    break;
  }
  m_tokenizer.advance();
}

std::string escapeXML(char c) {
  switch (c) {
  case '<':
    return "&lt;";
  case '>':
    return "&gt;";
  case '&':
    return "&amp;";
  case '"':
    return "&quot;";
  default:
    return std::string(1, c);
  }
}
