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
    const ClassFile* classe;
    bool erro_execucao;

    // Construtor: Permite criar o frame passando os limites N e o atributo 'code' diretamente do main
    VMContext(size_t max_stack, size_t max_locals, size_t max_temps, const std::vector<uint8_t>& atributo_code, const ClassFile* classe_carregada = nullptr)
        : pilha(max_stack, INT32_C(0)),
          topo_pilha(-1),
          locais(max_locals, INT32_C(0)),
          temporarias(max_temps, INT32_C(0)),
          pc(0),
          code(atributo_code),
          classe(classe_carregada),
          erro_execucao(false) {}

    void push(int32_t val) {
        if (static_cast<size_t>(topo_pilha + 1) < pilha.size()) {
            pilha[++topo_pilha] = val;
        }else{
            erro_execucao = true;
            std::cerr << "Erro: Stack overflow em VMContext (pc=" << pc << ").\n";
        }
    }

    int32_t pop() {
        if (topo_pilha >= 0) {
            return pilha[topo_pilha--];
        }
        erro_execucao = true;
        std::cerr << "Erro: Stack underflow em VMContext (pc=" << pc << ").\n";
        return INT32_C(0);
    }

    int32_t top() const {
        return (topo_pilha >= 0) ? pilha[topo_pilha] : INT32_C(0);
    }

    size_t size() const {
        return (topo_pilha >= 0) ? static_cast<size_t>(topo_pilha + 1) : 0;
    }

    bool empty() const {
        return topo_pilha < 0;
    }

    void set_local(size_t idx, int32_t val) {
        if (idx < locais.size()) {
            locais[idx] = val;
        } else {
            erro_execucao = true;
            std::cerr << "Aviso: Escrita ignorada em variavel local fora do limite (" << idx << ").\n";
        }
    }

    int32_t get_local(size_t idx) const {
        if (idx < locais.size()) {
            return locais[idx];
        }
        std::cerr << "Erro: Leitura fora dos limites em locais[" << idx << "].\n";
        return INT32_C(0);
    }
};

#endif