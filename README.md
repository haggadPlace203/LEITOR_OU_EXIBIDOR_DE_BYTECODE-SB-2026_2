# Leitor e Exibidor de Bytecode Java (JVM)

Este é um programa desenvolvido em **C++** projetado para ler, analisar e exibir de forma detalhada e estruturada o conteúdo de arquivos binários `.class` (bytecode Java), em conformidade com as especificações da *Java Virtual Machine Specification* (JVM). 

O software atua como uma ferramenta de engenharia reversa e análise estática, traduzindo o fluxo de bytes de um arquivo compilado pelo Java (`javac`) em informações textuais legíveis. Ele expõe toda a arquitetura interna do arquivo, incluindo tabelas de constantes, estruturas de classes, atributos, métodos e as instruções de bytecode associadas.

## 🔍 Funcionalidades Principais

*   **Verificação de Assinatura e Versão:** Validação do número mágico (*Magic Number* `0xCAFEBABE`) e identificação das versões maior (*major version*) e menor (*minor version*) do compilador Java utilizado.
*   **Decodificação do Constant Pool (Pool de Constantes):** Leitura completa e exibição detalhada de todos os elementos do pool de constantes (como `CONSTANT_Class`, `CONSTANT_Fieldref`, `CONSTANT_Methodref`, `CONSTANT_String`, `Utf8`, entre outros), resolvendo os índices e referências internas de forma amigável.
*   **Análise de Interfaces e Campos:** Mapeamento de todas as interfaces implementadas pela classe e detalhamento dos campos (*fields*), incluindo suas flags de acesso (público, privado, estático, final, etc.), nomes e descritores de tipo.
*   **Exibição Estruturada de Métodos:** Listagem de todos os métodos da classe, mapeando suas assinaturas e estruturas internas.
*   **Desmontagem de Bytecode (Disassembler):** Varredura e decodificação do atributo `Code` dentro dos métodos, traduzindo os opcodes binários (como `aload_0`, `invokespecial`, `return`) em mnemônicos legíveis, acompanhados de seus respectivos argumentos.
*   **Mapeamento de Atributos da JVM:** Leitura de atributos complexos essenciais para depuração e controle, tais como `LineNumberTable`, `SourceFile`, `LocalVariableTable` e tabelas de exceções.

## 🛠️ Tecnologias Utilizadas

*   **Linguagem:** C++ (padrão C++11 ou superior).
*   **Manipulação de Arquivos:** Fluxos binários (`std::ifstream`) para leitura precisa byte a byte, respeitando a ordenação *Big-Endian* nativa dos arquivos da JVM.
*   **Estrutura de Dados:** Alocação dinâmica e gerenciamento de coleções para representação fiel das tabelas de tamanho variável descritas na especificação do formato `.class`.

## 🚀 Como Compilar e Executar

### Pré-requisitos
*   Compilador G++ (GCC) instalado no sistema.

### 1. Compilação via Terminal
Abra o Prompt de Comando (CMD) ou terminal na pasta raiz do projeto e execute o comando abaixo para gerar o executável:

```bash
g++ DATASET.cpp -o DATASET.exe
```

### 2. Execução
Após a compilação bem-sucedida, você pode rodar o programa passando o arquivo `.class` que deseja analisar como parâmetro (caso seu código mude para aceitar argumentos via `argv`) ou simplesmente executando o arquivo:

```bash
DATASET.exe
```

## 📁 Estrutura sugerida do Repositório

```text
├── DATASET.cpp          # Código-fonte principal do Leitor/Exibidor
├── exemplos/            # Pasta contendo arquivos .class para testes
└── README.md            # Documentação do projeto
```
