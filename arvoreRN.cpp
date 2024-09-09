#include <iostream>
#include <list> // Para manter a ordem de inserção

using namespace std;

enum Cor { VERMELHO, PRETO };

struct No {
    int valor;
    Cor cor;
    No *esquerda, *direita, *pai;

    No(int valor) {
        this->valor = valor;
        esquerda = direita = pai = nullptr;
        this->cor = VERMELHO;
    }
};

class ArvoreRN {
private:
    No* raiz;
    list<No*> ordemInsercao; // Lista para manter a ordem de inserção

protected:
    void rotacaoEsquerda(No* &raiz, No* &no);
    void rotacaoDireita(No* &raiz, No* &no);
    void corrigirInsercao(No* &raiz, No* &no);
    void corrigirRemocao(No* &raiz, No* &no);
    No* minimo(No* no);
    void substituirNo(No* u, No* v);

public:
    ArvoreRN() { raiz = nullptr; }

    void inserir(int valor);
    void remover(int valor);
    void mostrarEmOrdem();
    void emOrdem();
    No* getRaiz() { return raiz; }
};

// Rotação à esquerda
void ArvoreRN::rotacaoEsquerda(No* &raiz, No* &no) {
    No* direitaNo = no->direita;
    no->direita = direitaNo->esquerda;

    if (direitaNo->esquerda != nullptr)
        direitaNo->esquerda->pai = no;

    direitaNo->pai = no->pai;

    if (no->pai == nullptr)
        raiz = direitaNo;
    else if (no == no->pai->esquerda)
        no->pai->esquerda = direitaNo;
    else
        no->pai->direita = direitaNo;

    direitaNo->esquerda = no;
    no->pai = direitaNo;
}

// Rotação à direita
void ArvoreRN::rotacaoDireita(No* &raiz, No* &no) {
    No* esquerdaNo = no->esquerda;
    no->esquerda = esquerdaNo->direita;

    if (esquerdaNo->direita != nullptr)
        esquerdaNo->direita->pai = no;

    esquerdaNo->pai = no->pai;

    if (no->pai == nullptr)
        raiz = esquerdaNo;
    else if (no == no->pai->esquerda)
        no->pai->esquerda = esquerdaNo;
    else
        no->pai->direita = esquerdaNo;

    esquerdaNo->direita = no;
    no->pai = esquerdaNo;
}

// Inserir um nó
void ArvoreRN::inserir(int valor) {
    No* novoNo = new No(valor);
    No* paiNo = nullptr;
    No* atual = raiz;

    while (atual != nullptr) {
        paiNo = atual;
        if (novoNo->valor < atual->valor)
            atual = atual->esquerda;
        else
            atual = atual->direita;
    }

    novoNo->pai = paiNo;

    if (paiNo == nullptr)
        raiz = novoNo;
    else if (novoNo->valor < paiNo->valor)
        paiNo->esquerda = novoNo;
    else
        paiNo->direita = novoNo;

    novoNo->esquerda = novoNo->direita = nullptr;
    novoNo->cor = VERMELHO;

    ordemInsercao.push_back(novoNo); // Adiciona o nó à lista de inserção

    corrigirInsercao(raiz, novoNo);
    mostrarEmOrdem(); // Exibe a árvore após a inserção
}

// Corrigir após inserção
void ArvoreRN::corrigirInsercao(No* &raiz, No* &no) {
    No* paiNo = nullptr;
    No* avoNo = nullptr;

    while ((no != raiz) && (no->cor != PRETO) && (no->pai->cor == VERMELHO)) {
        paiNo = no->pai;
        avoNo = no->pai->pai;

        if (paiNo == avoNo->esquerda) {
            No* tioNo = avoNo->direita;

            if (tioNo != nullptr && tioNo->cor == VERMELHO) {
                avoNo->cor = VERMELHO;
                paiNo->cor = PRETO;
                tioNo->cor = PRETO;
                no = avoNo;
            } else {
                if (no == paiNo->direita) {
                    rotacaoEsquerda(raiz, paiNo);
                    no = paiNo;
                    paiNo = no->pai;
                }
                rotacaoDireita(raiz, avoNo);
                swap(paiNo->cor, avoNo->cor);
                no = paiNo;
            }
        } else {
            No* tioNo = avoNo->esquerda;

            if (tioNo != nullptr && tioNo->cor == VERMELHO) {
                avoNo->cor = VERMELHO;
                paiNo->cor = PRETO;
                tioNo->cor = PRETO;
                no = avoNo;
            } else {
                if (no == paiNo->esquerda) {
                    rotacaoDireita(raiz, paiNo);
                    no = paiNo;
                    paiNo = no->pai;
                }
                rotacaoEsquerda(raiz, avoNo);
                swap(paiNo->cor, avoNo->cor);
                no = paiNo;
            }
        }
    }
    raiz->cor = PRETO;
}

// Encontrar o nó mínimo
No* ArvoreRN::minimo(No* no) {
    while (no->esquerda != nullptr)
        no = no->esquerda;
    return no;
}

// Substituir um nó
void ArvoreRN::substituirNo(No* u, No* v) {
    if (u->pai == nullptr)
        raiz = v;
    else if (u == u->pai->esquerda)
        u->pai->esquerda = v;
    else
        u->pai->direita = v;

    if (v != nullptr)
        v->pai = u->pai;
}

// Remover um nó
void ArvoreRN::remover(int valor) {
    if (raiz == nullptr) {
        cout << "A árvore está vazia!" << endl;
        return;
    }

    No* z = raiz;
    No* y;
    No* x;

    // Encontra o nó a ser removido
    while (z != nullptr && z->valor != valor) {
        if (valor < z->valor)
            z = z->esquerda;
        else
            z = z->direita;
    }

    if (z == nullptr) {
        cout << "O valor " << valor << " não foi encontrado na árvore!" << endl;
        return; // Nó não encontrado
    }

    y = z;
    Cor corOriginalY = y->cor;

    if (z->esquerda == nullptr) {
        x = z->direita;
        substituirNo(z, z->direita);
    } else if (z->direita == nullptr) {
        x = z->esquerda;
        substituirNo(z, z->esquerda);
    } else {
        y = minimo(z->direita);
        corOriginalY = y->cor;
        x = y->direita;

        if (y->pai == z)
            x->pai = y;
        else {
            substituirNo(y, y->direita);
            y->direita = z->direita;
            y->direita->pai = y;
        }

        substituirNo(z, y);
        y->esquerda = z->esquerda;
        y->esquerda->pai = y;
        y->cor = z->cor;
    }

    delete z;

    if (corOriginalY == PRETO)
        corrigirRemocao(raiz, x);

    mostrarEmOrdem(); // Exibe a árvore após a remoção
}

// Corrigir após remoção
void ArvoreRN::corrigirRemocao(No* &raiz, No* &x) {
    while (x != raiz && (x == nullptr || x->cor == PRETO)) {
        if (x == x->pai->esquerda) {
            No* w = x->pai->direita;
            if (w->cor == VERMELHO) {
                w->cor = PRETO;
                x->pai->cor = VERMELHO;
                rotacaoEsquerda(raiz, x->pai);
                w = x->pai->direita;
            }

            if ((w->esquerda == nullptr || w->esquerda->cor == PRETO) &&
                (w->direita == nullptr || w->direita->cor == PRETO)) {
                w->cor = VERMELHO;
                x = x->pai;
            } else {
                if (w->direita == nullptr || w->direita->cor == PRETO) {
                    if (w->esquerda != nullptr)
                        w->esquerda->cor = PRETO;
                    w->cor = VERMELHO;
                    rotacaoDireita(raiz, w);
                    w = x->pai->direita;
                }
                w->cor = x->pai->cor;
                x->pai->cor = PRETO;
                if (w->direita != nullptr)
                    w->direita->cor = PRETO;
                rotacaoEsquerda(raiz, x->pai);
                x = raiz;
            }
        } else {
            No* w = x->pai->esquerda;
            if (w->cor == VERMELHO) {
                w->cor = PRETO;
                x->pai->cor = VERMELHO;
                rotacaoDireita(raiz, x->pai);
                w = x->pai->esquerda;
            }

            if ((w->direita == nullptr || w->direita->cor == PRETO) &&
                (w->esquerda == nullptr || w->esquerda->cor == PRETO)) {
                w->cor = VERMELHO;
                x = x->pai;
            } else {
                if (w->esquerda == nullptr || w->esquerda->cor == PRETO) {
                    if (w->direita != nullptr)
                        w->direita->cor = PRETO;
                    w->cor = VERMELHO;
                    rotacaoEsquerda(raiz, w);
                    w = x->pai->esquerda;
                }
                w->cor = x->pai->cor;
                x->pai->cor = PRETO;
                if (w->esquerda != nullptr)
                    w->esquerda->cor = PRETO;
                rotacaoDireita(raiz, x->pai);
                x = raiz;
            }
        }
    }
    if (x != nullptr)
        x->cor = PRETO;
}

// Mostrar em ordem com cores
void ArvoreRN::mostrarEmOrdem() {
    emOrdem();
    cout << endl;
}

void ArvoreRN::emOrdem() {
    // Exibe os nós na ordem de inserção
    for (No* no : ordemInsercao) {
        // Exibe o nó com a cor correspondente
        if (no->cor == VERMELHO)
            cout << "\033[1;31m" << no->valor << "\033[0m ";  // Vermelho
        else
            cout << "\033[1;30m" << no->valor << "\033[0m ";  // Preto
    }
}

// Função principal
int main() {
    ArvoreRN arvore;
    int opcao, valor;

    do {
        cout << "\n1. Inserir nó\n2. Remover nó\n3. Sair\nEscolha: ";
        cin >> opcao;

        switch (opcao) {
        case 1:
            cout << "Insira o valor: ";
            cin >> valor;
            arvore.inserir(valor);
            break;
        case 2:
            cout << "Insira o valor a ser removido: ";
            cin >> valor;
            arvore.remover(valor);
            break;
        case 3:
            cout << "Saindo...\n";
            break;
        default:
            cout << "Opção inválida!\n";
        }
    } while (opcao != 3);

    return 0;
}
