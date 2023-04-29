#loop_annotation = #llvm.loop_annotation<mustProgress = true>
module attributes {dlti.dl_spec = #dlti.dl_spec<#dlti.dl_entry<!llvm.ptr, dense<64> : vector<4xi32>>, #dlti.dl_entry<i8, dense<8> : vector<2xi32>>, #dlti.dl_entry<i1, dense<8> : vector<2xi32>>, #dlti.dl_entry<i32, dense<32> : vector<2xi32>>, #dlti.dl_entry<i16, dense<16> : vector<2xi32>>, #dlti.dl_entry<f16, dense<16> : vector<2xi32>>, #dlti.dl_entry<!llvm.ptr<270>, dense<32> : vector<4xi32>>, #dlti.dl_entry<f128, dense<128> : vector<2xi32>>, #dlti.dl_entry<f64, dense<64> : vector<2xi32>>, #dlti.dl_entry<!llvm.ptr<272>, dense<64> : vector<4xi32>>, #dlti.dl_entry<!llvm.ptr<271>, dense<32> : vector<4xi32>>, #dlti.dl_entry<i64, dense<64> : vector<2xi32>>, #dlti.dl_entry<f80, dense<128> : vector<2xi32>>, #dlti.dl_entry<"dlti.stack_alignment", 128 : i32>, #dlti.dl_entry<"dlti.endianness", "little">>} {
  llvm.mlir.global private unnamed_addr constant @".str"("Enter the value for a: \00") {addr_space = 0 : i32, alignment = 1 : i64, dso_local}
  llvm.mlir.global private unnamed_addr constant @".str.1"("%d\00") {addr_space = 0 : i32, alignment = 1 : i64, dso_local}
  llvm.mlir.global private unnamed_addr constant @".str.2"("Enter the value for b: \00") {addr_space = 0 : i32, alignment = 1 : i64, dso_local}
  llvm.mlir.global private unnamed_addr constant @".str.3"("Enter the value for c: \00") {addr_space = 0 : i32, alignment = 1 : i64, dso_local}
  llvm.mlir.global private unnamed_addr constant @".str.4"("Output: %d\0A\00") {addr_space = 0 : i32, alignment = 1 : i64, dso_local}
  llvm.func @main() -> i32 attributes {passthrough = ["noinline", "nounwind", ["uwtable", "2"], ["frame-pointer", "all"], ["min-legal-vector-width", "0"], ["no-trapping-math", "true"], ["stack-protector-buffer-size", "8"], ["target-cpu", "x86-64"], ["target-features", "+cx8,+fxsr,+mmx,+sse,+sse2,+x87"], ["tune-cpu", "generic"]]} {
    %0 = llvm.mlir.constant(1 : i32) : i32
    %1 = llvm.mlir.constant(0 : i32) : i32
    %2 = llvm.mlir.constant("Enter the value for a: \00") : !llvm.array<24 x i8>
    %3 = llvm.mlir.addressof @".str" : !llvm.ptr
    %4 = llvm.mlir.constant("%d\00") : !llvm.array<3 x i8>
    %5 = llvm.mlir.addressof @".str.1" : !llvm.ptr
    %6 = llvm.mlir.constant("Enter the value for b: \00") : !llvm.array<24 x i8>
    %7 = llvm.mlir.addressof @".str.2" : !llvm.ptr
    %8 = llvm.mlir.constant("Enter the value for c: \00") : !llvm.array<24 x i8>
    %9 = llvm.mlir.addressof @".str.3" : !llvm.ptr
    %10 = llvm.mlir.constant("Output: %d\0A\00") : !llvm.array<12 x i8>
    %11 = llvm.mlir.addressof @".str.4" : !llvm.ptr
    %12 = llvm.alloca %0 x i32 {alignment = 4 : i64} : (i32) -> !llvm.ptr
    %13 = llvm.alloca %0 x i32 {alignment = 4 : i64} : (i32) -> !llvm.ptr
    %14 = llvm.alloca %0 x i32 {alignment = 4 : i64} : (i32) -> !llvm.ptr
    %15 = llvm.alloca %0 x i32 {alignment = 4 : i64} : (i32) -> !llvm.ptr
    %16 = llvm.alloca %0 x i32 {alignment = 4 : i64} : (i32) -> !llvm.ptr
    %17 = llvm.alloca %0 x i32 {alignment = 4 : i64} : (i32) -> !llvm.ptr
    %18 = llvm.alloca %0 x i32 {alignment = 4 : i64} : (i32) -> !llvm.ptr
    llvm.store %1, %12 {alignment = 4 : i64} : i32, !llvm.ptr
    llvm.store %1, %16 {alignment = 4 : i64} : i32, !llvm.ptr
    %19 = llvm.call @printf(%3) : (!llvm.ptr) -> i32
    %20 = llvm.call @__isoc99_scanf(%5, %13) : (!llvm.ptr, !llvm.ptr) -> i32
    %21 = llvm.call @printf(%7) : (!llvm.ptr) -> i32
    %22 = llvm.call @__isoc99_scanf(%5, %14) : (!llvm.ptr, !llvm.ptr) -> i32
    %23 = llvm.call @printf(%9) : (!llvm.ptr) -> i32
    %24 = llvm.call @__isoc99_scanf(%5, %15) : (!llvm.ptr, !llvm.ptr) -> i32
    llvm.store %1, %17 {alignment = 4 : i64} : i32, !llvm.ptr
    llvm.br ^bb1
  ^bb1:  // 2 preds: ^bb0, ^bb7
    %25 = llvm.load %17 {alignment = 4 : i64} : !llvm.ptr -> i32
    %26 = llvm.load %13 {alignment = 4 : i64} : !llvm.ptr -> i32
    %27 = llvm.icmp "slt" %25, %26 : i32
    llvm.cond_br %27, ^bb2, ^bb8
  ^bb2:  // pred: ^bb1
    %28 = llvm.load %15 {alignment = 4 : i64} : !llvm.ptr -> i32
    %29 = llvm.load %16 {alignment = 4 : i64} : !llvm.ptr -> i32
    %30 = llvm.add %29, %28  : i32
    llvm.store %30, %16 {alignment = 4 : i64} : i32, !llvm.ptr
    llvm.store %1, %18 {alignment = 4 : i64} : i32, !llvm.ptr
    llvm.br ^bb3
  ^bb3:  // 2 preds: ^bb2, ^bb5
    %31 = llvm.load %18 {alignment = 4 : i64} : !llvm.ptr -> i32
    %32 = llvm.load %14 {alignment = 4 : i64} : !llvm.ptr -> i32
    %33 = llvm.icmp "slt" %31, %32 : i32
    llvm.cond_br %33, ^bb4, ^bb6
  ^bb4:  // pred: ^bb3
    %34 = llvm.load %16 {alignment = 4 : i64} : !llvm.ptr -> i32
    %35 = llvm.add %34, %0  : i32
    llvm.store %35, %16 {alignment = 4 : i64} : i32, !llvm.ptr
    %36 = llvm.load %15 {alignment = 4 : i64} : !llvm.ptr -> i32
    %37 = llvm.load %16 {alignment = 4 : i64} : !llvm.ptr -> i32
    %38 = llvm.mul %37, %36  : i32
    llvm.store %38, %16 {alignment = 4 : i64} : i32, !llvm.ptr
    llvm.br ^bb5
  ^bb5:  // pred: ^bb4
    %39 = llvm.load %18 {alignment = 4 : i64} : !llvm.ptr -> i32
    %40 = llvm.add %39, %0  : i32
    llvm.store %40, %18 {alignment = 4 : i64} : i32, !llvm.ptr
    llvm.br ^bb3 {loop_annotation = #loop_annotation}
  ^bb6:  // pred: ^bb3
    llvm.br ^bb7
  ^bb7:  // pred: ^bb6
    %41 = llvm.load %17 {alignment = 4 : i64} : !llvm.ptr -> i32
    %42 = llvm.add %41, %0  : i32
    llvm.store %42, %17 {alignment = 4 : i64} : i32, !llvm.ptr
    llvm.br ^bb1 {loop_annotation = #loop_annotation}
  ^bb8:  // pred: ^bb1
    %43 = llvm.load %16 {alignment = 4 : i64} : !llvm.ptr -> i32
    %44 = llvm.call @printf(%11, %43) : (!llvm.ptr, i32) -> i32
    llvm.return %1 : i32
  }
  llvm.func @printf(!llvm.ptr {llvm.noundef}, ...) -> i32 attributes {passthrough = [["frame-pointer", "all"], ["no-trapping-math", "true"], ["stack-protector-buffer-size", "8"], ["target-cpu", "x86-64"], ["target-features", "+cx8,+fxsr,+mmx,+sse,+sse2,+x87"], ["tune-cpu", "generic"]]}
  llvm.func @__isoc99_scanf(!llvm.ptr {llvm.noundef}, ...) -> i32 attributes {passthrough = [["frame-pointer", "all"], ["no-trapping-math", "true"], ["stack-protector-buffer-size", "8"], ["target-cpu", "x86-64"], ["target-features", "+cx8,+fxsr,+mmx,+sse,+sse2,+x87"], ["tune-cpu", "generic"]]}
}
