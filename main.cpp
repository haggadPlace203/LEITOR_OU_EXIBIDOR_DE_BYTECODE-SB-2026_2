/*==================================================*/
/*  Informações sobre o desenvolvedor & assinatura  */
/*   Carlos Luan Silva Pereira - Eng. Mecatrônica   */
/*       Estudante da Universidade de Brasília      */
/*           Brasília - DF - 18/11/2026             */
/*==================================================*/

#include <iostream>
#include <string>
#include <vector>
#include <stdint.h>

// Os cabecalhos podem ser importados em qualquer ordem
#include "CARREGADOR.h"
#include "BYTECODE.h"
#include "METODOS.h"

int main() {
    std::string class_route;
    std::cout << "Digite o caminho do arquivo .class (ex: JAVA/TA1.class): ";
    std::getline(std::cin, class_route);

    // 1. Carrega o arquivo .class na estrutura separada ClassFile
    ClassFile classe = carregar_classe(class_route);
    if (!classe.carregado) {
        return 1;
    }

    std::cout << "========================================\n";
    std::cout << "Classe carregada: " << classe.this_class_name << "\n";
    std::cout << "Superclasse:      " << classe.super_class_name << "\n";
    std::cout << "Versao Java:      " << (classe.major_version - 44) << "\n";
    std::cout << "Entradas CP:      " << (classe.constant_pool_count - 1) << "\n";
    std::cout << "Metodos:          " << classe.methods.size() << "\n";
    std::cout << "========================================\n";

    /*===========VAI DORETO PARA EXECUÇÃO====================*/
    // 2. Localiza o metodo 'main' sem depender de nomes de classes externas
    const MethodInfo* metodo_main = classe.buscar_main();
    if (!metodo_main) {
        metodo_main = classe.buscar_metodo("main");
    }

    if (!metodo_main || !metodo_main->code_attr.has_code) {
        std::cerr << "Erro: Metodo 'main' com atributo Code nao encontrado.\n";
        return 1;
    }

    std::cout << "Executando metodo: " << metodo_main->name << metodo_main->descriptor
              << " (max_stack=" << metodo_main->code_attr.max_stack
              << ", max_locals=" << metodo_main->code_attr.max_locals
              << ", bytes=" << metodo_main->code_attr.code.size() << ")\n\n";

    // 3. Criacao do Frame de Metodo (VMContext) com os dados extraidos do .class
    VMContext frame_metodo(
        metodo_main->code_attr.max_stack,
        metodo_main->code_attr.max_locals,
        5,
        metodo_main->code_attr.code
    );

    // 4. Recupera a tabela hash vinculada ao tipo VMContext
    const auto& bytecode_map = get_instruction<VMContext>();

    // 5. Loop de execucao guiado pelo PC do frame
    while (frame_metodo.pc < frame_metodo.code.size()) {
        std::cout << "PCI = " << frame_metodo.pc << std::endl;
        uint8_t byte_atual = frame_metodo.code[frame_metodo.pc];
        frame_metodo.pc++;

        auto it = bytecode_map.find(byte_atual);
        if (it != bytecode_map.end()) {
            it->second(frame_metodo);
        } else {
            std::cout << "Bytecode desconhecido: 0x"
                      << std::hex << static_cast<int>(byte_atual) << std::dec << std::endl;
            break;
        }
    }

    // 6. Resultados finais no frame
    std::cout << "\n=== EXECUCAO DO METODO CONCLUIDA ===" << std::endl;
    std::cout << "Total de bytes executados (PC): " << frame_metodo.pc << std::endl;
    std::cout << "Topo da Pilha (32 bits): " << frame_metodo.top() << std::endl;
    if (frame_metodo.locais.size() > 1) {
        std::cout << "Variavel Local [1]: " << frame_metodo.locais[1] << std::endl;
    }
    if (frame_metodo.locais.size() > 2) {
        std::cout << "Variavel Local [2]: " << frame_metodo.locais[2] << std::endl;
    }
    if (frame_metodo.locais.size() > 3) {
        std::cout << "Variavel Local [3]: " << frame_metodo.locais[3] << std::endl;
    }

    return 0;
}