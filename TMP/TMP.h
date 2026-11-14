#ifndef TMP_H
#define TMP_H

#include <unordered_map>
#include <stdint.h>

// Define o tamanho máximo N para as três estruturas (ex: N = 256)
#define MAX_N 256

/*================================================*/
/*            Estrutura de Contexto da VM         */
/*================================================*/
struct VMContext {
    // 1. PILHA: contendo tamanho máximo N e controle de topo
    int32_t pilha[MAX_N];
    int32_t topo_pilha;

    // 2. VARIÁVEIS LOCAIS: contendo tamanho máximo N
    int32_t locais[MAX_N];

    // 3. VARIÁVEIS TEMPORÁRIAS: contendo tamanho máximo N
    int32_t temporarias[MAX_N];

    // Construtor: Inicializa o topo da pilha como vazio (-1) e zera os buffers
    VMContext() : topo_pilha(-1) {
        for (size_t i = 0; i < MAX_N; ++i) {
            pilha[i] = 0;
            locais[i] = 0;
            temporarias[i] = 0;
        }
    }

    // Métodos de controle da Pilha com proteção contra estouro (Tamanho N)
    void push(int32_t val) {
        if (topo_pilha + 1 < MAX_N) {
            pilha[++topo_pilha] = val;
        }
    }

    void pop() {
        if (topo_pilha >= 0) {
            topo_pilha--;
        }
    }

    int32_t top() const {
        if (topo_pilha >= 0) {
            return pilha[topo_pilha];
        }
        return 0;
    }

    size_t size() const {
        return static_cast<size_t>(topo_pilha + 1);
    }

    bool empty() const {
        return topo_pilha < 0;
    }
};

/*================================================*/
/*            Instruções (Operações)              */
/*================================================*/

// Byte 0x01: Coloca o valor 3 no topo da pilha
inline void op_01(VMContext& ctx) {
    ctx.push(INT32_C(3));
}

// Byte 0x02: Coloca o valor 3 no topo da pilha
inline void op_02(VMContext& ctx) {
    ctx.push(INT32_C(3));
}

// Byte 0x03: Remove os dois valores da pilha, soma e coloca no topo
inline void op_03(VMContext& ctx) {
    if (ctx.size() >= 2) {
        int32_t val1 = ctx.top();
        ctx.pop();

        int32_t val2 = ctx.top();
        ctx.pop();

        ctx.push(val2 + val1);
    }
}

// Byte 0x04: Retira o valor do topo da pilha e guarda na Variável Local [0]
inline void op_04_store_local0(VMContext& ctx) {
    if (!ctx.empty()) {
        ctx.locais[0] = ctx.top();
        ctx.pop();
    }
}

// Byte 0x05: Carrega o valor da Variável Local [0] para o topo da pilha
inline void op_05_load_local0(VMContext& ctx) {
    ctx.push(ctx.locais[0]);
}

// Byte 0x06: Copia o topo da pilha para a Variável Temporária [0] (sem desempilhar)
inline void op_06_temp_save(VMContext& ctx) {
    if (!ctx.empty()) {
        ctx.temporarias[0] = ctx.top();
    }
}

/*================================================*/
/*            Ponteiro de Função e Tabela         */
/*================================================*/

// O ponteiro de função agora recebe o contexto completo da VM por referência
typedef void (*OperationFunc)(VMContext&);

// Tabela Hash imutável global associando Byte -> Ponteiro de Função
inline const std::unordered_map<uint8_t, OperationFunc>& get_instruction() {
    static const std::unordered_map<uint8_t, OperationFunc> BYTECODE = [] {
        std::unordered_map<uint8_t, OperationFunc> map;

        map[0x01] = op_01;               // Empilha 3
        map[0x02] = op_02;               // Empilha 3
        map[0x03] = op_03;               // Soma topo da pilha
        map[0x04] = op_04_store_local0;  // Pilha -> Variável Local [0]
        map[0x05] = op_05_load_local0;   // Variável Local [0] -> Pilha
        map[0x06] = op_06_temp_save;     // Pilha -> Variável Temporária [0]

        return map;
    }();
    return BYTECODE;
}

/*================================================*/
/*            Comentários Auxiliares              */
/*                                                */
/* Descricao da arquitetura multicomponente:      */
/*                                                */
/* - VMContext: Agrega as 3 estruturas (Pilha,    */
/*   Locais, Temporarias) limitadas ao tamanho N. */
/* - Passagem por referência (VMContext&): Permite*/
/*   que qualquer função externa altere simultan- */
/*   eamente qualquer uma das 3 áreas de memória. */
/* - Segurança de Limites: Arrays estáticos de    */
/*   tamanho MAX_N com validação de índices e     */
/*   de topo previnem estouros de pilha.          */
/*================================================*/

#endif