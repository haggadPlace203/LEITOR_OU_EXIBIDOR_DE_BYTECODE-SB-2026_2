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