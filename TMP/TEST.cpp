#include <iostream>
#include <stdint.h>
#include "TMP.h"

int main() {
    // Cria o contexto da máquina virtual contendo as 3 estruturas de tamanho N
    VMContext contexto;

    const auto& bytecode_map = get_instruction();

    // Sequência de bytes de teste:
    // 0x01 -> Empilha 3          | Pilha: [3]
    // 0x02 -> Empilha 3          | Pilha: [3, 3]
    // 0x03 -> Soma               | Pilha: [6]
    // 0x04 -> Salva em Locais[0] | Pilha: [], Locais[0] = 6
    // 0x01 -> Empilha 3          | Pilha: [3]
    // 0x05 -> Carrega Locais[0]  | Pilha: [3, 6]
    // 0x03 -> Soma               | Pilha: [9]
    // 0x06 -> Salva em Temp[0]   | Pilha: [9], Temporarias[0] = 9
    uint8_t sequencia_bytes[] = { 0x01, 0x02, 0x03, 0x04, 0x01, 0x05, 0x03, 0x06 };

    for (uint8_t byte_atual : sequencia_bytes) {
        auto it = bytecode_map.find(byte_atual);
        if (it != bytecode_map.end()) {
            // Executa a função passando o contexto global da VM
            it->second(contexto);
        } else {
            std::cout << "Byte desconhecido: 0x" 
                      << std::hex << static_cast<int>(byte_atual) << std::dec << std::endl;
        }
    }

    // Inspecionando o estado final das 3 estruturas
    std::cout << "=== ESTADO FINAL DA VM (Tamanho Maximo N = " << MAX_N << ") ===" << std::endl;
    std::cout << "1. Valor no Topo da Pilha: " << contexto.top() << " (Tamanho atual da pilha: " << contexto.size() << ")" << std::endl;
    std::cout << "2. Valor na Variavel Local [0]: " << contexto.locais[0] << std::endl;
    std::cout << "3. Valor na Variavel Temporaria [0]: " << contexto.temporarias[0] << std::endl;

    return 0;
}