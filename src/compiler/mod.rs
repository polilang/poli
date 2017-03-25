extern crate llvm_sys;
extern crate libc;

use self::llvm_sys::core::*;
use self::llvm_sys::prelude::*;
use self::llvm_sys::target::*;
use self::llvm_sys::target_machine::*;
use self::llvm_sys::target_machine::LLVMCodeGenOptLevel::*;
use self::llvm_sys::target_machine::LLVMRelocMode::*;
use self::llvm_sys::target_machine::LLVMCodeModel::*;

use std::{io, ptr};
use std::ffi::{CStr, CString};
use std::io::Write;

macro_rules! c_str {
    ($string:expr) => (
        concat!($string, '\0').as_ptr() as *const self::libc::c_char
    )
}

pub struct Module {
    context: LLVMContextRef,
    module:  LLVMModuleRef,
}

impl Module {
    pub fn new<T: Into<Vec<u8>>>(name: T) -> Self {
        unsafe {
            let context = LLVMContextCreate();
            let name    = CString::new(name).unwrap();
            let module  = LLVMModuleCreateWithNameInContext(name.as_ptr(), context);

            let m = Module {
                context: context,
                module:  module,
            };

            let i32_type = LLVMInt32TypeInContext(m.context);
            let string_type = LLVMPointerType(LLVMInt8TypeInContext(m.context), 0);
            let mut arg_types = [string_type];

            let puts_type = LLVMFunctionType(
                i32_type, arg_types.as_mut_ptr(),
                arg_types.len() as libc::c_uint, 0
            );

            LLVMAddFunction(m.module, c_str!("print"), puts_type);

            m
        }
    }

    pub fn dump(&self) {
        unsafe {
            LLVMDumpModule(self.module)
        }
    }

    pub fn write_object(&self, path: &str) {
        unsafe {
            assert!(LLVM_InitializeNativeTarget()     == 0);
            assert!(LLVM_InitializeNativeAsmPrinter() == 0);

            let triple_target = LLVMGetDefaultTargetTriple();
            let mut target    = ptr::null_mut();

            assert!(LLVMGetTargetFromTriple(triple_target, &mut target, ptr::null_mut()) == 0);
            
            let target_machine = LLVMCreateTargetMachine(
                target, triple_target, c_str!(""), c_str!(""),

                LLVMCodeGenLevelNone,
                LLVMRelocDefault,
                LLVMCodeModelDefault,
            );

            let path_c    = CString::new(path).unwrap();
            let mut error = ptr::null_mut();

            let result = LLVMTargetMachineEmitToFile(
                target_machine, self.module, path_c.as_ptr() as *mut libc::c_char,
                LLVMCodeGenFileType::LLVMObjectFile,
                &mut error,
            );

            if result != 0 {
                println!("failed to write object file: {}", path);

                if !error.is_null() {
                    print!("LLVM error:");

                    io::stdout().write(CStr::from_ptr(error).to_bytes()).unwrap();

                    println!("");

                    LLVMDisposeMessage(error)
                }
            } else {
                LLVMDisposeTargetMachine(target_machine)
            }
        }
    }
}

impl Drop for Module {
    fn drop(&mut self) {
        unsafe {
            LLVMDisposeModule(self.module);
            LLVMContextDispose(self.context)
        }
    }
}