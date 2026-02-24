#include "Parser.h"
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

Parser::Parser(std::string fileName) : inputFile(fileName) {
  file.open(inputFile);
  if (!file.is_open()) {
    std::cerr << "Error opening file." << std::endl;
    throw std::runtime_error("Error opening file");
  }
  loadNextCommand();
}

void Parser::advance() {
  if (!moreCommands) {
    std::cerr << "Misused API" << std::endl;
    throw std::runtime_error("Misused API");
  }
  currentLine = nextLine;
  tokens = tokenise(currentLine);
  loadNextCommand();
}

CMD_TYPE Parser::commandType() {
  if (tokens[0] == "push") {
    return CMD_TYPE::C_PUSH;
  } else if (tokens[0] == "pop") {
    return CMD_TYPE::C_POP;
  } else if (tokens[0] == "label") {
    return CMD_TYPE::C_LABEL;
  } else if (tokens[0] == "goto") {
    return CMD_TYPE::C_GOTO;
  } else if (tokens[0] == "if-goto") {
    return CMD_TYPE::C_IF;
  } else if (tokens[0] == "function") {
    return CMD_TYPE::C_FUNCTION;
  } else if (tokens[0] == "call") {
    return CMD_TYPE::C_CALL;
  } else if (tokens[0] == "return") {
    return CMD_TYPE::C_RETURN;
  } else {
    return CMD_TYPE::C_ARITHMETIC;
  }
}

std::string Parser::arg1() {
  if (commandType() == CMD_TYPE::C_ARITHMETIC) {
    return tokens[0];
  } else {
    return tokens[1];
  }
}

int Parser::arg2() {
  CMD_TYPE cmdType = commandType();
  if (cmdType == CMD_TYPE::C_PUSH || cmdType == CMD_TYPE::C_POP ||
      cmdType == CMD_TYPE::C_FUNCTION || cmdType == CMD_TYPE::C_CALL) {
    return stoi(tokens[2]);
  } else {
    std::cerr << "No second argument for current command type" << std::endl;
    throw std::runtime_error("No second argument allowed");
  }
}

bool Parser::hasMoreCommands() { return moreCommands; }

std::vector<std::string> Parser::tokenise(std::string cmd) {
  std::vector<std::string> tokenised;

  std::istringstream iss(cmd);
  std::string word;

  while (iss >> word) {
    tokenised.push_back(word);
  }
  return tokenised;
}

void Parser::loadNextCommand() {
  std::string line;
  while (true) {
    if (!std::getline(file, line)) {
      moreCommands = false;
      return;
    }
    removeInline(line);
    if (onlyws(line) || line.empty()) {
      continue;
    } else {
      nextLine = line;
      moreCommands = true;
      return;
    }
  }
}

bool Parser::onlyws(std::string line) {
  for (char c : line) {
    if (!std::isspace(static_cast<unsigned int>(c))) {
      return false;
    }
  }
  return true;
}

void Parser::removeInline(std::string &line) {
  size_t pos = line.find("//");
  if (pos == std::string::npos) {
    return;
  } else {
    line.erase(pos);
  }
}
