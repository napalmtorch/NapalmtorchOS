#pragma once
#include <lib/types.h>
#include <kernel/runtime/runtime_exec.h>

typedef void (*runtime_instr_method_t)(runtime_t* vm);

typedef struct
{
    uint8_t           opcode;
    uint8_t           cycles;
    bool_t            increment;
    const char*       args;
    const char*       name;
    runtime_instr_method_t m_execute;
} runtime_instr_t;

void VMINSTR_METHOD_NOP(runtime_t* runtime);
void VMINSTR_METHOD_HLT(runtime_t* runtime);
void VMINSTR_METHOD_EXIT(runtime_t* runtime);
void VMINSTR_METHOD_TRAP(runtime_t* runtime);

void VMINSTR_METHOD_ADD(runtime_t* runtime);
void VMINSTR_METHOD_ADDR(runtime_t* runtime);
void VMINSTR_METHOD_SUB(runtime_t* vm);
void VMINSTR_METHOD_SUBR(runtime_t* vm);
void VMINSTR_METHOD_MUL(runtime_t* vm);
void VMINSTR_METHOD_MULR(runtime_t* vm);
void VMINSTR_METHOD_DIV(runtime_t* vm);
void VMINSTR_METHOD_DIVR(runtime_t* vm);

void VMINSTR_METHOD_OR(runtime_t* vm);
void VMINSTR_METHOD_ORR(runtime_t* vm);
void VMINSTR_METHOD_XOR(runtime_t* vm);
void VMINSTR_METHOD_XORR(runtime_t* vm);
void VMINSTR_METHOD_AND(runtime_t* vm);
void VMINSTR_METHOD_ANDR(runtime_t* vm);
void VMINSTR_METHOD_NOT(runtime_t* vm);

void VMINSTR_METHOD_JMP(runtime_t* vm);
void VMINSTR_METHOD_JMPR(runtime_t* vm);
void VMINSTR_METHOD_CALL(runtime_t* vm);
void VMINSTR_METHOD_CALLR(runtime_t* vm);
void VMINSTR_METHOD_RET(runtime_t* vm);

void VMINSTR_METHOD_IN(runtime_t* vm);
void VMINSTR_METHOD_INR(runtime_t* vm);
void VMINSTR_METHOD_OUT(runtime_t* vm);
void VMINSTR_METHOD_OUTR(runtime_t* vm);
void VMINSTR_METHOD_OUTRR(runtime_t* vm);

static const runtime_instr_t VMINSTR_NOP            = { 0x00, 1, TRUE,  "",  "NOP", VMINSTR_METHOD_NOP };
static const runtime_instr_t VMINSTR_HLT            = { 0xFF, 1, TRUE,  "",  "HLT", VMINSTR_METHOD_HLT };
static const runtime_instr_t VMINSTR_EXIT           = { 0xFE, 1, FALSE, "",  "EXIT", VMINSTR_METHOD_EXIT };
static const runtime_instr_t VMINSTR_TRAP           = { 0xF0, 3, TRUE,  "s", "TRAP", VMINSTR_METHOD_TRAP };

static const runtime_instr_t VMINSTR_ADD            = { 0x01, 6, TRUE, "ri", "ADD",  VMINSTR_METHOD_ADD };
static const runtime_instr_t VMINSTR_ADDR           = { 0x02, 3, TRUE, "rr", "ADDR", VMINSTR_METHOD_ADDR };
static const runtime_instr_t VMINSTR_SUB            = { 0x03, 0x06, TRUE, "ri", "SUB",  VMINSTR_METHOD_SUB };
static const runtime_instr_t VMINSTR_SUBR           = { 0x04, 0x03, TRUE, "rr", "SUBR", VMINSTR_METHOD_SUBR };
static const runtime_instr_t VMINSTR_MUL            = { 0x05, 0x06, TRUE, "ri", "MUL",  VMINSTR_METHOD_MUL };
static const runtime_instr_t VMINSTR_MULR           = { 0x06, 0x03, TRUE, "rr", "MULR", VMINSTR_METHOD_MULR };
static const runtime_instr_t VMINSTR_DIV            = { 0x07, 0x06, TRUE, "ri", "DIV",  VMINSTR_METHOD_DIV };
static const runtime_instr_t VMINSTR_DIVR           = { 0x08, 0x03, TRUE, "rr", "DIVR", VMINSTR_METHOD_DIVR };

static const runtime_instr_t VMINSTR_OR             = { 0x09, 0x06, TRUE, "ri", "OR",   VMINSTR_METHOD_OR };
static const runtime_instr_t VMINSTR_ORR            = { 0x0A, 0x03, TRUE, "rr", "ORR",  VMINSTR_METHOD_ORR };
static const runtime_instr_t VMINSTR_XOR            = { 0x0B, 0x06, TRUE, "ri", "XOR",  VMINSTR_METHOD_XOR };
static const runtime_instr_t VMINSTR_XORR           = { 0x0C, 0x03, TRUE, "rr", "XORR", VMINSTR_METHOD_XORR };
static const runtime_instr_t VMINSTR_AND            = { 0x0D, 0x06, TRUE, "ri", "AND",  VMINSTR_METHOD_AND };
static const runtime_instr_t VMINSTR_ANDR           = { 0x0E, 0x03, TRUE, "rr", "ANDR", VMINSTR_METHOD_ANDR };
static const runtime_instr_t VMINSTR_NOT            = { 0x0F, 0x02, TRUE, "r",  "NOT",  VMINSTR_METHOD_NOT };

static const runtime_instr_t VMINSTR_JMP            = { 0x10, 0x05, FALSE, "i", "JMP",   VMINSTR_METHOD_JMP };
static const runtime_instr_t VMINSTR_JMPR           = { 0x11, 0x02, FALSE, "r", "JMPR",  VMINSTR_METHOD_JMPR };
static const runtime_instr_t VMINSTR_CALL           = { 0x12, 0x05, FALSE, "i", "CALL",  VMINSTR_METHOD_CALL };
static const runtime_instr_t VMINSTR_CALLR          = { 0x13, 0x02, FALSE, "r", "CALLR", VMINSTR_METHOD_CALLR };
static const runtime_instr_t VMINSTR_RET            = { 0x14, 0x01, FALSE, "",  "RET",   VMINSTR_METHOD_RET };

static const runtime_instr_t VMINSTR_IN             = { 0x20, 0x04, TRUE, "rs", "IN",    VMINSTR_METHOD_IN };
static const runtime_instr_t VMINSTR_INR            = { 0x21, 0x03, TRUE, "rr", "INR",   VMINSTR_METHOD_INR };
static const runtime_instr_t VMINSTR_OUT            = { 0x22, 0x07, TRUE, "si", "OUT",   VMINSTR_METHOD_OUT };
static const runtime_instr_t VMINSTR_OUTR           = { 0x23, 0x04, TRUE, "sr", "OUTR",  VMINSTR_METHOD_OUTR };
static const runtime_instr_t VMINSTR_OUTRR          = { 0x24, 0x03, TRUE, "rr", "OUTRR", VMINSTR_METHOD_OUTRR };

static const runtime_instr_t* VM_ISA[] = 
{
    &VMINSTR_NOP,
    &VMINSTR_HLT,
    &VMINSTR_EXIT,
    &VMINSTR_TRAP,

    &VMINSTR_ADD,
    &VMINSTR_ADDR,
    &VMINSTR_SUB,
    &VMINSTR_SUBR,
    &VMINSTR_MUL,
    &VMINSTR_MULR,
    &VMINSTR_DIV,
    &VMINSTR_DIVR,

    &VMINSTR_OR,
    &VMINSTR_ORR,
    &VMINSTR_XOR,
    &VMINSTR_XORR,
    &VMINSTR_AND,
    &VMINSTR_ANDR,
    &VMINSTR_NOT,

    &VMINSTR_JMP,
    &VMINSTR_JMPR,
    &VMINSTR_CALL,
    &VMINSTR_CALLR,
    &VMINSTR_RET,

    &VMINSTR_IN,
    &VMINSTR_INR,
    &VMINSTR_OUT,
    &VMINSTR_OUTR,
    &VMINSTR_OUTRR,
};

static const uint32_t VM_ISA_COUNT = sizeof(VM_ISA) / sizeof(runtime_instr_t*);
