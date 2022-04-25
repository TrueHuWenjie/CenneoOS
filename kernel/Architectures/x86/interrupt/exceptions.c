// Cenneo OS
// /kernel/arch/x86/interrupt/exceptions.c
// Exceptions handle on x86 Architecture

#include <kvi.h>


// Divide Error
void excp_de_handle(int error_code)
{
	error("Divide Error.\n");
}

// Debug
void excp_db_handle(int error_code)
{
    error("Debug.\n");
}

// Breakpoint
void excp_bp_handle(int error_code)
{
    error("Breakpoint.\n");
}

// Overflow
void excp_of_handle(int error_code)
{
    error("Overflow.\n");
}

// BOUND Range Exceeded
void excp_br_handle(int error_code)
{
    error("BOUND Range Exceeded.\n");
}

// Invalid Opcode (Undefined Opcode)
void excp_ud_handle(int error_code)
{
	error("Invalid Opcode (Undefined Opcode).\n");
}

// Device Not Available (No Math Coprocessor)
void excp_nm_handle(int error_code)
{
    error("Device Not Available (No Math Coprocessor).\n");
}

// Double Fault
void excp_df_handle(int error_code)
{
	error("Double Fault.\n");
}

// Invalid TSS
void excp_ts_handle(int error_code)
{
	error("Invalid TSS.\n");
}

// Segment Not Present
void excp_np_handle(int error_code)
{
    error("Segment Not Present.\n");
}

// Stack-Segment Fault
void excp_ss_handle(int error_code)
{
    error("Stack-Segment Fault.\n");
}

// General Protection
void excp_gp_handle(int error_code)
{
	error("General Protection.\n");
}

// x87 FPU Floating-Point Error (Math Fault)
void excp_mf_handle(int error_code)
{
    error("x87 FPU Floating-Point Error (Math Fault).\n");
}

// Alignment Check
void excp_ac_handle(int error_code)
{
    error("Alignment Check.\n");
}

// Machine Check
void excp_mc_handle(int error_code)
{
    error("Machine Check.\n");
}

// SIMD Floating-Point Exception
void excp_xf_handle(int error_code)
{
    error("SIMD Floating-Point Exception.\n");
}
