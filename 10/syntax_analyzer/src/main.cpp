#include "CompilationEngine.h"
#include "JackTokenizer.h"
#include <filesystem>
#include <fstream>
#include <iostream>
#include <string>
#include <string_view>

void processFiles(const std::filesystem::path &originalPath);
std::string_view tokenTypeToXmlTag(TOKEN_TYPE type);
std::string escapeXML(char c);

int main(int argc, char *argv[]) {
  if (argc != 2) {
    std::cout << "Usage: ./build/bin/syntaxanalyzer <file.jack | directory>\n";
    return 1;
  }

  std::filesystem::path originalPath(argv[1]);

  if (std::filesystem::is_regular_file(originalPath) &&
      originalPath.extension() == ".jack") {
    processFiles(originalPath);
  } else if (std::filesystem::exists(originalPath) &&
             std::filesystem::is_directory(originalPath)) {
    for (const auto &entry :
         std::filesystem::directory_iterator(originalPath)) {
      if (entry.is_regular_file() && entry.path().extension() == ".jack") {
        processFiles(entry.path());
      }
    }
  } else {
    std::cerr << "File not found!" << std::endl;
    return 1;
  }

  return 0;
}

void processFiles(const std::filesystem::path &originalPath) {
  auto parent = originalPath.parent_path();
  auto stem = originalPath.stem().string();

  auto tokensOutputPath = parent / (stem + "T.xml");
  auto outputPath = parent / (stem + ".xml");

  JackTokenizer tokenizer(originalPath);
  std::ofstream outputFile(outputPath);
  if (!outputFile.is_open()) {
    std::cerr << "Failed to open output file!" << std::endl;
  }

  if (tokenizer.hasMoreTokens()) {
    // TOKEN_TYPE type = tokenizer.tokenType();
    // std::string_view typeTag = tokenTypeToXmlTag(type);
    //
    // std::string tokenValue = tokenizer.tokenString();
    //
    // if (type == TOKEN_TYPE::SYMBOL) {
    //   tokenValue = escapeXML(tokenizer.symbol());
    // }
    //
    // outputFile << "<" << typeTag << "> " << tokenValue << " </" << typeTag
    //            << ">\n";
  }

  tokenizer.advance();

  CompilationEngine engine(tokenizer, outputFile);
  engine.compileClass();

  outputFile.close();
}

std::string_view tokenTypeToXmlTag(TOKEN_TYPE type) {
  switch (type) {
  case TOKEN_TYPE::KEYWORD:
    return "keyword";
  case TOKEN_TYPE::SYMBOL:
    return "symbol";
  case TOKEN_TYPE::IDENTIFIER:
    return "identifier";
  case TOKEN_TYPE::INT_CONST:
    return "integerConstant";
  case TOKEN_TYPE::STRING_CONST:
    return "stringConstant";
  }
  return "";
}
