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


    BYTECODE[0xFF] = "";  // END OF FILE

    return 0;
}
