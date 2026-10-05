#include "JackTokenizer.h"
#include <cctype>
#include <fstream>
#include <iostream>
#include <iterator>
#include <string>
#include <unordered_map>

namespace {
const std::unordered_map<std::string, KEYWORD_TYPE> KEYWORD_MAP = {
    {"class", KEYWORD_TYPE::CLASS},
    {"constructor", KEYWORD_TYPE::CONSTRUCTOR},
    {"function", KEYWORD_TYPE::FUNCTION},
    {"method", KEYWORD_TYPE::METHOD},
    {"field", KEYWORD_TYPE::FIELD},
    {"static", KEYWORD_TYPE::STATIC},
    {"var", KEYWORD_TYPE::VAR},
    {"int", KEYWORD_TYPE::INT},
    {"char", KEYWORD_TYPE::CHAR},
    {"boolean", KEYWORD_TYPE::BOOLEAN},
    {"void", KEYWORD_TYPE::VOID},
    {"true", KEYWORD_TYPE::TRUE},
    {"false", KEYWORD_TYPE::FALSE},
    {"null", KEYWORD_TYPE::NULL_VAL},
    {"this", KEYWORD_TYPE::THIS},
    {"let", KEYWORD_TYPE::LET},
    {"do", KEYWORD_TYPE::DO},
    {"if", KEYWORD_TYPE::IF},
    {"else", KEYWORD_TYPE::ELSE},
    {"while", KEYWORD_TYPE::WHILE},
    {"return", KEYWORD_TYPE::RETURN}};
} // end anonymous namespace

JackTokenizer::JackTokenizer(const std::string &filename) {
  std::ifstream file(filename, std::ios::in | std::ios::binary);
  if (file.is_open()) {
    m_source.assign((std::istreambuf_iterator<char>(file)),
                    std::istreambuf_iterator<char>());
  }
}

bool JackTokenizer::hasMoreTokens() {
  skipTrivials();
  return m_cursor < m_source.size();
}

void JackTokenizer::advance() {
  skipTrivials();

  if (m_cursor >= m_source.size()) {
    return;
  }
  char c = m_source[m_cursor];
  static const std::string symbols = "{}()[].,;+-*/&|<>=~";
  if (symbols.find(c) != std::string::npos) {
    m_tokenType = TOKEN_TYPE::SYMBOL;
    m_tokenString = std::string(1, c);
    m_cursor++;
  } else if (c == '"') {
    m_tokenType = TOKEN_TYPE::STRING_CONST;
    m_cursor++;
    size_t start = m_cursor;
    while (m_cursor < m_source.size() && m_source[m_cursor] != '"') {
      m_cursor++;
    }
    m_tokenString = m_source.substr(start, m_cursor - start);
    m_cursor++;
  } else if (std::isdigit(c)) {
    m_tokenType = TOKEN_TYPE::INT_CONST;
    size_t start = m_cursor;
    while (m_cursor < m_source.size() && std::isdigit(m_source[m_cursor])) {
      m_cursor++;
    }
    m_tokenString = m_source.substr(start, m_cursor - start);
  } else if (std::isalpha(c) || c == '_') {
    size_t start = m_cursor;
    while (m_cursor < m_source.size() &&
           (std::isalnum(m_source[m_cursor]) || m_source[m_cursor] == '_')) {
      m_cursor++;
    }
    m_tokenString = m_source.substr(start, m_cursor - start);
    auto it = KEYWORD_MAP.find(m_tokenString);
    if (it != KEYWORD_MAP.end()) {
      m_tokenType = TOKEN_TYPE::KEYWORD;
      m_keyword = it->second;
    } else {
      m_tokenType = TOKEN_TYPE::IDENTIFIER;
    }
  } else {
    m_cursor++;
  }
}

const std::string &JackTokenizer::tokenString() const { return m_tokenString; }

TOKEN_TYPE JackTokenizer::tokenType() const { return m_tokenType; }

KEYWORD_TYPE JackTokenizer::keyWord() const { return m_keyword; }

char JackTokenizer::symbol() const { return m_tokenString[0]; }

const std::string &JackTokenizer::identifier() const { return m_tokenString; }

int JackTokenizer::intVal() const { return std::stoi(m_tokenString); }

const std::string &JackTokenizer::stringVal() const { return m_tokenString; }

void JackTokenizer::skipTrivials() {
  while (m_cursor < m_source.size()) {

    // CASE 1: Whitespace
    if (std::isspace(m_source[m_cursor])) {
      m_cursor++;
      continue;
    }

    // CASE 2: Single-line comment "//"
    if (m_source[m_cursor] == '/' && m_cursor + 1 < m_source.size() &&
        m_source[m_cursor + 1] == '/') {
      m_cursor++;
      m_cursor++;
      while (m_cursor < m_source.size() && m_source[m_cursor] != '\n') {
        m_cursor++;
      }
      m_cursor++;
      continue;
    }

    // CASE 3: Multi-line comment "/*" or "/**"
    if (m_source[m_cursor] == '/' && m_cursor + 1 < m_source.size() &&
        m_source[m_cursor + 1] == '*') {
      m_cursor++;
      m_cursor++;
      while (m_cursor + 1 < m_source.size() &&
             !(m_source[m_cursor] == '*' && m_source[m_cursor + 1] == '/')) {
        m_cursor++;
      }
      m_cursor++;
      m_cursor++;
      continue;
    }

    break;
  }
}
