/*==================================================*/
/*  Informações sobre o desenvolvedor & assinatura  */
/*   Carlos Luan Silva Pereira - Eng. Mecatrônica   */
/*       Estudante da Universidade de Brasília      */
/*           Brasília - DF - 19/11/2026             */
/*==================================================*/

#include <cstdint>
#include <iostream>
#include <string>
#include <vector>

#include "CARREGADOR.h"
#include "BYTECODE.h"
#include "METODOS.h"

int main(int argc, char* argv[]) {
    // 1. Entrada do caminho do .class (via argumento no terminal ou digitado)
    std::string class_route;
    if (argc >= 2) {
        class_route = argv[1];
    } else {
        std::cout << "Digite o caminho do arquivo .class (ex: JAVA/TA1.class): ";
        std::getline(std::cin, class_route);
    }

    if (class_route.empty()) {
        std::cerr << "Erro: Caminho do arquivo .class vazio.\n";
        return 1;
    }

    // 2. Carregamento e validacao estrutural do .class
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



    std::cout << "\n-----------------------------------\n";
    for(int sd = 0;sd < classe.constant_pool.size(); sd++){
        std::cout << "CP[" << sd << "] = 0x"
                  << std::hex << static_cast<int>(classe.constant_pool[sd].tag) << std::dec << "\n";
    }
    std::cout << "\0";
    std::cout << "\n-----------------------------------\n";



    // 3. Resolucao do ponto de entrada (public static void main(String[]))
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

    // 4. Criacao do Frame de execucao do metodo
    VMContext frame_metodo(
        metodo_main->code_attr.max_stack + 10,
        metodo_main->code_attr.max_locals,
        5,
        metodo_main->code_attr.code,
        &classe
    );

    // 5. Tabela de despacho de instrucoes (Opcode -> Funcao)
    const auto& bytecode_map = get_instruction<VMContext>();


    std::cout << "\n-----------------------------------\n";
    for(int sd = 0;sd < frame_metodo.code.size(); sd++){
        std::cout << "SD[" << sd << "] = 0x"
                  << std::hex << static_cast<int>(frame_metodo.code[sd]) << std::dec << "\n";
    }
    std::cout << "\n-----------------------------------\n";


    // 6. Ciclo Fetch -> Decode -> Execute
    while (frame_metodo.pc < frame_metodo.code.size() && !frame_metodo.erro_execucao) {
        uint32_t pc_instrucao = frame_metodo.pc;
        uint8_t opcode = frame_metodo.code[frame_metodo.pc++];

        std::cout << "PC[" << pc_instrucao << "] = 0x"
                  << std::hex << static_cast<int>(opcode) << std::dec << "\n";

        // Encerramento normal do metodo void (return = 0xB1)
        if (opcode == 0xB1) {
            std::cout << "Instrucao 'return' (0xB1) executada. Encerrando frame.\n";
            break;
        }

        auto it = bytecode_map.find(opcode);
        if (it != bytecode_map.end()) {
            it->second(frame_metodo);
        } else {
            std::cerr << "Erro: Bytecode desconhecido/nao implementado: 0x"
                      << std::hex << static_cast<int>(opcode) << std::dec
                      << " no PC=" << pc_instrucao << "\n";
            frame_metodo.erro_execucao = true;
            break;
        }

        std::cout << "LOCAIS = [";
        for(int sd = 0;sd < frame_metodo.locais.size(); sd++){
            std::cout << std::hex << static_cast<int>(frame_metodo.locais[sd]) << std::dec << " ";
        }
        std::cout << "]\n";
        std::cout << "PILHA = [";
        for(int sd = 0;sd < frame_metodo.pilha.size(); sd++){
            std::cout << std::hex << static_cast<int>(frame_metodo.pilha[sd]) << std::dec << " ";
        }
        std::cout << "]\n";
        std::cout << "TOPO PILHA = " << frame_metodo.topo_pilha << "\n";
        std::cout << "\n-----------------------------------\n";

    }

    // 7. Relatorio final do estado do Frame
    std::cout << "\n=== EXECUCAO DO METODO CONCLUIDA ===\n";
    std::cout << "Status:                  " << (frame_metodo.erro_execucao ? "ERRO" : "OK") << "\n";
    std::cout << "PC final:                " << frame_metodo.pc << "\n";
    std::cout << "Elementos na pilha:      " << frame_metodo.size() << "\n";
    std::cout << "Topo da Pilha (32 bits): " << frame_metodo.top() << "\n";

    for (size_t i = 0; i < frame_metodo.locais.size(); ++i) {
        std::cout << "Variavel Local [" << i << "]:      " << frame_metodo.get_local(i) << "\n";
    }

    return frame_metodo.erro_execucao ? 1 : 0;
}