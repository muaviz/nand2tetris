#include "CodeWriter.h"
#include <iostream>
#include <stdexcept>

CodeWriter::CodeWriter(std::string fileName) : outputFile(fileName) {
  file.open(outputFile);
  if (!file.is_open()) {
    std::cerr << "Error opening file." << std::endl;
    throw std::runtime_error("Error opening file");
  }
  labelCounter = 0;
}

void CodeWriter::setFileName(const std::string &fileName) {
  currentFileName = fileName;
}

void CodeWriter::WriteArithmetic(std::string command) {
  if (command == "add") {
    emitBinaryOperation("D+M");
  } else if (command == "sub") {
    emitBinaryOperation("M-D");
  } else if (command == "and") {
    emitBinaryOperation("D&M");
  } else if (command == "or") {
    emitBinaryOperation("D|M");
  } else if (command == "neg") {
    emitUnaryOperation("-");
  } else if (command == "not") {
    emitUnaryOperation("!");
  } else if (command == "eq") {
    emitCompOperation("JEQ");
  } else if (command == "gt") {
    emitCompOperation("JGT");
  } else if (command == "lt") {
    emitCompOperation("JLT");
  }
}

void CodeWriter::WritePushPop(CMD_TYPE command, std::string segment, int i) {

  std::string filename = outputFile;

  size_t slashPos = filename.find_last_of('/');
  if (slashPos != std::string::npos) {
    filename = filename.substr(slashPos + 1);
  }

  size_t dotPos = filename.find_last_of('.');
  if (dotPos != std::string::npos) {
    filename = filename.substr(0, dotPos);
  } else {
    std::cerr << "Error creating static instruction symbol" << std::endl;
    throw std::runtime_error("Error creating static instruction symbol");
  }

  std::string base;
  if (segment == "local") {
    base = "LCL";
  } else if (segment == "argument") {
    base = "ARG";
  } else if (segment == "this") {
    base = "THIS";
  } else if (segment == "that") {
    base = "THAT";
  }
  if (command == CMD_TYPE::C_PUSH) {
    if (segment == "constant") {
      // @i
      // D = A
      // @SP
      // A = M
      // M = D
      // @SP
      // M = M + 1
      file << "@" << i << "\n"
           << "D=A\n"
           << "@SP\n"
           << "A=M\n"
           << "M=D\n"
           << "@SP\n"
           << "M=M+1\n";
    } else if (segment == "temp") {
      // @i+5
      // D = M
      // @SP
      // A = M
      // M = D
      // @SP
      // M = M + 1
      file << "@" << i + 5 << "\nD=M\n"
           << "@SP\n"
           << "A=M\n"
           << "M=D\n"
           << "@SP\n"
           << "M=M+1\n";
    } else if (segment == "pointer") {
      if (i != 0 && i != 1) {
        std::cerr << "Invalid instruction" << std::endl;
        throw std::runtime_error("Invalid instruction");
      }
      // @THIS/@THAT
      // D = M
      // @SP
      // A = M
      // M = D
      // @SP
      // M = M + 1
      std::string reg = (i == 0) ? "@THIS\n" : "@THAT\n";
      file << reg << "D=M\n"
           << "@SP\n"
           << "A=M\n"
           << "M=D\n"
           << "@SP\n"
           << "M=M+1\n";
    } else if (segment == "static") {
      // @file.i
      // D = M
      // @SP
      // A = M
      // M = D
      // @SP
      // M = M + 1
      file << "@" << filename << "." << i << "\nD=M\n"
           << "@SP\n"
           << "A=M\n"
           << "M=D\n"
           << "@SP\n"
           << "M=M+1\n";
    }
    // @segment
    // D = M
    // @i
    // D = D + A
    // @addr
    // M = D
    // @addr
    // A = M
    // D = M
    // @SP
    // A = M
    // M = D
    // @SP
    // M = M + 1
    if (!base.empty()) {
      file << "@" << base << "\n"
           << "D=M\n"
           << "@" << i << "\n"
           << "D=D+A\n"
           << "@addr\n"
           << "M=D\n"
           << "@addr\n"
           << "A=M\n"
           << "D=M\n"
           << "@SP\n"
           << "A=M\n"
           << "M=D\n"
           << "@SP\n"
           << "M=M+1\n";
    }
  } else if (command == CMD_TYPE::C_POP) {
    if (segment == "constant") {
      std::cerr << "Invalid command syntax" << std::endl;
      throw std::runtime_error("Invalid syntax\n");
    } else if (segment == "temp") {
      // @SP
      // AM = M - 1
      // D = M
      // @i+5
      // M = D
      file << "@SP\n"
           << "AM=M-1\n"
           << "D=M\n"
           << "@" << i + 5 << "\n"
           << "M=D\n";
    } else if (segment == "pointer") {
      if (i != 0 && i != 1) {
        std::cerr << "Invalid instruction" << std::endl;
        throw std::runtime_error("Invalid instruction");
      }
      // @SP
      // AM = M - 1
      // D = M
      // @THIS/THAT
      // M = D
      std::string reg = (i == 0) ? "@THIS\n" : "@THAT\n";
      file << "@SP\n" << "AM=M-1\n" << "D=M\n" << reg << "M=D\n";
    } else if (segment == "static") {
      // @SP
      // AM = M - 1
      // D = M
      // @file.i
      // M = D
      file << "@SP\n"
           << "AM=M-1\n"
           << "D=M\n"
           << "@" << filename << "." << i << "\nM=D\n";
    }
    // @segment
    // D=M
    // @i
    // D=D+A
    // @addr
    // M = D
    // @SP
    // AM=M-1
    // D=M
    // @addr
    // A=M
    // M=D
    if (!base.empty()) {
      file << "@" << base << "\n"
           << "D=M\n"
           << "@" << i << "\n"
           << "D=D+A\n"
           << "@addr\n"
           << "M=D\n"
           << "@SP\n"
           << "AM=M-1\n"
           << "D=M\n"
           << "@addr\n"
           << "A=M\n"
           << "M=D\n";
    }
  }
}

// binaryops:
//     @SP
//     AM = M - 1
//     D = M
//     A = A - 1
//     M = D (op) M
void CodeWriter::emitBinaryOperation(std::string expr) {
  file << "@SP\n"
       << "AM=M-1\n"
       << "D=M\n"
       << "A=A-1\n"
       << "M=" << expr << "\n";
}

// unaryops:
//     @SP
//     A = M - 1
//     M = (op)M
void CodeWriter::emitUnaryOperation(std::string op) {
  file << "@SP\n"
       << "A=M-1\n"
       << "M=" << op << "M\n";
}
// compops:
//     @SP
//     AM = M - 1
//     D = M
//     A = A - 1
//     D = M - D
//     @TRUE_i
//     D;JEQ OR D;JGT OR D;JLT
//     @SP
//     A = M - 1
//     M = 0
//     @END_i
//     0;JMP
//     (TRUE_i)
//     @SP
//     A = M - 1
//     M = -1
//     (END_i)
//
void CodeWriter::emitCompOperation(std::string op) {
  file << "@SP\n"
       << "AM=M-1\n"
       << "D=M\n"
       << "A=A-1\n"
       << "D=M-D\n"
       << "@TRUE_" << labelCounter << "\nD;" << op << "\n"
       << "@SP\n"
       << "A=M-1\n"
       << "M=0\n"
       << "@END_" << labelCounter << "\n"
       << "0;JMP\n"
       << "(TRUE_" << labelCounter << ")\n"
       << "@SP\n"
       << "A=M-1\n"
       << "M=-1\n"
       << "(END_" << labelCounter << ")\n";

  labelCounter++;
}

void CodeWriter::writeLabel(const std::string label) {
  file << "(" << label << ")\n";
}

void CodeWriter::writeGoto(const std::string label) {
  file << "@" << label << "\n0;JMP\n";
}

void CodeWriter::writeIf(const std::string label) {
  file << "@SP\n" << "AM=M-1\n" << "D=M\n" << "@" << label << "\nD;JNE\n";
}

void CodeWriter::writeFunction(const std::string functionName, int nArgs) {
  file << "(" << functionName << ")\n";
  for (int i = 0; i < nArgs; i++) {
    file << "@0\n"
         << "D=A\n"
         << "@SP\n"
         << "A=M\n"
         << "M=D\n"
         << "@SP\n"
         << "M=M+1\n";
  }
}
