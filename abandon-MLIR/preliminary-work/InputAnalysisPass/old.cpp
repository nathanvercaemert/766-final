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

using namespace mlir;
using namespace llvm;

namespace {
struct InputAnalysisPass : public PassWrapper<InputAnalysisPass, mlir::func::FuncOp> {
    void runOnOperation() override {
        FuncOp f = getOperation();
    }
};
}

void InputAnalysisPass::runOnOperation() {
  auto module = getOperation();
  module.walk([&](func::FuncOp func) {
    for (auto &block : func) {
      for (auto &op : block) {
        if (op.getName().getStringRef() == "scf.for") {
          auto forOp = dyn_cast<scf::ForOp>(op);
          auto upperBound = forOp.getUpperBound();
          if (mlir::isa<mlir::BlockArgument>(upperBound)) {
            llvm::outs() << "User input " << upperBound << " affects loop upper bound in function " << func.getName() << ".\n";
          }
        }
      }
    }
  });
}

static PassRegistration<InputAnalysisPass> pass("input-analysis", "CSC766");
