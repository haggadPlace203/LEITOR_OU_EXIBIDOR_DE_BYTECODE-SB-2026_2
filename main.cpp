#include <iostream>
#include <vector>
#include <stdint.h>

// Liberdade total: ambos importados apenas aqui, em qualquer ordem
#include "BYTECODE.h"
#include "METODOS.h"

int main() {
    // 1. Atributo 'code' extraído de um método da classe Java
    std::vector<uint8_t> atributo_code_metodo = { 
        0x06, 0x10, 0xFE, 0xFF,
        0x06, 0x10, 0x55, 0xFF,
        0x11, 0x55, 0xAA, 0xFF,
        0x06, 0x10, 0x80, 0xFF
    };

    // 2. Criação do Frame de Método (VMContext) passando os limites N e o atributo 'code'
    /*============max stack, max local max temps*/
    VMContext frame_metodo(10, 5, 5, atributo_code_metodo);

    // 3. Recupera a tabela hash vinculada ao tipo VMContext
    const auto& bytecode_map = get_instruction<VMContext>();

    // 4. Loop de execução guiado pelo PC do frame
    while (frame_metodo.pc < frame_metodo.code.size()) {

        std::cout << "PCI = " << frame_metodo.pc << std::endl;
        uint8_t byte_atual = frame_metodo.code[frame_metodo.pc];
        frame_metodo.pc++;
        std::cout << "PCF = " << frame_metodo.pc << std::endl;

        auto it = bytecode_map.find(byte_atual);
        if (it != bytecode_map.end()) {
            it->second(frame_metodo);
        } else {
            std::cout << "Bytecode desconhecido: 0x"
                      << std::hex << static_cast<int>(byte_atual) << std::dec << std::endl;
        }
    }

    // 5. Resultados finais no frame
    std::cout << "=== EXECUCAO DO METODO CONCLUIDA ===" << std::endl;
    std::cout << "Total de bytes executados (PC): " << frame_metodo.pc << std::endl;
    std::cout << "Topo da Pilha (32 bits): " << frame_metodo.top() << std::endl;
    std::cout << "Variavel Local [0]: " << frame_metodo.locais[0] << std::endl;

    return 0;
}