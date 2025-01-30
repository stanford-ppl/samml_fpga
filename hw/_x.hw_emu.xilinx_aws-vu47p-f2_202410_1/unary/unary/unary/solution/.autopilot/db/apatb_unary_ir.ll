; ModuleID = '/home/ubuntu/samml_fpga/hw/_x.hw_emu.xilinx_aws-vu47p-f2_202410_1/unary/unary/unary/solution/.autopilot/db/a.g.ld.5.gdce.bc'
source_filename = "llvm-link"
target datalayout = "e-m:e-i64:64-i128:128-i256:256-i512:512-i1024:1024-i2048:2048-i4096:4096-n8:16:32:64-S128-v16:16-v24:32-v32:32-v48:64-v96:128-v192:256-v256:256-v512:512-v1024:1024"
target triple = "fpga64-xilinx-none"

%"class.hls::stream<ap_int<17>, 0>" = type { %"struct.ap_int<17>" }
%"struct.ap_int<17>" = type { %"struct.ap_int_base<17, true>" }
%"struct.ap_int_base<17, true>" = type { %"struct.ssdm_int<17, true>" }
%"struct.ssdm_int<17, true>" = type { i17 }

; Function Attrs: inaccessiblememonly nounwind willreturn
declare void @llvm.sideeffect() #0

; Function Attrs: inaccessiblemem_or_argmemonly noinline willreturn
define void @apatb_unary_ir(%"class.hls::stream<ap_int<17>, 0>"* noalias nocapture nonnull dereferenceable(4) %in_val, %"class.hls::stream<ap_int<17>, 0>"* noalias nocapture nonnull dereferenceable(4) %out_val) local_unnamed_addr #1 {
entry:
  %in_val_copy = alloca i17, align 512
  call void @llvm.sideeffect() #7 [ "stream_interface"(i17* %in_val_copy, i32 0) ]
  %out_val_copy = alloca i17, align 512
  call void @llvm.sideeffect() #7 [ "stream_interface"(i17* %out_val_copy, i32 0) ]
  call fastcc void @copy_in(%"class.hls::stream<ap_int<17>, 0>"* nonnull %in_val, i17* nonnull align 512 %in_val_copy, %"class.hls::stream<ap_int<17>, 0>"* nonnull %out_val, i17* nonnull align 512 %out_val_copy)
  call void @apatb_unary_hw(i17* %in_val_copy, i17* %out_val_copy)
  call void @copy_back(%"class.hls::stream<ap_int<17>, 0>"* %in_val, i17* %in_val_copy, %"class.hls::stream<ap_int<17>, 0>"* %out_val, i17* %out_val_copy)
  ret void
}

; Function Attrs: argmemonly noinline willreturn
define internal fastcc void @copy_in(%"class.hls::stream<ap_int<17>, 0>"* noalias "unpacked"="0", i17* noalias nocapture align 512 "unpacked"="1.0", %"class.hls::stream<ap_int<17>, 0>"* noalias "unpacked"="2", i17* noalias nocapture align 512 "unpacked"="3.0") unnamed_addr #2 {
entry:
  call fastcc void @"onebyonecpy_hls.p0class.hls::stream<ap_int<17>, 0>.13"(i17* align 512 %1, %"class.hls::stream<ap_int<17>, 0>"* %0)
  call fastcc void @"onebyonecpy_hls.p0class.hls::stream<ap_int<17>, 0>.13"(i17* align 512 %3, %"class.hls::stream<ap_int<17>, 0>"* %2)
  ret void
}

; Function Attrs: argmemonly noinline willreturn
define internal fastcc void @copy_out(%"class.hls::stream<ap_int<17>, 0>"* noalias "unpacked"="0", i17* noalias nocapture align 512 "unpacked"="1.0", %"class.hls::stream<ap_int<17>, 0>"* noalias "unpacked"="2", i17* noalias nocapture align 512 "unpacked"="3.0") unnamed_addr #3 {
entry:
  call fastcc void @"onebyonecpy_hls.p0class.hls::stream<ap_int<17>, 0>"(%"class.hls::stream<ap_int<17>, 0>"* %0, i17* align 512 %1)
  call fastcc void @"onebyonecpy_hls.p0class.hls::stream<ap_int<17>, 0>"(%"class.hls::stream<ap_int<17>, 0>"* %2, i17* align 512 %3)
  ret void
}

; Function Attrs: argmemonly noinline willreturn
define internal fastcc void @"onebyonecpy_hls.p0class.hls::stream<ap_int<17>, 0>"(%"class.hls::stream<ap_int<17>, 0>"* noalias "unpacked"="0" %dst, i17* noalias nocapture align 512 "unpacked"="1.0" %src) unnamed_addr #4 {
entry:
  %0 = icmp eq %"class.hls::stream<ap_int<17>, 0>"* %dst, null
  br i1 %0, label %ret, label %copy

copy:                                             ; preds = %entry
  call fastcc void @"streamcpy_hls.p0class.hls::stream<ap_int<17>, 0>.8"(%"class.hls::stream<ap_int<17>, 0>"* nonnull %dst, i17* align 512 %src)
  br label %ret

ret:                                              ; preds = %copy, %entry
  ret void
}

; Function Attrs: argmemonly noinline willreturn
define internal fastcc void @"streamcpy_hls.p0class.hls::stream<ap_int<17>, 0>.8"(%"class.hls::stream<ap_int<17>, 0>"* noalias nocapture "unpacked"="0", i17* noalias nocapture align 512 "unpacked"="1.0") unnamed_addr #5 {
entry:
  %2 = alloca i17
  %3 = alloca %"class.hls::stream<ap_int<17>, 0>"
  br label %empty

empty:                                            ; preds = %push, %entry
  %4 = bitcast i17* %1 to i8*
  %5 = call i1 @fpga_fifo_not_empty_4(i8* %4)
  br i1 %5, label %push, label %ret

push:                                             ; preds = %empty
  %6 = bitcast i17* %2 to i8*
  %7 = bitcast i17* %1 to i8*
  call void @fpga_fifo_pop_4(i8* %6, i8* %7)
  %8 = bitcast i17* %2 to i24*
  %9 = load i24, i24* %8
  %10 = trunc i24 %9 to i17
  %.ivi = insertvalue %"class.hls::stream<ap_int<17>, 0>" undef, i17 %10, 0, 0, 0, 0
  store %"class.hls::stream<ap_int<17>, 0>" %.ivi, %"class.hls::stream<ap_int<17>, 0>"* %3
  %11 = bitcast %"class.hls::stream<ap_int<17>, 0>"* %3 to i8*
  %12 = bitcast %"class.hls::stream<ap_int<17>, 0>"* %0 to i8*
  call void @fpga_fifo_push_4(i8* %11, i8* %12)
  br label %empty, !llvm.loop !5

ret:                                              ; preds = %empty
  ret void
}

; Function Attrs: argmemonly noinline willreturn
define internal fastcc void @"onebyonecpy_hls.p0class.hls::stream<ap_int<17>, 0>.13"(i17* noalias nocapture align 512 "unpacked"="0.0" %dst, %"class.hls::stream<ap_int<17>, 0>"* noalias "unpacked"="1" %src) unnamed_addr #4 {
entry:
  %0 = icmp eq %"class.hls::stream<ap_int<17>, 0>"* %src, null
  br i1 %0, label %ret, label %copy

copy:                                             ; preds = %entry
  call fastcc void @"streamcpy_hls.p0class.hls::stream<ap_int<17>, 0>.16"(i17* align 512 %dst, %"class.hls::stream<ap_int<17>, 0>"* nonnull %src)
  br label %ret

ret:                                              ; preds = %copy, %entry
  ret void
}

; Function Attrs: argmemonly noinline willreturn
define internal fastcc void @"streamcpy_hls.p0class.hls::stream<ap_int<17>, 0>.16"(i17* noalias nocapture align 512 "unpacked"="0.0", %"class.hls::stream<ap_int<17>, 0>"* noalias nocapture "unpacked"="1") unnamed_addr #5 {
entry:
  %2 = alloca %"class.hls::stream<ap_int<17>, 0>"
  %3 = alloca i17
  br label %empty

empty:                                            ; preds = %push, %entry
  %4 = bitcast %"class.hls::stream<ap_int<17>, 0>"* %1 to i8*
  %5 = call i1 @fpga_fifo_not_empty_4(i8* %4)
  br i1 %5, label %push, label %ret

push:                                             ; preds = %empty
  %6 = bitcast %"class.hls::stream<ap_int<17>, 0>"* %2 to i8*
  %7 = bitcast %"class.hls::stream<ap_int<17>, 0>"* %1 to i8*
  call void @fpga_fifo_pop_4(i8* %6, i8* %7)
  %8 = load volatile %"class.hls::stream<ap_int<17>, 0>", %"class.hls::stream<ap_int<17>, 0>"* %2
  %.evi = extractvalue %"class.hls::stream<ap_int<17>, 0>" %8, 0, 0, 0, 0
  store i17 %.evi, i17* %3
  %9 = bitcast i17* %3 to i8*
  %10 = bitcast i17* %0 to i8*
  call void @fpga_fifo_push_4(i8* %9, i8* %10)
  br label %empty, !llvm.loop !7

ret:                                              ; preds = %empty
  ret void
}

declare void @apatb_unary_hw(i17*, i17*)

; Function Attrs: argmemonly noinline willreturn
define internal fastcc void @copy_back(%"class.hls::stream<ap_int<17>, 0>"* noalias "unpacked"="0", i17* noalias nocapture align 512 "unpacked"="1.0", %"class.hls::stream<ap_int<17>, 0>"* noalias "unpacked"="2", i17* noalias nocapture align 512 "unpacked"="3.0") unnamed_addr #3 {
entry:
  call fastcc void @"onebyonecpy_hls.p0class.hls::stream<ap_int<17>, 0>"(%"class.hls::stream<ap_int<17>, 0>"* %0, i17* align 512 %1)
  call fastcc void @"onebyonecpy_hls.p0class.hls::stream<ap_int<17>, 0>"(%"class.hls::stream<ap_int<17>, 0>"* %2, i17* align 512 %3)
  ret void
}

define void @unary_hw_stub_wrapper(i17*, i17*) #6 {
entry:
  %2 = alloca %"class.hls::stream<ap_int<17>, 0>"
  %3 = alloca %"class.hls::stream<ap_int<17>, 0>"
  call void @copy_out(%"class.hls::stream<ap_int<17>, 0>"* %2, i17* %0, %"class.hls::stream<ap_int<17>, 0>"* %3, i17* %1)
  call void @unary_hw_stub(%"class.hls::stream<ap_int<17>, 0>"* %2, %"class.hls::stream<ap_int<17>, 0>"* %3)
  call void @copy_in(%"class.hls::stream<ap_int<17>, 0>"* %2, i17* %0, %"class.hls::stream<ap_int<17>, 0>"* %3, i17* %1)
  ret void
}

declare void @unary_hw_stub(%"class.hls::stream<ap_int<17>, 0>"* noalias nocapture nonnull, %"class.hls::stream<ap_int<17>, 0>"* noalias nocapture nonnull)

declare i1 @fpga_fifo_not_empty_4(i8*)

declare void @fpga_fifo_pop_4(i8*, i8*)

declare void @fpga_fifo_push_4(i8*, i8*)

attributes #0 = { inaccessiblememonly nounwind willreturn }
attributes #1 = { inaccessiblemem_or_argmemonly noinline willreturn "fpga.wrapper.func"="wrapper" }
attributes #2 = { argmemonly noinline willreturn "fpga.wrapper.func"="copyin" }
attributes #3 = { argmemonly noinline willreturn "fpga.wrapper.func"="copyout" }
attributes #4 = { argmemonly noinline willreturn "fpga.wrapper.func"="onebyonecpy_hls" }
attributes #5 = { argmemonly noinline willreturn "fpga.wrapper.func"="streamcpy_hls" }
attributes #6 = { "fpga.wrapper.func"="stub" }
attributes #7 = { inaccessiblememonly nounwind willreturn "xlx.port.bitwidth"="32" "xlx.source"="user" }

!llvm.dbg.cu = !{}
!llvm.ident = !{!0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0}
!llvm.module.flags = !{!1, !2, !3}
!blackbox_cfg = !{!4}

!0 = !{!"clang version 7.0.0 "}
!1 = !{i32 2, !"Dwarf Version", i32 4}
!2 = !{i32 2, !"Debug Info Version", i32 3}
!3 = !{i32 1, !"wchar_size", i32 4}
!4 = !{}
!5 = distinct !{!5, !6}
!6 = !{!"llvm.loop.rotate.disable"}
!7 = distinct !{!7, !6}
