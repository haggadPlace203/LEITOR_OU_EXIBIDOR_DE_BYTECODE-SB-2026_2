#include <iostream>
#include <cstdint>
#include <string>
#include "BYTECODE.h" // Importa a estrutura imutável

int main() {
    // 1. Pega a referência constante do mapa global imutável
    const auto& BYTECODE = get_instruction();
    // 2. Define a chave (opcode) que deseja buscar
    uint8_t chave_procurada = 0x10;
    // 3. Busca o valor pela chave usando .find()
    auto it = BYTECODE.find(chave_procurada);
    // 4. Verifica se encontrou e exibe o resultado
    if (it != BYTECODE.end()) {
        // it->first é a chave (uint8_t), it->second é o valor (std::string)
        std::cout << "Bytecode encontrado! instrucao: " << it->second << "\n";
    } else {
        std::cout << "Chave 0x" << std::hex << (int)chave_procurada << " nao encontrada no bytecode.\n";
    }

    return 0;
}


/*====================================================================*/
/*                    Comentários Auxiliares                          */
/*                                                                    */
/* Descrição Detalhada da Implementação e Lógica de Busca (Main):     */
/*                                                                    */
/* [1. Inclusão de Dependências e Cabeçalho]                          */
/* - <iostream>: Biblioteca padrão para gerenciar a saída de dados    */
/* no terminal utilizando o operador de inserção (std::cout)          */
/* - <cstdint>: Fornece tipos inteiros de largura exata, como o       */
/*uint8_t, essencial para representar bytes exatos de 0 a 255         */
/* - <string>: Gerencia os mnemônicos textuais das instruções         */
/* - "BYTECODE.h": Importa o cabeçalho personalizado contendo a       */
/*   estrutura de dados imutável global.                              */
/*                                                                    */
/* [2. Obtenção do Mapa Imutável via Referência Constante]            */
/* - Chamada da função de acesso (ex: get_instruction())              */
/* - Uso de 'const auto&' para capturar o retorno por referência      */
/*   constante, evitando cópias desnecessárias na memória e           */
/*   assegurando que o mapa seja estritamente de leitur.              */
/*                                                                    */
/* [3. Definição da Chave de Busca (Opcode)]                          */
/* - Declaração de uma variável do tipo uint8_t (ex: 0x10) que        */
/*   representa o byte da instrução a ser traduzida.                  */
/*                                                                    */
/* [4. Busca Eficiente O(1) com o Método .find()]                     */
/* - Execução de BYTECODE.find(chave_procurada).                      */
/* - A função retorna um iterador ('it') apontando diretamente        */
/*   para o elemento encontrado ou para o fim do mapa                 */
/*   (BYTECODE.end()), garantindo desempenho ótimo.                   */
/*                                                                    */
/* [5. Validação do Iterador e Tratamento de Erros]                   */
/* - Condicional de verificação (if (it != BYTECODE.end())).          */
/* - Se verdadeiro: A chave existe. O mnemônico textual é acessado    */
/*   através de 'it->second' e impresso no console.                   */
/* - Se falso (else): O byte não possui instrução mapeada, exibindo   */
/*   uma mensagem informativa formatada em hexadecimal.               */
/*====================================================================*/
