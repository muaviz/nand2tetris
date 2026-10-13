#pragma once
#include <string>

enum class TOKEN_TYPE { KEYWORD, SYMBOL, IDENTIFIER, INT_CONST, STRING_CONST };

enum class KEYWORD_TYPE {
  CLASS,
  METHOD,
  FUNCTION,
  CONSTRUCTOR,
  INT,
  BOOLEAN,
  CHAR,
  VOID,
  VAR,
  STATIC,
  FIELD,
  LET,
  DO,
  IF,
  ELSE,
  WHILE,
  RETURN,
  TRUE,
  FALSE,
  NULL_VAL,
  THIS
};

class JackTokenizer {
public:
  JackTokenizer(const std::string &filename);
  bool hasMoreTokens();
  void advance();
  const std::string &tokenString() const;
  TOKEN_TYPE tokenType() const;
  KEYWORD_TYPE keyWord() const;
  char symbol() const;
  const std::string &identifier() const;
  int intVal() const;
  const std::string &stringVal() const;
  std::string peekNextToken();

private:
  std::string m_source;
  size_t m_cursor{0};
  std::string m_tokenString;
  TOKEN_TYPE m_tokenType;
  KEYWORD_TYPE m_keyword;

  void skipTrivials();
};
