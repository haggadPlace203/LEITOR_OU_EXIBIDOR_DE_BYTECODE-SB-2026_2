/*==================================================*/
/*  Informações sobre o desenvolvedor & assinatura  */
/* Carlos Luan Silva Pereira,Luiz Fernando C. Soaris*/ 
/* Natalia Leal e Enzo Luz -  Eng. Mecatrônica      */
/*       Estudante da Universidade de Brasília      */
/*           Brasília - DF - 02/11/2026             */
/*==================================================*/

#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <iomanip>
#include <cstdint>
#include "BYTECODE.h"

// Funções utilitárias para leitura Big-Endian (padrão da JVM)
uint16_t read_u2(std::ifstream& file) {
    uint8_t bytes[2];
    file.read(reinterpret_cast<char*>(bytes), 2);
    return (static_cast<uint16_t>(bytes[0]) << 8) | static_cast<uint16_t>(bytes[1]);
}

uint32_t read_u4(std::ifstream& file) {
    uint8_t bytes[4];
    file.read(reinterpret_cast<char*>(bytes), 4);
    return (static_cast<uint32_t>(bytes[0]) << 24) |
           (static_cast<uint32_t>(bytes[1]) << 16) |
           (static_cast<uint32_t>(bytes[2]) << 8)  |
           static_cast<uint32_t>(bytes[3]);
}

// Estrutura para armazenar as entradas do Constant Pool
struct ConstantPoolEntry {
    uint8_t tag;
    uint16_t index1 = 0;
    uint16_t index2 = 0;
    uint32_t bytes_val = 0;
    std::string utf8_str = "";
};

// Função auxiliar para resolver strings e referências do Constant Pool de forma legível
std::string get_cp_string(const std::vector<ConstantPoolEntry>& cp, uint16_t index) {
    if (index == 0 || index >= cp.size()) return "#?";
    if (cp[index].tag == 1) {
        return cp[index].utf8_str;
    }
    else if (cp[index].tag == 7) { // CONSTANT_Class
        return get_cp_string(cp, cp[index].index1);
    }
    else if (cp[index].tag == 12) { // CONSTANT_NameAndType
        std::string name = get_cp_string(cp, cp[index].index1);
        std::string desc = get_cp_string(cp, cp[index].index2);
        return name + ":" + desc;
    }
    return "#" + std::to_string(index);
}

int main() {
    std::cout << "Digite o caminho do arquivo .class (ex: CLASSE.class): ";

    //otmização de parâmetros
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(NULL);

    // const std::string Class_Route = "JAVA/TA1.class";

    std::string Class_Route;
    // Usamos std::getline para aceitar caminhos que possam conter espaços em branco
    std::getline(std::cin, Class_Route);

    std::ifstream file(Class_Route, std::ios::binary);
    
    if (!file.is_open()) {
        std::cerr << "Erro: Nao foi possivel abrir o arquivo " << Class_Route << std::endl;
        return 1;
    }

    std::cout << "========================================================\n";
    std::cout << "  LEITOR E EXIBIDOR DE BYTECODE JAVA - VISUALIZADOR\n";
    std::cout << "========================================================\n";

    // 1. Magic Number
    uint32_t magic = read_u4(file);
    std::cout << "Magic Number: 0x" << std::hex << std::setw(8) << std::setfill('\0') << magic << std::dec << "\n";
    if (magic != 0xCAFEBABE) {
        std::cerr << "Erro Critico: O arquivo nao possui o Magic Number valido da JVM (0xCAFEBABE)!\n";
        return 1;
    }

    // 2. Versões
    uint16_t minor_version = read_u2(file);
    uint16_t major_version = read_u2(file);
    std::cout << "Minor Version: " << minor_version << "\n";
    std::cout << "Major Version: " << major_version << " (Java " << (major_version - 44) << ")\n";

    // 3. Constant Pool (Item 1: Exibição detalhada estilo jclasslib)
    uint16_t constant_pool_count = read_u2(file);
    std::cout << "\n--------------------------------------------------------\n";
    std::cout << "CONSTANT POOL (" << (constant_pool_count - 1) << " entradas):\n";
    std::cout << "--------------------------------------------------------\n";
    
    std::vector<ConstantPoolEntry> constant_pool(constant_pool_count);
    for (int i = 1; i < constant_pool_count; ++i) {
        uint8_t tag;
        file.read(reinterpret_cast<char*>(&tag), 1);
        constant_pool[i].tag = tag;

        std::cout << " #" << std::setw(2) << i << " = ";

        switch (tag) {
            case 1: { // CONSTANT_Utf8
                uint16_t length = read_u2(file);
                std::string str(length, '\0');
                file.read(&str[0], length);
                constant_pool[i].utf8_str = str;
                std::cout << "Utf8               \"" << str << "\"\n";
                break;
            }
            case 3: { // CONSTANT_Integer
                constant_pool[i].bytes_val = read_u4(file);
                std::cout << "Integer            " << constant_pool[i].bytes_val << "\n";
                break;
            }
            case 4: { // CONSTANT_Float
                constant_pool[i].bytes_val = read_u4(file);
                std::cout << "Float              " << constant_pool[i].bytes_val << "\n";
                break;
            }
            case 5: { // CONSTANT_Long
                constant_pool[i].bytes_val = read_u4(file);
                read_u4(file);
                std::cout << "Long               [Double Slot]\n";
                i++; 
                break;
            }
            case 6: { // CONSTANT_Double
                constant_pool[i].bytes_val = read_u4(file);
                read_u4(file);
                std::cout << "Double             [Double Slot]\n";
                i++; 
                break;
            }
            case 7: { // CONSTANT_Class
                constant_pool[i].index1 = read_u2(file);
                std::cout << "Class              #" << constant_pool[i].index1 << "\n";
                break;
            }
            case 8: { // CONSTANT_String
                constant_pool[i].index1 = read_u2(file);
                std::cout << "String             #" << constant_pool[i].index1 << "\n";
                break;
            }
            case 9: { // CONSTANT_Fieldref
                constant_pool[i].index1 = read_u2(file);
                constant_pool[i].index2 = read_u2(file);
                std::cout << "Fieldref           #" << constant_pool[i].index1 << ".#" << constant_pool[i].index2 << "\n";
                break;
            }
            case 10: { // CONSTANT_Methodref
                constant_pool[i].index1 = read_u2(file);
                constant_pool[i].index2 = read_u2(file);
                std::cout << "Methodref          #" << constant_pool[i].index1 << ".#" << constant_pool[i].index2 << "\n";
                break;
            }
            case 11: { // CONSTANT_InterfaceMethodref
                constant_pool[i].index1 = read_u2(file);
                constant_pool[i].index2 = read_u2(file);
                std::cout << "InterfaceMethodref #" << constant_pool[i].index1 << ".#" << constant_pool[i].index2 << "\n";
                break;
            }
            case 12: { // CONSTANT_NameAndType
                constant_pool[i].index1 = read_u2(file);
                constant_pool[i].index2 = read_u2(file);
                std::cout << "NameAndType        #" << constant_pool[i].index1 << ":#" << constant_pool[i].index2 << "\n";
                break;
            }
            case 15: { // CONSTANT_MethodHandle
                char ref_kind;
                file.read(&ref_kind, 1);
                constant_pool[i].index1 = read_u2(file);
                std::cout << "MethodHandle       " << (int)ref_kind << ", #" << constant_pool[i].index1 << "\n";
                break;
            }
            case 16: { // CONSTANT_MethodType
                constant_pool[i].index1 = read_u2(file);
                std::cout << "MethodType         #" << constant_pool[i].index1 << "\n";
                break;
            }
            case 18: { // CONSTANT_InvokeDynamic
                constant_pool[i].index1 = read_u2(file);
                constant_pool[i].index2 = read_u2(file);
                std::cout << "InvokeDynamic      #" << constant_pool[i].index1 << ":#" << constant_pool[i].index2 << "\n";
                break;
            }
            default:
                std::cout << "Desconhecido (Tag " << (int)tag << ")\n";
                break;
        }
    }

    // 4. Access Flags, This Class, Super Class
    uint16_t access_flags = read_u2(file);
    uint16_t this_class = read_u2(file);
    uint16_t super_class = read_u2(file);

    std::cout << "\n--------------------------------------------------------\n";
    std::cout << "INFOS DA CLASSE:\n";
    std::cout << "Access Flags: 0x" << std::hex << access_flags << std::dec << "\n";
    std::cout << "This Class: #" << this_class << " (" << get_cp_string(constant_pool, this_class) << ")\n";
    std::cout << "Super Class: #" << super_class << " (" << get_cp_string(constant_pool, super_class) << ")\n";

    // 5. Interfaces
    uint16_t interfaces_count = read_u2(file);
    for (int i = 0; i < interfaces_count; ++i) {
        read_u2(file);
    }

    // 6. Fields
    uint16_t fields_count = read_u2(file);
    for (int i = 0; i < fields_count; ++i) {
        read_u2(file); read_u2(file); read_u2(file);
        uint16_t attr_cnt = read_u2(file);
        for (int j = 0; j < attr_cnt; ++j) {
            read_u2(file);
            uint32_t len = read_u4(file);
            file.seekg(len, std::ios::cur);
        }
    }

    // 7. Methods (Item 2: Resolução de parâmetros de bytecode apontando para o Constant Pool)
    uint16_t methods_count = read_u2(file);
    std::cout << "\n--------------------------------------------------------\n";
    std::cout << "METODOS E BYTECODE DESMONTADO:\n";
    std::cout << "--------------------------------------------------------\n";
    
    const auto& bytecode_map = get_instruction();

    for (int i = 0; i < methods_count; ++i) {
        uint16_t m_access_flags = read_u2(file);
        uint16_t m_name_index = read_u2(file);
        uint16_t m_desc_index = read_u2(file);
        uint16_t m_attributes_count = read_u2(file);

        std::string method_name = get_cp_string(constant_pool, m_name_index);
        std::string method_desc = get_cp_string(constant_pool, m_desc_index);
        
        std::cout << "\nMetodo: " << method_name << method_desc << "\n";

        for (int j = 0; j < m_attributes_count; ++j) {
            uint16_t attr_name_index = read_u2(file);
            uint32_t attr_length = read_u4(file);
            std::string attr_name = get_cp_string(constant_pool, attr_name_index);

            if (attr_name == "Code") {
                uint16_t max_stack = read_u2(file);
                uint16_t max_locals = read_u2(file);
                uint32_t code_length = read_u4(file);

                std::cout << "  [Atributo Code: stack=" << max_stack << ", locals=" << max_locals << ", length=" << code_length << "]\n";

                std::vector<uint8_t> code_bytes(code_length);
                file.read(reinterpret_cast<char*>(code_bytes.data()), code_length);

                size_t pc = 0;
                while (pc < code_length) {
                    uint8_t opcode = code_bytes[pc];
                    auto it = bytecode_map.find(opcode);
                    std::string mnemonic = (it != bytecode_map.end()) ? it->second : "desconhecido";
                    
                    std::cout << "     " << std::setw(4) << std::setfill('\0') << pc << ": " << std::left << std::setw(15) << mnemonic;

                    // Leitura de argumentos baseados no opcode para exibir a resolução do Constant Pool
                    if (opcode == 0x10 || opcode == 0x15 || opcode == 0x16 || opcode == 0x17 || opcode == 0x18 || opcode == 0x19 || 
                        opcode == 0x36 || opcode == 0x37 || opcode == 0x38 || opcode == 0x39 || opcode == 0x3A) { // bipush / iload / istore, etc. (1 byte de argumento)
                        if (pc + 1 < code_length) {
                            int val = code_bytes[pc + 1];
                            std::cout << " " << val;
                            pc += 1;
                        }
                    }
                    else if (opcode == 0x12) { // ldc (1 byte index)
                        if (pc + 1 < code_length) {
                            uint8_t cp_idx = code_bytes[pc + 1];
                            std::cout << " #" << (int)cp_idx << " // " << get_cp_string(constant_pool, cp_idx);
                            pc += 1;
                        }
                    }
                    else if (opcode == 0xB2 || opcode == 0xB3 || opcode == 0xB4 || opcode == 0xB5 || // getstatic, putstatic, getfield, putfield
                             opcode == 0xB6 || opcode == 0xB7 || opcode == 0xB8 || opcode == 0xBB) { // invokevirtual, invokespecial, invokestatic, new
                        if (pc + 2 < code_length) {
                            uint16_t cp_idx = (static_cast<uint16_t>(code_bytes[pc + 1]) << 8) | code_bytes[pc + 2];
                            std::cout << " #" << cp_idx << " // " << get_cp_string(constant_pool, cp_idx);
                            pc += 2;
                        }
                    }

                    std::cout << "\n";
                    pc++;
                }

                uint16_t exc_table_length = read_u2(file);
                file.seekg(exc_table_length * 8, std::ios::cur);

                uint16_t code_attrs_count = read_u2(file);
                for (int k = 0; k < code_attrs_count; ++k) {
                    read_u2(file);
                    uint32_t sub_attr_len = read_u4(file);
                    file.seekg(sub_attr_len, std::ios::cur);
                }
            } else {
                file.seekg(attr_length, std::ios::cur);
            }
        }
    }

    std::cout << "\n--------------------------------------------------------\n";
    std::cout << "Leitura e exibicao concluidas com sucesso!\n";

    return 0;
}

/*====================================================================*/
/*                     Comentários Auxiliares                         */
/*                                                                    */
/* Descrição Detalhada da Implementação e Leitura em Chunks:          */
/*                                                                    */
/* [0. Configuração do Buffer de Memória para Arquivos Binários]      */
/* - Tamanho sugerido: 64 KB a 2 MB (Otimizado para I/O massivo)      */
/* - Reduz chamadas de sistema (syscalls) e otimiza leitura em SSD/HD */
/* - Exemplo: const size_t BUFFER_SIZE = 1024 * 1024; // 1 MB         */
/*                                                                    */
/* [1. Importação de Módulos Essenciais]                              */
/* - <iostream>: Disponibiliza os canais padrão de saída e erro       */
/*   (std::cout e std::cerr) para feedback no console.                */
/* - <fstream>: Oferece suporte à leitura e manipulação de fluxos     */
/*   de arquivos diretamente do armazenamento secundário.             */
/* - <vector>: Gerencia matrizes dinâmicas unidimensionais, servindo  */
/*   como o buffer de memória RAM seguro e autogerenciado.            */
/*                                                                    */
/* [2. Rotina de Manipulação do Bloco Atual]                          */
/* - Função capaz de receber o endereço do buffer e a quantidade de   */
/* bytes lidos para efetuar o processamento isolado do segmento.      */
/*                                                                    */
/* [3. Ponto de Entrada e Ajustes de Desempenho]                      */
/* - Desativação da sincronização C/C++ via sync_with_stdio(false)    */
/*   para agilizar as operações de E/S.                               */
/* - Declaração imutável contendo o diretório ou nome do arquivo.     */
/*                                                                    */
/* [4. Abertura de Canal Binário e Checagem]                          */
/* - Instanciação do fluxo de entrada em modo binário puro            */
/*   (std::ios::binary) para preservar a integridade dos dados.       */
/* - Validação imediata com is_open() para barrar falhas de caminho.  */
/*                                                                    */
/* [5. Dimensionamento do Buffer Fixo na RAM]                         */
/* - Definição em tempo de compilação (constexpr) de 64 Kilobytes     */
/*   por lote de leitura.                                             */
/* - Alocação do vetor de caracteres correspondente ao tamanho fixo.  */
/*                                                                    */
/* [6. Laço de Leitura Iterativa por Partes]                          */
/* - Execução contínua do método read() combinada à verificação de    */
/*   bytes efetivamente resgatados (.gcount()), assegurando que o     */
/*   fragmento final do arquivo não seja descartado.                  */
/*                                                                    */
/* [7. Auditoria de Falhas de E/S e Encerramento]                     */
/* - Emprego de file.bad() para detectar anomalias físicas na mídia.  */
/* - Conclusão do programa retornando código de sucesso ao SO.        */
/*                                                                    */
/* [8. Compando g++ para cxompilar]                                   */
/* - Compilar com: "g++ CARREGADOR.cpp -std=c++11 -o CARREGADOR.exe"  */
/*====================================================================*/

/*!!!Assinado Por CLSP & Cruzeirão Prof. M. Ladeira!!!*/
