#include <kernel/runtime/runtime_isa.h>
#include <kernel/core/kernel.h>

void VMINSTR_METHOD_NOP(runtime_t* runtime) { }

void VMINSTR_METHOD_HLT(runtime_t* runtime) { runtime_halt(runtime); }

void VMINSTR_METHOD_EXIT(runtime_t* runtime) { runtime_exit(runtime); }

void VMINSTR_METHOD_TRAP(runtime_t* runtime)
{

}

void VMINSTR_METHOD_ADD(runtime_t* runtime)
{
    VMREG reg = (VMREG)runtime_read8(runtime, runtime->registers[VMREG_EIP] + 1);
    uint32_t val = runtime_read32(runtime, runtime->registers[VMREG_EIP] + 2);
    runtime_perform_op_imm(runtime, reg, val, VMOP_ADD);
}

void VMINSTR_METHOD_ADDR(runtime_t* runtime)
{
    VMREG rx = (VMREG)runtime_read8(runtime, runtime->registers[VMREG_EIP] + 1);
    VMREG ry = (VMREG)runtime_read8(runtime, runtime->registers[VMREG_EIP] + 2);
    runtime_perform_op_reg(runtime, rx, ry, VMOP_ADD);
}

void VMINSTR_METHOD_SUB(runtime_t* runtime)
{
    VMREG reg = (VMREG)runtime_read8(runtime, runtime->registers[VMREG_EIP] + 1);
    uint32_t val = runtime_read32(runtime, runtime->registers[VMREG_EIP] + 2);
    runtime_perform_op_imm(runtime, reg, val, VMOP_SUBTRACT);
}

void VMINSTR_METHOD_SUBR(runtime_t* runtime)
{
    VMREG rx = (VMREG)runtime_read8(runtime, runtime->registers[VMREG_EIP] + 1);
    VMREG ry = (VMREG)runtime_read8(runtime, runtime->registers[VMREG_EIP] + 2);
    runtime_perform_op_reg(runtime, rx, ry, VMOP_SUBTRACT);
}

void VMINSTR_METHOD_MUL(runtime_t* runtime)
{
    VMREG reg = (VMREG)runtime_read8(runtime, runtime->registers[VMREG_EIP] + 1);
    uint32_t val = runtime_read32(runtime, runtime->registers[VMREG_EIP] + 2);
    runtime_perform_op_imm(runtime, reg, val, VMOP_MULTIPLY);
}

void VMINSTR_METHOD_MULR(runtime_t* runtime)
{
    VMREG rx = (VMREG)runtime_read8(runtime, runtime->registers[VMREG_EIP] + 1);
    VMREG ry = (VMREG)runtime_read8(runtime, runtime->registers[VMREG_EIP] + 2);
    runtime_perform_op_reg(runtime, rx, ry, VMOP_MULTIPLY);
}

void VMINSTR_METHOD_DIV(runtime_t* runtime)
{
    VMREG reg = (VMREG)runtime_read8(runtime, runtime->registers[VMREG_EIP] + 1);
    uint32_t val = runtime_read32(runtime, runtime->registers[VMREG_EIP] + 2);
    runtime_perform_op_imm(runtime, reg, val, VMOP_DIVIDE);
}

void VMINSTR_METHOD_DIVR(runtime_t* runtime)
{
    VMREG rx = (VMREG)runtime_read8(runtime, runtime->registers[VMREG_EIP] + 1);
    VMREG ry = (VMREG)runtime_read8(runtime, runtime->registers[VMREG_EIP] + 2);
    runtime_perform_op_reg(runtime, rx, ry, VMOP_DIVIDE);
}

void VMINSTR_METHOD_OR(runtime_t* runtime)
{
    VMREG reg = (VMREG)runtime_read8(runtime, runtime->registers[VMREG_EIP] + 1);
    uint32_t val = runtime_read32(runtime, runtime->registers[VMREG_EIP] + 2);
    runtime_perform_op_imm(runtime, reg, val, VMOP_OR);
}

void VMINSTR_METHOD_ORR(runtime_t* runtime)
{
    VMREG rx = (VMREG)runtime_read8(runtime, runtime->registers[VMREG_EIP] + 1);
    VMREG ry = (VMREG)runtime_read8(runtime, runtime->registers[VMREG_EIP] + 2);
    runtime_perform_op_reg(runtime, rx, ry, VMOP_OR);
}

void VMINSTR_METHOD_XOR(runtime_t* runtime)
{
    VMREG reg = (VMREG)runtime_read8(runtime, runtime->registers[VMREG_EIP] + 1);
    uint32_t val = runtime_read32(runtime, runtime->registers[VMREG_EIP] + 2);
    runtime_perform_op_imm(runtime, reg, val, VMOP_XOR);
}

void VMINSTR_METHOD_XORR(runtime_t* runtime)
{   
    VMREG rx = (VMREG)runtime_read8(runtime, runtime->registers[VMREG_EIP] + 1);
    VMREG ry = (VMREG)runtime_read8(runtime, runtime->registers[VMREG_EIP] + 2);
    runtime_perform_op_reg(runtime, rx, ry, VMOP_XOR);
}

void VMINSTR_METHOD_AND(runtime_t* runtime)
{
    VMREG reg = (VMREG)runtime_read8(runtime, runtime->registers[VMREG_EIP] + 1);
    uint32_t val = runtime_read32(runtime, runtime->registers[VMREG_EIP] + 2);
    runtime_perform_op_imm(runtime, reg, val, VMOP_AND);
}

void VMINSTR_METHOD_ANDR(runtime_t* runtime)
{
    VMREG rx = (VMREG)runtime_read8(runtime, runtime->registers[VMREG_EIP] + 1);
    VMREG ry = (VMREG)runtime_read8(runtime, runtime->registers[VMREG_EIP] + 2);
    runtime_perform_op_reg(runtime, rx, ry, VMOP_AND);
}

void VMINSTR_METHOD_NOT(runtime_t* runtime)
{
    VMREG reg = (VMREG)runtime_read8(runtime, runtime->registers[VMREG_EIP] + 1);
    runtime_perform_op_imm(runtime, reg, 0, VMOP_NOT);
}

void VMINSTR_METHOD_IN(runtime_t* runtime)
{

}

void VMINSTR_METHOD_INR(runtime_t* runtime)
{

}

void VMINSTR_METHOD_OUT(runtime_t* runtime)
{
    uint16_t port = runtime_read16(runtime, runtime->registers[VMREG_EIP] + 1);
    uint32_t data = runtime_read32(runtime, runtime->registers[VMREG_EIP] + 3);
    //runtime_writeport(runtime, port, data);
}

void VMINSTR_METHOD_OUTR(runtime_t* runtime)
{

}

void VMINSTR_METHOD_OUTRR(runtime_t* runtime)
{

}

void VMINSTR_METHOD_JMP(runtime_t* runtime)
{
    uint32_t addr = runtime_read32(runtime, runtime->registers[VMREG_EIP] + 1);
    runtime->registers[VMREG_EIP] = addr;
}

void VMINSTR_METHOD_JMPR(runtime_t* runtime)
{
    uint8_t reg = runtime_read8(runtime, runtime->registers[VMREG_EIP] + 1);
    runtime->registers[VMREG_EIP] = runtime->registers[reg];
}

void VMINSTR_METHOD_CALL(runtime_t* runtime)
{
    uint32_t addr = runtime_read32(runtime, runtime->registers[VMREG_EIP] + 1);
    runtime_stackpush(runtime, runtime->registers[VMREG_EIP] + VMINSTR_CALL.cycles);
    runtime->registers[VMREG_EIP] = addr;
}

void VMINSTR_METHOD_CALLR(runtime_t* runtime)
{
    uint8_t reg = runtime_read8(runtime, runtime->registers[VMREG_EIP] + 1);
    runtime_stackpush(runtime, runtime->registers[VMREG_EIP] + VMINSTR_CALLR.cycles);
    runtime->registers[VMREG_EIP] = runtime->registers[reg];
}

void VMINSTR_METHOD_RET(runtime_t* runtime)
{
    runtime->registers[VMREG_EIP] = runtime_stackpop(runtime);
}
