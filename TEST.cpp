#include <iostream>
#include <stack>
#include <stdint.h>
#include "TMP.h"

int main() {
    // Pilha principal de 32 bits
    std::stack<int32_t> pilha_execucao;

    const auto& bytecode_map = get_instruction();

    // Sequência de execução:
    // 0x01 -> empilha 3        | Pilha: [3]
    // 0x02 -> empilha 3        | Pilha: [3, 3]
    // 0x03 -> desempilha 3 e 3, soma e empilha 6 | Pilha: [6]
    uint8_t sequencia_bytes[] = { 0x01, 0x02, 0x03 };

    for (uint8_t byte_atual : sequencia_bytes) {
        auto it = bytecode_map.find(byte_atual);
        if (it != bytecode_map.end()) {
            it->second(pilha_execucao);
        } else {
            std::cout << "Byte desconhecido: 0x" 
                      << std::hex << static_cast<int>(byte_atual) << std::dec << std::endl;
        }
    }

    // Exibe o resultado final que restou no topo da pilha
    if (!pilha_execucao.empty()) {
        std::cout << "Tamanho final da pilha: " << pilha_execucao.size() << std::endl;
        std::cout << "Valor no topo da pilha (32 bits): " << pilha_execucao.top() << std::endl;
    }

    return 0;
}