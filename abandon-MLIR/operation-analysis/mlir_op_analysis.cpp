#include "mlir/Parser/Parser.h"
#include <iostream>

using namespace mlir;

int main(int argc, char **argv) {
  if (argc != 2) {
    std::cerr << "Usage: " << argv[0] << " <input .mlir file>\n";
    return 1;
  }

  std::string inputFilename = argv[1];

  // Parse the MLIR module.
  mlir::MLIRContext context;
  OwningOpRef op = mlir::parseSourceFile(inputFilename, &context);
  if (!op) {
    std::cerr << "Error parsing MLIR file\n";
    return 1;
  }

  std::cout << "Operation: " << (*op)->getName().getStringRef().str() << "\n";

  return 0;
}
