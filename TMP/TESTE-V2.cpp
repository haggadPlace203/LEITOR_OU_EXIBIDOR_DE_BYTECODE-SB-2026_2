#include <iostream>
#include <stack>
#include <stdint.h>
#include "TMP.h"

int main() {
    // Pilha de 32 bits declarada no escopo principal
    std::stack<int32_t> pilha_execucao;

    const auto& bytecode_map = get_instruction();

    // Exemplo: executando uma sequência de bytes (soma, subtração e multiplicação)
    uint8_t sequencia_bytes[] = { 0x01, 0x02, 0x03 };

    for (uint8_t byte_atual : sequencia_bytes) {
        auto it = bytecode_map.find(byte_atual);
        if (it != bytecode_map.end()) {
            // Passa a pilha do main por referência para a função apontada
            it->second(pilha_execucao);
        } else {
            std::cout << "Byte desconhecido: 0x" 
                      << std::hex << static_cast<int>(byte_atual) << std::dec << std::endl;
        }
    }

    // Inspecionando e desempilhando os resultados modificados pelas funções externas
    std::cout << "Tamanho final da pilha: " << pilha_execucao.size() << "\n\n";

    while (!pilha_execucao.empty()) {
        std::cout << "Topo da pilha (32 bits): " << pilha_execucao.top() << std::endl;
        pilha_execucao.pop();
    }

    return 0;
}