#ifndef TMP_H
#define TMP_H

#include <unordered_map>
#include <stack>
#include <stdint.h>

// Byte 0x01: Coloca o valor 3 (32 bits) no topo da pilha
inline void op_01(std::stack<int32_t>& pilha) {
    pilha.push(INT32_C(2));
}

// Byte 0x02: Coloca o valor 3 (32 bits) no topo da pilha
inline void op_02(std::stack<int32_t>& pilha) {
    pilha.push(INT32_C(3));
}

// Byte 0x03: Remove os dois valores do topo da pilha, soma e empilha o resultado
inline void op_03(std::stack<int32_t>& pilha) {
    if (pilha.size() >= 2) {
        int32_t val1 = pilha.top();
        pilha.pop();

        int32_t val2 = pilha.top();
        pilha.pop();

        pilha.push(val2 + val1);
    }
}

// Tipo do ponteiro de função que recebe a pilha de 32 bits por referência
typedef void (*OperationFunc)(std::stack<int32_t>&);

// Tabela hash mapeando o byte para o ponteiro de função
inline const std::unordered_map<uint8_t, OperationFunc>& get_instruction() {
    static const std::unordered_map<uint8_t, OperationFunc> BYTECODE = [] {
        std::unordered_map<uint8_t, OperationFunc> map;

        map[0x01] = op_01; // Push 3
        map[0x02] = op_02; // Push 3
        map[0x03] = op_03; // Pop, Pop, Soma e Push

        return map;
    }();
    return BYTECODE;
}

#endif