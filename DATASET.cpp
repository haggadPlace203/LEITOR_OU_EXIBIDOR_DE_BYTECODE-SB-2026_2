#include <iostream>
#include <unordered_map>
#include <string>
#include <cstdint> // Para uint8_t

// EXEMPLO TESTE NÃO OFICIAL

int main() {
    // Cria o dicionário: Chave (uint8_t / byte) -> Valor (std::string)
    std::unordered_map<uint8_t, std::string> dicionarioBytes;

    // 1. Inserindo dados (Exemplo com prefixos hexadecimais ou decimais)
    dicionarioBytes[0x01] = "Comando Iniciar";
    dicionarioBytes[0x02] = "Comando Parar";
    dicionarioBytes[0xFF] = "Erro Critico";

    // 2. Buscando um valor
    uint8_t byteRecebido = 0x02;

    // Verifica se o byte existe no dicionário antes de acessar
    auto it = dicionarioBytes.find(byteRecebido);
    if (it != dicionarioBytes.end()) {
        std::cout << "O byte " << (int)byteRecebido << " significa: " << it->second << std::endl;
    } else {
        std::cout << "Byte nao encontrado no dicionario." << std::endl;
    }

    return 0;
}
