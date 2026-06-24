#include "CodeWriter.h"
#include "Parser.h"
#include <filesystem>
#include <iostream>
#include <string>
#include <vector>

void TranslationHandler(std::filesystem::path src, CodeWriter &C);

int main(int argc, char *argv[]) {
  (void)argv;
  if (argc != 2) {
    std::cout << "usage: ./build/VMTranslator <file.vm | directory>\n";
    return 1;
  }
  std::filesystem::path SOURCE(argv[1]);
  std::string ASM_NAME;
  bool src_is_file;

  if (std::filesystem::is_regular_file(SOURCE)) {
    src_is_file = true;
    std::filesystem::path out = SOURCE;
    ASM_NAME = out.replace_extension(".asm").string();
  } else if (std::filesystem::is_directory(SOURCE)) {
    src_is_file = false;
    ASM_NAME = (SOURCE / SOURCE.filename()).string() + ".asm";
  } else {
    std::cerr << "Invalid path\n";
    return 1;
  }

  CodeWriter C(ASM_NAME);

  if (src_is_file) {
    C.setFileName(SOURCE.stem().string());
    TranslationHandler(SOURCE, C);
  } else {
    std::vector<std::filesystem::path> VM_srcs;
    for (const auto &entry : std::filesystem::directory_iterator(SOURCE)) {
      if (entry.is_regular_file() && entry.path().extension() == ".vm") {
        VM_srcs.push_back(entry.path());
      }
    }
    for (const auto &entry : VM_srcs) {
      C.setFileName(entry.stem().string());
      TranslationHandler(entry, C);
    }
  }

  return 0;
}
void TranslationHandler(std::filesystem::path src, CodeWriter &C) {
  Parser P(src);

  while (P.hasMoreCommands()) {
    P.advance();
    CMD_TYPE type = P.commandType();
    if (type == CMD_TYPE::C_ARITHMETIC) {
      C.WriteArithmetic(P.arg1());
    } else if (type == CMD_TYPE::C_PUSH) {
      C.WritePushPop(CMD_TYPE::C_PUSH, P.arg1(), P.arg2());
    } else if (type == CMD_TYPE::C_POP) {
      C.WritePushPop(CMD_TYPE::C_POP, P.arg1(), P.arg2());
    } else if (type == CMD_TYPE::C_LABEL) {
      C.writeLabel(P.arg1());
    } else if (type == CMD_TYPE::C_GOTO) {
      C.writeGoto(P.arg1());
    } else if (type == CMD_TYPE::C_IF) {
      C.writeIf(P.arg1());
    }
  }
}
