#ifndef BYTECODE_H
#define BYTECODE_H

#include <unordered_map>
#include <stack>
#include <stdint.h>

// Cada função recebe a referência da pilha de 32 bits do main e empilha o resultado
inline void soma(std::stack<int32_t>& pilha) {
    const int32_t a = INT32_C(3);
    const int32_t b = INT32_C(2);
    pilha.push(a + b);
}

inline void subtracao(std::stack<int32_t>& pilha) {
    const int32_t a = INT32_C(3);
    const int32_t b = INT32_C(2);
    pilha.push(a - b);
}

inline void multiplicacao(std::stack<int32_t>& pilha) {
    const int32_t a = INT32_C(3);
    const int32_t b = INT32_C(2);
    pilha.push(a * b);
}

// O ponteiro de função agora recebe a referência da pilha de 32 bits
typedef void (*OperationFunc)(std::stack<int32_t>&);

// Tabela hash mapeando o byte para o ponteiro de função
inline const std::unordered_map<uint8_t, OperationFunc>& get_instruction() {
    static const std::unordered_map<uint8_t, OperationFunc> BYTECODE = [] {
        std::unordered_map<uint8_t, OperationFunc> map;

        map[0x01] = soma;          // Byte 0x01 -> Empilha (3 + 2)
        map[0x02] = subtracao;     // Byte 0x02 -> Empilha (3 - 2)
        map[0x03] = multiplicacao; // Byte 0x03 -> Empilha (3 * 2)

        return map;
    }();
    return BYTECODE;
}

#endif