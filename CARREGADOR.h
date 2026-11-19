/*==================================================*/
/*  Informações sobre o desenvolvedor & assinatura  */
/* Carlos Luan Silva Pereira,Luiz Fernando C. Soaris*/
/* Natalia Leal e Enzo Luz -  Eng. Mecatrônica      */
/*       Estudante da Universidade da Brasília      */
/*           Brasília - DF - 18/11/2026             */
/*==================================================*/

#ifndef CARREGADOR_H
#define CARREGADOR_H

#include <cstdint>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>

/*==================================================*/
/*     1. ESTRUTURA DO CONSTANT POOL (cp_info)      */
/*==================================================*/

struct ConstantPoolEntry {
    uint8_t tag = 0;
    uint16_t index1 = 0;
    uint16_t index2 = 0;
    uint8_t ref_kind = 0;        // CONSTANT_MethodHandle (tag 15)
    uint32_t bytes_val = 0;      // Integer / Float (4 bytes)
    uint64_t long_bytes_val = 0; // Long / Double (8 bytes)
    std::string utf8_str = "";   // CONSTANT_Utf8 (tag 1)
};

/*==================================================*/
/*      2. ATRIBUTOS GENERICOS E ATRIBUTO CODE      */
/*==================================================*/

struct AttributeInfo {
    uint16_t attribute_name_index = 0;
    std::string name;
    std::vector<uint8_t> info;
};

struct ExceptionTableEntry {
    uint16_t start_pc = 0;
    uint16_t end_pc = 0;
    uint16_t handler_pc = 0;
    uint16_t catch_type = 0;
};

struct CodeAttribute {
    bool has_code = false;
    uint16_t max_stack = 0;
    uint16_t max_locals = 0;
    std::vector<uint8_t> code;
    std::vector<ExceptionTableEntry> exception_table;
    std::vector<AttributeInfo> attributes;
};

/*==================================================*/
/*       3. CAMPOS (fields) E METODOS (methods)     */
/*==================================================*/

struct FieldInfo {
    uint16_t access_flags = 0;
    uint16_t name_index = 0;
    uint16_t descriptor_index = 0;
    std::string name;
    std::string descriptor;
    std::vector<AttributeInfo> attributes;
};

struct MethodInfo {
    uint16_t access_flags = 0;
    uint16_t name_index = 0;
    uint16_t descriptor_index = 0;
    std::string name;
    std::string descriptor;
    CodeAttribute code_attr;
    std::vector<AttributeInfo> attributes;
};

/*==================================================*/
/*     4. ESTRUTURA PRINCIPAL DO ARQUIVO .CLASS     */
/*==================================================*/

struct ClassFile {
    bool carregado = false;

    // Cabecalho e Versoes
    uint32_t magic = 0;
    uint16_t minor_version = 0;
    uint16_t major_version = 0;

    // Constant Pool (acessivel via classe.constant_pool[X])
    uint16_t constant_pool_count = 0;
    std::vector<ConstantPoolEntry> constant_pool;

    // Metadados da Classe
    uint16_t access_flags = 0;
    uint16_t this_class = 0;
    uint16_t super_class = 0;
    std::string this_class_name;
    std::string super_class_name;

    // Tabelas Separadas
    std::vector<uint16_t> interfaces;
    std::vector<FieldInfo> fields;
    std::vector<MethodInfo> methods;
    std::vector<AttributeInfo> attributes;

    // Busca um metodo pelo nome (ex: "main" ou "<init>")
    const MethodInfo* buscar_metodo(const std::string& nome) const {
        for (size_t i = 0; i < methods.size(); ++i) {
            if (methods[i].name == nome) {
                return &methods[i];
            }
        }
        return nullptr;
    }

    // Busca o metodo main estatico que recebe array e retorna void (sem citar nome de classe)
    const MethodInfo* buscar_main() const {
        for (size_t i = 0; i < methods.size(); ++i) {
            const MethodInfo& m = methods[i];
            const std::string& d = m.descriptor;
            bool is_static   = (m.access_flags & 0x0008) != 0;
            bool param_array = (d.size() >= 4 && d[0] == '(' && d[1] == '[');
            bool ret_void    = (d.size() >= 2 && d[d.size() - 2] == ')' && d[d.size() - 1] == 'V');

            if (m.name == "main" && is_static && param_array && ret_void) {
                return &m;
            }
        }
        return nullptr;
    }
};

/*==================================================*/
/*    5. FUNCOES AUXILIARES DE LEITURA BIG-ENDIAN   */
/*==================================================*/

inline uint8_t read_u1(std::ifstream& file) {
    uint8_t byte = 0;
    file.read(reinterpret_cast<char*>(&byte), 1);
    return byte;
}

inline uint16_t read_u2(std::ifstream& file) {
    uint8_t bytes[2] = {0, 0};
    file.read(reinterpret_cast<char*>(bytes), 2);
    return (static_cast<uint16_t>(bytes[0]) << 8) | static_cast<uint16_t>(bytes[1]);
}

inline uint32_t read_u4(std::ifstream& file) {
    uint8_t bytes[4] = {0, 0, 0, 0};
    file.read(reinterpret_cast<char*>(bytes), 4);
    return (static_cast<uint32_t>(bytes[0]) << 24) |
           (static_cast<uint32_t>(bytes[1]) << 16) |
           (static_cast<uint32_t>(bytes[2]) << 8)  |
           static_cast<uint32_t>(bytes[3]);
}

inline std::string resolver_cp_string(const std::vector<ConstantPoolEntry>& cp, uint16_t index) {
    if (index == 0 || index >= cp.size()) return "#?";
    if (cp[index].tag == 1) {
        return cp[index].utf8_str;
    } else if (cp[index].tag == 7 || cp[index].tag == 8 || cp[index].tag == 16) {
        return resolver_cp_string(cp, cp[index].index1);
    } else if (cp[index].tag == 12) {
        return resolver_cp_string(cp, cp[index].index1) + ":" +
               resolver_cp_string(cp, cp[index].index2);
    }
    return "#" + std::to_string(index);
}

inline AttributeInfo ler_atributo_generico(std::ifstream& file, const std::vector<ConstantPoolEntry>& cp) {
    AttributeInfo attr;
    attr.attribute_name_index = read_u2(file);
    attr.name = resolver_cp_string(cp, attr.attribute_name_index);
    uint32_t attr_length = read_u4(file);
    attr.info.resize(attr_length);
    if (attr_length > 0) {
        file.read(reinterpret_cast<char*>(attr.info.data()), attr_length);
    }
    return attr;
}

/*==================================================*/
/*      6. FUNCAO PRINCIPAL DE CARREGAMENTO         */
/*==================================================*/

inline ClassFile carregar_classe(const std::string& class_route) {
    ClassFile classe;

    std::ifstream file(class_route, std::ios::binary);
    if (!file.is_open()) {
        std::cerr << "Erro: Nao foi possivel abrir o arquivo: " << class_route << "\n";
        return classe;
    }

    // 1. Magic Number e Versoes
    classe.magic = read_u4(file);
    if (classe.magic != 0xCAFEBABE) {
        std::cerr << "Erro Critico: Magic Number invalido (esperado 0xCAFEBABE).\n";
        return classe;
    }
    classe.minor_version = read_u2(file);
    classe.major_version = read_u2(file);

    // 2. Constant Pool (indices de 1 ate constant_pool_count - 1)
    classe.constant_pool_count = read_u2(file);
    classe.constant_pool.resize(classe.constant_pool_count);

    for (uint16_t i = 1; i < classe.constant_pool_count; ++i) {
        uint8_t tag = read_u1(file);
        classe.constant_pool[i].tag = tag;

        switch (tag) {
            case 1: { // CONSTANT_Utf8
                uint16_t len = read_u2(file);
                std::string str(len, '\0');
                if (len > 0) {
                    file.read(&str[0], len);
                }
                classe.constant_pool[i].utf8_str = str;
                break;
            }
            case 3: // CONSTANT_Integer
            case 4: // CONSTANT_Float
                classe.constant_pool[i].bytes_val = read_u4(file);
                break;

            case 5: // CONSTANT_Long (ocupa 2 slots)
            case 6: { // CONSTANT_Double (ocupa 2 slots)
                uint64_t high = read_u4(file);
                uint64_t low  = read_u4(file);
                classe.constant_pool[i].long_bytes_val = (high << 32) | low;
                ++i;
                break;
            }
            case 7:  // CONSTANT_Class
            case 8:  // CONSTANT_String
            case 16: // CONSTANT_MethodType
                classe.constant_pool[i].index1 = read_u2(file);
                break;

            case 9:  // CONSTANT_Fieldref
            case 10: // CONSTANT_Methodref
            case 11: // CONSTANT_InterfaceMethodref
            case 12: // CONSTANT_NameAndType
            case 18: // CONSTANT_InvokeDynamic
                classe.constant_pool[i].index1 = read_u2(file);
                classe.constant_pool[i].index2 = read_u2(file);
                break;

            case 15: // CONSTANT_MethodHandle
                classe.constant_pool[i].ref_kind = read_u1(file);
                classe.constant_pool[i].index1 = read_u2(file);
                break;

            default:
                std::cerr << "Aviso: Tag desconhecida no Constant Pool: " << static_cast<int>(tag) << "\n";
                break;
        }
    }

    // 3. Access Flags, This Class, Super Class
    classe.access_flags = read_u2(file);
    classe.this_class   = read_u2(file);
    classe.super_class  = read_u2(file);
    classe.this_class_name  = resolver_cp_string(classe.constant_pool, classe.this_class);
    classe.super_class_name = (classe.super_class == 0)
                                  ? "none"
                                  : resolver_cp_string(classe.constant_pool, classe.super_class);

    // 4. Interfaces
    uint16_t interfaces_count = read_u2(file);
    classe.interfaces.resize(interfaces_count);
    for (uint16_t i = 0; i < interfaces_count; ++i) {
        classe.interfaces[i] = read_u2(file);
    }

    // 5. Fields
    uint16_t fields_count = read_u2(file);
    classe.fields.resize(fields_count);
    for (uint16_t i = 0; i < fields_count; ++i) {
        FieldInfo& f = classe.fields[i];
        f.access_flags     = read_u2(file);
        f.name_index       = read_u2(file);
        f.descriptor_index = read_u2(file);
        f.name             = resolver_cp_string(classe.constant_pool, f.name_index);
        f.descriptor       = resolver_cp_string(classe.constant_pool, f.descriptor_index);

        uint16_t attr_count = read_u2(file);
        f.attributes.resize(attr_count);
        for (uint16_t a = 0; a < attr_count; ++a) {
            f.attributes[a] = ler_atributo_generico(file, classe.constant_pool);
        }
    }

    // 6. Methods (com extracao completa do atributo Code)
    uint16_t methods_count = read_u2(file);
    classe.methods.resize(methods_count);
    for (uint16_t i = 0; i < methods_count; ++i) {
        MethodInfo& m = classe.methods[i];
        m.access_flags     = read_u2(file);
        m.name_index       = read_u2(file);
        m.descriptor_index = read_u2(file);
        m.name             = resolver_cp_string(classe.constant_pool, m.name_index);
        m.descriptor       = resolver_cp_string(classe.constant_pool, m.descriptor_index);

        uint16_t attr_count = read_u2(file);
        for (uint16_t a = 0; a < attr_count; ++a) {
            uint16_t attr_name_index = read_u2(file);
            uint32_t attr_length     = read_u4(file);
            std::string attr_name    = resolver_cp_string(classe.constant_pool, attr_name_index);

            if (attr_name == "Code") {
                m.code_attr.has_code   = true;
                m.code_attr.max_stack  = read_u2(file);
                m.code_attr.max_locals = read_u2(file);

                uint32_t code_length = read_u4(file);
                m.code_attr.code.resize(code_length);
                if (code_length > 0) {
                    file.read(reinterpret_cast<char*>(m.code_attr.code.data()), code_length);
                }

                uint16_t exc_len = read_u2(file);
                m.code_attr.exception_table.resize(exc_len);
                for (uint16_t e = 0; e < exc_len; ++e) {
                    m.code_attr.exception_table[e].start_pc   = read_u2(file);
                    m.code_attr.exception_table[e].end_pc     = read_u2(file);
                    m.code_attr.exception_table[e].handler_pc = read_u2(file);
                    m.code_attr.exception_table[e].catch_type = read_u2(file);
                }

                uint16_t code_attrs_count = read_u2(file);
                m.code_attr.attributes.resize(code_attrs_count);
                for (uint16_t ca = 0; ca < code_attrs_count; ++ca) {
                    m.code_attr.attributes[ca] = ler_atributo_generico(file, classe.constant_pool);
                }
            } else {
                AttributeInfo generic_attr;
                generic_attr.attribute_name_index = attr_name_index;
                generic_attr.name = attr_name;
                generic_attr.info.resize(attr_length);
                if (attr_length > 0) {
                    file.read(reinterpret_cast<char*>(generic_attr.info.data()), attr_length);
                }
                m.attributes.push_back(std::move(generic_attr));
            }
        }
    }

    // 7. Atributos da Classe
    uint16_t class_attrs_count = read_u2(file);
    classe.attributes.resize(class_attrs_count);
    for (uint16_t i = 0; i < class_attrs_count; ++i) {
        classe.attributes[i] = ler_atributo_generico(file, classe.constant_pool);
    }

    classe.carregado = true;
    return classe;
}

/*================================================*/
/*            Comentários Auxiliares              */
/*                                                */
/* Descricao da Estrutura (CARREGADOR.h):         */
/*                                                */
/* - Separacao de Dados: Cada tabela da JVM       */
/*   (constant_pool, interfaces, fields, methods, */
/*   attributes) e armazenada em seu proprio      */
/*   vetor dentro de ClassFile.                   */
/* - Acesso Direto: Permite consultar entradas    */
/*   como classe.constant_pool[X] ou entregar     */
/*   classe.methods[i].code_attr diretamente ao   */
/*   construtor de VMContext.                     */
/*================================================*/

/*!!!Assinado Por CLSP & Cruzeirão Prof. M. Ladeira!!!*/

#endif