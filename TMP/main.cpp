#include <iostream>
#include <vector>
#include <stdint.h>

// Liberdade total: ambos importados apenas aqui, em qualquer ordem
#include "BYTECODE.h"
#include "METODOS.h"

int main() {
    // 1. Atributo 'code' extraído de um método da classe Java
    std::vector<uint8_t> atributo_code_metodo = { 
        // --- INICIALIZAÇÃO (contador = 0) ---
        0x03,             // [0]  iconst_0   : Empilha 0
        0x3B,             // [1]  istore_0   : Salva 0 na variável local 0 (contador)

        // --- INÍCIO DO LAÇO (PC = 2) ---
        0x1A,             // [2]  iload_0    : Carrega contador na pilha
        0x10, 0x0A,       // [3]  bipush 10  : Empilha 10 (limite do laço)
        0xA2, 0x00, 0x0C, // [5]  if_icmpge +12 : Se contador >= 10, pula para [17] (5 + 12 = 17)

        // --- CORPO DO LAÇO ---
        0x1A,             // [8]  iload_0    : Carrega contador na pilha
        0x59,             // [9]  dup        : Duplica o valor no topo da pilha
        0xFF,             // [10] debug      : Desempilha a cópia e imprime (sobra o original na pilha)
        0x57,             // [11] pop        : Descarta o valor original para manter a pilha limpa

        // --- INCREMENTO E RETORNO ---
        0x84, 0x00, 0x01, // [12] iinc 0, 1  : Incrementa variável local 0 em +1
        0xA7, 0xFF, 0xF3, // [15] goto -13   : Volta para [2] (15 + (-13) = 2)

        // --- FIM DO PROGRAMA (PC = 18) ---
        0xB1              // [18] return     : Encerra o método
        
        /*0x06, 0x10, 0xFE, 0xFF,
        0x06, 0x10, 0x55, 0xFF,
        0x11, 0x55, 0xAA, 0xFF,
        0x06, 0x10, 0x80, 0xFF*/
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
        //std::cout << "PCF = " << frame_metodo.pc << std::endl;

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