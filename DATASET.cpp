/*==================================================*/
/*  Informações sobre o desenvolvedor & assinatura  */
/*   Carlos Luan Silva Pereira - Eng. Mecatrônica   */
/*       Estudante da Universidade de Brasília      */
/*           Brasília - DF - 30/10/2026             */
/*==================================================*/

#include <iostream>
#include <unordered_map>
#include <string>
#include <cstdint>

int main(){

    std::unordered_map<uint8_t, std::string> BYTECODE;

    BYTECODE[0x00] = "nop";
    BYTECODE[0x01] = "aconst_null";
    BYTECODE[0x02] = "iconst_m1";

    BYTECODE[0x03] = "iconst_0";
    BYTECODE[0x04] = "iconst_1";
    BYTECODE[0x05] = "iconst_2";
    BYTECODE[0x06] = "iconst_3";
    BYTECODE[0x07] = "iconst_4";
    BYTECODE[0x08] = "iconst_5";
    BYTECODE[0x09] = "lconst_0";
    BYTECODE[0x0A] = "lconst_1";
    BYTECODE[0x0B] = "fconst_0";
    BYTECODE[0x0C] = "fconst_1";
    BYTECODE[0x0D] = "fconst_2";
    BYTECODE[0x0E] = "dconst_0";
    BYTECODE[0x0F] = "dconst_1";

    BYTECODE[0x10] = "bipush";
    BYTECODE[0x11] = "sipush";
    BYTECODE[0x12] = "ldc";
    BYTECODE[0x13] = "ldc_w";
    BYTECODE[0x14] = "ldc2_w";

    BYTECODE[0x15] = "iload";
    BYTECODE[0x16] = "lload";
    BYTECODE[0x17] = "fload";
    BYTECODE[0x18] = "dload";
    BYTECODE[0x19] = "aload";

    BYTECODE[0x1A] = "iload_0";
    BYTECODE[0x1B] = "iload_1";
    BYTECODE[0x1C] = "iload_2";
    BYTECODE[0x1D] = "iload_3";
    BYTECODE[0x1E] = "lload_0";
    BYTECODE[0x1F] = "lload_1";
    BYTECODE[0x20] = "lload_2";
    BYTECODE[0x21] = "lload_3";
    BYTECODE[0x22] = "fload_0";
    BYTECODE[0x23] = "fload_1";
    BYTECODE[0x24] = "fload_2";
    BYTECODE[0x25] = "fload_3";
    BYTECODE[0x26] = "dload_0";
    BYTECODE[0x27] = "dload_1";
    BYTECODE[0x28] = "dload_2";
    BYTECODE[0x29] = "dload_3";
    BYTECODE[0x2A] = "aload_0";
    BYTECODE[0x2B] = "aload_1";
    BYTECODE[0x2C] = "aload_2";
    BYTECODE[0x2D] = "aload_3";

    BYTECODE[0x2E] = "iaload";
    BYTECODE[0x2F] = "laload";
    BYTECODE[0x30] = "faload";
    BYTECODE[0x31] = "daload";
    BYTECODE[0x32] = "aaload";
    BYTECODE[0x33] = "baload";
    BYTECODE[0x34] = "caload";
    BYTECODE[0x35] = "saload";

    BYTECODE[0x36] = "istore";
    BYTECODE[0x37] = "lstore";
    BYTECODE[0x38] = "fstore";
    BYTECODE[0x39] = "dstore";
    BYTECODE[0x3A] = "astore";

    BYTECODE[0x3B] = "istore_0";
    BYTECODE[0x3C] = "istore_1";
    BYTECODE[0x3D] = "istore_2";
    BYTECODE[0x3E] = "istore_3";
    BYTECODE[0x3F] = "lstore_0";
    BYTECODE[0x40] = "lstore_1";
    BYTECODE[0x41] = "lstore_2";
    BYTECODE[0x42] = "lstore_3";
    BYTECODE[0x43] = "fstore_0";
    BYTECODE[0x44] = "fstore_1";
    BYTECODE[0x45] = "fstore_2";
    BYTECODE[0x46] = "fstore_3";
    BYTECODE[0x47] = "dstore_0";
    BYTECODE[0x48] = "dstore_1";
    BYTECODE[0x49] = "dstore_2";
    BYTECODE[0x4A] = "dstore_3";
    BYTECODE[0x4B] = "astore_0";
    BYTECODE[0x4C] = "astore_1";
    BYTECODE[0x4D] = "astore_2";
    BYTECODE[0x4E] = "astore_3";

    BYTECODE[0x4F] = "iastore";
    BYTECODE[0x50] = "lastore";
    BYTECODE[0x51] = "fastore";
    BYTECODE[0x52] = "dastore";
    BYTECODE[0x53] = "aastore";
    BYTECODE[0x54] = "bastore";
    BYTECODE[0x55] = "castore";
    BYTECODE[0x56] = "sastore";

    BYTECODE[0x57] = "pop";
    BYTECODE[0x58] = "pop2";
    BYTECODE[0x59] = "dup";
    BYTECODE[0x5A] = "dup_x1";
    BYTECODE[0x5B] = "dup_x2";
    BYTECODE[0x5C] = "dup2";
    BYTECODE[0x5D] = "dup2_x1";
    BYTECODE[0x5E] = "dup2_x2";
    BYTECODE[0x5F] = "swap";

    BYTECODE[0x60] = "iadd";
    BYTECODE[0x61] = "ladd";
    BYTECODE[0x62] = "fadd";
    BYTECODE[0x63] = "dadd";

    BYTECODE[0x64] = "isub";
    BYTECODE[0x65] = "lsub";
    BYTECODE[0x66] = "fsub";
    BYTECODE[0x67] = "dsub";

    BYTECODE[0x68] = "imul";
    BYTECODE[0x69] = "lmul";
    BYTECODE[0x6A] = "fmul";
    BYTECODE[0x6B] = "dmul";

    BYTECODE[0x6C] = "idiv";
    BYTECODE[0x6D] = "ldiv";
    BYTECODE[0x6E] = "fdiv";
    BYTECODE[0x6F] = "ddiv";

    BYTECODE[0x70] = "irem";
    BYTECODE[0x71] = "lrem";
    BYTECODE[0x72] = "frem";
    BYTECODE[0x73] = "drem";

    BYTECODE[0x74] = "ineg";
    BYTECODE[0x75] = "lneg";
    BYTECODE[0x76] = "fneg";
    BYTECODE[0x77] = "dneg";

    BYTECODE[0x78] = "ishl";
    BYTECODE[0x79] = "lshl";
    BYTECODE[0x7A] = "ishr";
    BYTECODE[0x7B] = "lshr";
    BYTECODE[0x7C] = "iushr";
    BYTECODE[0x7D] = "lushr";

    BYTECODE[0x7E] = "iand";
    BYTECODE[0x7F] = "land";
    BYTECODE[0x80] = "ior";
    BYTECODE[0x81] = "lor";
    BYTECODE[0x82] = "ixor";
    BYTECODE[0x83] = "lxor";

    BYTECODE[0x84] = "iinc";
    BYTECODE[0x85] = "i2l";
    BYTECODE[0x86] = "i2f";
    BYTECODE[0x87] = "i2d";
    BYTECODE[0x88] = "l2i";
    BYTECODE[0x89] = "l2f";
    BYTECODE[0x8A] = "l2d";
    BYTECODE[0x8B] = "f2i";
    BYTECODE[0x8C] = "f2l";
    BYTECODE[0x8D] = "f2d";
    BYTECODE[0x8E] = "d2i";
    BYTECODE[0x8F] = "d2l";
    BYTECODE[0x90] = "d2f";
    BYTECODE[0x91] = "i2b";
    BYTECODE[0x92] = "i2c";
    BYTECODE[0x93] = "i2s";

    BYTECODE[0x94] = "lcmp";
    BYTECODE[0x95] = "fcmpl";
    BYTECODE[0x96] = "fcmpg";
    BYTECODE[0x97] = "dcmpl";
    BYTECODE[0x98] = "dcmpg";
    BYTECODE[0x99] = "ifeq";
    BYTECODE[0x9A] = "ifne";
    BYTECODE[0x9B] = "iflt";
    BYTECODE[0x9C] = "ifge";
    BYTECODE[0x9D] = "ifgt";
    BYTECODE[0x9E] = "ifle";
    BYTECODE[0x9F] = "if_icmpeq";
    BYTECODE[0xA0] = "if_icmpne";
    BYTECODE[0xA1] = "if_icmplt";
    BYTECODE[0xA2] = "if_icmpge";
    BYTECODE[0xA3] = "if_icmpgt";
    BYTECODE[0xA4] = "if_icmple";
    BYTECODE[0xA5] = "if_acmpeq";
    BYTECODE[0xA6] = "if_acmpne";

    BYTECODE[0xA7] = "goto";
    BYTECODE[0xA8] = "jsr";
    BYTECODE[0xA9] = "ret";
    BYTECODE[0xAA] = "tableswitch";
    BYTECODE[0xAB] = "lookupswitch";

    BYTECODE[0xAC] = "ireturn";
    BYTECODE[0xAD] = "lreturn";
    BYTECODE[0xAE] = "freturn";
    BYTECODE[0xAF] = "dreturn";
    BYTECODE[0xB0] = "areturn";
    BYTECODE[0xB1] = "return";

    BYTECODE[0xB2] = "getstatic";
    BYTECODE[0xB3] = "putstatic";
    BYTECODE[0xB4] = "getfield";
    BYTECODE[0xB5] = "putfield";
    BYTECODE[0xB6] = "invokevirtual";
    BYTECODE[0xB7] = "invokespecial";
    BYTECODE[0xB8] = "invokestatic";
    BYTECODE[0xB9] = "invokeinterface";
    BYTECODE[0xBA] = "invokedynamic";

    BYTECODE[0xBB] = "new";
    BYTECODE[0xBC] = "newarray";
    BYTECODE[0xBD] = "anewarray";
    BYTECODE[0xBE] = "arraylength";
    BYTECODE[0xBF] = "athrow";

    BYTECODE[0xC0] = "checkcast";
    BYTECODE[0xC1] = "instanceof";
    BYTECODE[0xC2] = "monitorenter";
    BYTECODE[0xC3] = "monitorexit";
    BYTECODE[0xC4] = "wide";
    BYTECODE[0xC5] = "multianewarray";
    BYTECODE[0xC6] = "ifnull";
    BYTECODE[0xC7] = "ifnonnull";
    BYTECODE[0xC8] = "goto_w";
    BYTECODE[0xC9] = "jsr_w";

    BYTECODE[0xCA] = "breakpoint"; // reservado pela JVM
    BYTECODE[0xCB] = "impdep1"; // reservado pela JVM
    BYTECODE[0xCC] = "impdep2"; // reservado pela JVM

    BYTECODE[0xCD] = "no_implementation";
    BYTECODE[0xCE] = "no_implementation";
    BYTECODE[0xCF] = "no_implementation";
    BYTECODE[0xD0] = "no_implementation";
    BYTECODE[0xD1] = "no_implementation";
    BYTECODE[0xD2] = "no_implementation";
    BYTECODE[0xD3] = "no_implementation";
    BYTECODE[0xD4] = "no_implementation";
    BYTECODE[0xD5] = "no_implementation";
    BYTECODE[0xD6] = "no_implementation";
    BYTECODE[0xD7] = "no_implementation";
    BYTECODE[0xD8] = "no_implementation";
    BYTECODE[0xD9] = "no_implementation";
    BYTECODE[0xDA] = "no_implementation";
    BYTECODE[0xDB] = "no_implementation";
    BYTECODE[0xDC] = "no_implementation";
    BYTECODE[0xDD] = "no_implementation";
    BYTECODE[0xDE] = "no_implementation";
    BYTECODE[0xDF] = "no_implementation";
    BYTECODE[0xE0] = "no_implementation";
    BYTECODE[0xE1] = "no_implementation";
    BYTECODE[0xE2] = "no_implementation";
    BYTECODE[0xE3] = "no_implementation";
    BYTECODE[0xE4] = "no_implementation";
    BYTECODE[0xE5] = "no_implementation";
    BYTECODE[0xE6] = "no_implementation";
    BYTECODE[0xE7] = "no_implementation";
    BYTECODE[0xE8] = "no_implementation";
    BYTECODE[0xE9] = "no_implementation";
    BYTECODE[0xEA] = "no_implementation";
    BYTECODE[0xEB] = "no_implementation";
    BYTECODE[0xEC] = "no_implementation";
    BYTECODE[0xED] = "no_implementation";
    BYTECODE[0xEE] = "no_implementation";
    BYTECODE[0xEF] = "no_implementation";
    BYTECODE[0xF0] = "no_implementation";
    BYTECODE[0xF1] = "no_implementation";
    BYTECODE[0xF2] = "no_implementation";
    BYTECODE[0xF3] = "no_implementation";
    BYTECODE[0xF4] = "no_implementation";
    BYTECODE[0xF5] = "no_implementation";
    BYTECODE[0xF6] = "no_implementation";
    BYTECODE[0xF7] = "no_implementation";
    BYTECODE[0xF8] = "no_implementation";
    BYTECODE[0xF9] = "no_implementation";
    BYTECODE[0xFA] = "no_implementation";
    BYTECODE[0xFB] = "no_implementation";
    BYTECODE[0xFC] = "no_implementation";
    BYTECODE[0xFD] = "no_implementation";
    BYTECODE[0xFE] = "no_implementation";
    BYTECODE[0xFF] = "no_implementation";
    // END OF FILE

    return 0;
}


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
/* [Estrutura: unordered_map<uint8_t, string>]    */
/* - uint8_t (Chave): O byte do opcode (ex: 0x40) */
/*   garantindo precisao e baixo uso de memoria.  */
/* - std::string (Valor): O nome textual da       */
/*   instrucao do bytecode (ex: "saload").        */
/*================================================*/
