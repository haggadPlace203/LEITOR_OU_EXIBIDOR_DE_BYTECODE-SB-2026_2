/*==================================================*/
/*  Informações sobre o desenvolvedor & assinatura  */
/*   Carlos Luan Silva Pereira - Eng. Mecatrônica   */
/*       Estudante da Universidade de Brasília      */
/*           Brasília - DF - 20/11/2026             */
/*==================================================*/

#ifndef BYTECODE_H
#define BYTECODE_H

#include <unordered_map>
#include <stdint.h>
#include <cstring>
#include <cmath>


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

template <typename Context>
inline void ldc_resolver_1slot(Context& ctx, uint16_t cp_index, const char* mnemonico) {
    if (!ctx.classe || cp_index == 0 || cp_index >= ctx.classe->constant_pool.size()) {
        std::cerr << "Erro: indice invalido no Constant Pool em " << mnemonico
                  << " (#" << cp_index << ") no PC=" << ctx.pc << "\n";
        ctx.erro_execucao = true;
        return;
    }

    const auto& cp = ctx.classe->constant_pool;
    const auto& entry = cp[cp_index];

    switch (entry.tag) {
        case 3: { // CONSTANT_Integer (32 bits)
            ctx.push(static_cast<int32_t>(entry.bytes_val));
            break;
        }
        case 4: { // CONSTANT_Float (32 bits IEEE 754)
            ctx.push(static_cast<int32_t>(entry.bytes_val));
            break;
        }
        case 7: { // CONSTANT_Class (ex.: MinhaClasse.class) -> index1 aponta para CONSTANT_Utf8
            std::string nome_classe = resolver_cp_string(cp, entry.index1);
            std::cout << "[" << mnemonico << " #" << cp_index << "] Class: " << nome_classe << "\n";
            ctx.push(static_cast<int32_t>(cp_index));
            break;
        }
        case 8: { // CONSTANT_String -> index1 aponta para CONSTANT_Utf8
            std::string texto = resolver_cp_string(cp, entry.index1);
            std::cout << "[" << mnemonico << " #" << cp_index << "] String: \"" << texto << "\"\n";
            ctx.push(static_cast<int32_t>(cp_index));
            break;
        }
        case 15: { // CONSTANT_MethodHandle -> ref_kind e index1
            std::string alvo = resolver_cp_string(cp, entry.index1);
            std::cout << "[" << mnemonico << " #" << cp_index
                      << "] MethodHandle (ref_kind=" << static_cast<int>(entry.ref_kind)
                      << "): " << alvo << "\n";
            ctx.push(static_cast<int32_t>(cp_index));
            break;
        }
        case 16: { // CONSTANT_MethodType -> index1 aponta para descritor UTF-8
            std::string descritor = resolver_cp_string(cp, entry.index1);
            std::cout << "[" << mnemonico << " #" << cp_index << "] MethodType: " << descritor << "\n";
            ctx.push(static_cast<int32_t>(cp_index));
            break;
        }
        default: {
            std::cerr << "Erro: tag nao suportada em " << mnemonico
                      << " (#" << cp_index << ", tag=" << static_cast<int>(entry.tag) << ")\n";
            ctx.erro_execucao = true;
            break;
        }
    }
}

// Byte 0x12: ldc - Carrega constante de 1 slot do Constant Pool (indice de 1 byte)
template <typename Context>
inline void ldc(Context& ctx) {
    if (ctx.pc >= ctx.code.size()) {
        std::cerr << "Erro: ldc sem operando no PC=" << ctx.pc << "\n";
        ctx.erro_execucao = true;
        return;
    }

    uint8_t b1 = ctx.code[ctx.pc++];
    ldc_resolver_1slot(ctx, static_cast<uint16_t>(b1), "ldc");
}

// Byte 0x13: ldc_w - Carrega constante de 1 slot do Constant Pool (indice de 2 bytes)
template <typename Context>
inline void ldc_w(Context& ctx) {
    if (ctx.pc + 1 >= ctx.code.size()) {
        std::cerr << "Erro: ldc_w sem operandos suficientes no PC=" << ctx.pc << "\n";
        ctx.erro_execucao = true;
        return;
    }

    uint16_t high = static_cast<uint16_t>(ctx.code[ctx.pc++]);
    uint16_t low  = static_cast<uint16_t>(ctx.code[ctx.pc++]);
    uint16_t cp_index = static_cast<uint16_t>((high << 8) | low);
    ldc_resolver_1slot(ctx, cp_index, "ldc_w");
}

// Byte 0x14: ldc2_w - Carrega constante de 64 bits (2 slots: long ou double) do Constant Pool
template <typename Context>
inline void ldc2_w(Context& ctx) {
    if (ctx.pc + 1 >= ctx.code.size()) {
        std::cerr << "Erro: ldc2_w sem operandos suficientes no PC=" << ctx.pc << "\n";
        ctx.erro_execucao = true;
        return;
    }

    uint16_t high_idx = static_cast<uint16_t>(ctx.code[ctx.pc++]);
    uint16_t low_idx  = static_cast<uint16_t>(ctx.code[ctx.pc++]);
    uint16_t cp_index = static_cast<uint16_t>((high_idx << 8) | low_idx);

    if (!ctx.classe || cp_index == 0 || cp_index >= ctx.classe->constant_pool.size()) {
        std::cerr << "Erro: indice invalido no Constant Pool em ldc2_w (#" << cp_index
                  << ") no PC=" << ctx.pc << "\n";
        ctx.erro_execucao = true;
        return;
    }

    const auto& entry = ctx.classe->constant_pool[cp_index];

    if (entry.tag == 5 || entry.tag == 6) { // CONSTANT_Long (5) ou CONSTANT_Double (6)
        uint64_t val = entry.long_bytes_val;
        int32_t high_word = static_cast<int32_t>(static_cast<uint32_t>(val >> 32));
        int32_t low_word  = static_cast<int32_t>(static_cast<uint32_t>(val & 0xFFFFFFFFULL));
        ctx.push(high_word);
        ctx.push(low_word);
    } else {
        std::cerr << "Erro: tag invalida em ldc2_w (#" << cp_index
                  << ", tag=" << static_cast<int>(entry.tag)
                  << "; esperado 5 para long ou 6 para double)\n";
        ctx.erro_execucao = true;
    }
}

// Byte 0x15: pega Variável Local e coloca na pilha
template <typename Context>
inline void iload(Context& ctx){
    uint8_t b1 = ctx.code[ctx.pc];
    ctx.pc++;
    ctx.push(ctx.get_local(static_cast<uint32_t>(b1)));
}
// Byte 0x16: pega Variável Local e coloca na pilha
template <typename Context>
inline void lload(Context& ctx){
    uint8_t b1 = ctx.code[ctx.pc];
    ctx.pc++;
    ctx.push(ctx.get_local(static_cast<uint32_t>(b1)));
    ctx.push(ctx.get_local(static_cast<uint32_t>(b1) + 1));
}
// Byte 0x17: pega Variável Local e coloca na pilha
template <typename Context>
inline void fload(Context& ctx){
    uint8_t b1 = ctx.code[ctx.pc];
    ctx.pc++;
    ctx.push(ctx.get_local(static_cast<uint32_t>(b1)));
}
// Byte 0x18: pega Variável Local e coloca na pilha
template <typename Context>
inline void dload(Context& ctx){
    uint8_t b1 = ctx.code[ctx.pc];
    ctx.pc++;
    ctx.push(ctx.get_local(static_cast<uint32_t>(b1)));
    ctx.push(ctx.get_local(static_cast<uint32_t>(b1) + 1));
}
// Byte 0x19: pega Variável Local e coloca na pilha
template <typename Context>
inline void aload(Context& ctx){
    uint8_t b1 = ctx.code[ctx.pc];
    ctx.pc++;
    ctx.push(ctx.get_local(static_cast<uint32_t>(b1)));
}
// Byte 0x1A: pega Variável Local[0] (int) e coloca na pilha
template <typename Context>
inline void iload_0(Context& ctx) {
    ctx.push(ctx.get_local(0));
}
// Byte 0x1B: pega Variável Local[1] (int) e coloca na pilha
template <typename Context>
inline void iload_1(Context& ctx) {
    ctx.push(ctx.get_local(1));
}
// Byte 0x1C: pega Variável Local[2] (int) e coloca na pilha
template <typename Context>
inline void iload_2(Context& ctx) {
    ctx.push(ctx.get_local(2));
}
// Byte 0x1D: pega Variável Local[3] (int) e coloca na pilha
template <typename Context>
inline void iload_3(Context& ctx) {
    ctx.push(ctx.get_local(3));
}
// Byte 0x1E: pega Variável Local (long nos slots 0 e 1) e coloca na pilha
template <typename Context>
inline void lload_0(Context& ctx) {
    ctx.push(ctx.get_local(0));
    ctx.push(ctx.get_local(1)); // ou ctx.push(0) se cada slot já guarda 64 bits
}
// Byte 0x1F: pega Variável Local (long nos slots 1 e 2) e coloca na pilha
template <typename Context>
inline void lload_1(Context& ctx) {
    ctx.push(ctx.get_local(1));
    ctx.push(ctx.get_local(2));
}
// Byte 0x20: pega Variável Local (long nos slots 2 e 3) e coloca na pilha
template <typename Context>
inline void lload_2(Context& ctx) {
    ctx.push(ctx.get_local(2));
    ctx.push(ctx.get_local(3));
}
// Byte 0x21: pega Variável Local (long nos slots 3 e 4) e coloca na pilha
template <typename Context>
inline void lload_3(Context& ctx) {
    ctx.push(ctx.get_local(3));
    ctx.push(ctx.get_local(4));
}
// Byte 0x22: pega Variável Local (float) e coloca na pilha
template <typename Context>
inline void fload_0(Context& ctx) {
    ctx.push(ctx.get_local(0));
}
// Byte 0x23: pega Variável Local (float) e coloca na pilha
template <typename Context>
inline void fload_1(Context& ctx) {
    ctx.push(ctx.get_local(1));
}
// Byte 0x24: pega Variável Local (float) e coloca na pilha
template <typename Context>
inline void fload_2(Context& ctx) {
    ctx.push(ctx.get_local(2));
}
// Byte 0x25: pega Variável Local (float) e coloca na pilha
template <typename Context>
inline void fload_3(Context& ctx) {
    ctx.push(ctx.get_local(3));
}
// Byte 0x26: pega Variável Local (double nos slots 0 e 1) e coloca na pilha
template <typename Context>
inline void dload_0(Context& ctx) {
    ctx.push(ctx.get_local(0));
    ctx.push(ctx.get_local(1)); // ou ctx.push(0) se cada slot já guarda 64 bits
}
// Byte 0x27: pega Variável Local (double nos slots 1 e 2) e coloca na pilha
template <typename Context>
inline void dload_1(Context& ctx) {
    ctx.push(ctx.get_local(1));
    ctx.push(ctx.get_local(2));
}
// Byte 0x28: pega Variável Local (double nos slots 2 e 3) e coloca na pilha
template <typename Context>
inline void dload_2(Context& ctx) {
    ctx.push(ctx.get_local(2));
    ctx.push(ctx.get_local(3));
}
// Byte 0x29: pega Variável Local (double nos slots 3 e 4) e coloca na pilha
template <typename Context>
inline void dload_3(Context& ctx) {
    ctx.push(ctx.get_local(3));
    ctx.push(ctx.get_local(4));
}
// Byte 0x2A: pega Variável Local (referência no slot 0) e coloca na pilha
template <typename Context>
inline void aload_0(Context& ctx) {
    ctx.push(ctx.get_local(0));
}
// Byte 0x2B: pega Variável Local (referência no slot 1) e coloca na pilha
template <typename Context>
inline void aload_1(Context& ctx) {
    ctx.push(ctx.get_local(1));
}
// Byte 0x2C: pega Variável Local (referência no slot 2) e coloca na pilha
template <typename Context>
inline void aload_2(Context& ctx) {
    ctx.push(ctx.get_local(2));
}
// Byte 0x2D: pega Variável Local (referência no slot 3) e coloca na pilha
template <typename Context>
inline void aload_3(Context& ctx) {
    ctx.push(ctx.get_local(3));
}

template <typename Context>
inline bool preparar_array_load(Context& ctx, const char* mnemonico, int32_t& arrayref, int32_t& index) {
    if (ctx.size() < 2) {
        std::cerr << "Erro: pilha insuficiente para " << mnemonico << " no PC=" << ctx.pc << "\n";
        ctx.erro_execucao = true;
        return false;
    }

    index = ctx.pop();
    arrayref = ctx.pop();
    if (ctx.erro_execucao) return false;

    if (arrayref == 0) {
        std::cerr << "Erro: NullPointerException em " << mnemonico << " no PC=" << ctx.pc << "\n";
        ctx.erro_execucao = true;
        return false;
    }

    if (index < 0) {
        std::cerr << "Erro: ArrayIndexOutOfBoundsException em " << mnemonico
                  << " (index=" << index << ") no PC=" << ctx.pc << "\n";
        ctx.erro_execucao = true;
        return false;
    }

    return true;
}

// Byte 0x2E: iaload - Carrega elemento int (32 bits, 1 slot) de array
template <typename Context>
inline void iaload(Context& ctx) {
    int32_t arrayref = 0, index = 0;
    if (!preparar_array_load(ctx, "iaload", arrayref, index)) return;

    // TODO: substituir pelo valor real em heap[arrayref][index] quando o Heap existir
    int32_t elem = INT32_C(0);
    ctx.push(elem);
}

// Byte 0x2F: laload - Carrega elemento long (64 bits, 2 slots: high depois low) de array
template <typename Context>
inline void laload(Context& ctx) {
    int32_t arrayref = 0, index = 0;
    if (!preparar_array_load(ctx, "laload", arrayref, index)) return;

    // TODO: substituir pelo long real em heap[arrayref][index] quando o Heap existir
    int64_t elem = INT64_C(0);
    uint64_t raw = static_cast<uint64_t>(elem);
    ctx.push(static_cast<int32_t>(static_cast<uint32_t>(raw >> 32)));
    ctx.push(static_cast<int32_t>(static_cast<uint32_t>(raw & 0xFFFFFFFFULL)));
}

// Byte 0x30: faload - Carrega elemento float (32 bits IEEE 754, 1 slot) de array
template <typename Context>
inline void faload(Context& ctx) {
    int32_t arrayref = 0, index = 0;
    if (!preparar_array_load(ctx, "faload", arrayref, index)) return;

    // TODO: substituir pelo float real em heap[arrayref][index] quando o Heap existir
    uint32_t raw = UINT32_C(0);
    ctx.push(static_cast<int32_t>(raw));
}

// Byte 0x31: daload - Carrega elemento double (64 bits IEEE 754, 2 slots: high depois low) de array
template <typename Context>
inline void daload(Context& ctx) {
    int32_t arrayref = 0, index = 0;
    if (!preparar_array_load(ctx, "daload", arrayref, index)) return;

    // TODO: substituir pelo double real em heap[arrayref][index] quando o Heap existir
    uint64_t raw = UINT64_C(0);
    ctx.push(static_cast<int32_t>(static_cast<uint32_t>(raw >> 32)));
    ctx.push(static_cast<int32_t>(static_cast<uint32_t>(raw & 0xFFFFFFFFULL)));
}

// Byte 0x32: aaload - Carrega referencia de objeto (1 slot) de array de referencias
template <typename Context>
inline void aaload(Context& ctx) {
    int32_t arrayref = 0, index = 0;
    if (!preparar_array_load(ctx, "aaload", arrayref, index)) return;

    // TODO: substituir pela referencia real em heap[arrayref][index] quando o Heap existir
    int32_t ref_elem = INT32_C(0);
    ctx.push(ref_elem);
}

// Byte 0x33: baload - Carrega byte (com sinal) ou boolean de array e estende para int (1 slot)
template <typename Context>
inline void baload(Context& ctx) {
    int32_t arrayref = 0, index = 0;
    if (!preparar_array_load(ctx, "baload", arrayref, index)) return;

    // TODO: substituir pelo byte/boolean real em heap[arrayref][index] quando o Heap existir
    int8_t raw_byte = 0;
    ctx.push(static_cast<int32_t>(raw_byte));
}

// Byte 0x34: caload - Carrega char (sem sinal 16 bits) de array e estende para int (1 slot)
template <typename Context>
inline void caload(Context& ctx) {
    int32_t arrayref = 0, index = 0;
    if (!preparar_array_load(ctx, "caload", arrayref, index)) return;

    // TODO: substituir pelo char real em heap[arrayref][index] quando o Heap existir
    uint16_t raw_char = 0;
    ctx.push(static_cast<int32_t>(raw_char));
}

// Byte 0x35: saload - Carrega short (com sinal 16 bits) de array e estende para int (1 slot)
template <typename Context>
inline void saload(Context& ctx) {
    int32_t arrayref = 0, index = 0;
    if (!preparar_array_load(ctx, "saload", arrayref, index)) return;

    // TODO: substituir pelo short real em heap[arrayref][index] quando o Heap existir
    int16_t raw_short = 0;
    ctx.push(static_cast<int32_t>(raw_short));
}

// Byte 0x36: desempilha int e guarda na Variável Local[b1]
template <typename Context>
inline void istore(Context& ctx) {
    uint8_t b1 = ctx.code[ctx.pc];
    ctx.pc++;
    ctx.set_local(static_cast<uint32_t>(b1), ctx.pop());
}
// Byte 0x37: desempilha long (2 slots) e guarda nas Variáveis Locais[b1] e [b1 + 1]
template <typename Context>
inline void lstore(Context& ctx) {
    uint8_t b1 = ctx.code[ctx.pc];
    ctx.pc++;
    // Ordem inversa do lload: o segundo slot (b1 + 1) está no topo da pilha
    ctx.set_local(static_cast<uint32_t>(b1) + 1, ctx.pop());
    ctx.set_local(static_cast<uint32_t>(b1), ctx.pop());
}
// Byte 0x38: desempilha float e guarda na Variável Local[b1]
template <typename Context>
inline void fstore(Context& ctx) {
    uint8_t b1 = ctx.code[ctx.pc];
    ctx.pc++;
    ctx.set_local(static_cast<uint32_t>(b1), ctx.pop());
}
// Byte 0x39: desempilha double (2 slots) e guarda nas Variáveis Locais[b1] e [b1 + 1]
template <typename Context>
inline void dstore(Context& ctx) {
    uint8_t b1 = ctx.code[ctx.pc];
    ctx.pc++;
    // Ordem inversa do dload: o segundo slot (b1 + 1) está no topo da pilha
    ctx.set_local(static_cast<uint32_t>(b1) + 1, ctx.pop());
    ctx.set_local(static_cast<uint32_t>(b1), ctx.pop());
}
// Byte 0x3A: desempilha referência e guarda na Variável Local[b1]
template <typename Context>
inline void astore(Context& ctx) {
    uint8_t b1 = ctx.code[ctx.pc];
    ctx.pc++;
    ctx.set_local(static_cast<uint32_t>(b1), ctx.pop());
}
// Byte 0x3B: desempilha int e guarda na Variável Local[0]
template <typename Context>
inline void istore_0(Context& ctx) {
    ctx.set_local(0, ctx.pop());
}
// Byte 0x3C: desempilha int e guarda na Variável Local[1]
template <typename Context>
inline void istore_1(Context& ctx) {
    ctx.set_local(1, ctx.pop());
}
// Byte 0x3D: desempilha int e guarda na Variável Local[2]
template <typename Context>
inline void istore_2(Context& ctx) {
    ctx.set_local(2, ctx.pop());
}
// Byte 0x3E: desempilha int e guarda na Variável Local[3]
template <typename Context>
inline void istore_3(Context& ctx) {
    ctx.set_local(3, ctx.pop());
}
// Byte 0x3F: desempilha long e guarda nas Variáveis Locais[0] e [1]
template <typename Context>
inline void lstore_0(Context& ctx) {
    ctx.set_local(1, ctx.pop());
    ctx.set_local(0, ctx.pop());
}
// Byte 0x40: desempilha long e guarda nas Variáveis Locais[1] e [2]
template <typename Context>
inline void lstore_1(Context& ctx) {
    ctx.set_local(2, ctx.pop());
    ctx.set_local(1, ctx.pop());
}
// Byte 0x41: desempilha long e guarda nas Variáveis Locais[2] e [3]
template <typename Context>
inline void lstore_2(Context& ctx) {
    ctx.set_local(3, ctx.pop());
    ctx.set_local(2, ctx.pop());
}
// Byte 0x42: desempilha long e guarda nas Variáveis Locais[3] e [4]
template <typename Context>
inline void lstore_3(Context& ctx) {
    ctx.set_local(4, ctx.pop());
    ctx.set_local(3, ctx.pop());
}
// Byte 0x43: desempilha float e guarda na Variável Local[0]
template <typename Context>
inline void fstore_0(Context& ctx) {
    ctx.set_local(0, ctx.pop());
}
// Byte 0x44: desempilha float e guarda na Variável Local[1]
template <typename Context>
inline void fstore_1(Context& ctx) {
    ctx.set_local(1, ctx.pop());
}
// Byte 0x45: desempilha float e guarda na Variável Local[2]
template <typename Context>
inline void fstore_2(Context& ctx) {
    ctx.set_local(2, ctx.pop());
}
// Byte 0x46: desempilha float e guarda na Variável Local[3]
template <typename Context>
inline void fstore_3(Context& ctx) {
    ctx.set_local(3, ctx.pop());
}
// Byte 0x47: desempilha double e guarda nas Variáveis Locais[0] e [1]
template <typename Context>
inline void dstore_0(Context& ctx) {
    ctx.set_local(1, ctx.pop());
    ctx.set_local(0, ctx.pop());
}
// Byte 0x48: desempilha double e guarda nas Variáveis Locais[1] e [2]
template <typename Context>
inline void dstore_1(Context& ctx) {
    ctx.set_local(2, ctx.pop());
    ctx.set_local(1, ctx.pop());
}
// Byte 0x49: desempilha double e guarda nas Variáveis Locais[2] e [3]
template <typename Context>
inline void dstore_2(Context& ctx) {
    ctx.set_local(3, ctx.pop());
    ctx.set_local(2, ctx.pop());
}
// Byte 0x4A: desempilha double e guarda nas Variáveis Locais[3] e [4]
template <typename Context>
inline void dstore_3(Context& ctx) {
    ctx.set_local(4, ctx.pop());
    ctx.set_local(3, ctx.pop());
}
// Byte 0x4B: desempilha referência e guarda na Variável Local[0]
template <typename Context>
inline void astore_0(Context& ctx) {
    ctx.set_local(0, ctx.pop());
}
// Byte 0x4C: desempilha referência e guarda na Variável Local[1]
template <typename Context>
inline void astore_1(Context& ctx) {
    ctx.set_local(1, ctx.pop());
}
// Byte 0x4D: desempilha referência e guarda na Variável Local[2]
template <typename Context>
inline void astore_2(Context& ctx) {
    ctx.set_local(2, ctx.pop());
}
// Byte 0x4E: desempilha referência e guarda na Variável Local[3]
template <typename Context>
inline void astore_3(Context& ctx) {
    ctx.set_local(3, ctx.pop());
}

template <typename Context>
inline bool preparar_array_store_1slot(Context& ctx,
                                       const char* mnemonico,
                                       int32_t& arrayref,
                                       int32_t& index,
                                       int32_t& value) {
    if (ctx.size() < 3) {
        std::cerr << "Erro: pilha insuficiente para " << mnemonico << " no PC=" << ctx.pc << "\n";
        ctx.erro_execucao = true;
        return false;
    }

    value = ctx.pop();
    index = ctx.pop();
    arrayref = ctx.pop();
    if (ctx.erro_execucao) return false;

    if (arrayref == 0) {
        std::cerr << "Erro: NullPointerException em " << mnemonico << " no PC=" << ctx.pc << "\n";
        ctx.erro_execucao = true;
        return false;
    }

    if (index < 0) {
        std::cerr << "Erro: ArrayIndexOutOfBoundsException em " << mnemonico
                  << " (index=" << index << ") no PC=" << ctx.pc << "\n";
        ctx.erro_execucao = true;
        return false;
    }

    return true;
}

template <typename Context>
inline bool preparar_array_store_2slots(Context& ctx,
                                        const char* mnemonico,
                                        int32_t& arrayref,
                                        int32_t& index,
                                        uint64_t& raw_64) {
    if (ctx.size() < 4) {
        std::cerr << "Erro: pilha insuficiente para " << mnemonico << " no PC=" << ctx.pc << "\n";
        ctx.erro_execucao = true;
        return false;
    }

    int32_t low_word = ctx.pop();
    int32_t high_word = ctx.pop();
    index = ctx.pop();
    arrayref = ctx.pop();
    if (ctx.erro_execucao) return false;

    if (arrayref == 0) {
        std::cerr << "Erro: NullPointerException em " << mnemonico << " no PC=" << ctx.pc << "\n";
        ctx.erro_execucao = true;
        return false;
    }

    if (index < 0) {
        std::cerr << "Erro: ArrayIndexOutOfBoundsException em " << mnemonico
                  << " (index=" << index << ") no PC=" << ctx.pc << "\n";
        ctx.erro_execucao = true;
        return false;
    }

    raw_64 = (static_cast<uint64_t>(static_cast<uint32_t>(high_word)) << 32) |
             static_cast<uint64_t>(static_cast<uint32_t>(low_word));
    return true;
}

// Byte 0x4F: iastore - Grava int (32 bits) em array no Heap
template <typename Context>
inline void iastore(Context& ctx) {
    int32_t arrayref = 0, index = 0, value = 0;
    if (!preparar_array_store_1slot(ctx, "iastore", arrayref, index, value)) return;

    // TODO: gravar int32_t value em heap[arrayref][index] quando o Heap existir
    (void)value;
}

// Byte 0x50: lastore - Grava long (64 bits, 2 slots) em array no Heap
template <typename Context>
inline void lastore(Context& ctx) {
    int32_t arrayref = 0, index = 0;
    uint64_t raw_64 = 0;
    if (!preparar_array_store_2slots(ctx, "lastore", arrayref, index, raw_64)) return;

    // TODO: gravar static_cast<int64_t>(raw_64) em heap[arrayref][index] quando o Heap existir
    (void)raw_64;
}

// Byte 0x51: fastore - Grava float (32 bits IEEE 754) em array no Heap
template <typename Context>
inline void fastore(Context& ctx) {
    int32_t arrayref = 0, index = 0, value = 0;
    if (!preparar_array_store_1slot(ctx, "fastore", arrayref, index, value)) return;

    // TODO: gravar uint32_t bits (static_cast<uint32_t>(value)) em heap[arrayref][index]
    (void)value;
}

// Byte 0x52: dastore - Grava double (64 bits IEEE 754, 2 slots) em array no Heap
template <typename Context>
inline void dastore(Context& ctx) {
    int32_t arrayref = 0, index = 0;
    uint64_t raw_64 = 0;
    if (!preparar_array_store_2slots(ctx, "dastore", arrayref, index, raw_64)) return;

    // TODO: gravar uint64_t bits raw_64 em heap[arrayref][index] quando o Heap existir
    (void)raw_64;
}

// Byte 0x53: aastore - Grava referencia de objeto em array no Heap
template <typename Context>
inline void aastore(Context& ctx) {
    int32_t arrayref = 0, index = 0, ref_val = 0;
    if (!preparar_array_store_1slot(ctx, "aastore", arrayref, index, ref_val)) return;

    // TODO: gravar referencia ref_val em heap[arrayref][index] (e validar ArrayStoreException se aplicavel)
    (void)ref_val;
}

// Byte 0x54: bastore - Grava byte (8 bits com sinal) ou boolean em array no Heap
template <typename Context>
inline void bastore(Context& ctx) {
    int32_t arrayref = 0, index = 0, value = 0;
    if (!preparar_array_store_1slot(ctx, "bastore", arrayref, index, value)) return;

    // Na JVM, bastore trunca o int para byte (8 bits) ou boolean (bit 0)
    int8_t byte_val = static_cast<int8_t>(value & 0xFF);
    // TODO: gravar byte_val em heap[arrayref][index] quando o Heap existir
    (void)byte_val;
}

// Byte 0x55: castore - Grava char (16 bits sem sinal) em array no Heap
template <typename Context>
inline void castore(Context& ctx) {
    int32_t arrayref = 0, index = 0, value = 0;
    if (!preparar_array_store_1slot(ctx, "castore", arrayref, index, value)) return;

    uint16_t char_val = static_cast<uint16_t>(value & 0xFFFF);
    // TODO: gravar char_val em heap[arrayref][index] quando o Heap existir
    (void)char_val;
}

// Byte 0x56: sastore - Grava short (16 bits com sinal) em array no Heap
template <typename Context>
inline void sastore(Context& ctx) {
    int32_t arrayref = 0, index = 0, value = 0;
    if (!preparar_array_store_1slot(ctx, "sastore", arrayref, index, value)) return;

    int16_t short_val = static_cast<int16_t>(value & 0xFFFF);
    // TODO: gravar short_val em heap[arrayref][index] quando o Heap existir
    (void)short_val;
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
    auto v1 = ctx.top(); ctx.pop();
    ctx.push(v1);
    ctx.push(v1);
}
// Byte 0x5A: duplica o topo e insere 1 slot abaixo [..., v2, v1] -> [..., v1, v2, v1]
template <typename Context>
inline void dup_x1(Context& ctx) {
    auto v1 = ctx.top(); ctx.pop();
    auto v2 = ctx.top(); ctx.pop();
    ctx.push(v1);
    ctx.push(v2);
    ctx.push(v1);
}
// Byte 0x5B: duplica o topo e insere 2 slots abaixo [..., v3, v2, v1] -> [..., v1, v3, v2, v1]
template <typename Context>
inline void dup_x2(Context& ctx) {
    auto v1 = ctx.top(); ctx.pop();
    auto v2 = ctx.top(); ctx.pop();
    auto v3 = ctx.top(); ctx.pop();
    ctx.push(v1);
    ctx.push(v3);
    ctx.push(v2);
    ctx.push(v1);
}
// Byte 0x5C: duplica os 2 slots do topo [..., v2, v1] -> [..., v2, v1, v2, v1]
template <typename Context>
inline void dup2(Context& ctx) {
    auto v1 = ctx.top(); ctx.pop();
    auto v2 = ctx.top(); ctx.pop();
    ctx.push(v2);
    ctx.push(v1);
    ctx.push(v2);
    ctx.push(v1);
}
// Byte 0x5D: duplica os 2 slots do topo e insere 1 slot abaixo [..., v3, v2, v1] -> [..., v2, v1, v3, v2, v1]
template <typename Context>
inline void dup2_x1(Context& ctx) {
    auto v1 = ctx.top(); ctx.pop();
    auto v2 = ctx.top(); ctx.pop();
    auto v3 = ctx.top(); ctx.pop();
    ctx.push(v2);
    ctx.push(v1);
    ctx.push(v3);
    ctx.push(v2);
    ctx.push(v1);
}
// Byte 0x5E: duplica os 2 slots do topo e insere 2 slots abaixo [..., v4, v3, v2, v1] -> [..., v2, v1, v4, v3, v2, v1]
template <typename Context>
inline void dup2_x2(Context& ctx) {
    auto v1 = ctx.top(); ctx.pop();
    auto v2 = ctx.top(); ctx.pop();
    auto v3 = ctx.top(); ctx.pop();
    auto v4 = ctx.top(); ctx.pop();
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
    auto v1 = ctx.top(); ctx.pop();
    auto v2 = ctx.top(); ctx.pop();
    ctx.push(v1);
    ctx.push(v2);
}
// Byte 0x60: iadd - Soma dois int (32-bit, consome 2 slots de 1 slot cada)
template <typename Context>
inline void iadd(Context& ctx) {
    if (ctx.size() >= 2) {
        int32_t val1 = ctx.top(); ctx.pop();
        int32_t val2 = ctx.top(); ctx.pop();
        ctx.push(val2 + val1);
    }
}
// Byte 0x61: ladd - Soma dois long (64-bit, consome 4 slots na pilha de 32 bits: 2 para val1, 2 para val2)
template <typename Context>
inline void ladd(Context& ctx) {
    if (ctx.size() >= 4) {
        // Desempilha val1 (64-bit dividido em high e low)
        uint32_t v1_low = ctx.top(); ctx.pop();
        uint32_t v1_high = ctx.top(); ctx.pop();
        int64_t val1 = ((int64_t)v1_high << 32) | v1_low;

        // Desempilha val2 (64-bit dividido em high e low)
        uint32_t v2_low = ctx.top(); ctx.pop();
        uint32_t v2_high = ctx.top(); ctx.pop();
        int64_t val2 = ((int64_t)v2_high << 32) | v2_low;

        int64_t result = val2 + val1;

        // Empilha o resultado de volta (ocupando 2 slots de 32 bits)
        ctx.push((uint32_t)(result >> 32));         // high word
        ctx.push((uint32_t)(result & 0xFFFFFFFF));  // low word
    }
}
// Byte 0x62: fadd - Soma dois float (32-bit, 1 slot cada)
template <typename Context>
inline void fadd(Context& ctx) {
    if (ctx.size() >= 2) {
        float val1, val2;
        // Assume conversão segura do slot de 32 bits para float
        uint32_t raw1 = ctx.top(); ctx.pop();
        uint32_t raw2 = ctx.top(); ctx.pop();
        std::memcpy(&val1, &raw1, sizeof(float));
        std::memcpy(&val2, &raw2, sizeof(float));
        
        float res = val2 + val1;
        uint32_t raw_res;
        std::memcpy(&raw_res, &res, sizeof(uint32_t));
        ctx.push(raw_res);
    }
}
// Byte 0x63: dadd - Soma dois double (64-bit, consome 4 slots na pilha de 32 bits)
template <typename Context>
inline void dadd(Context& ctx) {
    if (ctx.size() >= 4) {
        uint32_t v1_low = ctx.top(); ctx.pop();
        uint32_t v1_high = ctx.top(); ctx.pop();
        uint64_t raw_v1 = ((uint64_t)v1_high << 32) | v1_low;
        double val1;
        std::memcpy(&val1, &raw_v1, sizeof(double));

        uint32_t v2_low = ctx.top(); ctx.pop();
        uint32_t v2_high = ctx.top(); ctx.pop();
        uint64_t raw_v2 = ((uint64_t)v2_high << 32) | v2_low;
        double val2;
        std::memcpy(&val2, &raw_v2, sizeof(double));

        double result = val2 + val1;
        uint64_t raw_res;
        std::memcpy(&raw_res, &result, sizeof(uint64_t));

        ctx.push((uint32_t)(raw_res >> 32));
        ctx.push((uint32_t)(raw_res & 0xFFFFFFFF));
    }
}
// Byte 0x64: isub - Subtrai dois int (val2 - val1)
template <typename Context>
inline void isub(Context& ctx) {
    if (ctx.size() >= 2) {
        int32_t val1 = ctx.top(); ctx.pop();
        int32_t val2 = ctx.top(); ctx.pop();
        ctx.push(val2 - val1);
    }
}
// Byte 0x65: lsub - Subtrai dois long (64-bit, consome 4 slots)
template <typename Context>
inline void lsub(Context& ctx) {
    if (ctx.size() >= 4) {
        uint32_t v1_low = ctx.top(); ctx.pop();
        uint32_t v1_high = ctx.top(); ctx.pop();
        int64_t val1 = ((int64_t)v1_high << 32) | v1_low;

        uint32_t v2_low = ctx.top(); ctx.pop();
        uint32_t v2_high = ctx.top(); ctx.pop();
        int64_t val2 = ((int64_t)v2_high << 32) | v2_low;

        int64_t result = val2 - val1;

        ctx.push((uint32_t)(result >> 32));
        ctx.push((uint32_t)(result & 0xFFFFFFFF));
    }
}
// Byte 0x66: fsub - Subtrai dois float (val2 - val1)
template <typename Context>
inline void fsub(Context& ctx) {
    if (ctx.size() >= 2) {
        float val1, val2;
        uint32_t raw1 = ctx.top(); ctx.pop();
        uint32_t raw2 = ctx.top(); ctx.pop();
        std::memcpy(&val1, &raw1, sizeof(float));
        std::memcpy(&val2, &raw2, sizeof(float));
        
        float res = val2 - val1;
        uint32_t raw_res;
        std::memcpy(&raw_res, &res, sizeof(uint32_t));
        ctx.push(raw_res);
    }
}
// Byte 0x67: dsub - Subtrai dois double (64-bit, val2 - val1)
template <typename Context>
inline void dsub(Context& ctx) {
    if (ctx.size() >= 4) {
        uint32_t v1_low = ctx.top(); ctx.pop();
        uint32_t v1_high = ctx.top(); ctx.pop();
        uint64_t raw_v1 = ((uint64_t)v1_high << 32) | v1_low;
        double val1; std::memcpy(&val1, &raw_v1, sizeof(double));

        uint32_t v2_low = ctx.top(); ctx.pop();
        uint32_t v2_high = ctx.top(); ctx.pop();
        uint64_t raw_v2 = ((uint64_t)v2_high << 32) | v2_low;
        double val2; std::memcpy(&val2, &raw_v2, sizeof(double));

        double result = val2 - val1;
        uint64_t raw_res; std::memcpy(&raw_res, &result, sizeof(uint64_t));

        ctx.push((uint32_t)(raw_res >> 32));
        ctx.push((uint32_t)(raw_res & 0xFFFFFFFF));
    }
}
// Byte 0x68: imul - Multiplica dois int
template <typename Context>
inline void imul(Context& ctx) {
    if (ctx.size() >= 2) {
        int32_t val1 = ctx.top(); ctx.pop();
        int32_t val2 = ctx.top(); ctx.pop();
        ctx.push(val2 * val1);
    }
}
// Byte 0x69: lmul - Multiplica dois long (64-bit, consome 4 slots)
template <typename Context>
inline void lmul(Context& ctx) {
    if (ctx.size() >= 4) {
        uint32_t v1_low = ctx.top(); ctx.pop();
        uint32_t v1_high = ctx.top(); ctx.pop();
        int64_t val1 = ((int64_t)v1_high << 32) | v1_low;

        uint32_t v2_low = ctx.top(); ctx.pop();
        uint32_t v2_high = ctx.top(); ctx.pop();
        int64_t val2 = ((int64_t)v2_high << 32) | v2_low;

        int64_t result = val2 * val1;

        ctx.push((uint32_t)(result >> 32));
        ctx.push((uint32_t)(result & 0xFFFFFFFF));
    }
}
// Byte 0x6A: fmul - Multiplica dois float
template <typename Context>
inline void fmul(Context& ctx) {
    if (ctx.size() >= 2) {
        float val1, val2;
        uint32_t raw1 = ctx.top(); ctx.pop();
        uint32_t raw2 = ctx.top(); ctx.pop();
        std::memcpy(&val1, &raw1, sizeof(float));
        std::memcpy(&val2, &raw2, sizeof(float));
        
        float res = val2 * val1;
        uint32_t raw_res; std::memcpy(&raw_res, &res, sizeof(uint32_t));
        ctx.push(raw_res);
    }
}
// Byte 0x6B: dmul - Multiplica dois double (64-bit, consome 4 slots)
template <typename Context>
inline void dmul(Context& ctx) {
    if (ctx.size() >= 4) {
        uint32_t v1_low = ctx.top(); ctx.pop();
        uint32_t v1_high = ctx.top(); ctx.pop();
        uint64_t raw_v1 = ((uint64_t)v1_high << 32) | v1_low;
        double val1; std::memcpy(&val1, &raw_v1, sizeof(double));

        uint32_t v2_low = ctx.top(); ctx.pop();
        uint32_t v2_high = ctx.top(); ctx.pop();
        uint64_t raw_v2 = ((uint64_t)v2_high << 32) | v2_low;
        double val2; std::memcpy(&val2, &raw_v2, sizeof(double));

        double result = val2 * val1;
        uint64_t raw_res; std::memcpy(&raw_res, &result, sizeof(uint64_t));

        ctx.push((uint32_t)(raw_res >> 32));
        ctx.push((uint32_t)(raw_res & 0xFFFFFFFF));
    }
}
// Byte 0x6C: idiv - Divide dois int (val2 / val1)
template <typename Context>
inline void idiv(Context& ctx) {
    if (ctx.size() >= 2) {
        int32_t val1 = ctx.top(); ctx.pop();
        int32_t val2 = ctx.top(); ctx.pop();
        if (val1 == 0) {
            // Lança ArithmeticException na JVM
            std::cerr << "Erro: ArithmeticException (/ by zero) em idiv no PC=" << ctx.pc << "\n";
            ctx.erro_execucao = true;
            return;
        }
        ctx.push(val2 / val1);
    }
}
// Byte 0x6D: ldiv - Divide dois long (64-bit, consome 4 slots)
template <typename Context>
inline void ldiv_JVM(Context& ctx) {
    if (ctx.size() >= 4) {
        uint32_t v1_low = ctx.top(); ctx.pop();
        uint32_t v1_high = ctx.top(); ctx.pop();
        int64_t val1 = ((int64_t)v1_high << 32) | v1_low;

        uint32_t v2_low = ctx.top(); ctx.pop();
        uint32_t v2_high = ctx.top(); ctx.pop();
        int64_t val2 = ((int64_t)v2_high << 32) | v2_low;

        if (val1 == 0) {
            // Lança ArithmeticException
            std::cerr << "Erro: ArithmeticException (/ by zero) em ldiv no PC=" << ctx.pc << "\n";
            ctx.erro_execucao = true;
            return;
        }

        int64_t result = val2 / val1;

        ctx.push((uint32_t)(result >> 32));
        ctx.push((uint32_t)(result & 0xFFFFFFFF));
    }
}
// Byte 0x6E: fdiv - Divide dois float (val2 / val1)
template <typename Context>
inline void fdiv_JVM(Context& ctx) {
    if (ctx.size() >= 2) {
        float val1, val2;
        uint32_t raw1 = ctx.top(); ctx.pop();
        uint32_t raw2 = ctx.top(); ctx.pop();
        std::memcpy(&val1, &raw1, sizeof(float));
        std::memcpy(&val2, &raw2, sizeof(float));
        
        float res = val2 / val1;
        uint32_t raw_res; std::memcpy(&raw_res, &res, sizeof(uint32_t));
        ctx.push(raw_res);
    }
}
// Byte 0x6F: ddiv - Divide dois double (64-bit, consome 4 slots)
template <typename Context>
inline void ddiv(Context& ctx) {
    if (ctx.size() >= 4) {
        uint32_t v1_low = ctx.top(); ctx.pop();
        uint32_t v1_high = ctx.top(); ctx.pop();
        uint64_t raw_v1 = ((uint64_t)v1_high << 32) | v1_low;
        double val1; std::memcpy(&val1, &raw_v1, sizeof(double));

        uint32_t v2_low = ctx.top(); ctx.pop();
        uint32_t v2_high = ctx.top(); ctx.pop();
        uint64_t raw_v2 = ((uint64_t)v2_high << 32) | v2_low;
        double val2; std::memcpy(&val2, &raw_v2, sizeof(double));

        double result = val2 / val1;
        uint64_t raw_res; std::memcpy(&raw_res, &result, sizeof(uint64_t));

        ctx.push((uint32_t)(raw_res >> 32));
        ctx.push((uint32_t)(raw_res & 0xFFFFFFFF));
    }
}
// Byte 0x70: irem - Resto da divisão de dois int (val2 % val1)
template <typename Context>
inline void irem(Context& ctx) {
    if (ctx.size() >= 2) {
        int32_t val1 = ctx.top(); ctx.pop();
        int32_t val2 = ctx.top(); ctx.pop();
        if (val1 == 0) {
            // Lança ArithmeticException
            std::cerr << "Erro: ArithmeticException (/ by zero) em idiv no PC=" << ctx.pc << "\n";
            ctx.erro_execucao = true;
            return;
        }
        ctx.push(val2 % val1);
    }
}
// Byte 0x71: lrem - Resto da divisão de dois long (64-bit, consome 4 slots)
template <typename Context>
inline void lrem(Context& ctx) {
    if (ctx.size() >= 4) {
        uint32_t v1_low = ctx.top(); ctx.pop();
        uint32_t v1_high = ctx.top(); ctx.pop();
        int64_t val1 = ((int64_t)v1_high << 32) | v1_low;

        uint32_t v2_low = ctx.top(); ctx.pop();
        uint32_t v2_high = ctx.top(); ctx.pop();
        int64_t val2 = ((int64_t)v2_high << 32) | v2_low;

        if (val1 == 0) {
            // Lança ArithmeticException
            std::cerr << "Erro: ArithmeticException (/ by zero) em ldiv no PC=" << ctx.pc << "\n";
            ctx.erro_execucao = true;
            return;
        }

        int64_t result = val2 % val1;

        ctx.push((uint32_t)(result >> 32));
        ctx.push((uint32_t)(result & 0xFFFFFFFF));
    }
}
// Byte 0x72: frem - Resto da divisão de dois float
template <typename Context>
inline void frem(Context& ctx) {
    if (ctx.size() >= 2) {
        float val1, val2;
        uint32_t raw1 = ctx.top(); ctx.pop();
        uint32_t raw2 = ctx.top(); ctx.pop();
        std::memcpy(&val1, &raw1, sizeof(float));
        std::memcpy(&val2, &raw2, sizeof(float));
        
        float res = std::fmod(val2, val1);
        uint32_t raw_res; std::memcpy(&raw_res, &res, sizeof(uint32_t));
        ctx.push(raw_res);
    }
}
// Byte 0x73: drem - Resto da divisão de dois double (64-bit, consome 4 slots)
template <typename Context>
inline void drem(Context& ctx) {
    if (ctx.size() >= 4) {
        uint32_t v1_low = ctx.top(); ctx.pop();
        uint32_t v1_high = ctx.top(); ctx.pop();
        uint64_t raw_v1 = ((uint64_t)v1_high << 32) | v1_low;
        double val1; std::memcpy(&val1, &raw_v1, sizeof(double));

        uint32_t v2_low = ctx.top(); ctx.pop();
        uint32_t v2_high = ctx.top(); ctx.pop();
        uint64_t raw_v2 = ((uint64_t)v2_high << 32) | v2_low;
        double val2; std::memcpy(&val2, &raw_v2, sizeof(double));

        double result = std::fmod(val2, val1);
        uint64_t raw_res; std::memcpy(&raw_res, &result, sizeof(uint64_t));

        ctx.push((uint32_t)(raw_res >> 32));
        ctx.push((uint32_t)(raw_res & 0xFFFFFFFF));
    }
}
// Byte 0x74: ineg - Inverte o sinal de um int (32-bit, 1 slot)
template <typename Context>
inline void ineg(Context& ctx) {
    if (ctx.size() >= 1) {
        int32_t val = ctx.top(); ctx.pop();
        ctx.push(-val);
    }
}
// Byte 0x75: lneg - Inverte o sinal de um long (64-bit, consome 2 slots)
template <typename Context>
inline void lneg(Context& ctx) {
    if (ctx.size() >= 2) {
        uint32_t low = ctx.top(); ctx.pop();
        uint32_t high = ctx.top(); ctx.pop();
        int64_t val = ((int64_t)high << 32) | low;
        
        int64_t result = -val;

        ctx.push((uint32_t)(result >> 32));
        ctx.push((uint32_t)(result & 0xFFFFFFFF));
    }
}
// Byte 0x76: fneg - Inverte o sinal de um float (32-bit, 1 slot)
template <typename Context>
inline void fneg(Context& ctx) {
    if (ctx.size() >= 1) {
        float val;
        uint32_t raw = ctx.top(); ctx.pop();
        std::memcpy(&val, &raw, sizeof(float));
        
        float res = -val;
        uint32_t raw_res; std::memcpy(&raw_res, &res, sizeof(uint32_t));
        ctx.push(raw_res);
    }
}
// Byte 0x77: dneg - Inverte o sinal de um double (64-bit, consome 2 slots)
template <typename Context>
inline void dneg(Context& ctx) {
    if (ctx.size() >= 2) {
        uint32_t low = ctx.top(); ctx.pop();
        uint32_t high = ctx.top(); ctx.pop();
        uint64_t raw = ((uint64_t)high << 32) | low;
        double val; std::memcpy(&val, &raw, sizeof(double));

        double res = -val;
        uint64_t raw_res; std::memcpy(&raw_res, &res, sizeof(uint64_t));

        ctx.push((uint32_t)(raw_res >> 32));
        ctx.push((uint32_t)(raw_res & 0xFFFFFFFF));
    }
}
// Byte 0x78: ishl - Shift à esquerda de int (value2 << value1)
template <typename Context>
inline void ishl(Context& ctx) {
    if (ctx.size() >= 2) {
        int32_t val1 = ctx.top(); ctx.pop(); // distância (32-bit)
        int32_t val2 = ctx.top(); ctx.pop(); // valor
        ctx.push(val2 << (val1 & 0x1F));
    }
}
// Byte 0x79: lshl - Shift à esquerda de long (consome 3 slots: long embaixox2, int topo)
template <typename Context>
inline void lshl(Context& ctx) {
    if (ctx.size() >= 3) {
        int32_t val1 = ctx.top(); ctx.pop(); // distância (int)
        
        uint32_t v2_low = ctx.top(); ctx.pop();
        uint32_t v2_high = ctx.top(); ctx.pop();
        int64_t val2 = ((int64_t)v2_high << 32) | v2_low; // valor (long)

        int64_t result = val2 << (val1 & 0x3F);

        ctx.push((uint32_t)(result >> 32));
        ctx.push((uint32_t)(result & 0xFFFFFFFF));
    }
}
// Byte 0x7A: ishr - Shift aritmético à direita de int (value2 >> value1)
template <typename Context>
inline void ishr(Context& ctx) {
    if (ctx.size() >= 2) {
        int32_t val1 = ctx.top(); ctx.pop();
        int32_t val2 = ctx.top(); ctx.pop();
        ctx.push(val2 >> (val1 & 0x1F));
    }
}
// Byte 0x7B: lshr - Shift aritmético à direita de long (consome 3 slots)
template <typename Context>
inline void lshr(Context& ctx) {
    if (ctx.size() >= 3) {
        int32_t val1 = ctx.top(); ctx.pop();
        
        uint32_t v2_low = ctx.top(); ctx.pop();
        uint32_t v2_high = ctx.top(); ctx.pop();
        int64_t val2 = ((int64_t)v2_high << 32) | v2_low;

        int64_t result = val2 >> (val1 & 0x3F);

        ctx.push((uint32_t)(result >> 32));
        ctx.push((uint32_t)(result & 0xFFFFFFFF));
    }
}
// Byte 0x7C: iushr - Shift lógico/sem sinal à direita de int (unsigned)
template <typename Context>
inline void iushr(Context& ctx) {
    if (ctx.size() >= 2) {
        int32_t val1 = ctx.top(); ctx.pop();
        uint32_t val2 = (uint32_t)ctx.top(); ctx.pop();
        ctx.push((int32_t)(val2 >> (val1 & 0x1F)));
    }
}
// Byte 0x7D: lushr - Shift lógico/sem sinal à direita de long (consome 3 slots)
template <typename Context>
inline void lushr(Context& ctx) {
    if (ctx.size() >= 3) {
        int32_t val1 = ctx.top(); ctx.pop();
        
        uint32_t v2_low = ctx.top(); ctx.pop();
        uint32_t v2_high = ctx.top(); ctx.pop();
        uint64_t val2 = ((uint64_t)v2_high << 32) | v2_low;

        uint64_t result = val2 >> (val1 & 0x3F);

        ctx.push((uint32_t)(result >> 32));
        ctx.push((uint32_t)(result & 0xFFFFFFFF));
    }
}
// Byte 0x7E: iand - AND bitwise entre dois int
template <typename Context>
inline void iand(Context& ctx) {
    if (ctx.size() >= 2) {
        int32_t val1 = ctx.top(); ctx.pop();
        int32_t val2 = ctx.top(); ctx.pop();
        ctx.push(val2 & val1);
    }
}
// Byte 0x7F: land - AND bitwise entre dois long (64-bit, consome 4 slots)
template <typename Context>
inline void land(Context& ctx) {
    if (ctx.size() >= 4) {
        uint32_t v1_low = ctx.top(); ctx.pop();
        uint32_t v1_high = ctx.top(); ctx.pop();
        int64_t val1 = ((int64_t)v1_high << 32) | v1_low;

        uint32_t v2_low = ctx.top(); ctx.pop();
        uint32_t v2_high = ctx.top(); ctx.pop();
        int64_t val2 = ((int64_t)v2_high << 32) | v2_low;

        int64_t result = val2 & val1;

        ctx.push((uint32_t)(result >> 32));
        ctx.push((uint32_t)(result & 0xFFFFFFFF));
    }
}
// Byte 0x80: ior - OR inclusivo bitwise entre dois int
template <typename Context>
inline void ior(Context& ctx) {
    if (ctx.size() >= 2) {
        int32_t val1 = ctx.top(); ctx.pop();
        int32_t val2 = ctx.top(); ctx.pop();
        ctx.push(val2 | val1);
    }
}
// Byte 0x81: lor - OR inclusivo bitwise entre dois long (64-bit, consome 4 slots)
template <typename Context>
inline void lor(Context& ctx) {
    if (ctx.size() >= 4) {
        uint32_t v1_low = ctx.top(); ctx.pop();
        uint32_t v1_high = ctx.top(); ctx.pop();
        int64_t val1 = ((int64_t)v1_high << 32) | v1_low;

        uint32_t v2_low = ctx.top(); ctx.pop();
        uint32_t v2_high = ctx.top(); ctx.pop();
        int64_t val2 = ((int64_t)v2_high << 32) | v2_low;

        int64_t result = val2 | val1;

        ctx.push((uint32_t)(result >> 32));
        ctx.push((uint32_t)(result & 0xFFFFFFFF));
    }
}
// Byte 0x82: ixor - XOR (ou exclusivo) bitwise entre dois int
template <typename Context>
inline void ixor(Context& ctx) {
    if (ctx.size() >= 2) {
        int32_t val1 = ctx.top(); ctx.pop();
        int32_t val2 = ctx.top(); ctx.pop();
        ctx.push(val2 ^ val1);
    }
}
// Byte 0x83: lxor - XOR bitwise entre dois long (64-bit, consome 4 slots)
template <typename Context>
inline void lxor(Context& ctx) {
    if (ctx.size() >= 4) {
        uint32_t v1_low = ctx.top(); ctx.pop();
        uint32_t v1_high = ctx.top(); ctx.pop();
        int64_t val1 = ((int64_t)v1_high << 32) | v1_low;

        uint32_t v2_low = ctx.top(); ctx.pop();
        uint32_t v2_high = ctx.top(); ctx.pop();
        int64_t val2 = ((int64_t)v2_high << 32) | v2_low;

        int64_t result = val2 ^ val1;

        ctx.push((uint32_t)(result >> 32));
        ctx.push((uint32_t)(result & 0xFFFFFFFF));
    }
}
// Byte 0x84: iinc - Incrementa uma variável local do tipo int por uma constante imediata
template <typename Context>
inline void iinc(Context& ctx) {
    // 1. Lê o índice sem sinal (0 a 255) e avança o PC
    uint8_t index = ctx.code[ctx.pc];ctx.pc++;
    // 2. Lê a constante com sinal (-128 a 127) e avança o PC
    int8_t b1 = static_cast<int8_t>(ctx.code[ctx.pc]);ctx.pc++;
    // 3. Extensão de sinal de 8 bits para 32 bits
    int32_t constante = static_cast<int32_t>(b1);
    // 4. Soma diretamente na variável local apontada por 'index' (sem tocar na pilha!)
    ctx.set_local(index, ctx.get_local(index) + 1);
}
// Byte 0x85: i2l - Converte int (1 slot) para long (2 slots)
template <typename Context>
inline void i2l(Context& ctx) {
    if (ctx.size() >= 1) {
        int32_t val = (int32_t)ctx.top(); ctx.pop();
        int64_t res = (int64_t)val;
        ctx.push((uint32_t)(res >> 32));
        ctx.push((uint32_t)(res & 0xFFFFFFFF));
    }
}
// Byte 0x86: i2f - Converte int (1 slot) para float (1 slot)
template <typename Context>
inline void i2f(Context& ctx) {
    if (ctx.size() >= 1) {
        int32_t val = (int32_t)ctx.top(); ctx.pop();
        float res = (float)val;
        uint32_t raw; std::memcpy(&raw, &res, sizeof(uint32_t));
        ctx.push(raw);
    }
}
// Byte 0x87: i2d - Converte int (1 slot) para double (2 slots)
template <typename Context>
inline void i2d(Context& ctx) {
    if (ctx.size() >= 1) {
        int32_t val = (int32_t)ctx.top(); ctx.pop();
        double res = (double)val;
        uint64_t raw; std::memcpy(&raw, &res, sizeof(uint64_t));
        ctx.push((uint32_t)(raw >> 32));
        ctx.push((uint32_t)(raw & 0xFFFFFFFF));
    }
}
// Byte 0x88: l2i - Converte long (2 slots) para int (1 slot)
template <typename Context>
inline void l2i(Context& ctx) {
    if (ctx.size() >= 2) {
        uint32_t low = ctx.top(); ctx.pop();
        uint32_t high = ctx.top(); ctx.pop();
        int64_t val = ((int64_t)high << 32) | low;
        int32_t res = (int32_t)val;
        ctx.push(res);
    }
}
// Byte 0x89: l2f - Converte long (2 slots) para float (1 slot)
template <typename Context>
inline void l2f(Context& ctx) {
    if (ctx.size() >= 2) {
        uint32_t low = ctx.top(); ctx.pop();
        uint32_t high = ctx.top(); ctx.pop();
        int64_t val = ((int64_t)high << 32) | low;
        float res = (float)val;
        uint32_t raw; std::memcpy(&raw, &res, sizeof(uint32_t));
        ctx.push(raw);
    }
}
// Byte 0x8A: l2d - Converte long (2 slots) para double (2 slots)
template <typename Context>
inline void l2d(Context& ctx) {
    if (ctx.size() >= 2) {
        uint32_t low = ctx.top(); ctx.pop();
        uint32_t high = ctx.top(); ctx.pop();
        int64_t val = ((int64_t)high << 32) | low;
        double res = (double)val;
        uint64_t raw; std::memcpy(&raw, &res, sizeof(uint64_t));
        ctx.push((uint32_t)(raw >> 32));
        ctx.push((uint32_t)(raw & 0xFFFFFFFF));
    }
}
// Byte 0x8B: f2i - Converte float (1 slot) para int (1 slot)
template <typename Context>
inline void f2i(Context& ctx) {
    if (ctx.size() >= 1) {
        float val;
        uint32_t raw = ctx.top(); ctx.pop();
        std::memcpy(&val, &raw, sizeof(float));
        int32_t res = (int32_t)val;
        ctx.push(res);
    }
}
// Byte 0x8C: f2l - Converte float (1 slot) para long (2 slots)
template <typename Context>
inline void f2l(Context& ctx) {
    if (ctx.size() >= 1) {
        float val;
        uint32_t raw = ctx.top(); ctx.pop();
        std::memcpy(&val, &raw, sizeof(float));
        int64_t res = (int64_t)val;
        ctx.push((uint32_t)(res >> 32));
        ctx.push((uint32_t)(res & 0xFFFFFFFF));
    }
}
// Byte 0x8D: f2d - Converte float (1 slot) para double (2 slots)
template <typename Context>
inline void f2d(Context& ctx) {
    if (ctx.size() >= 1) {
        float val;
        uint32_t raw = ctx.top(); ctx.pop();
        std::memcpy(&val, &raw, sizeof(float));
        double res = (double)val;
        uint64_t raw_res; std::memcpy(&raw_res, &res, sizeof(uint64_t));
        ctx.push((uint32_t)(raw_res >> 32));
        ctx.push((uint32_t)(raw_res & 0xFFFFFFFF));
    }
}
// Byte 0x8E: d2i - Converte double (2 slots) para int (1 slot)
template <typename Context>
inline void d2i(Context& ctx) {
    if (ctx.size() >= 2) {
        uint32_t low = ctx.top(); ctx.pop();
        uint32_t high = ctx.top(); ctx.pop();
        uint64_t raw = ((uint64_t)high << 32) | low;
        double val; std::memcpy(&val, &raw, sizeof(double));
        int32_t res = (int32_t)val;
        ctx.push(res);
    }
}
// Byte 0x8F: d2l - Converte double (2 slots) para long (2 slots)
template <typename Context>
inline void d2l(Context& ctx) {
    if (ctx.size() >= 2) {
        uint32_t low = ctx.top(); ctx.pop();
        uint32_t high = ctx.top(); ctx.pop();
        uint64_t raw = ((uint64_t)high << 32) | low;
        double val; std::memcpy(&val, &raw, sizeof(double));
        int64_t res = (int64_t)val;
        ctx.push((uint32_t)(res >> 32));
        ctx.push((uint32_t)(res & 0xFFFFFFFF));
    }
}
// Byte 0x90: d2f - Converte double (2 slots) para float (1 slot)
template <typename Context>
inline void d2f(Context& ctx) {
    if (ctx.size() >= 2) {
        uint32_t low = ctx.top(); ctx.pop();
        uint32_t high = ctx.top(); ctx.pop();
        uint64_t raw = ((uint64_t)high << 32) | low;
        double val; std::memcpy(&val, &raw, sizeof(double));
        float res = (float)val;
        uint32_t raw_res; std::memcpy(&raw_res, &res, sizeof(uint32_t));
        ctx.push(raw_res);
    }
}
// Byte 0x91: i2b - Trunca int para byte (com sinal) e empilha como int
template <typename Context>
inline void i2b(Context& ctx) {
    if (ctx.size() >= 1) {
        int32_t val = (int32_t)ctx.top(); ctx.pop();
        int32_t res = (int32_t)(int8_t)val;
        ctx.push(res);
    }
}
// Byte 0x92: i2c - Converte int para char (sem sinal, 16 bits) e empilha como int
template <typename Context>
inline void i2c(Context& ctx) {
    if (ctx.size() >= 1) {
        int32_t val = (int32_t)ctx.top(); ctx.pop();
        int32_t res = (int32_t)(uint16_t)val;
        ctx.push(res);
    }
}
// Byte 0x93: i2s - Trunca int para short (com sinal, 16 bits) e empilha como int
template <typename Context>
inline void i2s(Context& ctx) {
    if (ctx.size() >= 1) {
        int32_t val = (int32_t)ctx.top(); ctx.pop();
        int32_t res = (int32_t)(int16_t)val;
        ctx.push(res);
    }
}
// Byte 0x94: lcmp - Compara dois long (consome 4 slots) e empilha -1, 0 ou 1
template <typename Context>
inline void lcmp(Context& ctx) {
    if (ctx.size() >= 4) {
        uint32_t v1_low = ctx.top(); ctx.pop();
        uint32_t v1_high = ctx.top(); ctx.pop();
        int64_t val1 = ((int64_t)v1_high << 32) | v1_low;

        uint32_t v2_low = ctx.top(); ctx.pop();
        uint32_t v2_high = ctx.top(); ctx.pop();
        int64_t val2 = ((int64_t)v2_high << 32) | v2_low;

        if (val2 > val1) ctx.push(1);
        else if (val2 == val1) ctx.push(0);
        else ctx.push(-1);
    }
}
// Byte 0x95: fcmpl - Compara dois float (consome 2 slots), NaN empilha -1
template <typename Context>
inline void fcmpl(Context& ctx) {
    if (ctx.size() >= 2) {
        float val1, val2;
        uint32_t raw1 = ctx.top(); ctx.pop();
        uint32_t raw2 = ctx.top(); ctx.pop();
        std::memcpy(&val1, &raw1, sizeof(float));
        std::memcpy(&val2, &raw2, sizeof(float));

        if (std::isnan(val1) || std::isnan(val2)) {
            ctx.push(-1);
        } else if (val2 > val1) {
            ctx.push(1);
        } else if (val2 == val1) {
            ctx.push(0);
        } else {
            ctx.push(-1);
        }
    }
}
// Byte 0x96: fcmpg - Compara dois float (consome 2 slots), NaN empilha 1
template <typename Context>
inline void fcmpg(Context& ctx) {
    if (ctx.size() >= 2) {
        float val1, val2;
        uint32_t raw1 = ctx.top(); ctx.pop();
        uint32_t raw2 = ctx.top(); ctx.pop();
        std::memcpy(&val1, &raw1, sizeof(float));
        std::memcpy(&val2, &raw2, sizeof(float));

        if (std::isnan(val1) || std::isnan(val2)) {
            ctx.push(1);
        } else if (val2 > val1) {
            ctx.push(1);
        } else if (val2 == val1) {
            ctx.push(0);
        } else {
            ctx.push(-1);
        }
    }
}
// Byte 0x97: dcmpl - Compara dois double (consome 4 slots), NaN empilha -1
template <typename Context>
inline void dcmpl(Context& ctx) {
    if (ctx.size() >= 4) {
        uint32_t v1_low = ctx.top(); ctx.pop();
        uint32_t v1_high = ctx.top(); ctx.pop();
        uint64_t raw_v1 = ((uint64_t)v1_high << 32) | v1_low;
        double val1; std::memcpy(&val1, &raw_v1, sizeof(double));

        uint32_t v2_low = ctx.top(); ctx.pop();
        uint32_t v2_high = ctx.top(); ctx.pop();
        uint64_t raw_v2 = ((uint64_t)v2_high << 32) | v2_low;
        double val2; std::memcpy(&val2, &raw_v2, sizeof(double));

        if (std::isnan(val1) || std::isnan(val2)) {
            ctx.push(-1);
        } else if (val2 > val1) {
            ctx.push(1);
        } else if (val2 == val1) {
            ctx.push(0);
        } else {
            ctx.push(-1);
        }
    }
}
// Byte 0x98: dcmpg - Compara dois double (consome 4 slots), NaN empilha 1
template <typename Context>
inline void dcmpg(Context& ctx) {
    if (ctx.size() >= 4) {
        uint32_t v1_low = ctx.top(); ctx.pop();
        uint32_t v1_high = ctx.top(); ctx.pop();
        uint64_t raw_v1 = ((uint64_t)v1_high << 32) | v1_low;
        double val1; std::memcpy(&val1, &raw_v1, sizeof(double));

        uint32_t v2_low = ctx.top(); ctx.pop();
        uint32_t v2_high = ctx.top(); ctx.pop();
        uint64_t raw_v2 = ((uint64_t)v2_high << 32) | v2_low;
        double val2; std::memcpy(&val2, &raw_v2, sizeof(double));

        if (std::isnan(val1) || std::isnan(val2)) {
            ctx.push(1);
        } else if (val2 > val1) {
            ctx.push(1);
        } else if (val2 == val1) {
            ctx.push(0);
        } else {
            ctx.push(-1);
        }
    }
}
// Byte 0x99: ifeq - Desvia se o int no topo for igual a 0 (== 0)
template <typename Context>
inline void ifeq(Context& ctx) {
    if (ctx.size() >= 1) {
        int32_t val = ctx.top(); ctx.pop();
        int32_t branch_pc = ctx.pc - 1; // Posição do opcode ifeq
        
        int16_t high = ctx.code[ctx.pc]; ctx.pc++; // ou ctx.code[ctx.pc++]
        int16_t low = ctx.code[ctx.pc];  ctx.pc++; // ou ctx.code[ctx.pc++]
        int16_t offset = (high << 8) | low;

        if (val == 0) {
            ctx.pc = branch_pc + offset;
        }
    }
}
// Byte 0x9A: ifne - Desvia se o int no topo for diferente de 0 (!= 0)
template <typename Context>
inline void ifne(Context& ctx) {
    if (ctx.size() >= 1) {
        int32_t val = ctx.top(); ctx.pop();
        int32_t branch_pc = ctx.pc - 1;
        
        int16_t high = ctx.code[ctx.pc]; ctx.pc++;
        int16_t low = ctx.code[ctx.pc];  ctx.pc++;
        int16_t offset = (high << 8) | low;

        if (val != 0) {
            ctx.pc = branch_pc + offset;
        }
    }
}
// Byte 0x9B: iflt - Desvia se o int no topo for menor que 0 (< 0)
template <typename Context>
inline void iflt(Context& ctx) {
    if (ctx.size() >= 1) {
        int32_t val = ctx.top(); ctx.pop();
        int32_t branch_pc = ctx.pc - 1;
        
        int16_t high = ctx.code[ctx.pc]; ctx.pc++;
        int16_t low = ctx.code[ctx.pc];  ctx.pc++;
        int16_t offset = (high << 8) | low;

        if (val < 0) {
            ctx.pc = branch_pc + offset;
        }
    }
}
// Byte 0x9C: ifge - Desvia se o int no topo for maior ou igual a 0 (>= 0)
template <typename Context>
inline void ifge(Context& ctx) {
    if (ctx.size() >= 1) {
        int32_t val = ctx.top(); ctx.pop();
        int32_t branch_pc = ctx.pc - 1;
        
        int16_t high = ctx.code[ctx.pc]; ctx.pc++;
        int16_t low = ctx.code[ctx.pc];  ctx.pc++;
        int16_t offset = (high << 8) | low;

        if (val >= 0) {
            ctx.pc = branch_pc + offset;
        }
    }
}
// Byte 0x9D: ifgt - Desvia se o int no topo for maior que 0 (> 0)
template <typename Context>
inline void ifgt(Context& ctx) {
    if (ctx.size() >= 1) {
        int32_t val = ctx.top(); ctx.pop();
        int32_t branch_pc = ctx.pc - 1;
        
        int16_t high = ctx.code[ctx.pc]; ctx.pc++;
        int16_t low = ctx.code[ctx.pc];  ctx.pc++;
        int16_t offset = (high << 8) | low;

        if (val > 0) {
            ctx.pc = branch_pc + offset;
        }
    }
}
// Byte 0x9E: ifle - Desvia se o int no topo for menor ou igual a 0 (<= 0)
template <typename Context>
inline void ifle(Context& ctx) {
    if (ctx.size() >= 1) {
        int32_t val = ctx.top(); ctx.pop();
        int32_t branch_pc = ctx.pc - 1;
        
        int16_t high = ctx.code[ctx.pc]; ctx.pc++;
        int16_t low = ctx.code[ctx.pc];  ctx.pc++;
        int16_t offset = (high << 8) | low;

        if (val <= 0) {
            ctx.pc = branch_pc + offset;
        }
    }
}
// Byte 0x9F: if_icmpeq - Desvia se val2 == val1
template <typename Context>
inline void if_icmpeq(Context& ctx) {
    if (ctx.size() >= 2) {
        int32_t val1 = ctx.top(); ctx.pop();
        int32_t val2 = ctx.top(); ctx.pop();
        int32_t branch_pc = ctx.pc - 1;
        
        int16_t high = ctx.code[ctx.pc]; ctx.pc++;
        int16_t low = ctx.code[ctx.pc];  ctx.pc++;
        int16_t offset = (high << 8) | low;

        if (val2 == val1) {
            ctx.pc = branch_pc + offset;
        }
    }
}
// Byte 0xA0: if_icmpne - Desvia se val2 != val1
template <typename Context>
inline void if_icmpne(Context& ctx) {
    if (ctx.size() >= 2) {
        int32_t val1 = ctx.top(); ctx.pop();
        int32_t val2 = ctx.top(); ctx.pop();
        int32_t branch_pc = ctx.pc - 1;
        
        int16_t high = ctx.code[ctx.pc]; ctx.pc++;
        int16_t low = ctx.code[ctx.pc];  ctx.pc++;
        int16_t offset = (high << 8) | low;

        if (val2 != val1) {
            ctx.pc = branch_pc + offset;
        }
    }
}
// Byte 0xA1: if_icmplt - Desvia se val2 < val1
template <typename Context>
inline void if_icmplt(Context& ctx) {
    if (ctx.size() >= 2) {
        int32_t val1 = ctx.top(); ctx.pop();
        int32_t val2 = ctx.top(); ctx.pop();
        int32_t branch_pc = ctx.pc - 1;
        
        int16_t high = ctx.code[ctx.pc]; ctx.pc++;
        int16_t low = ctx.code[ctx.pc];  ctx.pc++;
        int16_t offset = (high << 8) | low;

        if (val2 < val1) {
            ctx.pc = branch_pc + offset;
        }
    }
}
// Byte 0xA2: if_icmpge - Desvia se val2 >= val1
template <typename Context>
inline void if_icmpge(Context& ctx) {
    if (ctx.size() >= 2) {
        int32_t val1 = ctx.top(); ctx.pop();
        int32_t val2 = ctx.top(); ctx.pop();
        int32_t branch_pc = ctx.pc - 1;
        
        int16_t high = ctx.code[ctx.pc]; ctx.pc++;
        int16_t low = ctx.code[ctx.pc];  ctx.pc++;
        int16_t offset = (high << 8) | low;

        if (val2 >= val1) {
            ctx.pc = branch_pc + offset;
        }
    }
}
// Byte 0xA3: if_icmpgt - Desvia se val2 > val1
template <typename Context>
inline void if_icmpgt(Context& ctx) {
    if (ctx.size() >= 2) {
        int32_t val1 = ctx.top(); ctx.pop();
        int32_t val2 = ctx.top(); ctx.pop();
        int32_t branch_pc = ctx.pc - 1;
        
        int16_t high = ctx.code[ctx.pc]; ctx.pc++;
        int16_t low = ctx.code[ctx.pc];  ctx.pc++;
        int16_t offset = (high << 8) | low;

        if (val2 > val1) {
            ctx.pc = branch_pc + offset;
        }
    }
}
// Byte 0xA4: if_icmple - Desvia se val2 <= val1
template <typename Context>
inline void if_icmple(Context& ctx) {
    if (ctx.size() >= 2) {
        int32_t val1 = ctx.top(); ctx.pop();
        int32_t val2 = ctx.top(); ctx.pop();
        int32_t branch_pc = ctx.pc - 1;
        
        int16_t high = ctx.code[ctx.pc]; ctx.pc++;
        int16_t low = ctx.code[ctx.pc];  ctx.pc++;
        int16_t offset = (high << 8) | low;

        if (val2 <= val1) {
            ctx.pc = branch_pc + offset;
        }
    }
}
// Byte 0xA5: if_acmpeq - Desvia se ref2 == ref1
template <typename Context>
inline void if_acmpeq(Context& ctx) {
    if (ctx.size() >= 2) {
        uint32_t val1 = ctx.top(); ctx.pop();
        uint32_t val2 = ctx.top(); ctx.pop();
        int32_t branch_pc = ctx.pc - 1;
        
        int16_t high = ctx.code[ctx.pc]; ctx.pc++;
        int16_t low = ctx.code[ctx.pc];  ctx.pc++;
        int16_t offset = (high << 8) | low;

        if (val2 == val1) {
            ctx.pc = branch_pc + offset;
        }
    }
}
// Byte 0xA6: if_acmpne - Desvia se ref2 != ref1
template <typename Context>
inline void if_acmpne(Context& ctx) {
    if (ctx.size() >= 2) {
        uint32_t val1 = ctx.top(); ctx.pop();
        uint32_t val2 = ctx.top(); ctx.pop();
        int32_t branch_pc = ctx.pc - 1;
        
        int16_t high = ctx.code[ctx.pc]; ctx.pc++;
        int16_t low = ctx.code[ctx.pc];  ctx.pc++;
        int16_t offset = (high << 8) | low;

        if (val2 != val1) {
            ctx.pc = branch_pc + offset;
        }
    }
}
// Byte 0xA7: goto - Salto incondicional de 16 bits
template <typename Context>
inline void goto_JVM(Context& ctx) {
    int32_t branch_pc = ctx.pc - 1; // Posição do opcode goto
    
    int16_t high = ctx.code[ctx.pc]; ctx.pc++;
    int16_t low = ctx.code[ctx.pc];  ctx.pc++;
    int16_t offset = (high << 8) | low;

    ctx.pc = branch_pc + offset;
}
inline int16_t ler_i16_be(const std::vector<uint8_t>& code, size_t pos) {
    uint16_t raw = static_cast<uint16_t>(
        (static_cast<uint16_t>(code[pos]) << 8) |
         static_cast<uint16_t>(code[pos + 1])
    );
    return static_cast<int16_t>(raw);
}

inline int32_t ler_i32_be(const std::vector<uint8_t>& code, size_t pos) {
    uint32_t raw = (static_cast<uint32_t>(code[pos])     << 24) |
                   (static_cast<uint32_t>(code[pos + 1]) << 16) |
                   (static_cast<uint32_t>(code[pos + 2]) << 8)  |
                    static_cast<uint32_t>(code[pos + 3]);
    return static_cast<int32_t>(raw);
}

inline bool aplicar_salto_relativo(uint32_t base_pc, int32_t offset, size_t code_size, uint32_t& out_pc) {
    int64_t alvo = static_cast<int64_t>(base_pc) + static_cast<int64_t>(offset);
    if (alvo < 0 || static_cast<uint64_t>(alvo) >= code_size) {
        return false;
    }
    out_pc = static_cast<uint32_t>(alvo);
    return true;
}
// Byte 0xA8: jsr - Jump subroutine (empilha endereco de retorno e salta para offset de 16 bits)
template <typename Context>
inline void jsr(Context& ctx) {
    // base_pc e o endereco do proprio opcode 0xA8 (ja que o loop incrementa pc apos ler o opcode)
    uint32_t base_pc = (ctx.pc > 0) ? (ctx.pc - 1) : 0;

    if (static_cast<size_t>(base_pc) + 2 >= ctx.code.size()) {
        std::cerr << "Erro: jsr sem operandos suficientes no PC=" << base_pc << "\n";
        ctx.erro_execucao = true;
        return;
    }

    int16_t branch = ler_i16_be(ctx.code, static_cast<size_t>(base_pc) + 1);
    uint32_t retorno_pc = base_pc + 3;
    ctx.push(static_cast<int32_t>(retorno_pc));
    if (ctx.erro_execucao) return;

    uint32_t novo_pc = 0;
    if (!aplicar_salto_relativo(base_pc, static_cast<int32_t>(branch), ctx.code.size(), novo_pc)) {
        std::cerr << "Erro: salto invalido em jsr no PC=" << base_pc << "\n";
        ctx.erro_execucao = true;
        return;
    }

    ctx.pc = novo_pc;
}
// Byte 0xA9: ret - Retorna de sub-rotina usando endereco salvo na variavel local [index]
template <typename Context>
inline void ret(Context& ctx) {
    uint32_t base_pc = (ctx.pc > 0) ? (ctx.pc - 1) : 0;

    if (ctx.pc >= ctx.code.size()) {
        std::cerr << "Erro: ret sem operando de indice no PC=" << base_pc << "\n";
        ctx.erro_execucao = true;
        return;
    }

    uint8_t local_idx = ctx.code[ctx.pc];
    if (static_cast<size_t>(local_idx) >= ctx.locais.size()) {
        std::cerr << "Erro: indice de variavel local invalido em ret: "
                  << static_cast<int>(local_idx) << "\n";
        ctx.erro_execucao = true;
        return;
    }

    int32_t addr = ctx.get_local(local_idx);
    if (ctx.erro_execucao || addr < 0 || static_cast<size_t>(addr) >= ctx.code.size()) {
        std::cerr << "Erro: endereco de retorno invalido em ret (" << addr << ")\n";
        ctx.erro_execucao = true;
        return;
    }

    ctx.pc = static_cast<uint32_t>(addr);
}
// Byte 0xAA: tableswitch - Switch indexado por faixa [low, high] com alinhamento de 4 bytes
template <typename Context>
inline void tableswitch(Context& ctx) {
    uint32_t base_pc = (ctx.pc > 0) ? (ctx.pc - 1) : 0;

    if (ctx.empty()) {
        std::cerr << "Erro: pilha vazia em tableswitch no PC=" << base_pc << "\n";
        ctx.erro_execucao = true;
        return;
    }

    int32_t index = ctx.pop();
    if (ctx.erro_execucao) return;

    // Padding: avanca ate o proximo endereco multiplo de 4 a partir de base_pc + 1
    size_t cursor = (static_cast<size_t>(base_pc) + 4u) & ~static_cast<size_t>(3u);
    if (cursor + 12 > ctx.code.size()) {
        std::cerr << "Erro: tableswitch truncado no cabecalho no PC=" << base_pc << "\n";
        ctx.erro_execucao = true;
        return;
    }

    int32_t default_offset = ler_i32_be(ctx.code, cursor);
    int32_t low            = ler_i32_be(ctx.code, cursor + 4);
    int32_t high           = ler_i32_be(ctx.code, cursor + 8);
    cursor += 12;

    if (high < low) {
        std::cerr << "Erro: tableswitch com high (" << high << ") < low (" << low << ")\n";
        ctx.erro_execucao = true;
        return;
    }

    int64_t count = static_cast<int64_t>(high) - static_cast<int64_t>(low) + 1LL;
    size_t table_bytes = static_cast<size_t>(count) * 4u;
    if (cursor + table_bytes > ctx.code.size()) {
        std::cerr << "Erro: tableswitch truncado na tabela de offsets no PC=" << base_pc << "\n";
        ctx.erro_execucao = true;
        return;
    }

    int32_t chosen_offset = default_offset;
    if (static_cast<int64_t>(index) >= static_cast<int64_t>(low) &&
        static_cast<int64_t>(index) <= static_cast<int64_t>(high)) {
        size_t pos_idx = static_cast<size_t>(static_cast<int64_t>(index) - static_cast<int64_t>(low));
        chosen_offset = ler_i32_be(ctx.code, cursor + pos_idx * 4u);
    }

    uint32_t novo_pc = 0;
    if (!aplicar_salto_relativo(base_pc, chosen_offset, ctx.code.size(), novo_pc)) {
        std::cerr << "Erro: salto invalido em tableswitch no PC=" << base_pc << "\n";
        ctx.erro_execucao = true;
        return;
    }

    ctx.pc = novo_pc;
}
// Byte 0xAB: lookupswitch - Switch por pares (match, offset) com alinhamento de 4 bytes
template <typename Context>
inline void lookupswitch(Context& ctx) {
    uint32_t base_pc = (ctx.pc > 0) ? (ctx.pc - 1) : 0;

    if (ctx.empty()) {
        std::cerr << "Erro: pilha vazia em lookupswitch no PC=" << base_pc << "\n";
        ctx.erro_execucao = true;
        return;
    }

    int32_t key = ctx.pop();
    if (ctx.erro_execucao) return;

    size_t cursor = (static_cast<size_t>(base_pc) + 4u) & ~static_cast<size_t>(3u);
    if (cursor + 8 > ctx.code.size()) {
        std::cerr << "Erro: lookupswitch truncado no cabecalho no PC=" << base_pc << "\n";
        ctx.erro_execucao = true;
        return;
    }

    int32_t default_offset = ler_i32_be(ctx.code, cursor);
    int32_t npairs         = ler_i32_be(ctx.code, cursor + 4);
    cursor += 8;

    if (npairs < 0) {
        std::cerr << "Erro: npairs negativo em lookupswitch (" << npairs << ")\n";
        ctx.erro_execucao = true;
        return;
    }

    size_t pairs_bytes = static_cast<size_t>(npairs) * 8u;
    if (cursor + pairs_bytes > ctx.code.size()) {
        std::cerr << "Erro: lookupswitch truncado na tabela de pares no PC=" << base_pc << "\n";
        ctx.erro_execucao = true;
        return;
    }

    int32_t chosen_offset = default_offset;
    for (int32_t i = 0; i < npairs; ++i) {
        size_t pair_pos = cursor + static_cast<size_t>(i) * 8u;
        int32_t match   = ler_i32_be(ctx.code, pair_pos);
        int32_t offset  = ler_i32_be(ctx.code, pair_pos + 4u);
        if (match == key) {
            chosen_offset = offset;
            break;
        }
    }

    uint32_t novo_pc = 0;
    if (!aplicar_salto_relativo(base_pc, chosen_offset, ctx.code.size(), novo_pc)) {
        std::cerr << "Erro: salto invalido em lookupswitch no PC=" << base_pc << "\n";
        ctx.erro_execucao = true;
        return;
    }

    ctx.pc = novo_pc;
}
// Auxiliar interno para armazenar/expor o retorno do frame atual sem poluir a pilha ativa
template <typename Context>
inline void registrar_retorno_1slot(Context& ctx, const char* mnemonico, int32_t val) {
    (void)mnemonico;
    (void)val;
    // Se o seu Context tiver um campo de retorno (ex.: ctx.retorno_pendente / ctx.frame_encerrado),
    // você pode salvá-lo aqui e marcar ctx.frame_encerrado = true.
}
template <typename Context>
inline void registrar_retorno_2slots(Context& ctx, const char* mnemonico, int32_t high_word, int32_t low_word) {
    (void)mnemonico;
    (void)high_word;
    (void)low_word;
}
// Byte 0xAC: ireturn - Retorna um int (32 bits, 1 slot) de um metodo
template <typename Context>
inline void ireturn(Context& ctx) {
    if (ctx.size() < 1) {
        std::cerr << "Erro: pilha vazia em ireturn no PC=" << ctx.pc << "\n";
        ctx.erro_execucao = true;
        return;
    }

    int32_t val = ctx.pop();
    if (ctx.erro_execucao) return;

    registrar_retorno_1slot(ctx, "ireturn", val);
    ctx.pc = static_cast<uint32_t>(ctx.code.size());
}
// Byte 0xAD: lreturn - Retorna um long (64 bits, 2 slots: high no topo-1, low no topo)
template <typename Context>
inline void lreturn(Context& ctx) {
    if (ctx.size() < 2) {
        std::cerr << "Erro: pilha insuficiente para lreturn no PC=" << ctx.pc << "\n";
        ctx.erro_execucao = true;
        return;
    }

    int32_t low_word  = ctx.pop();
    int32_t high_word = ctx.pop();
    if (ctx.erro_execucao) return;

    registrar_retorno_2slots(ctx, "lreturn", high_word, low_word);
    ctx.pc = static_cast<uint32_t>(ctx.code.size());
}
// Byte 0xAE: freturn - Retorna um float (32 bits IEEE 754, 1 slot) de um metodo
template <typename Context>
inline void freturn(Context& ctx) {
    if (ctx.size() < 1) {
        std::cerr << "Erro: pilha vazia em freturn no PC=" << ctx.pc << "\n";
        ctx.erro_execucao = true;
        return;
    }

    int32_t raw_float = ctx.pop();
    if (ctx.erro_execucao) return;

    registrar_retorno_1slot(ctx, "freturn", raw_float);
    ctx.pc = static_cast<uint32_t>(ctx.code.size());
}
// Byte 0xAF: dreturn - Retorna um double (64 bits IEEE 754, 2 slots: high no topo-1, low no topo)
template <typename Context>
inline void dreturn(Context& ctx) {
    if (ctx.size() < 2) {
        std::cerr << "Erro: pilha insuficiente para dreturn no PC=" << ctx.pc << "\n";
        ctx.erro_execucao = true;
        return;
    }

    int32_t low_word  = ctx.pop();
    int32_t high_word = ctx.pop();
    if (ctx.erro_execucao) return;

    registrar_retorno_2slots(ctx, "dreturn", high_word, low_word);
    ctx.pc = static_cast<uint32_t>(ctx.code.size());
}
// Byte 0xB0: areturn - Retorna uma referencia de objeto/array (32 bits, 1 slot) de um metodo
template <typename Context>
inline void areturn(Context& ctx) {
    if (ctx.size() < 1) {
        std::cerr << "Erro: pilha vazia em areturn no PC=" << ctx.pc << "\n";
        ctx.erro_execucao = true;
        return;
    }

    int32_t obj_ref = ctx.pop();
    if (ctx.erro_execucao) return;

    registrar_retorno_1slot(ctx, "areturn", obj_ref);
    ctx.pc = static_cast<uint32_t>(ctx.code.size());
}
// Byte 0xB1: return - Retorna de metodo void (0 slots)
template <typename Context>
inline void return_void(Context& ctx) {
    std::cout << "Instrucao 'return' (0xB1) executada. Encerrando frame.\n";
    ctx.pc = static_cast<uint32_t>(ctx.code.size());
}
inline bool ler_u16_be_seguro(const std::vector<uint8_t>& code,
                               size_t& pos,
                               const char* mnemonico,
                               bool& erro) {
    if (pos + 1 >= code.size()) {
        std::cerr << "Erro: bytes insuficientes para indice em " << mnemonico
                  << " no PC=" << pos << "\n";
        erro = true;
        return false;
    }
    return true;
}

template <typename Context>
inline bool extrair_ref_cp(Context& ctx,
                           const char* mnemonico,
                           uint16_t& cp_index,
                           std::string& classe_nome,
                           std::string& nome_membro,
                           std::string& descritor) {
    if (static_cast<size_t>(ctx.pc) + 1 >= ctx.code.size() || !ctx.classe) {
        std::cerr << "Erro: contexto invalido ou operandos faltando em " << mnemonico
                  << " no PC=" << ctx.pc << "\n";
        ctx.erro_execucao = true;
        return false;
    }

    uint16_t high = static_cast<uint16_t>(ctx.code[ctx.pc++]);
    uint16_t low  = static_cast<uint16_t>(ctx.code[ctx.pc++]);
    cp_index = static_cast<uint16_t>((high << 8) | low);

    const auto& cp = ctx.classe->constant_pool;
    if (cp_index == 0 || cp_index >= cp.size()) {
        std::cerr << "Erro: indice #" << cp_index << " fora do Constant Pool em "
                  << mnemonico << "\n";
        ctx.erro_execucao = true;
        return false;
    }

    const auto& entry = cp[cp_index];

    // CONSTANT_InvokeDynamic (tag 18): index1 = bootstrap_method_attr_index, index2 = NameAndType
    if (entry.tag == 18) {
        classe_nome = "<bootstrap#" + std::to_string(entry.index1) + ">";
        if (entry.index2 < cp.size() && cp[entry.index2].tag == 12) {
            nome_membro = resolver_cp_string(cp, cp[entry.index2].index1);
            descritor   = resolver_cp_string(cp, cp[entry.index2].index2);
        } else {
            nome_membro = resolver_cp_string(cp, entry.index2);
            descritor.clear();
        }
        return true;
    }

    classe_nome = resolver_cp_string(cp, entry.index1);
    if (entry.index2 < cp.size() && cp[entry.index2].tag == 12) {
        nome_membro = resolver_cp_string(cp, cp[entry.index2].index1);
        descritor   = resolver_cp_string(cp, cp[entry.index2].index2);
    } else {
        std::string combinado = resolver_cp_string(cp, entry.index2);
        auto pos = combinado.find(':');
        if (pos != std::string::npos) {
            nome_membro = combinado.substr(0, pos);
            descritor   = combinado.substr(pos + 1);
        } else {
            nome_membro = combinado;
            descritor.clear();
        }
    }

    return true;
}

inline bool descritor_e_64bits(const std::string& descritor) {
    auto pos = descritor.find(')');
    std::string ret = (pos != std::string::npos && pos + 1 < descritor.size())
                          ? descritor.substr(pos + 1)
                          : descritor;
    return (!ret.empty() && (ret[0] == 'J' || ret[0] == 'D'));
}

inline bool descritor_e_void(const std::string& descritor) {
    auto pos = descritor.find(')');
    std::string ret = (pos != std::string::npos && pos + 1 < descritor.size())
                          ? descritor.substr(pos + 1)
                          : descritor;
    return (!ret.empty() && ret[0] == 'V');
}

// Conta slots de argumentos a partir do descritor "(...)" (ignora o tipo de retorno apos ')')
inline int contar_slots_argumentos(const std::string& descritor) {
    int slots = 0;
    size_t i = 0;
    if (!descritor.empty() && descritor[0] == '(') {
        i = 1;
    }
    while (i < descritor.size() && descritor[i] != ')') {
        char c = descritor[i];
        if (c == 'J' || c == 'D') {
            slots += 2;
            ++i;
        } else if (c == 'L') {
            slots += 1;
            auto ponto_virgula = descritor.find(';', i + 1);
            if (ponto_virgula == std::string::npos) break;
            i = ponto_virgula + 1;
        } else if (c == '[') {
            slots += 1;
            ++i;
            while (i < descritor.size() && descritor[i] == '[') ++i;
            if (i < descritor.size() && descritor[i] == 'L') {
                auto ponto_virgula = descritor.find(';', i + 1);
                if (ponto_virgula == std::string::npos) break;
                i = ponto_virgula + 1;
            } else if (i < descritor.size()) {
                ++i;
            }
        } else {
            slots += 1;
            ++i;
        }
    }
    return slots;
}

template <typename Context>
inline void empilhar_retorno_simbolico(Context& ctx, const std::string& descritor, int32_t ref_simbolica) {
    if (descritor_e_void(descritor)) {
        return;
    }
    if (descritor_e_64bits(descritor)) {
        ctx.push(INT32_C(0)); // high_word placeholder
        ctx.push(INT32_C(0)); // low_word placeholder
    } else {
        ctx.push(ref_simbolica);
    }
}

// Byte 0xB2: getstatic - Carrega campo estatico na pilha
template <typename Context>
inline void getstatic_JVM(Context& ctx) {
    uint16_t idx = 0;
    std::string classe_nome, nome_campo, descritor;
    if (!extrair_ref_cp(ctx, "getstatic", idx, classe_nome, nome_campo, descritor)) return;

    std::cout << "[getstatic #" << idx << "] " << classe_nome << "." << nome_campo
              << (descritor.empty() ? "" : (":" + descritor)) << "\n";

    // Referencia simbolica padrao (ex.: System.out ou campo estatico de 1/2 slots)
    empilhar_retorno_simbolico(ctx, descritor.empty() ? "L" : descritor, INT32_C(-1));
}

// Byte 0xB3: putstatic - Grava topo da pilha em campo estatico
template <typename Context>
inline void putstatic_JVM(Context& ctx) {
    uint16_t idx = 0;
    std::string classe_nome, nome_campo, descritor;
    if (!extrair_ref_cp(ctx, "putstatic", idx, classe_nome, nome_campo, descritor)) return;

    int slots = descritor_e_64bits(descritor) ? 2 : 1;
    if (ctx.size() < slots) {
        std::cerr << "Erro: pilha insuficiente em putstatic no PC=" << ctx.pc << "\n";
        ctx.erro_execucao = true;
        return;
    }

    for (int i = 0; i < slots; ++i) {
        (void)ctx.pop();
        if (ctx.erro_execucao) return;
    }

    std::cout << "[putstatic #" << idx << "] " << classe_nome << "." << nome_campo
              << (descritor.empty() ? "" : (":" + descritor)) << "\n";
}

// Byte 0xB4: getfield - Substitui objectref do topo pelo valor do campo de instancia
template <typename Context>
inline void getfield_JVM(Context& ctx) {
    uint16_t idx = 0;
    std::string classe_nome, nome_campo, descritor;
    if (!extrair_ref_cp(ctx, "getfield", idx, classe_nome, nome_campo, descritor)) return;

    if (ctx.size() < 1) {
        std::cerr << "Erro: pilha vazia em getfield no PC=" << ctx.pc << "\n";
        ctx.erro_execucao = true;
        return;
    }

    int32_t obj_ref = ctx.pop();
    if (ctx.erro_execucao) return;

    if (obj_ref == 0) {
        std::cerr << "Erro: NullPointerException em getfield no PC=" << ctx.pc << "\n";
        ctx.erro_execucao = true;
        return;
    }

    std::cout << "[getfield #" << idx << "] " << classe_nome << "." << nome_campo
              << (descritor.empty() ? "" : (":" + descritor)) << "\n";

    empilhar_retorno_simbolico(ctx, descritor.empty() ? "I" : descritor, INT32_C(0));
}

// Byte 0xB5: putfield - Grava valor em campo de instancia de objectref
template <typename Context>
inline void putfield_JVM(Context& ctx) {
    uint16_t idx = 0;
    std::string classe_nome, nome_campo, descritor;
    if (!extrair_ref_cp(ctx, "putfield", idx, classe_nome, nome_campo, descritor)) return;

    int val_slots = descritor_e_64bits(descritor) ? 2 : 1;
    // Precisa do valor (1 ou 2 slots) + objectref (1 slot)
    if (ctx.size() < val_slots + 1) {
        std::cerr << "Erro: pilha insuficiente em putfield no PC=" << ctx.pc << "\n";
        ctx.erro_execucao = true;
        return;
    }

    for (int i = 0; i < val_slots; ++i) {
        (void)ctx.pop();
        if (ctx.erro_execucao) return;
    }

    int32_t obj_ref = ctx.pop();
    if (ctx.erro_execucao) return;

    if (obj_ref == 0) {
        std::cerr << "Erro: NullPointerException em putfield no PC=" << ctx.pc << "\n";
        ctx.erro_execucao = true;
        return;
    }

    std::cout << "[putfield #" << idx << "] " << classe_nome << "." << nome_campo
              << (descritor.empty() ? "" : (":" + descritor)) << "\n";
}
// Auxiliar comum para chamadas com objectref (invokevirtual, invokespecial, invokeinterface)
template <typename Context>
inline void executar_chamada_de_instancia(Context& ctx, const char* mnemonico, uint16_t idx,
                                          const std::string& classe_nome,
                                          const std::string& nome_metodo,
                                          const std::string& descritor) {
    int arg_slots = contar_slots_argumentos(descritor);
    std::vector<int32_t> args_invertidos;
    args_invertidos.reserve(static_cast<size_t>(arg_slots));

    if (ctx.size() < arg_slots + 1) {
        std::cerr << "Erro: pilha insuficiente para argumentos/receptor em " << mnemonico
                  << " no PC=" << ctx.pc << "\n";
        ctx.erro_execucao = true;
        return;
    }

    for (int i = 0; i < arg_slots; ++i) {
        args_invertidos.push_back(ctx.pop());
        if (ctx.erro_execucao) return;
    }

    int32_t obj_ref = ctx.pop();
    if (ctx.erro_execucao) return;

    if (obj_ref == 0) {
        std::cerr << "Erro: NullPointerException em " << mnemonico << " ("
                  << classe_nome << "." << nome_metodo << descritor << ")\n";
        ctx.erro_execucao = true;
        return;
    }

    std::cout << "[" << mnemonico << " #" << idx << "] "
              << classe_nome << "." << nome_metodo << descritor << "\n";

    // Caso especial conhecido: PrintStream.print / println com (I)V
    if (classe_nome == "java/io/PrintStream" && descritor == "(I)V" && !args_invertidos.empty()) {
        int32_t valor = args_invertidos[0];
        if (nome_metodo == "println") {
            std::cout << "System.out => " << valor << "\n";
        } else if (nome_metodo == "print") {
            std::cout << "System.out => " << valor << "\n";
        }
        return;
    }

    empilhar_retorno_simbolico(ctx, descritor, INT32_C(0));
}
// Byte 0xB6: invokevirtual - Despacho virtual em instancia
template <typename Context>
inline void invokevirtual_JVM(Context& ctx) {
    uint16_t idx = 0;
    std::string classe_nome, nome_metodo, descritor;
    if (!extrair_ref_cp(ctx, "invokevirtual", idx, classe_nome, nome_metodo, descritor)) return;

    executar_chamada_de_instancia(ctx, "invokevirtual", idx, classe_nome, nome_metodo, descritor);
}
// Byte 0xB7: invokespecial - Construtor (<init>), metodos privados ou super
template <typename Context>
inline void invokespecial_JVM(Context& ctx) {
    uint16_t idx = 0;
    std::string classe_nome, nome_metodo, descritor;
    if (!extrair_ref_cp(ctx, "invokespecial", idx, classe_nome, nome_metodo, descritor)) return;

    executar_chamada_de_instancia(ctx, "invokespecial", idx, classe_nome, nome_metodo, descritor);
}
// Byte 0xB8: invokestatic - Metodo estatico (nao consome objectref da pilha)
template <typename Context>
inline void invokestatic_JVM(Context& ctx) {
    uint16_t idx = 0;
    std::string classe_nome, nome_metodo, descritor;
    if (!extrair_ref_cp(ctx, "invokestatic", idx, classe_nome, nome_metodo, descritor)) return;

    int arg_slots = contar_slots_argumentos(descritor);
    if (ctx.size() < arg_slots) {
        std::cerr << "Erro: pilha insuficiente para argumentos em invokestatic no PC="
                  << ctx.pc << "\n";
        ctx.erro_execucao = true;
        return;
    }

    for (int i = 0; i < arg_slots; ++i) {
        (void)ctx.pop();
        if (ctx.erro_execucao) return;
    }

    std::cout << "[invokestatic #" << idx << "] "
              << classe_nome << "." << nome_metodo << descritor << "\n";

    empilhar_retorno_simbolico(ctx, descritor, INT32_C(0));
}
// Byte 0xB9: invokeinterface - Chamada de metodo de interface (4 bytes de operandos)
template <typename Context>
inline void invokeinterface_JVM(Context& ctx) {
    uint16_t idx = 0;
    std::string classe_nome, nome_metodo, descritor;
    if (!extrair_ref_cp(ctx, "invokeinterface", idx, classe_nome, nome_metodo, descritor)) return;

    if (static_cast<size_t>(ctx.pc) + 1 >= ctx.code.size()) {
        std::cerr << "Erro: bytes finais ausentes em invokeinterface no PC=" << ctx.pc << "\n";
        ctx.erro_execucao = true;
        return;
    }

    uint8_t count = ctx.code[ctx.pc++];
    uint8_t zero_byte = ctx.code[ctx.pc++];
    (void)count;
    if (zero_byte != 0) {
        std::cerr << "Aviso: byte reservado nao-zero em invokeinterface: "
                  << static_cast<int>(zero_byte) << "\n";
    }

    executar_chamada_de_instancia(ctx, "invokeinterface", idx, classe_nome, nome_metodo, descritor);
}
// Byte 0xBA: invokedynamic - Call site dinamico (4 bytes de operandos, sem objectref obrigatorio)
template <typename Context>
inline void invokedynamic_JVM(Context& ctx) {
    uint16_t idx = 0;
    std::string bootstrap_info, nome_metodo, descritor;
    if (!extrair_ref_cp(ctx, "invokedynamic", idx, bootstrap_info, nome_metodo, descritor)) return;

    if (static_cast<size_t>(ctx.pc) + 1 >= ctx.code.size()) {
        std::cerr << "Erro: bytes finais ausentes em invokedynamic no PC=" << ctx.pc << "\n";
        ctx.erro_execucao = true;
        return;
    }

    uint8_t b3 = ctx.code[ctx.pc++];
    uint8_t b4 = ctx.code[ctx.pc++];
    if (b3 != 0 || b4 != 0) {
        std::cerr << "Aviso: bytes reservados nao-zero em invokedynamic no PC=" << ctx.pc << "\n";
    }

    int arg_slots = contar_slots_argumentos(descritor);
    if (ctx.size() < arg_slots) {
        std::cerr << "Erro: pilha insuficiente para argumentos em invokedynamic no PC="
                  << ctx.pc << "\n";
        ctx.erro_execucao = true;
        return;
    }

    for (int i = 0; i < arg_slots; ++i) {
        (void)ctx.pop();
        if (ctx.erro_execucao) return;
    }

    std::cout << "[invokedynamic #" << idx << "] " << bootstrap_info << " -> "
              << nome_metodo << descritor << "\n";

    empilhar_retorno_simbolico(ctx, descritor, INT32_C(0));
}
// Auxiliar interno para gerar uma referencia simbolica nao-nula enquanto o Heap real nao existe
template <typename Context>
inline int32_t alocar_ref_simbolica(Context& ctx, uint32_t seed) {
    (void)ctx;
    int32_t ref = static_cast<int32_t>((seed & 0x7FFFFFFFu) + 1u);
    return (ref == 0) ? INT32_C(1) : ref;
}
// Byte 0xBB: new - Instancia um objeto de uma classe do Constant Pool (2 bytes de indice)
// Nao chama <init>; apenas aloca e empilha a referencia do objeto nao inicializado.
template <typename Context>
inline void new_obj(Context& ctx) {
    if (static_cast<size_t>(ctx.pc) + 1 >= ctx.code.size() || !ctx.classe) {
        std::cerr << "Erro: contexto invalido ou operandos faltando em new no PC=" << ctx.pc << "\n";
        ctx.erro_execucao = true;
        return;
    }

    uint16_t high = static_cast<uint16_t>(ctx.code[ctx.pc++]);
    uint16_t low  = static_cast<uint16_t>(ctx.code[ctx.pc++]);
    uint16_t cp_index = static_cast<uint16_t>((high << 8) | low);

    const auto& cp = ctx.classe->constant_pool;
    if (cp_index == 0 || cp_index >= cp.size()) {
        std::cerr << "Erro: indice #" << cp_index << " fora do Constant Pool em new\n";
        ctx.erro_execucao = true;
        return;
    }

    const auto& entry = cp[cp_index];
    if (entry.tag != 7) { // CONSTANT_Class
        std::cerr << "Erro: entrada #" << cp_index << " em new nao e CONSTANT_Class (tag="
                  << static_cast<int>(entry.tag) << ")\n";
        ctx.erro_execucao = true;
        return;
    }

    std::string nome_classe = resolver_cp_string(cp, entry.index1);
    std::cout << "[new #" << cp_index << "] Classe: " << nome_classe << "\n";

    // TODO: substituir pela referencia real alocada no Heap quando o Heap existir
    ctx.push(alocar_ref_simbolica(ctx, static_cast<uint32_t>(cp_index) * 16u + ctx.pc));
}
// Byte 0xBC: newarray - Aloca array de tipo primitivo (1 byte atype + count na pilha)
template <typename Context>
inline void newarray(Context& ctx) {
    if (ctx.pc >= ctx.code.size()) {
        std::cerr << "Erro: newarray sem operando atype no PC=" << ctx.pc << "\n";
        ctx.erro_execucao = true;
        return;
    }

    uint8_t atype = ctx.code[ctx.pc++];

    if (ctx.empty()) {
        std::cerr << "Erro: pilha vazia em newarray no PC=" << ctx.pc << "\n";
        ctx.erro_execucao = true;
        return;
    }

    int32_t count = ctx.pop();
    if (ctx.erro_execucao) return;

    if (count < 0) {
        std::cerr << "Erro: NegativeArraySizeException em newarray (count=" << count
                  << ") no PC=" << ctx.pc << "\n";
        ctx.erro_execucao = true;
        return;
    }

    const char* tipo_nome = "desconhecido";
    switch (atype) {
        case 4:  tipo_nome = "boolean"; break; // T_BOOLEAN
        case 5:  tipo_nome = "char";    break; // T_CHAR
        case 6:  tipo_nome = "float";   break; // T_FLOAT
        case 7:  tipo_nome = "double";  break; // T_DOUBLE
        case 8:  tipo_nome = "byte";    break; // T_BYTE
        case 9:  tipo_nome = "short";   break; // T_SHORT
        case 10: tipo_nome = "int";     break; // T_INT
        case 11: tipo_nome = "long";    break; // T_LONG
        default:
            std::cerr << "Erro: atype invalido em newarray (" << static_cast<int>(atype)
                      << ") no PC=" << ctx.pc << "\n";
            ctx.erro_execucao = true;
            return;
    }

    std::cout << "[newarray] tipo=" << tipo_nome << " (atype=" << static_cast<int>(atype)
              << "), tamanho=" << count << "\n";

    // TODO: alocar array primitivo de tamanho `count` no Heap e empilhar sua referencia
    ctx.push(alocar_ref_simbolica(ctx, static_cast<uint32_t>(atype) * 1024u + static_cast<uint32_t>(count)));
}
// Byte 0xBD: anewarray - Aloca array de referencias de objetos/arrays (2 bytes de indice + count na pilha)
template <typename Context>
inline void anewarray(Context& ctx) {
    if (static_cast<size_t>(ctx.pc) + 1 >= ctx.code.size() || !ctx.classe) {
        std::cerr << "Erro: contexto invalido ou operandos faltando em anewarray no PC="
                  << ctx.pc << "\n";
        ctx.erro_execucao = true;
        return;
    }

    uint16_t high = static_cast<uint16_t>(ctx.code[ctx.pc++]);
    uint16_t low  = static_cast<uint16_t>(ctx.code[ctx.pc++]);
    uint16_t cp_index = static_cast<uint16_t>((high << 8) | low);

    const auto& cp = ctx.classe->constant_pool;
    if (cp_index == 0 || cp_index >= cp.size()) {
        std::cerr << "Erro: indice #" << cp_index << " fora do Constant Pool em anewarray\n";
        ctx.erro_execucao = true;
        return;
    }

    if (ctx.empty()) {
        std::cerr << "Erro: pilha vazia em anewarray no PC=" << ctx.pc << "\n";
        ctx.erro_execucao = true;
        return;
    }

    int32_t count = ctx.pop();
    if (ctx.erro_execucao) return;

    if (count < 0) {
        std::cerr << "Erro: NegativeArraySizeException em anewarray (count=" << count
                  << ") no PC=" << ctx.pc << "\n";
        ctx.erro_execucao = true;
        return;
    }

    std::string nome_elemento = resolver_cp_string(cp, cp[cp_index].index1);
    std::cout << "[anewarray #" << cp_index << "] elemento=" << nome_elemento
              << ", tamanho=" << count << "\n";

    // TODO: alocar array de referencias de tamanho `count` no Heap e empilhar sua referencia
    ctx.push(alocar_ref_simbolica(ctx, static_cast<uint32_t>(cp_index) * 256u + static_cast<uint32_t>(count)));
}
// Byte 0xBE: arraylength - Desempilha arrayref e empilha o tamanho do array (int >= 0)
template <typename Context>
inline void arraylength(Context& ctx) {
    if (ctx.empty()) {
        std::cerr << "Erro: pilha vazia em arraylength no PC=" << ctx.pc << "\n";
        ctx.erro_execucao = true;
        return;
    }

    int32_t arrayref = ctx.pop();
    if (ctx.erro_execucao) return;

    if (arrayref == 0) {
        std::cerr << "Erro: NullPointerException em arraylength no PC=" << ctx.pc << "\n";
        ctx.erro_execucao = true;
        return;
    }

    // TODO: ler o tamanho real do array em heap[arrayref].size() quando o Heap existir
    int32_t length = INT32_C(0);
    std::cout << "[arraylength] arrayref=" << arrayref << " -> length=" << length << "\n";
    ctx.push(length);
}
// Byte 0xBF: athrow - Desempilha referencia de excecao e lanca/propaga a excecao
template <typename Context>
inline void athrow(Context& ctx) {
    if (ctx.empty()) {
        std::cerr << "Erro: pilha vazia em athrow no PC=" << ctx.pc << "\n";
        ctx.erro_execucao = true;
        return;
    }

    int32_t exc_ref = ctx.pop();
    if (ctx.erro_execucao) return;

    if (exc_ref == 0) {
        std::cerr << "Erro: NullPointerException lancada por athrow (referencia nula) no PC="
                  << ctx.pc << "\n";
        ctx.erro_execucao = true;
        return;
    }

    // TODO: quando houver exception_table no metodo, procurar handler compativel;
    // se nao houver (ou ainda sem tabela de excecoes), limpa a pilha e encerra com erro.
    while (!ctx.empty() && !ctx.erro_execucao) {
        (void)ctx.pop();
    }
    ctx.push(exc_ref);

    std::cerr << "Excecao nao tratada lancada por athrow (ref=" << exc_ref
              << ") no PC=" << ctx.pc << "\n";
    ctx.erro_execucao = true;
}
// Byte 0xC0: checkcast - Verifica se objectref (no topo) e compativel com o tipo do CP.
// Na JVM, nao remove objectref da pilha (apenas lanca ClassCastException se falhar).
template <typename Context>
inline void checkcast(Context& ctx) {
    if (static_cast<size_t>(ctx.pc) + 1 >= ctx.code.size() || !ctx.classe) {
        std::cerr << "Erro: contexto invalido ou operandos faltando em checkcast no PC="
                  << ctx.pc << "\n";
        ctx.erro_execucao = true;
        return;
    }

    uint16_t high = static_cast<uint16_t>(ctx.code[ctx.pc++]);
    uint16_t low  = static_cast<uint16_t>(ctx.code[ctx.pc++]);
    uint16_t cp_index = static_cast<uint16_t>((high << 8) | low);

    const auto& cp = ctx.classe->constant_pool;
    if (cp_index == 0 || cp_index >= cp.size()) {
        std::cerr << "Erro: indice #" << cp_index << " fora do Constant Pool em checkcast\n";
        ctx.erro_execucao = true;
        return;
    }

    if (ctx.empty()) {
        std::cerr << "Erro: pilha vazia em checkcast no PC=" << ctx.pc << "\n";
        ctx.erro_execucao = true;
        return;
    }

    int32_t obj_ref = ctx.top();
    std::string tipo_alvo = resolver_cp_string(cp, cp[cp_index].index1);

    // Em Java, null passa no checkcast sem erro
    if (obj_ref == 0) {
        return;
    }

    // TODO: validar tipo real de obj_ref no Heap contra tipo_alvo (ClassCastException se incompativel)
    std::cout << "[checkcast #" << cp_index << "] ref=" << obj_ref
              << " como " << tipo_alvo << "\n";
}
// Byte 0xC1: instanceof - Desempilha objectref e empilha 1 (true) ou 0 (false).
// Se objectref == 0 (null), empilha 0 sem erro.
template <typename Context>
inline void instanceof_JVM(Context& ctx) {
    if (static_cast<size_t>(ctx.pc) + 1 >= ctx.code.size() || !ctx.classe) {
        std::cerr << "Erro: contexto invalido ou operandos faltando em instanceof no PC="
                  << ctx.pc << "\n";
        ctx.erro_execucao = true;
        return;
    }

    uint16_t high = static_cast<uint16_t>(ctx.code[ctx.pc++]);
    uint16_t low  = static_cast<uint16_t>(ctx.code[ctx.pc++]);
    uint16_t cp_index = static_cast<uint16_t>((high << 8) | low);

    const auto& cp = ctx.classe->constant_pool;
    if (cp_index == 0 || cp_index >= cp.size()) {
        std::cerr << "Erro: indice #" << cp_index << " fora do Constant Pool em instanceof\n";
        ctx.erro_execucao = true;
        return;
    }

    if (ctx.empty()) {
        std::cerr << "Erro: pilha vazia em instanceof no PC=" << ctx.pc << "\n";
        ctx.erro_execucao = true;
        return;
    }

    int32_t obj_ref = ctx.pop();
    if (ctx.erro_execucao) return;

    if (obj_ref == 0) {
        ctx.push(INT32_C(0));
        return;
    }

    std::string tipo_alvo = resolver_cp_string(cp, cp[cp_index].index1);
    // TODO: verificar no Heap se obj_ref e instancia de tipo_alvo
    std::cout << "[instanceof #" << cp_index << "] ref=" << obj_ref
              << " instanceof " << tipo_alvo << "\n";
    ctx.push(INT32_C(1));
}
// Byte 0xC2: monitorenter - Adquire monitor de objectref (lanca NullPointerException se 0)
template <typename Context>
inline void monitorenter(Context& ctx) {
    if (ctx.empty()) {
        std::cerr << "Erro: pilha vazia em monitorenter no PC=" << ctx.pc << "\n";
        ctx.erro_execucao = true;
        return;
    }

    int32_t obj_ref = ctx.pop();
    if (ctx.erro_execucao) return;

    if (obj_ref == 0) {
        std::cerr << "Erro: NullPointerException em monitorenter no PC=" << ctx.pc << "\n";
        ctx.erro_execucao = true;
        return;
    }

    std::cout << "[monitorenter] lock em ref=" << obj_ref << "\n";
}
// Byte 0xC3: monitorexit - Libera monitor de objectref (lanca NullPointerException se 0)
template <typename Context>
inline void monitorexit(Context& ctx) {
    if (ctx.empty()) {
        std::cerr << "Erro: pilha vazia em monitorexit no PC=" << ctx.pc << "\n";
        ctx.erro_execucao = true;
        return;
    }

    int32_t obj_ref = ctx.pop();
    if (ctx.erro_execucao) return;

    if (obj_ref == 0) {
        std::cerr << "Erro: NullPointerException em monitorexit no PC=" << ctx.pc << "\n";
        ctx.erro_execucao = true;
        return;
    }

    std::cout << "[monitorexit] unlock em ref=" << obj_ref << "\n";
}
// Byte 0xC4: wide - Estende indice de variavel local para 16 bits (e constante para 16 bits em iinc)
template <typename Context>
inline void wide(Context& ctx) {
    uint32_t base_pc = (ctx.pc > 0) ? (ctx.pc - 1) : 0;

    if (ctx.pc >= ctx.code.size()) {
        std::cerr << "Erro: wide sem sub-opcode no PC=" << base_pc << "\n";
        ctx.erro_execucao = true;
        return;
    }

    uint8_t sub_op = ctx.code[ctx.pc++];

    // Caso especial: wide iinc (0x84) -> indexbyte1, indexbyte2, constbyte1, constbyte2
    if (sub_op == 0x84) {
        if (static_cast<size_t>(ctx.pc) + 3 >= ctx.code.size()) {
            std::cerr << "Erro: wide iinc truncado no PC=" << base_pc << "\n";
            ctx.erro_execucao = true;
            return;
        }
        uint16_t idx = static_cast<uint16_t>(
            (static_cast<uint16_t>(ctx.code[ctx.pc]) << 8) |
             static_cast<uint16_t>(ctx.code[ctx.pc + 1])
        );
        int16_t inc = ler_i16_be(ctx.code, static_cast<size_t>(ctx.pc) + 2);
        ctx.pc += 4;

        if (static_cast<size_t>(idx) >= ctx.locais.size()) {
            std::cerr << "Erro: indice local fora dos limites em wide iinc: " << idx << "\n";
            ctx.erro_execucao = true;
            return;
        }
        ctx.locais[idx] += static_cast<int32_t>(inc);
        return;
    }

    // Demais sub-opcodes com indice de 2 bytes:
    // iload(0x15), fload(0x17), aload(0x19), lload(0x16), dload(0x18),
    // istore(0x36), fstore(0x38), astore(0x3A), lstore(0x37), dstore(0x39), ret(0xA9)
    if (static_cast<size_t>(ctx.pc) + 1 >= ctx.code.size()) {
        std::cerr << "Erro: wide sem indice de 2 bytes no PC=" << base_pc << "\n";
        ctx.erro_execucao = true;
        return;
    }

    uint16_t idx = static_cast<uint16_t>(
        (static_cast<uint16_t>(ctx.code[ctx.pc]) << 8) |
         static_cast<uint16_t>(ctx.code[ctx.pc + 1])
    );
    ctx.pc += 2;

    if (static_cast<size_t>(idx) >= ctx.locais.size()) {
        std::cerr << "Erro: indice local fora dos limites em wide: " << idx << "\n";
        ctx.erro_execucao = true;
        return;
    }

    switch (sub_op) {
        case 0x15: // iload
        case 0x17: // fload
        case 0x19: // aload
            ctx.push(ctx.locais[idx]);
            break;

        case 0x16: // lload (2 slots: high depois low)
        case 0x18: // dload (2 slots: high depois low)
            if (static_cast<size_t>(idx) + 1 >= ctx.locais.size()) {
                std::cerr << "Erro: indice local de 2 slots fora dos limites em wide load\n";
                ctx.erro_execucao = true;
                return;
            }
            ctx.push(ctx.locais[idx]);
            ctx.push(ctx.locais[idx + 1]);
            break;

        case 0x36: // istore
        case 0x38: // fstore
        case 0x3A: // astore
            if (ctx.empty()) {
                std::cerr << "Erro: pilha vazia em wide store\n";
                ctx.erro_execucao = true;
                return;
            }
            ctx.locais[idx] = ctx.pop();
            break;

        case 0x37: // lstore (2 slots: topo e low, proximo e high)
        case 0x39: // dstore
            if (ctx.size() < 2 || static_cast<size_t>(idx) + 1 >= ctx.locais.size()) {
                std::cerr << "Erro: pilha ou variaveis locais insuficientes em wide store 2 slots\n";
                ctx.erro_execucao = true;
                return;
            }
            ctx.locais[idx + 1] = ctx.pop(); // low
            ctx.locais[idx]     = ctx.pop(); // high
            break;

        case 0xA9: { // ret
            int32_t addr = ctx.locais[idx];
            if (addr < 0 || static_cast<size_t>(addr) >= ctx.code.size()) {
                std::cerr << "Erro: endereco invalido em wide ret: " << addr << "\n";
                ctx.erro_execucao = true;
                return;
            }
            ctx.pc = static_cast<uint32_t>(addr);
            break;
        }

        default:
            std::cerr << "Erro: sub-opcode invalido em wide: 0x" << std::hex
                      << static_cast<int>(sub_op) << std::dec << "\n";
            ctx.erro_execucao = true;
            break;
    }
}
// Byte 0xC5: multianewarray - Aloca array multidimensional (2 bytes CP + 1 byte dimensions)
template <typename Context>
inline void multianewarray(Context& ctx) {
    if (static_cast<size_t>(ctx.pc) + 2 >= ctx.code.size() || !ctx.classe) {
        std::cerr << "Erro: contexto invalido ou operandos faltando em multianewarray no PC="
                  << ctx.pc << "\n";
        ctx.erro_execucao = true;
        return;
    }

    uint16_t high = static_cast<uint16_t>(ctx.code[ctx.pc++]);
    uint16_t low  = static_cast<uint16_t>(ctx.code[ctx.pc++]);
    uint16_t cp_index = static_cast<uint16_t>((high << 8) | low);
    uint8_t dimensions = ctx.code[ctx.pc++];

    const auto& cp = ctx.classe->constant_pool;
    if (cp_index == 0 || cp_index >= cp.size()) {
        std::cerr << "Erro: indice #" << cp_index << " fora do Constant Pool em multianewarray\n";
        ctx.erro_execucao = true;
        return;
    }

    if (dimensions == 0) {
        std::cerr << "Erro: dimensions == 0 em multianewarray\n";
        ctx.erro_execucao = true;
        return;
    }

    if (ctx.size() < static_cast<int>(dimensions)) {
        std::cerr << "Erro: pilha insuficiente para " << static_cast<int>(dimensions)
                  << " dimensoes em multianewarray\n";
        ctx.erro_execucao = true;
        return;
    }

    std::vector<int32_t> contagens(dimensions, 0);
    // Na pilha, count[dimensions - 1] esta no topo e count[0] esta mais abaixo
    for (int i = static_cast<int>(dimensions) - 1; i >= 0; --i) {
        contagens[static_cast<size_t>(i)] = ctx.pop();
        if (ctx.erro_execucao) return;
        if (contagens[static_cast<size_t>(i)] < 0) {
            std::cerr << "Erro: NegativeArraySizeException em multianewarray (dim "
                      << i << " = " << contagens[static_cast<size_t>(i)] << ")\n";
            ctx.erro_execucao = true;
            return;
        }
    }

    std::string tipo_array = resolver_cp_string(cp, cp[cp_index].index1);
    std::cout << "[multianewarray #" << cp_index << "] tipo=" << tipo_array
              << ", dims=" << static_cast<int>(dimensions) << "\n";

    // TODO: alocar estrutura multidimensional no Heap quando o Heap existir
    ctx.push(alocar_ref_simbolica(ctx, static_cast<uint32_t>(cp_index) * 512u + dimensions));
}
// Byte 0xC6: ifnull - Desvia se referencia no topo da pilha for 0 (null)
template <typename Context>
inline void ifnull(Context& ctx) {
    uint32_t base_pc = (ctx.pc > 0) ? (ctx.pc - 1) : 0;

    if (static_cast<size_t>(ctx.pc) + 1 >= ctx.code.size()) {
        std::cerr << "Erro: ifnull sem offset de 16 bits no PC=" << base_pc << "\n";
        ctx.erro_execucao = true;
        return;
    }

    int16_t branch = ler_i16_be(ctx.code, ctx.pc);
    ctx.pc += 2;

    if (ctx.empty()) {
        std::cerr << "Erro: pilha vazia em ifnull no PC=" << base_pc << "\n";
        ctx.erro_execucao = true;
        return;
    }

    int32_t ref = ctx.pop();
    if (ctx.erro_execucao) return;

    if (ref == 0) {
        uint32_t novo_pc = 0;
        if (!aplicar_salto_relativo(base_pc, static_cast<int32_t>(branch), ctx.code.size(), novo_pc)) {
            std::cerr << "Erro: salto invalido em ifnull no PC=" << base_pc << "\n";
            ctx.erro_execucao = true;
            return;
        }
        ctx.pc = novo_pc;
    }
}
// Byte 0xC7: ifnonnull - Desvia se referencia no topo da pilha nao for 0 (nao-null)
template <typename Context>
inline void ifnonnull(Context& ctx) {
    uint32_t base_pc = (ctx.pc > 0) ? (ctx.pc - 1) : 0;

    if (static_cast<size_t>(ctx.pc) + 1 >= ctx.code.size()) {
        std::cerr << "Erro: ifnonnull sem offset de 16 bits no PC=" << base_pc << "\n";
        ctx.erro_execucao = true;
        return;
    }

    int16_t branch = ler_i16_be(ctx.code, ctx.pc);
    ctx.pc += 2;

    if (ctx.empty()) {
        std::cerr << "Erro: pilha vazia em ifnonnull no PC=" << base_pc << "\n";
        ctx.erro_execucao = true;
        return;
    }

    int32_t ref = ctx.pop();
    if (ctx.erro_execucao) return;

    if (ref != 0) {
        uint32_t novo_pc = 0;
        if (!aplicar_salto_relativo(base_pc, static_cast<int32_t>(branch), ctx.code.size(), novo_pc)) {
            std::cerr << "Erro: salto invalido em ifnonnull no PC=" << base_pc << "\n";
            ctx.erro_execucao = true;
            return;
        }
        ctx.pc = novo_pc;
    }
}
// Byte 0xC8: goto_w - Salto incondicional com deslocamento de 32 bits (4 bytes)
template <typename Context>
inline void goto_w(Context& ctx) {
    uint32_t base_pc = (ctx.pc > 0) ? (ctx.pc - 1) : 0;

    if (static_cast<size_t>(ctx.pc) + 3 >= ctx.code.size()) {
        std::cerr << "Erro: goto_w sem offset de 32 bits no PC=" << base_pc << "\n";
        ctx.erro_execucao = true;
        return;
    }

    int32_t branch = ler_i32_be(ctx.code, ctx.pc);
    ctx.pc += 4;

    uint32_t novo_pc = 0;
    if (!aplicar_salto_relativo(base_pc, branch, ctx.code.size(), novo_pc)) {
        std::cerr << "Erro: salto invalido em goto_w no PC=" << base_pc << "\n";
        ctx.erro_execucao = true;
        return;
    }
    ctx.pc = novo_pc;
}
// Byte 0xC9: jsr_w - Jump subroutine com deslocamento de 32 bits (4 bytes)
template <typename Context>
inline void jsr_w(Context& ctx) {
    uint32_t base_pc = (ctx.pc > 0) ? (ctx.pc - 1) : 0;

    if (static_cast<size_t>(ctx.pc) + 3 >= ctx.code.size()) {
        std::cerr << "Erro: jsr_w sem offset de 32 bits no PC=" << base_pc << "\n";
        ctx.erro_execucao = true;
        return;
    }

    int32_t branch = ler_i32_be(ctx.code, ctx.pc);
    ctx.pc += 4;

    // Endereco da proxima instrucao apos jsr_w (opcode de 1 byte + offset de 4 bytes)
    uint32_t retorno_pc = base_pc + 5u;
    ctx.push(static_cast<int32_t>(retorno_pc));
    if (ctx.erro_execucao) return;

    uint32_t novo_pc = 0;
    if (!aplicar_salto_relativo(base_pc, branch, ctx.code.size(), novo_pc)) {
        std::cerr << "Erro: salto invalido em jsr_w no PC=" << base_pc << "\n";
        ctx.erro_execucao = true;
        return;
    }
    ctx.pc = novo_pc;
}
// Byte 0xCA: breakpoint - Opcode reservado para depuradores na JVM
template <typename Context>
inline void breakpoint_JVM(Context& ctx) {
    uint32_t base_pc = (ctx.pc > 0) ? (ctx.pc - 1) : 0;
    std::cerr << "Erro: opcode reservado 'breakpoint' (0xCA) encontrado no PC="
              << base_pc << "\n";
    ctx.erro_execucao = true;
}
// Byte 0xCB: impdep1 - Opcode reservado dependente da implementacao na JVM
template <typename Context>
inline void impdep1_JVM(Context& ctx) {
    uint32_t base_pc = (ctx.pc > 0) ? (ctx.pc - 1) : 0;
    std::cerr << "Erro: opcode reservado 'impdep1' (0xCB) encontrado no PC="
              << base_pc << "\n";
    ctx.erro_execucao = true;
}
// Byte 0xCC: impdep2 - Opcode reservado dependente da implementacao na JVM
template <typename Context>
inline void impdep2_JVM(Context& ctx) {
    uint32_t base_pc = (ctx.pc > 0) ? (ctx.pc - 1) : 0;
    std::cerr << "Erro: opcode reservado 'impdep2' (0xCC) encontrado no PC="
              << base_pc << "\n";
    ctx.erro_execucao = true;
}
// Byte 0xCC: impdep2 - Opcode reservado dependente da implementacao na JVM
template <typename Context>
inline void no_implementation(Context& ctx) {
    uint32_t base_pc = (ctx.pc > 0) ? (ctx.pc - 1) : 0;
    std::cerr << "Erro: opcode não implementado encontrado no PC="
              << base_pc << "\n";
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

        map[0x60] = iadd<Context>;
        map[0x61] = ladd<Context>;
        map[0x62] = fadd<Context>;
        map[0x63] = dadd<Context>;

        map[0x64] = isub<Context>;
        map[0x65] = lsub<Context>;
        map[0x66] = fsub<Context>;
        map[0x67] = dsub<Context>;

        map[0x68] = imul<Context>;
        map[0x69] = lmul<Context>;
        map[0x6A] = fmul<Context>;
        map[0x6B] = dmul<Context>;

        map[0x6C] = idiv<Context>;
        map[0x6D] = ldiv_JVM<Context>;
        map[0x6E] = fdiv_JVM<Context>;
        map[0x6F] = ddiv<Context>;

        map[0x70] = irem<Context>;
        map[0x71] = lrem<Context>;
        map[0x72] = frem<Context>;
        map[0x73] = drem<Context>;

        map[0x74] = ineg<Context>;
        map[0x75] = lneg<Context>;
        map[0x76] = fneg<Context>;
        map[0x77] = dneg<Context>;

        map[0x78] = ishl<Context>;
        map[0x79] = lshl<Context>;
        map[0x7A] = ishr<Context>;
        map[0x7B] = lshr<Context>;
        map[0x7C] = iushr<Context>;
        map[0x7D] = lushr<Context>;

        map[0x7E] = iand<Context>;
        map[0x7F] = land<Context>;
        map[0x80] = ior<Context>;
        map[0x81] = lor<Context>;
        map[0x82] = ixor<Context>;
        map[0x83] = lxor<Context>;

        map[0x84] = iinc<Context>;
        map[0x85] = i2l<Context>;
        map[0x86] = i2f<Context>;
        map[0x87] = i2d<Context>;
        map[0x88] = l2i<Context>;
        map[0x89] = l2f<Context>;
        map[0x8A] = l2d<Context>;
        map[0x8B] = f2i<Context>;
        map[0x8C] = f2l<Context>;
        map[0x8D] = f2d<Context>;
        map[0x8E] = d2i<Context>;
        map[0x8F] = d2l<Context>;
        map[0x90] = d2f<Context>;
        map[0x91] = i2b<Context>;
        map[0x92] = i2c<Context>;
        map[0x93] = i2s<Context>;

        map[0x94] = lcmp<Context>;
        map[0x95] = fcmpl<Context>;
        map[0x96] = fcmpg<Context>;
        map[0x97] = dcmpl<Context>;
        map[0x98] = dcmpg<Context>;
        map[0x99] = ifeq<Context>;
        map[0x9A] = ifne<Context>;
        map[0x9B] = iflt<Context>;
        map[0x9C] = ifge<Context>;
        map[0x9D] = ifgt<Context>;
        map[0x9E] = ifle<Context>;
        map[0x9F] = if_icmpeq<Context>;
        map[0xA0] = if_icmpne<Context>;
        map[0xA1] = if_icmplt<Context>;
        map[0xA2] = if_icmpge<Context>;
        map[0xA3] = if_icmpgt<Context>;
        map[0xA4] = if_icmple<Context>;
        map[0xA5] = if_acmpeq<Context>;
        map[0xA6] = if_acmpne<Context>;

        map[0xA7] = goto_JVM<Context>;       
        map[0xA8] = jsr<Context>;
        map[0xA9] = ret<Context>;
        map[0xAA] = tableswitch<Context>;
        map[0xAB] = lookupswitch<Context>;

        map[0xAC] = ireturn<Context>;
        map[0xAD] = lreturn<Context>;
        map[0xAE] = freturn<Context>;
        map[0xAF] = dreturn<Context>;
        map[0xB0] = areturn<Context>;
        map[0xB1] = return_void<Context>;

        map[0xB2] = getstatic_JVM<Context>;
        map[0xB3] = putstatic_JVM<Context>;
        map[0xB4] = getfield_JVM<Context>;
        map[0xB5] = putfield_JVM<Context>;
        map[0xB6] = invokevirtual_JVM<Context>;
        map[0xB7] = invokespecial_JVM<Context>;
        map[0xB8] = invokestatic_JVM<Context>;
        map[0xB9] = invokeinterface_JVM<Context>;
        map[0xBA] = invokedynamic_JVM<Context>;

        map[0xBB] = new_obj<Context>;
        map[0xBC] = newarray<Context>;
        map[0xBD] = anewarray<Context>;
        map[0xBE] = arraylength<Context>;
        map[0xBF] = athrow<Context>;

        map[0xC0] = checkcast<Context>;
        map[0xC1] = instanceof_JVM<Context>;
        map[0xC2] = monitorenter<Context>;
        map[0xC3] = monitorexit<Context>;
        map[0xC4] = wide<Context>;
        map[0xC5] = multianewarray<Context>;
        map[0xC6] = ifnull<Context>;
        map[0xC7] = ifnonnull<Context>;
        map[0xC8] = goto_w<Context>;
        map[0xC9] = jsr_w<Context>;

        map[0xCA] = breakpoint_JVM<Context>;
        map[0xCB] = impdep1_JVM<Context>;
        map[0xCC] = impdep2_JVM<Context>;

        map[0xCD] = no_implementation<Context>;
        map[0xCE] = no_implementation<Context>;
        map[0xCF] = no_implementation<Context>;
        map[0xD0] = no_implementation<Context>;
        map[0xD1] = no_implementation<Context>;
        map[0xD2] = no_implementation<Context>;
        map[0xD3] = no_implementation<Context>;
        map[0xD4] = no_implementation<Context>;
        map[0xD5] = no_implementation<Context>;
        map[0xD6] = no_implementation<Context>;
        map[0xD7] = no_implementation<Context>;
        map[0xD8] = no_implementation<Context>;
        map[0xD9] = no_implementation<Context>;
        map[0xDA] = no_implementation<Context>;
        map[0xDB] = no_implementation<Context>;
        map[0xDC] = no_implementation<Context>;
        map[0xDD] = no_implementation<Context>;
        map[0xDE] = no_implementation<Context>;
        map[0xDF] = no_implementation<Context>;
        map[0xE0] = no_implementation<Context>;
        map[0xE1] = no_implementation<Context>;
        map[0xE2] = no_implementation<Context>;
        map[0xE3] = no_implementation<Context>;
        map[0xE4] = no_implementation<Context>;
        map[0xE5] = no_implementation<Context>;
        map[0xE6] = no_implementation<Context>;
        map[0xE7] = no_implementation<Context>;
        map[0xE8] = no_implementation<Context>;
        map[0xE9] = no_implementation<Context>;
        map[0xEA] = no_implementation<Context>;
        map[0xEB] = no_implementation<Context>;
        map[0xEC] = no_implementation<Context>;
        map[0xED] = no_implementation<Context>;
        map[0xEE] = no_implementation<Context>;
        map[0xEF] = no_implementation<Context>;
        map[0xF0] = no_implementation<Context>;
        map[0xF1] = no_implementation<Context>;
        map[0xF2] = no_implementation<Context>;
        map[0xF3] = no_implementation<Context>;
        map[0xF4] = no_implementation<Context>;
        map[0xF5] = no_implementation<Context>;
        map[0xF6] = no_implementation<Context>;
        map[0xF7] = no_implementation<Context>;
        map[0xF8] = no_implementation<Context>;
        map[0xF9] = no_implementation<Context>;
        map[0xFA] = no_implementation<Context>;
        map[0xFB] = no_implementation<Context>;
        map[0xFC] = no_implementation<Context>;
        map[0xFD] = no_implementation<Context>;
        map[0xFE] = no_implementation<Context>;
        map[0xFF] = no_implementation<Context>;
        // END OF FILE

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