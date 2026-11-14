#ifndef BYTECODE_H // Verifica se a macro BYTECODE_H ainda não foi definida para evitar inclusões duplicadas
#define BYTECODE_H // Define a macro BYTECODE_H marcando que este cabeçalho já foi processado

#include <unordered_map> // Inclui a biblioteca padrão C++11 para tabelas hash (std::unordered_map)
#include <stack>         // Inclui a biblioteca padrão para estrutura de pilha LIFO (std::stack)
#include <stdint.h>      // Inclui a biblioteca C de inteiros com largura fixa em bits (uint8_t, int32_t)

inline void op_01(std::stack<int32_t>& pilha) { // Declara função inline que recebe o endereço real (referência) da pilha de 32 bits
    pilha.push(INT32_C(3));                     // Converte o literal 3 para exatos 32 bits com sinal e o insere no topo da pilha
}                                               // Encerra a execução da instrução op_01

inline void op_02(std::stack<int32_t>& pilha) { // Declara função inline que recebe a referência da mesma pilha de 32 bits do main
    pilha.push(INT32_C(3));                     // Insere o segundo valor constante 3 (32 bits) no topo da pilha
}                                               // Encerra a execução da instrução op_02

inline void op_03(std::stack<int32_t>& pilha) { // Declara função inline que consome dois operandos da pilha e empilha a soma
    if (pilha.size() >= 2) {                    // Garante que existem pelo menos 2 elementos na pilha para evitar Stack Underflow
        int32_t val1 = pilha.top();             // Lê o valor que está no topo da pilha (último a entrar) em uma variável de 32 bits
        pilha.pop();                            // Remove o elemento do topo da pilha

        int32_t val2 = pilha.top();             // Lê o novo elemento do topo da pilha (penúltimo a entrar) em 32 bits
        pilha.pop();                            // Remove esse segundo elemento do topo da pilha

        pilha.push(val2 + val1);                // Soma os dois valores de 32 bits e insere o resultado no topo da pilha
    }                                           // Encerra o bloco condicional de segurança da pilha
}                                               // Encerra a execução da instrução op_03

typedef void (*OperationFunc)(std::stack<int32_t>&); // Define o tipo OperationFunc como ponteiro para funções void que recebem std::stack<int32_t>&

inline const std::unordered_map<uint8_t, OperationFunc>& get_instruction() { // Retorna referência constante para a tabela hash global de instruções
    static const std::unordered_map<uint8_t, OperationFunc> BYTECODE = [] {  // Cria tabela hash estática única via expressão lambda imediata (C++11)
        std::unordered_map<uint8_t, OperationFunc> map;                      // Instancia o mapa temporário que associará cada byte a um ponteiro de função

        map[0x01] = op_01;                                                   // Associa o bytecode 0x01 ao endereço de memória da função op_01
        map[0x02] = op_02;                                                   // Associa o bytecode 0x02 ao endereço de memória da função op_02
        map[0x03] = op_03;                                                   // Associa o bytecode 0x03 ao endereço de memória da função op_03

        return map;                                                          // Retorna o mapa preenchido para inicializar a constante estática BYTECODE
    }();                                                                     // Invoca imediatamente a função lambda definida acima
    return BYTECODE;                                                         // Retorna a referência da tabela hash imutável para quem chamou a função
}                                                                            // Encerra a função get_instruction()

/*================================================*/
/*            Comentários Auxiliares              */
/*                                                */
/* Descricao da estrutura e bibliotecas usadas:   */
/*                                                */
/* [Bibliotecas Utilizadas]                       */
/* - <unordered_map>: Fornece a tabela hash para  */
/*   buscas rápidas O(1) de chaves e ponteiros.   */
/* - <stack>: Estrutura LIFO para a pilha de      */
/*   operandos compartilhada com o programa.      */
/* - <stdint.h>: Garante tipos estritos de 8 bits */
/*   (uint8_t) e 32 bits (int32_t).               */
/*                                                */
/* [Arquitetura de Execução e Despacho]           */
/* - Passagem por Referência (&): As funções      */
/*   externas operam diretamente na pilha do main.*/
/* - Tipos de 32 Bits: Garantidos via int32_t e   */
/*   a macro de pré-compilação INT32_C(3).        */
/* - Lambda IIFE e Estático: Inicializa o mapa    */
/*   global imutável de forma segura em C++11.    */
/*================================================*/

#endif // Finaliza a diretiva de guarda de inclusão BYTECODE_H