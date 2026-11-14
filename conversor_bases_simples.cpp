#include <climits>
#include <iostream>
#include <string>
#include <vector>

using namespace std;

// arrays'vetores com os dados das bases
const vector<int> BASES = {2, 5, 8, 10, 16};

const vector<string> NOMES_BASES = {"Binario", "Pental", "Octal", "Decimal", "Hexadecimal"};

const vector<string> DIGITOS_PERMITIDOS = {
    "'0' e '1'",
    "'0' a '4'",
    "'0' a '7'",
    "'0' a '9'",
    "'0' a '9' e 'A' a 'F' (maiusculas ou minusculas)"};

const string DIGITOS = "0123456789ABCDEF";

// func das bases disponiveis

int posicaoDaBase(int base) {
    for (size_t i = 0; i < BASES.size(); i++) {
        if (BASES[i] == base) {
            return (int)i;
        }
    }
    return -1;
}

bool baseValida(int base) {
    return posicaoDaBase(base) != -1;
}

string nomeDaBase(int base) {
    int pos = posicaoDaBase(base);
    if (pos == -1) {
        return "Desconhecida";
    }
    return NOMES_BASES[pos];
}

string digitosPermitidos(int base) {
    int pos = posicaoDaBase(base);
    if (pos == -1) {
        return "";
    }
    return DIGITOS_PERMITIDOS[pos];
}

int valorDoDigito(char c) {
    if (c >= '0' && c <= '9') return c - '0';
    if (c >= 'A' && c <= 'F') return c - 'A' + 10;
    if (c >= 'a' && c <= 'f') return c - 'a' + 10;
    return -1;
}

char digitoDoValor(int valor) {
    return DIGITOS[valor];
}

// validar os num recebidos

bool numeroValido(string numero, int base) {
    if (numero.empty()) {
        return false;
    }
    for (size_t i = 0; i < numero.size(); i++) {
        int valor = valorDoDigito(numero[i]);
        if (valor == -1 || valor >= base) {
            return false;
        }
    }
    return true;
}

// area de conversoes

bool paraDecimal(string numero, int base, unsigned long long &resultado) {
    if (!baseValida(base) || !numeroValido(numero, base)) {
        return false;
    }

    resultado = 0;
    for (size_t i = 0; i < numero.size(); i++) {
        unsigned long long digito = valorDoDigito(numero[i]);

        // Antes de fazer resultado * base + digito, confere se vai caber
        if (resultado > (ULLONG_MAX - digito) / base) {
            return false;
        }
        resultado = resultado * base + digito;
    }
    return true;
}

string deDecimal(unsigned long long valor, int base) {
    if (valor == 0) {
        return "0";
    }

    string resultado = "";
    while (valor > 0) {
        int resto = valor % base;
        resultado = digitoDoValor(resto) + resultado;  
        valor = valor / base;
    }
    return resultado;
}

bool converter(string numero, int origem, int destino, string &saida) {
    unsigned long long decimal = 0;
    if (!paraDecimal(numero, origem, decimal)) {
        return false;
    }
    saida = deDecimal(decimal, destino);
    return true;
}

// inputs do user e leitura

string tirarEspacos(string texto) {
    size_t inicio = texto.find_first_not_of(" \t\r\n");
    if (inicio == string::npos) {
        return "";
    }
    size_t fim = texto.find_last_not_of(" \t\r\n");
    return texto.substr(inicio, fim - inicio + 1);
}

bool lerLinha(string mensagem, string &linha) {
    cout << mensagem;
    if (!getline(cin, linha)) {
        return false;
    }
    linha = tirarEspacos(linha);
    return true;
}

bool soTemDigitos(string texto) {
    if (texto.empty()) {
        return false;
    }
    for (size_t i = 0; i < texto.size(); i++) {
        if (texto[i] < '0' || texto[i] > '9') {
            return false;
        }
    }
    return true;
}

string listaDeBases() {
    string texto = "";
    for (size_t i = 0; i < BASES.size(); i++) {
        if (i > 0 && i == BASES.size() - 1) {
            texto += " ou ";
        } else if (i > 0) {
            texto += ", ";
        }
        texto += to_string(BASES[i]);
    }
    return texto;
}

int lerBase(string mensagem) {
    string linha;
    while (true) {
        if (!lerLinha(mensagem, linha)) {
            return -1;
        }

        if (soTemDigitos(linha) && linha.size() <= 3) {
            int base = stoi(linha);
            if (baseValida(base)) {
                return base;
            }
        }
        cout << "  [ERRO] Opcao invalida! Digite apenas " << listaDeBases() << "." << endl;
    }
}

void mostrarMenu() {
    cout << endl;
    cout << "Bases disponiveis:" << endl;
    for (size_t i = 0; i < BASES.size(); i++) {
        cout << "  " << BASES[i] << " - " << NOMES_BASES[i] << endl;
    }
}

bool escolherBases(int &origem, int &destino) {
    string opcoes = "(" + listaDeBases() + "): ";

    while (true) {
        mostrarMenu();
        cout << endl;

        origem = lerBase("Base de ORIGEM  " + opcoes);
        if (origem == -1) return false;

        destino = lerBase("Base de DESTINO " + opcoes);
        if (destino == -1) return false;

        if (origem == destino) {
            cout << "  [AVISO] A base de origem e a de destino nao podem ser iguais. "
                 << "Escolha novamente." << endl;
        } else {
            return true;
        }
    }
}

bool lerNumero(int origem, unsigned long long &decimal) {
    string linha;
    while (true) {
        if (!lerLinha("Digite o numero em " + nomeDaBase(origem) + ": ", linha)) {
            return false;
        }

        if (!numeroValido(linha, origem)) {
            cout << "  [ERRO] Numero invalido para a base " << nomeDaBase(origem)
                 << ". Digitos permitidos: " << digitosPermitidos(origem)
                 << ". Tente novamente." << endl;
        } else if (!paraDecimal(linha, origem, decimal)) {
            cout << "  [ERRO] Numero muito grande! O maximo aceito e "
                 << ULLONG_MAX << " (em decimal). Tente novamente." << endl;
        } else {
            return true;
        }
    }
}

bool querContinuar() {
    vector<string> respostasSim = {"s", "S", "sim", "SIM", "Sim"};
    vector<string> respostasNao = {"n", "N", "nao", "NAO", "Nao"};

    string resposta;
    while (true) {
        if (!lerLinha("\nDeseja fazer outra conversao? (s/n): ", resposta)) {
            return false;
        }
        for (size_t i = 0; i < respostasSim.size(); i++) {
            if (resposta == respostasSim[i]) return true;
        }
        for (size_t i = 0; i < respostasNao.size(); i++) {
            if (resposta == respostasNao[i]) return false;
        }
        cout << "  [ERRO] Responda com 's' ou 'n'." << endl;
    }
}

void main() {
    cout << " ! CONVERSOR DE BASES NUMERICAS ! " << endl;

    bool continuar = true;
    while (continuar) {
        int origem = 0;
        int destino = 0;
        unsigned long long decimal = 0;

        if (!escolherBases(origem, destino)) break;
        if (!lerNumero(origem, decimal)) break;

        cout << "------------------------------------------" << endl;
        cout << "Valor em decimal : " << decimal << endl;
        cout << "Resultado (" << nomeDaBase(destino) << ", base " << destino << "): "
             << deDecimal(decimal, destino) << endl;
        cout << "------------------------------------------" << endl;

        continuar = querContinuar();
    }

    cout << "\nEncerrando o programa. Ate logo!" << endl;
}

// Casos de teste do QA Leonardo top (rodar com: ./conversor_bases_simples --teste ou ./conversor_bases_simples.exe --teste)
int totalTestes = 0;
int testesFalharam = 0;

void verificar(string descricao, bool condicao) {
    totalTestes++;
    if (condicao) {
        cout << "[ok]     " << descricao << endl;
    } else {
        testesFalharam++;
        cout << "[FALHOU] " << descricao << endl;
    }
}

// Caso de teste de validacao: o numero 'numero' na base 'base' deve ser valido? (esperado = true)
struct CasoValidacao {
    string numero;
    int base;
    bool esperado;
};

// Caso de teste de conversao: 'numero' da base 'origem' para a base 'destino' deve dar 'esperado'
struct CasoConversao {
    string numero;
    int origem;
    int destino;
    string esperado;
};

const string INVALIDO = "<invalido>";

// Converte e devolve o resultado, ou "<invalido>" se a conversao falhar
string conv(string numero, int origem, int destino) {
    string saida;
    if (converter(numero, origem, destino, saida)) {
        return saida;
    }
    return INVALIDO;
}

int rodarTestes() {
    // CT01 - bases validas (todas as do vetor) e invalidas
    for (size_t i = 0; i < BASES.size(); i++) {
        verificar("base " + to_string(BASES[i]) + " e valida", baseValida(BASES[i]));
    }
    vector<int> basesInvalidas = {-2, 0, 1, 3, 7, 9, 11, 15, 32};
    for (size_t i = 0; i < basesInvalidas.size(); i++) {
        verificar("base " + to_string(basesInvalidas[i]) + " e invalida",
                  !baseValida(basesInvalidas[i]));
    }

    // CT02 e CT03 - validacao do numero (caracteres invalidos e validos, inclusive hex minusculo)
    vector<CasoValidacao> casosValidacao = {
        {"9", 2, false},          {"2", 2, false},    {"5", 5, false},
        {"9", 5, false},          {"8", 8, false},    {"A", 10, false},
        {"G", 16, false},         {"g", 16, false},   {"", 10, false},
        {"-1", 10, false},        {"1 0", 2, false},  {"1010", 2, true},
        {"04321", 5, true},       {"777", 8, true},   {"1234567890", 10, true},
        {"1a3f", 16, true},       {"1A3F", 16, true}, {"aBcDeF", 16, true}};

    for (size_t i = 0; i < casosValidacao.size(); i++) {
        CasoValidacao c = casosValidacao[i];
        string descricao = "'" + c.numero + "' em " + nomeDaBase(c.base) +
                           (c.esperado ? " e valido" : " e invalido");
        verificar(descricao, numeroValido(c.numero, c.base) == c.esperado);
    }

    // CT04 - zero em todas as combinacoes de bases diferentes
    for (size_t i = 0; i < BASES.size(); i++) {
        for (size_t j = 0; j < BASES.size(); j++) {
            if (i != j) {
                verificar("0 (" + NOMES_BASES[i] + ") = 0 (" + NOMES_BASES[j] + ")",
                          conv("0", BASES[i], BASES[j]) == "0");
            }
        }
    }

    // CT05 e CT06 - conversoes em geral (passando pelo decimal) e valores limite
    vector<CasoConversao> casosConversao = {
        {"1a3f", 16, 10, "6719"},
        {"1A3F", 16, 10, "6719"},
        {"1a3f", 16, 5, "203334"},
        {"ff", 16, 2, "11111111"},
        {"255", 10, 16, "FF"},
        {"0000", 2, 16, "0"},
        {"1010", 2, 10, "10"},
        {"10", 10, 2, "1010"},
        {"100", 10, 5, "400"},
        {"400", 5, 10, "100"},
        {"777", 8, 10, "511"},
        {"511", 10, 8, "777"},
        {"777", 8, 2, "111111111"},
        {"400", 5, 16, "64"},
        {"FFFFFFFFFFFFFFFF", 16, 10, "18446744073709551615"},
        {"10000000000000000", 16, 10, INVALIDO},
        {"18446744073709551616", 10, 16, INVALIDO},
        {"9", 2, 10, INVALIDO}};

    for (size_t i = 0; i < casosConversao.size(); i++) {
        CasoConversao c = casosConversao[i];
        string descricao = "'" + c.numero + "' (" + nomeDaBase(c.origem) + ") -> ";
        if (c.esperado == INVALIDO) {
            descricao += "rejeitado (" + nomeDaBase(c.destino) + ")";
        } else {
            descricao += "'" + c.esperado + "' (" + nomeDaBase(c.destino) + ")";
        }
        verificar(descricao, conv(c.numero, c.origem, c.destino) == c.esperado);
    }

    cout << endl << "Resultado: " << (totalTestes - testesFalharam) << "/" << totalTestes
         << " testes passaram." << endl;

    if (testesFalharam == 0) {
        return 0;
    }
    return 1;
}

int main(int argc, char *argv[]) {
    if (argc > 1 && string(argv[1]) == "--teste") {
        return rodarTestes();
    }
    programaPrincipal();
    return 0;
}
