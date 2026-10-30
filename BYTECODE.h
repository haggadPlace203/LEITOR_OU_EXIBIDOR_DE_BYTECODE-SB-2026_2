/*==================================================*/
/*  Informações sobre o desenvolvedor & assinatura  */
/*   Carlos Luan Silva Pereira - Eng. Mecatrônica   */
/*       Estudante da Universidade de Brasília      */
/*           Brasília - DF - 30/10/2026             */
/*==================================================*/

#ifndef BYTECODE_H
#define BYTECODE_H

#include <unordered_map>
#include <string>
#include <cstdint>

// Função compatível com -std=c++11 que retorna o mapa imutável global
inline const std::unordered_map<uint8_t, std::string>& get_instruction(){
    static const std::unordered_map<uint8_t, std::string> BYTECODE = []{
        std::unordered_map<uint8_t, std::string> map;

        map[0x00] = "nop";
        map[0x01] = "aconst_null";
        map[0x02] = "iconst_m1";

        map[0x03] = "iconst_0";
        map[0x04] = "iconst_1";
        map[0x05] = "iconst_2";
        map[0x06] = "iconst_3";
        map[0x07] = "iconst_4";
        map[0x08] = "iconst_5";
        map[0x09] = "lconst_0";
        map[0x0A] = "lconst_1";
        map[0x0B] = "fconst_0";
        map[0x0C] = "fconst_1";
        map[0x0D] = "fconst_2";
        map[0x0E] = "dconst_0";
        map[0x0F] = "dconst_1";

        map[0x10] = "bipush";
        map[0x11] = "sipush";
        map[0x12] = "ldc";
        map[0x13] = "ldc_w";
        map[0x14] = "ldc2_w";

        map[0x15] = "iload";
        map[0x16] = "lload";
        map[0x17] = "fload";
        map[0x18] = "dload";
        map[0x19] = "aload";

        map[0x1A] = "iload_0";
        map[0x1B] = "iload_1";
        map[0x1C] = "iload_2";
        map[0x1D] = "iload_3";
        map[0x1E] = "lload_0";
        map[0x1F] = "lload_1";
        map[0x20] = "lload_2";
        map[0x21] = "lload_3";
        map[0x22] = "fload_0";
        map[0x23] = "fload_1";
        map[0x24] = "fload_2";
        map[0x25] = "fload_3";
        map[0x26] = "dload_0";
        map[0x27] = "dload_1";
        map[0x28] = "dload_2";
        map[0x29] = "dload_3";
        map[0x2A] = "aload_0";
        map[0x2B] = "aload_1";
        map[0x2C] = "aload_2";
        map[0x2D] = "aload_3";

        map[0x2E] = "iaload";
        map[0x2F] = "laload";
        map[0x30] = "faload";
        map[0x31] = "daload";
        map[0x32] = "aaload";
        map[0x33] = "baload";
        map[0x34] = "caload";
        map[0x35] = "saload";

        map[0x36] = "istore";
        map[0x37] = "lstore";
        map[0x38] = "fstore";
        map[0x39] = "dstore";
        map[0x3A] = "astore";

        map[0x3B] = "istore_0";
        map[0x3C] = "istore_1";
        map[0x3D] = "istore_2";
        map[0x3E] = "istore_3";
        map[0x3F] = "lstore_0";
        map[0x40] = "lstore_1";
        map[0x41] = "lstore_2";
        map[0x42] = "lstore_3";
        map[0x43] = "fstore_0";
        map[0x44] = "fstore_1";
        map[0x45] = "fstore_2";
        map[0x46] = "fstore_3";
        map[0x47] = "dstore_0";
        map[0x48] = "dstore_1";
        map[0x49] = "dstore_2";
        map[0x4A] = "dstore_3";
        map[0x4B] = "astore_0";
        map[0x4C] = "astore_1";
        map[0x4D] = "astore_2";
        map[0x4E] = "astore_3";

        map[0x4F] = "iastore";
        map[0x50] = "lastore";
        map[0x51] = "fastore";
        map[0x52] = "dastore";
        map[0x53] = "aastore";
        map[0x54] = "bastore";
        map[0x55] = "castore";
        map[0x56] = "sastore";

        map[0x57] = "pop";
        map[0x58] = "pop2";
        map[0x59] = "dup";
        map[0x5A] = "dup_x1";
        map[0x5B] = "dup_x2";
        map[0x5C] = "dup2";
        map[0x5D] = "dup2_x1";
        map[0x5E] = "dup2_x2";
        map[0x5F] = "swap";

        map[0x60] = "iadd";
        map[0x61] = "ladd";
        map[0x62] = "fadd";
        map[0x63] = "dadd";

        map[0x64] = "isub";
        map[0x65] = "lsub";
        map[0x66] = "fsub";
        map[0x67] = "dsub";

        map[0x68] = "imul";
        map[0x69] = "lmul";
        map[0x6A] = "fmul";
        map[0x6B] = "dmul";

        map[0x6C] = "idiv";
        map[0x6D] = "ldiv";
        map[0x6E] = "fdiv";
        map[0x6F] = "ddiv";

        map[0x70] = "irem";
        map[0x71] = "lrem";
        map[0x72] = "frem";
        map[0x73] = "drem";

        map[0x74] = "ineg";
        map[0x75] = "lneg";
        map[0x76] = "fneg";
        map[0x77] = "dneg";

        map[0x78] = "ishl";
        map[0x79] = "lshl";
        map[0x7A] = "ishr";
        map[0x7B] = "lshr";
        map[0x7C] = "iushr";
        map[0x7D] = "lushr";

        map[0x7E] = "iand";
        map[0x7F] = "land";
        map[0x80] = "ior";
        map[0x81] = "lor";
        map[0x82] = "ixor";
        map[0x83] = "lxor";

        map[0x84] = "iinc";
        map[0x85] = "i2l";
        map[0x86] = "i2f";
        map[0x87] = "i2d";
        map[0x88] = "l2i";
        map[0x89] = "l2f";
        map[0x8A] = "l2d";
        map[0x8B] = "f2i";
        map[0x8C] = "f2l";
        map[0x8D] = "f2d";
        map[0x8E] = "d2i";
        map[0x8F] = "d2l";
        map[0x90] = "d2f";
        map[0x91] = "i2b";
        map[0x92] = "i2c";
        map[0x93] = "i2s";

        map[0x94] = "lcmp";
        map[0x95] = "fcmpl";
        map[0x96] = "fcmpg";
        map[0x97] = "dcmpl";
        map[0x98] = "dcmpg";
        map[0x99] = "ifeq";
        map[0x9A] = "ifne";
        map[0x9B] = "iflt";
        map[0x9C] = "ifge";
        map[0x9D] = "ifgt";
        map[0x9E] = "ifle";
        map[0x9F] = "if_icmpeq";
        map[0xA0] = "if_icmpne";
        map[0xA1] = "if_icmplt";
        map[0xA2] = "if_icmpge";
        map[0xA3] = "if_icmpgt";
        map[0xA4] = "if_icmple";
        map[0xA5] = "if_acmpeq";
        map[0xA6] = "if_acmpne";

        map[0xA7] = "goto";
        map[0xA8] = "jsr";
        map[0xA9] = "ret";
        map[0xAA] = "tableswitch";
        map[0xAB] = "lookupswitch";

        map[0xAC] = "ireturn";
        map[0xAD] = "lreturn";
        map[0xAE] = "freturn";
        map[0xAF] = "dreturn";
        map[0xB0] = "areturn";
        map[0xB1] = "return";

        map[0xB2] = "getstatic";
        map[0xB3] = "putstatic";
        map[0xB4] = "getfield";
        map[0xB5] = "putfield";
        map[0xB6] = "invokevirtual";
        map[0xB7] = "invokespecial";
        map[0xB8] = "invokestatic";
        map[0xB9] = "invokeinterface";
        map[0xBA] = "invokedynamic";

        map[0xBB] = "new";
        map[0xBC] = "newarray";
        map[0xBD] = "anewarray";
        map[0xBE] = "arraylength";
        map[0xBF] = "athrow";

        map[0xC0] = "checkcast";
        map[0xC1] = "instanceof";
        map[0xC2] = "monitorenter";
        map[0xC3] = "monitorexit";
        map[0xC4] = "wide";
        map[0xC5] = "multianewarray";
        map[0xC6] = "ifnull";
        map[0xC7] = "ifnonnull";
        map[0xC8] = "goto_w";
        map[0xC9] = "jsr_w";

        map[0xCA] = "breakpoint"; // reservado pela JVM
        map[0xCB] = "impdep1"; // reservado pela JVM
        map[0xCC] = "impdep2"; // reservado pela JVM

        map[0xCD] = "no_implementation";
        map[0xCE] = "no_implementation";
        map[0xCF] = "no_implementation";
        map[0xD0] = "no_implementation";
        map[0xD1] = "no_implementation";
        map[0xD2] = "no_implementation";
        map[0xD3] = "no_implementation";
        map[0xD4] = "no_implementation";
        map[0xD5] = "no_implementation";
        map[0xD6] = "no_implementation";
        map[0xD7] = "no_implementation";
        map[0xD8] = "no_implementation";
        map[0xD9] = "no_implementation";
        map[0xDA] = "no_implementation";
        map[0xDB] = "no_implementation";
        map[0xDC] = "no_implementation";
        map[0xDD] = "no_implementation";
        map[0xDE] = "no_implementation";
        map[0xDF] = "no_implementation";
        map[0xE0] = "no_implementation";
        map[0xE1] = "no_implementation";
        map[0xE2] = "no_implementation";
        map[0xE3] = "no_implementation";
        map[0xE4] = "no_implementation";
        map[0xE5] = "no_implementation";
        map[0xE6] = "no_implementation";
        map[0xE7] = "no_implementation";
        map[0xE8] = "no_implementation";
        map[0xE9] = "no_implementation";
        map[0xEA] = "no_implementation";
        map[0xEB] = "no_implementation";
        map[0xEC] = "no_implementation";
        map[0xED] = "no_implementation";
        map[0xEE] = "no_implementation";
        map[0xEF] = "no_implementation";
        map[0xF0] = "no_implementation";
        map[0xF1] = "no_implementation";
        map[0xF2] = "no_implementation";
        map[0xF3] = "no_implementation";
        map[0xF4] = "no_implementation";
        map[0xF5] = "no_implementation";
        map[0xF6] = "no_implementation";
        map[0xF7] = "no_implementation";
        map[0xF8] = "no_implementation";
        map[0xF9] = "no_implementation";
        map[0xFA] = "no_implementation";
        map[0xFB] = "no_implementation";
        map[0xFC] = "no_implementation";
        map[0xFD] = "no_implementation";
        map[0xFE] = "no_implementation";
        map[0xFF] = "no_implementation";
        // END OF FILE


        return map;
    }();
    return BYTECODE;
}

#endif

/*================================================*/
/*            Comentários Auxiliares              */
/*                                                */
/* Descricao da estrutura e bibliotecas usadas:   */
/*                                                */
/* [Bibliotecas]                                  */
/* - <unordered_map>: Tabela hash para buscas     */
/*   rapidas O(1) de chaves e valores.            */
/* - <cstdint>: Tipos de tamanho fixo, como o     */
/*   uint8_t, essencial para bytes (0-255).       */
/* - <string>: Gerencia os mnemonicos em texto.   */
/*                                                */
/* [Estrutura do Arquivo de Cabecalho (.h)]       */
/* - Implementado como uma funcao global "inline" */
/*   permitindo ser incluido em varios arquivos   */
/*   do projeto sem conflitos de definicao.       */
/* - Usa o padrao Meyers' Singleton (C++11) com   */
/*   static const e uma funcao lambda imediata    */
/*   (IIFE) para inicializar o mapa uma unica vez */
/*   de forma thread-safe.                        */
/* - Retorna uma referencia constante (const &)   */
/*   assegurando imutabilidade (somente leitura)  */
/*   para qualquer funcao que o importar.         */
/* - uint8_t (Chave): O byte do opcode.           */
/* - std::string (Valor): O mnemonico da instrucao*/
/*================================================*/

/*================================================*/
/*            Comentários Auxiliares              */
/*                                                */
/* Descricao de Desempenho e Busca (256 OpCodes): */
/*                                                */
/* [Eficiencia da Busca: .find()]                 */
/* - Custo em Tempo: Praticamente instantaneo,    */
/*   na faixa de nanossegundos (bilionésimos de   */
/*   segundo), permitindo milhoes de buscas por   */
/*   segundo.                                     */
/* - Fatores de Otimizacao:                       */
/*   1. Tamanho Compacto: Apenas 256 elementos    */
/*      cabem inteiramente na cache L1/L2 do      */
/*      processador, evitando acessos lentos à    */
/*      memoria RAM.                              */
/*   2. Complexidade O(1): A tabela hash do       */
/*      unordered_map garante busca de acesso     */
/*      direto por chave (uint8_t).               */
/* - Impacto Pratico: Desempenho maximo sem       */
/*   nenhum gargalo perceptivel na leitura e      */
/*   traducao do fluxo de bytes do programa.      */
/*================================================*/

/*!!!Assinado Por CLSP & Cruzeirão Prof. M. Ladeira!!!*/
