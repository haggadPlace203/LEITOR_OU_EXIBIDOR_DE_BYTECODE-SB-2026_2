/*==================================================*/
/*  Informações sobre o desenvolvedor & assinatura  */
/*   Carlos Luan Silva Pereira - Eng. Mecatrônica   */
/*       Estudante da Universidade de Brasília      */
/*           Brasília - DF - 14/11/2026             */
/*==================================================*/

#ifndef BYTECODE_H
#define BYTECODE_H

#include <unordered_map>
#include <stdint.h>

#include <iostream> /*!!!!!!!!!!!!!!!!!!!!!!!!*/

/*========Funções otimizadas para execução do bytecode JVM ===========*/

// Byte 0x00: Num faz nada
template <typename Context>
inline void nop(Context& ctx){
    int nop = 0;
}
// Byte 0x01: se vc leu é G**
template <typename Context>
inline void aconst_null(Context& ctx){
    ctx.push(INT32_C(0));
}
// Byte 0x02: Empilha -1 no topo da pilha
template <typename Context>
inline void iconst_m1(Context& ctx){
    ctx.push(INT32_C(-1));
}


// Byte 0x03: Empilha 0 no topo da pilha
template <typename Context>
inline void iconst_0(Context& ctx){
    ctx.push(INT32_C(0));
}
// Byte 0x04: Empilha 1 no topo da pilha
template <typename Context>
inline void iconst_1(Context& ctx){
    ctx.push(INT32_C(1));
}
// Byte 0x05: Empilha 2 no topo da pilha
template <typename Context>
inline void iconst_2(Context& ctx){
    ctx.push(INT32_C(2));
}
// Byte 0x06: Empilha 3 no topo da pilha
template <typename Context>
inline void iconst_3(Context& ctx){
    ctx.push(INT32_C(3));
}
// Byte 0x07: Empilha 4 no topo da pilha
template <typename Context>
inline void iconst_4(Context& ctx){
    ctx.push(INT32_C(4));
}
// Byte 0x08: Empilha 5 no topo da pilha
template <typename Context>
inline void iconst_5(Context& ctx){
    ctx.push(INT32_C(5));
}
// Byte 0x09: Empilha long 0L no topo da pilha (64 bits -> 2 slots)
template <typename Context>
inline void lconst_0(Context& ctx){
    ctx.push(INT32_C(0));
    ctx.push(INT32_C(0));
}
// Byte 0x0A: Empilha long 1L no topo da pilha (64 bits -> 2 slots)
template <typename Context>
inline void lconst_1(Context& ctx){
    ctx.push(INT32_C(0));
    ctx.push(INT32_C(1));
}
// Byte 0x0B: Empilha float 0.0f no topo da pilha (IEEE 754: 0x00000000)
template <typename Context>
inline void fconst_0(Context& ctx){
    ctx.push(INT32_C(0x00000000));
}
// Byte 0x0C: Empilha float 1.0f no topo da pilha (IEEE 754: 0x3F800000)
template <typename Context>
inline void fconst_1(Context& ctx){
    ctx.push(INT32_C(0x3F800000));
}
// Byte 0x0D: Empilha float 2.0f no topo da pilha (IEEE 754: 0x40000000)
template <typename Context>
inline void fconst_2(Context& ctx){
    ctx.push(INT32_C(0x40000000));
}
// Byte 0x0E: Empilha double 0.0 no topo da pilha (IEEE 754: 0x0000000000000000)
template <typename Context>
inline void dconst_0(Context& ctx){
    ctx.push(INT32_C(0x00000000));
    ctx.push(INT32_C(0x00000000));
}
// Byte 0x0F: Empilha double 1.0 no topo da pilha (IEEE 754: 0x3FF0000000000000)
template <typename Context>
inline void dconst_1(Context& ctx){
    ctx.push(INT32_C(0x3FF00000));
    ctx.push(INT32_C(0x00000000));
}
// Byte 0x10: pegar 1 byte apos extende e guarda no topo da pilha
template <typename Context>
inline void bipush(Context& ctx){
    uint8_t b1 = ctx.code[ctx.pc];
    ctx.pc++;
    ctx.push(static_cast<int32_t>(static_cast<int8_t>(b1)));
}
// Byte 0x11: pegar 2 bytes apos extende e guarda no topo da pilha
template <typename Context>
inline void sipush(Context& ctx){
    uint8_t b1 = ctx.code[ctx.pc];
    ctx.pc++;
    uint8_t b2 = ctx.code[ctx.pc];
    ctx.pc++;
    uint16_t b3 = (b1 << 8) + b2;
    ctx.push(static_cast<int32_t>(static_cast<int16_t>(b3)));
}
// Byte 0x12: 
template <typename Context>
inline void ldc(Context& ctx){
    uint8_t b1 = ctx.code[ctx.pc];
    ctx.pc++;
    std::cout << "AINDA A SER IMPLEMENTADO" << frame_metodo.pc << std::endl;
}
// Byte 0x13: 
template <typename Context>
inline void ldc_w(Context& ctx){
    uint8_t b1 = ctx.code[ctx.pc];
    ctx.pc++;
    std::cout << "AINDA A SER IMPLEMENTADO" << frame_metodo.pc << std::endl;
}
// Byte 0x14: 
template <typename Context>
inline void ldc2_w(Context& ctx){
    uint8_t b1 = ctx.code[ctx.pc];
    ctx.pc++;
    std::cout << "AINDA A SER IMPLEMENTADO" << frame_metodo.pc << std::endl;
}
// Byte 0x15: pega Variável Local e coloca na pilha
template <typename Context>
inline void iload(Context& ctx){
    uint8_t b1 = ctx.code[ctx.pc];
    ctx.pc++;
    ctx.push(ctx.locais[static_cast<uint32_t>(b1)]);
}
// Byte 0x16: pega Variável Local e coloca na pilha
template <typename Context>
inline void lload(Context& ctx){
    uint8_t b1 = ctx.code[ctx.pc];
    ctx.pc++;
    ctx.push(ctx.locais[static_cast<uint32_t>(b1)]);
    ctx.push(ctx.locais[static_cast<uint32_t>(b1) + 1]);
}
// Byte 0x17: pega Variável Local e coloca na pilha
template <typename Context>
inline void fload(Context& ctx){
    uint8_t b1 = ctx.code[ctx.pc];
    ctx.pc++;
    ctx.push(ctx.locais[static_cast<uint32_t>(b1)]);
}
// Byte 0x18: pega Variável Local e coloca na pilha
template <typename Context>
inline void dload(Context& ctx){
    uint8_t b1 = ctx.code[ctx.pc];
    ctx.pc++;
    ctx.push(ctx.locais[static_cast<uint32_t>(b1)]);
    ctx.push(ctx.locais[static_cast<uint32_t>(b1) + 1]);
}
// Byte 0x19: pega Variável Local e coloca na pilha
template <typename Context>
inline void aload(Context& ctx){
    uint8_t b1 = ctx.code[ctx.pc];
    ctx.pc++;
    ctx.push(ctx.locais[static_cast<uint32_t>(b1)]);
}
// Byte 0x1A: pega Variável Local[0] (int) e coloca na pilha
template <typename Context>
inline void iload_0(Context& ctx) {
    ctx.push(ctx.locais[0]);
}
// Byte 0x1B: pega Variável Local[1] (int) e coloca na pilha
template <typename Context>
inline void iload_1(Context& ctx) {
    ctx.push(ctx.locais[1]);
}
// Byte 0x1C: pega Variável Local[2] (int) e coloca na pilha
template <typename Context>
inline void iload_2(Context& ctx) {
    ctx.push(ctx.locais[2]);
}
// Byte 0x1D: pega Variável Local[3] (int) e coloca na pilha
template <typename Context>
inline void iload_3(Context& ctx) {
    ctx.push(ctx.locais[3]);
}
// Byte 0x1E: pega Variável Local (long nos slots 0 e 1) e coloca na pilha
template <typename Context>
inline void lload_0(Context& ctx) {
    ctx.push(ctx.locais[0]);
    ctx.push(ctx.locais[1]); // ou ctx.push(0) se cada slot já guarda 64 bits
}
// Byte 0x1F: pega Variável Local (long nos slots 1 e 2) e coloca na pilha
template <typename Context>
inline void lload_1(Context& ctx) {
    ctx.push(ctx.locais[1]);
    ctx.push(ctx.locais[2]);
}
// Byte 0x20: pega Variável Local (long nos slots 2 e 3) e coloca na pilha
template <typename Context>
inline void lload_2(Context& ctx) {
    ctx.push(ctx.locais[2]);
    ctx.push(ctx.locais[3]);
}
// Byte 0x21: pega Variável Local (long nos slots 3 e 4) e coloca na pilha
template <typename Context>
inline void lload_3(Context& ctx) {
    ctx.push(ctx.locais[3]);
    ctx.push(ctx.locais[4]);
}
// Byte 0x22: pega Variável Local (float) e coloca na pilha
template <typename Context>
inline void fload_0(Context& ctx) {
    ctx.push(ctx.locais);
}
// Byte 0x23: pega Variável Local (float) e coloca na pilha
template <typename Context>
inline void fload_1(Context& ctx) {
    ctx.push(ctx.locais);
}
// Byte 0x24: pega Variável Local (float) e coloca na pilha
template <typename Context>
inline void fload_2(Context& ctx) {
    ctx.push(ctx.locais);
}
// Byte 0x25: pega Variável Local (float) e coloca na pilha
template <typename Context>
inline void fload_3(Context& ctx) {
    ctx.push(ctx.locais);
}
// Byte 0x26: pega Variável Local (double nos slots 0 e 1) e coloca na pilha
template <typename Context>
inline void dload_0(Context& ctx) {
    ctx.push(ctx.locais[0]);
    ctx.push(ctx.locais[1]); // ou ctx.push(0) se cada slot já guarda 64 bits
}
// Byte 0x27: pega Variável Local (double nos slots 1 e 2) e coloca na pilha
template <typename Context>
inline void dload_1(Context& ctx) {
    ctx.push(ctx.locais[1]);
    ctx.push(ctx.locais[2]);
}
// Byte 0x28: pega Variável Local (double nos slots 2 e 3) e coloca na pilha
template <typename Context>
inline void dload_2(Context& ctx) {
    ctx.push(ctx.locais[2]);
    ctx.push(ctx.locais[3]);
}
// Byte 0x29: pega Variável Local (double nos slots 3 e 4) e coloca na pilha
template <typename Context>
inline void dload_3(Context& ctx) {
    ctx.push(ctx.locais[3]);
    ctx.push(ctx.locais[4]);
}
// Byte 0x2A: pega Variável Local (referência no slot 0) e coloca na pilha
template <typename Context>
inline void aload_0(Context& ctx) {
    ctx.push(ctx.locais[0]);
}
// Byte 0x2B: pega Variável Local (referência no slot 1) e coloca na pilha
template <typename Context>
inline void aload_1(Context& ctx) {
    ctx.push(ctx.locais[1]);
}
// Byte 0x2C: pega Variável Local (referência no slot 2) e coloca na pilha
template <typename Context>
inline void aload_2(Context& ctx) {
    ctx.push(ctx.locais[2]);
}
// Byte 0x2D: pega Variável Local (referência no slot 3) e coloca na pilha
template <typename Context>
inline void aload_3(Context& ctx) {
    ctx.push(ctx.locais[3]);
}
// Byte 0x2E: 
template <typename Context>
inline void iaload(Context& ctx){
    std::cout << "AINDA A SER IMPLEMENTADO" << frame_metodo.pc << std::endl;
}
// Byte 0x2F: 
template <typename Context>
inline void laload(Context& ctx){
    std::cout << "AINDA A SER IMPLEMENTADO" << frame_metodo.pc << std::endl;
}
// Byte 0x30: 
template <typename Context>
inline void faload(Context& ctx){
    std::cout << "AINDA A SER IMPLEMENTADO" << frame_metodo.pc << std::endl;
}
// Byte 0x31: 
template <typename Context>
inline void daload(Context& ctx){
    std::cout << "AINDA A SER IMPLEMENTADO" << frame_metodo.pc << std::endl;
}
// Byte 0x32: 
template <typename Context>
inline void aaload(Context& ctx){
    std::cout << "AINDA A SER IMPLEMENTADO" << frame_metodo.pc << std::endl;
}
// Byte 0x33: 
template <typename Context>
inline void baload(Context& ctx){
    std::cout << "AINDA A SER IMPLEMENTADO" << frame_metodo.pc << std::endl;
}
// Byte 0x34: 
template <typename Context>
inline void caload(Context& ctx){
    std::cout << "AINDA A SER IMPLEMENTADO" << frame_metodo.pc << std::endl;
}
// Byte 0x35: 
template <typename Context>
inline void saload(Context& ctx){
    std::cout << "AINDA A SER IMPLEMENTADO" << frame_metodo.pc << std::endl;
}
// Byte 0x36: desempilha int e guarda na Variável Local[b1]
template <typename Context>
inline void istore(Context& ctx) {
    uint8_t b1 = ctx.code[ctx.pc];
    ctx.pc++;
    ctx.locais[static_cast<uint32_t>(b1)] = ctx.pop();
}
// Byte 0x37: desempilha long (2 slots) e guarda nas Variáveis Locais[b1] e [b1 + 1]
template <typename Context>
inline void lstore(Context& ctx) {
    uint8_t b1 = ctx.code[ctx.pc];
    ctx.pc++;
    // Ordem inversa do lload: o segundo slot (b1 + 1) está no topo da pilha
    ctx.locais[static_cast<uint32_t>(b1) + 1] = ctx.pop();
    ctx.locais[static_cast<uint32_t>(b1)]     = ctx.pop();
}
// Byte 0x38: desempilha float e guarda na Variável Local[b1]
template <typename Context>
inline void fstore(Context& ctx) {
    uint8_t b1 = ctx.code[ctx.pc];
    ctx.pc++;
    ctx.locais[static_cast<uint32_t>(b1)] = ctx.pop();
}
// Byte 0x39: desempilha double (2 slots) e guarda nas Variáveis Locais[b1] e [b1 + 1]
template <typename Context>
inline void dstore(Context& ctx) {
    uint8_t b1 = ctx.code[ctx.pc];
    ctx.pc++;
    // Ordem inversa do dload: o segundo slot (b1 + 1) está no topo da pilha
    ctx.locais[static_cast<uint32_t>(b1) + 1] = ctx.pop();
    ctx.locais[static_cast<uint32_t>(b1)]     = ctx.pop();
}
// Byte 0x3A: desempilha referência e guarda na Variável Local[b1]
template <typename Context>
inline void astore(Context& ctx) {
    uint8_t b1 = ctx.code[ctx.pc];
    ctx.pc++;
    ctx.locais[static_cast<uint32_t>(b1)] = ctx.pop();
}
// Byte 0x3B: desempilha int e guarda na Variável Local[0]
template <typename Context>
inline void istore_0(Context& ctx) {
    ctx.locais[0] = ctx.pop();
}
// Byte 0x3C: desempilha int e guarda na Variável Local[1]
template <typename Context>
inline void istore_1(Context& ctx) {
    ctx.locais[1] = ctx.pop();
}
// Byte 0x3D: desempilha int e guarda na Variável Local[2]
template <typename Context>
inline void istore_2(Context& ctx) {
    ctx.locais[2] = ctx.pop();
}
// Byte 0x3E: desempilha int e guarda na Variável Local[3]
template <typename Context>
inline void istore_3(Context& ctx) {
    ctx.locais[3] = ctx.pop();
}
// Byte 0x3F: desempilha long e guarda nas Variáveis Locais[0] e [1]
template <typename Context>
inline void lstore_0(Context& ctx) {
    ctx.locais[1] = ctx.pop();
    ctx.locais[0] = ctx.pop();
}
// Byte 0x40: desempilha long e guarda nas Variáveis Locais[1] e [2]
template <typename Context>
inline void lstore_1(Context& ctx) {
    ctx.locais[2] = ctx.pop();
    ctx.locais[1] = ctx.pop();
}
// Byte 0x41: desempilha long e guarda nas Variáveis Locais[2] e [3]
template <typename Context>
inline void lstore_2(Context& ctx) {
    ctx.locais[3] = ctx.pop();
    ctx.locais[2] = ctx.pop();
}
// Byte 0x42: desempilha long e guarda nas Variáveis Locais[3] e [4]
template <typename Context>
inline void lstore_3(Context& ctx) {
    ctx.locais[4] = ctx.pop();
    ctx.locais[3] = ctx.pop();
}
// Byte 0x43: desempilha float e guarda na Variável Local[0]
template <typename Context>
inline void fstore_0(Context& ctx) {
    ctx.locais[0] = ctx.pop();
}
// Byte 0x44: desempilha float e guarda na Variável Local[1]
template <typename Context>
inline void fstore_1(Context& ctx) {
    ctx.locais[1] = ctx.pop();
}
// Byte 0x45: desempilha float e guarda na Variável Local[2]
template <typename Context>
inline void fstore_2(Context& ctx) {
    ctx.locais[2] = ctx.pop();
}
// Byte 0x46: desempilha float e guarda na Variável Local[3]
template <typename Context>
inline void fstore_3(Context& ctx) {
    ctx.locais[3] = ctx.pop();
}
// Byte 0x47: desempilha double e guarda nas Variáveis Locais[0] e [1]
template <typename Context>
inline void dstore_0(Context& ctx) {
    ctx.locais[1] = ctx.pop();
    ctx.locais[0] = ctx.pop();
}
// Byte 0x48: desempilha double e guarda nas Variáveis Locais[1] e [2]
template <typename Context>
inline void dstore_1(Context& ctx) {
    ctx.locais[2] = ctx.pop();
    ctx.locais[1] = ctx.pop();
}
// Byte 0x49: desempilha double e guarda nas Variáveis Locais[2] e [3]
template <typename Context>
inline void dstore_2(Context& ctx) {
    ctx.locais[3] = ctx.pop();
    ctx.locais[2] = ctx.pop();
}
// Byte 0x4A: desempilha double e guarda nas Variáveis Locais[3] e [4]
template <typename Context>
inline void dstore_3(Context& ctx) {
    ctx.locais[4] = ctx.pop();
    ctx.locais[3] = ctx.pop();
}
// Byte 0x4B: desempilha referência e guarda na Variável Local[0]
template <typename Context>
inline void astore_0(Context& ctx) {
    ctx.locais[0] = ctx.pop();
}
// Byte 0x4C: desempilha referência e guarda na Variável Local[1]
template <typename Context>
inline void astore_1(Context& ctx) {
    ctx.locais[1] = ctx.pop();
}
// Byte 0x4D: desempilha referência e guarda na Variável Local[2]
template <typename Context>
inline void astore_2(Context& ctx) {
    ctx.locais[2] = ctx.pop();
}
// Byte 0x4E: desempilha referência e guarda na Variável Local[3]
template <typename Context>
inline void astore_3(Context& ctx) {
    ctx.locais[3] = ctx.pop();
}
// Byte 0x4F: grava int em array no Heap
template <typename Context>
inline void iastore(Context& ctx) {
    std::cout << "AINDA A SER IMPLEMENTADO" << ctx.pc << std::endl;
}
// Byte 0x50: grava long em array no Heap
template <typename Context>
inline void lastore(Context& ctx) {
    std::cout << "AINDA A SER IMPLEMENTADO" << ctx.pc << std::endl;
}
// Byte 0x51: grava float em array no Heap
template <typename Context>
inline void fastore(Context& ctx) {
    std::cout << "AINDA A SER IMPLEMENTADO" << ctx.pc << std::endl;
}
// Byte 0x52: grava double em array no Heap
template <typename Context>
inline void dastore(Context& ctx) {
    std::cout << "AINDA A SER IMPLEMENTADO" << ctx.pc << std::endl;
}
// Byte 0x53: grava referência em array no Heap
template <typename Context>
inline void aastore(Context& ctx) {
    std::cout << "AINDA A SER IMPLEMENTADO" << ctx.pc << std::endl;
}
// Byte 0x54: grava byte/boolean em array no Heap
template <typename Context>
inline void bastore(Context& ctx) {
    std::cout << "AINDA A SER IMPLEMENTADO" << ctx.pc << std::endl;
}
// Byte 0x55: grava char em array no Heap
template <typename Context>
inline void castore(Context& ctx) {
    std::cout << "AINDA A SER IMPLEMENTADO" << ctx.pc << std::endl;
}
// Byte 0x56: grava short em array no Heap
template <typename Context>
inline void sastore(Context& ctx) {
    std::cout << "AINDA A SER IMPLEMENTADO" << ctx.pc << std::endl;
}
// Byte 0x57: desempilha 1 slot do topo
template <typename Context>
inline void pop(Context& ctx) {
    ctx.pop();
}
// Byte 0x58: desempilha 2 slots do topo (dois de 32 bits ou um de 64 bits)
template <typename Context>
inline void pop2(Context& ctx) {
    ctx.pop();
    ctx.pop();
}
// Byte 0x59: duplica o slot do topo [..., v1] -> [..., v1, v1]
template <typename Context>
inline void dup(Context& ctx) {
    auto v1 = ctx.pop();
    ctx.push(v1);
    ctx.push(v1);
}
// Byte 0x5A: duplica o topo e insere 1 slot abaixo [..., v2, v1] -> [..., v1, v2, v1]
template <typename Context>
inline void dup_x1(Context& ctx) {
    auto v1 = ctx.pop();
    auto v2 = ctx.pop();
    ctx.push(v1);
    ctx.push(v2);
    ctx.push(v1);
}
// Byte 0x5B: duplica o topo e insere 2 slots abaixo [..., v3, v2, v1] -> [..., v1, v3, v2, v1]
template <typename Context>
inline void dup_x2(Context& ctx) {
    auto v1 = ctx.pop();
    auto v2 = ctx.pop();
    auto v3 = ctx.pop();
    ctx.push(v1);
    ctx.push(v3);
    ctx.push(v2);
    ctx.push(v1);
}
// Byte 0x5C: duplica os 2 slots do topo [..., v2, v1] -> [..., v2, v1, v2, v1]
template <typename Context>
inline void dup2(Context& ctx) {
    auto v1 = ctx.pop();
    auto v2 = ctx.pop();
    ctx.push(v2);
    ctx.push(v1);
    ctx.push(v2);
    ctx.push(v1);
}
// Byte 0x5D: duplica os 2 slots do topo e insere 1 slot abaixo [..., v3, v2, v1] -> [..., v2, v1, v3, v2, v1]
template <typename Context>
inline void dup2_x1(Context& ctx) {
    auto v1 = ctx.pop();
    auto v2 = ctx.pop();
    auto v3 = ctx.pop();
    ctx.push(v2);
    ctx.push(v1);
    ctx.push(v3);
    ctx.push(v2);
    ctx.push(v1);
}
// Byte 0x5E: duplica os 2 slots do topo e insere 2 slots abaixo [..., v4, v3, v2, v1] -> [..., v2, v1, v4, v3, v2, v1]
template <typename Context>
inline void dup2_x2(Context& ctx) {
    auto v1 = ctx.pop();
    auto v2 = ctx.pop();
    auto v3 = ctx.pop();
    auto v4 = ctx.pop();
    ctx.push(v2);
    ctx.push(v1);
    ctx.push(v4);
    ctx.push(v3);
    ctx.push(v2);
    ctx.push(v1);
}
// Byte 0x5F: inverte os 2 slots do topo [..., v2, v1] -> [..., v1, v2]
template <typename Context>
inline void swap(Context& ctx) {
    auto v1 = ctx.pop();
    auto v2 = ctx.pop();
    ctx.push(v1);
    ctx.push(v2);
}









/*=========EXEMPLOS A SEREM APAGADOS DEPOIS =========*/

// Byte 0x03: Remove os dois valores do topo, soma em 32 bits e empilha o resultado
template <typename Context>
inline void op_03(Context& ctx) {
    if (ctx.size() >= 2) {
        int32_t val1 = ctx.top();
        ctx.pop();

        int32_t val2 = ctx.top();
        ctx.pop();

        ctx.push(val2 + val1);
    }
}

// Byte 0x04: Desempilha o topo e salva na Variável Local [0]
template <typename Context>
inline void LOCAL(Context& ctx) {
    if (!ctx.empty()) {
        ctx.locais[0] = ctx.top();
        ctx.pop();
    }
}

// Byte 0x05: Carrega da Variável Local [0] e empilha no topo
template <typename Context>
inline void op_05_load0(Context& ctx) {
    ctx.push(ctx.locais[0]);
}

// Byte 0xFF: NÃO FAZ WBGObnglrbLN6544684&*%$$$ SINBORAUM
template <typename Context>
inline void TMP(Context& ctx){
    // Imprime o valor do topo da pilha na tela
    std::cout << "Topo da pilha: " << std::hex << ctx.top() << std::dec << std::endl;
    ctx.pop();
}

/*======TABELA HASH FUNÇÕES BYTECODE=====*/
// Tabela hash parametrizada pelo tipo Context (instanciada no main.cpp)
template <typename Context>
inline const std::unordered_map<uint8_t, void (*)(Context&)>& get_instruction() {
    typedef void (*OperationFunc)(Context&);

    static const std::unordered_map<uint8_t, OperationFunc> BYTECODE = [] {
        std::unordered_map<uint8_t, OperationFunc> map;

        map[0x00] = nop<Context>;
        map[0x01] = aconst_null<Context>;
        map[0x02] = iconst_m1<Context>;

        map[0x03] = iconst_0<Context>;
        map[0x04] = iconst_1<Context>;
        map[0x05] = iconst_2<Context>;
        map[0x06] = iconst_3<Context>;
        map[0x07] = iconst_4<Context>;
        map[0x08] = iconst_5<Context>;
        map[0x09] = lconst_0<Context>;
        map[0x0A] = lconst_1<Context>;
        map[0x0B] = fconst_0<Context>;
        map[0x0C] = fconst_1<Context>;
        map[0x0D] = fconst_2<Context>;
        map[0x0E] = dconst_0<Context>;
        map[0x0F] = dconst_1<Context>;

        map[0x10] = bipush<Context>;
        map[0x11] = sipush<Context>;
        map[0x12] = ldc<Context>;
        map[0x13] = ldc_w<Context>;
        map[0x14] = ldc2_w<Context>;

        map[0x15] = iload<Context>;
        map[0x16] = lload<Context>;
        map[0x17] = fload<Context>;
        map[0x18] = dload<Context>;
        map[0x19] = aload<Context>;

        map[0x1A] = iload_0<Context>;
        map[0x1B] = iload_1<Context>;
        map[0x1C] = iload_2<Context>;
        map[0x1D] = iload_3<Context>;
        map[0x1E] = lload_0<Context>;
        map[0x1F] = lload_1<Context>;
        map[0x20] = lload_2<Context>;
        map[0x21] = lload_3<Context>;
        map[0x22] = fload_0<Context>;
        map[0x23] = fload_1<Context>;
        map[0x24] = fload_2<Context>;
        map[0x25] = fload_3<Context>;
        map[0x26] = dload_0<Context>;
        map[0x27] = dload_1<Context>;
        map[0x28] = dload_2<Context>;
        map[0x29] = dload_3<Context>;
        map[0x2A] = aload_0<Context>;
        map[0x2B] = aload_1<Context>;
        map[0x2C] = aload_2<Context>;
        map[0x2D] = aload_3<Context>;

        map[0x2E] = iaload<Context>;
        map[0x2F] = laload<Context>;
        map[0x30] = faload<Context>;
        map[0x31] = daload<Context>;
        map[0x32] = aaload<Context>;
        map[0x33] = baload<Context>;
        map[0x34] = caload<Context>;
        map[0x35] = saload<Context>;

        map[0x36] = istore<Context>;
        map[0x37] = lstore<Context>;
        map[0x38] = fstore<Context>;
        map[0x39] = dstore<Context>;
        map[0x3A] = astore<Context>;

        map[0x3B] = istore_0<Context>;
        map[0x3C] = istore_1<Context>;
        map[0x3D] = istore_2<Context>;
        map[0x3E] = istore_3<Context>;
        map[0x3F] = lstore_0<Context>;
        map[0x40] = lstore_1<Context>;
        map[0x41] = lstore_2<Context>;
        map[0x42] = lstore_3<Context>;
        map[0x43] = fstore_0<Context>;
        map[0x44] = fstore_1<Context>;
        map[0x45] = fstore_2<Context>;
        map[0x46] = fstore_3<Context>;
        map[0x47] = dstore_0<Context>;
        map[0x48] = dstore_1<Context>;
        map[0x49] = dstore_2<Context>;
        map[0x4A] = dstore_3<Context>;
        map[0x4B] = astore_0<Context>;
        map[0x4C] = astore_1<Context>;
        map[0x4D] = astore_2<Context>;
        map[0x4E] = astore_3<Context>;

        map[0x4F] = iastore<Context>;
        map[0x50] = lastore<Context>;
        map[0x51] = fastore<Context>;
        map[0x52] = dastore<Context>;
        map[0x53] = aastore<Context>;
        map[0x54] = bastore<Context>;
        map[0x55] = castore<Context>;
        map[0x56] = sastore<Context>;

        map[0x57] = pop<Context>;
        map[0x58] = pop2<Context>;
        map[0x59] = dup<Context>;
        map[0x5A] = dup_x1<Context>;
        map[0x5B] = dup_x2<Context>;
        map[0x5C] = dup2<Context>;
        map[0x5D] = dup2_x1<Context>;
        map[0x5E] = dup2_x2<Context>;
        map[0x5F] = swap<Context>;
        



        map[0xFE] = LOCAL<Context>;
        map[0xFF] = TMP<Context>;

        return map;
    }();
    return BYTECODE;
}

/*================================================*/
/*            Comentários Auxiliares              */
/*                                                */
/* Descricao do Desacoplamento (TMP.h):           */
/*                                                */
/* - Templates (typename Context): Eliminam a     */
/*   dependencia direta de "METODOS.h" neste      */
/*   cabecalho, permitindo que as inclusoes sejam */
/*   gerenciadas livremente apenas no main.cpp.   */
/* - Instanciacao Tardía: O compilador resolve os */
/*   ponteiros de funcao (void (*)(Context&)) no  */
/*   momento em que get_instruction<VMContext>()  */
/*   e invocado no codigo principal.              */
/*================================================*/


/*!!!Assinado Por CLSP & Cruzeirão Prof. M. Ladeira!!!*/

#endif