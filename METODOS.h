#ifndef METODOS_H
#define METODOS_H

#include <vector>
#include <stdint.h>

struct VMContext {
    // 1. PILHA DE OPERANDOS: tamanho máximo N
    std::vector<int32_t> pilha;
    int32_t topo_pilha;

    // 2. VARIÁVEIS LOCAIS: tamanho máximo N
    std::vector<int32_t> locais;

    // 3. VARIÁVEIS TEMPORÁRIAS: tamanho máximo N
    std::vector<int32_t> temporarias;

    // 4. CONTROLE DO MÉTODO: Program Counter (pc) e o atributo 'code' vindo do main
    uint32_t pc;
    const std::vector<uint8_t>& code;

    // Construtor: Permite criar o frame passando os limites N e o atributo 'code' diretamente do main
    VMContext(size_t max_stack, size_t max_locals, size_t max_temps, const std::vector<uint8_t>& atributo_code)
        : pilha(max_stack, INT32_C(0)),
          topo_pilha(-1),
          locais(max_locals, INT32_C(0)),
          temporarias(max_temps, INT32_C(0)),
          pc(0),
          code(atributo_code){}

    void push(int32_t val) {
        if (static_cast<size_t>(topo_pilha + 1) < pilha.size()) {
            pilha[++topo_pilha] = val;
        }
    }

    void pop() {
        if (topo_pilha >= 0) {
            topo_pilha--;
        }
    }

    int32_t top() const {
        return (topo_pilha >= 0) ? pilha[topo_pilha] : INT32_C(0);
    }

    size_t size() const {
        return static_cast<size_t>(topo_pilha + 1);
    }

    bool empty() const {
        return topo_pilha < 0;
    }
};

#endif