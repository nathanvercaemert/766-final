#include "llvm/ADT/STLExtras.h"
#include "llvm/Support/CommandLine.h"
#include "llvm/Support/ErrorOr.h"
#include "llvm/Support/FileUtilities.h"
#include "llvm/Support/InitLLVM.h"
#include "llvm/Support/SourceMgr.h"
#include "llvm/Support/ToolOutputFile.h"
#include "llvm/Support/CommandLine.h"
#include "llvm/Support/InitLLVM.h"
#include "mlir/Dialect/Func/IR/FuncOps.h"
#include "mlir/Dialect/Func/Transforms/Passes.h"
#include "mlir/Dialect/SCF/IR/SCF.h"
#include "mlir/IR/Block.h"
#include "mlir/IR/Builders.h"
#include "mlir/IR/MLIRContext.h"
#include "mlir/IR/BuiltinOps.h"
#include "mlir/IR/Operation.h"
#include "mlir/Pass/Pass.h"
#include "mlir/Pass/PassManager.h"
#include "mlir/Support/FileUtilities.h"
#include "mlir/Support/LogicalResult.h"
#include "mlir/Support/ToolUtilities.h"
#include "mlir/Support/LLVM.h"
#include "mlir/Tools/mlir-opt/MlirOptMain.h"
#include "mlir/Transforms/Passes.h"
#include "mlir/Transforms/RegionUtils.h"

using namespace llvm;
using namespace mlir;

namespace {

class InputAnalysisPass : public PassWrapper<InputAnalysisPass, OperationPass<ModuleOp>> {
public:
  StringRef getArgument() const final { return "input-analysis-pass"; }
  StringRef getDescription() const final { return "An MLIR pass that prints 'Hello World'"; }

  void runOnOperation() final {
    llvm::outs() << "Hello World\n";
  }
};

} // end anonymous namespace

std::unique_ptr<Pass> createInputAnalysisPass() {
  return std::make_unique<InputAnalysisPass>();
}

static PassRegistration<InputAnalysisPass> pass("input-analysis-pass", "An MLIR pass that prints 'Hello World'");

int main(int argc, char **argv) {
  InitLLVM y(argc, argv);

  cl::opt<std::string> inputFilename(cl::Positional, cl::desc("<input .mlir file>"), cl::init("-"));
  cl::opt<std::string> outputFilename("o", cl::desc("Output filename"), cl::value_desc("filename"), cl::init("-"));

  cl::ParseCommandLineOptions(argc, argv, "MLIR input analysis pass\n");

  // Set up the input file.
  ErrorOr<std::unique_ptr<MemoryBuffer>> fileOrErr = MemoryBuffer::getFileOrSTDIN(inputFilename);
  if (std::error_code EC = fileOrErr.getError()) {
    errs() << "Error opening input file: " << EC.message() << "\n";
    return 1;
  }

  // Set up the output file.
  std::error_code error;
  auto outputFile = std::make_unique<ToolOutputFile>(outputFilename, error, sys::fs::OF_None);
  if (error) {
    errs() << "Error opening output file: " << error.message() << "\n";
    return 1;
  }

  // Parse the input file.
  MLIRContext context;
  OwningModuleRef module = parseSourceFile(fileOrErr.get()->getMemBufferRef(), &context);
  if (!module) {
    errs() << "Error parsing input file\n";
    return 1;
  }

  // Run the pass.
  PassManager pm(&context);
  pm.addPass(createInputAnalysisPass());
  if (failed(pm.run(*module))) {
    errs() << "Error running the input analysis pass\n";
    return 1;
  }

  // Print the output.
  module->print(outputFile->os());
  outputFile->keep();

  return 0;
}
