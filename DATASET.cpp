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


    BYTECODE[0xFF] = "";  // END OF FILE

    return 0;
}
