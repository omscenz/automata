#include <iostream>
#include <string>
#include <vector>
#include <regex>

using namespace std;

// REGEX PRINCIPALES

// Identificador valido
const regex reg_id(R"(^[a-zA-Z_][a-zA-Z0-9_]*$)");

// Numero entero
const regex reg_ent(R"(^-?\d+$)");

// Numero decimal
const regex reg_dec(R"(^-?\d+\.\d+$)");

// Cadena entre comillas
const regex reg_cad(R"(^"[^"]*"$)");

// Cadena sin cerrar
const regex reg_cad_sin_cerrar(R"(^"[^"]*$)");

// Booleano
const regex reg_bool(R"(^(true|false)$)");

// Caracter invalido
const regex reg_char_inv(R"([^a-zA-Z0-9_.\-"])");

// Empieza con mayuscula
const regex reg_may(R"(^[A-Z])");

// ESTRUCTURA PARA VALIDAR TOKENS

struct ResultadoToken {
    bool valido;
    string tipo;
    string razon;
};

// ESTRUCTURA PARA EL ANALISIS COMPLETO

struct ResultadoAnalisis {
    bool valido;

    string identificador;
    string operador;
    string valor;
    string tipoValor;
    string terminador;

    vector<string> errores;
};

// ELIMINA ESPACIOS

string trim(const string& s) {
    size_t inicio = s.find_first_not_of(" \t\r\n");

    if (inicio == string::npos) {
        return "";
    }

    size_t fin = s.find_last_not_of(" \t\r\n");

    return s.substr(inicio, fin - inicio + 1);
}

// VERIFICA SI TERMINA EN UN CARACTER

bool terminaCon(const string& s, char c) {
    return !s.empty() && s.back() == c;
}

// VALIDA IDENTIFICADORES

ResultadoToken validarIdentificador(const string& token) {
    ResultadoToken res;
    res.valido = false;

    if (token.empty()) {
        res.razon = "El identificador esta vacio";
        return res;
    }

    if (isdigit((unsigned char)token[0])) {
        res.razon = "Los identificadores no pueden comenzar con numeros";
        return res;
    }

    if (token.find('-') != string::npos) {
        res.razon = "El guion \"-\" no esta permitido en identificadores";
        return res;
    }

    if (!regex_match(token, reg_id)) {
        smatch m;
        regex rx_inv(R"([^a-zA-Z0-9_])");

        string charInv = "?";

        if (regex_search(token, m, rx_inv)) {
            charInv = m[0];
        }

        res.razon =
            "Caracter especial invalido \"" +
            charInv +
            "\" en el identificador";

        return res;
    }

    res.valido = true;
    return res;
}

// VALIDA VALORES

ResultadoToken validarValor(const string& token) {
    ResultadoToken res;
    res.valido = false;

    if (token.empty()) {
        res.razon = "El valor esta vacio o faltante";
        return res;
    }

    if (regex_match(token, reg_cad_sin_cerrar)) {
        res.razon =
            "Cadena de texto sin cerrar: falta la comilla doble de cierre (\")";
        return res;
    }

    if (regex_match(token, reg_cad)) {
        res.valido = true;
        res.tipo = "cadena";
        return res;
    }

    if (regex_match(token, reg_dec)) {
        res.valido = true;
        res.tipo = "decimal";
        return res;
    }

    if (regex_match(token, reg_ent)) {
        res.valido = true;
        res.tipo = "entero";
        return res;
    }

    if (regex_match(token, reg_bool)) {
        res.valido = true;
        res.tipo = "booleano";
        return res;
    }

    if (regex_match(token, reg_id)) {
        if (regex_search(token, reg_may)) {
            res.razon =
                "Parece un texto pero le faltan las comillas; usa \"" +
                token +
                "\"";
            return res;
        }

        res.valido = true;
        res.tipo = "identificador";
        return res;
    }

    smatch m;

    if (regex_search(token, m, reg_char_inv)) {
        res.razon =
            "El caracter \"" +
            m[0].str() +
            "\" no esta permitido en el valor";
    } else {
        res.razon =
            "Valor no reconocido: \"" +
            token +
            "\"";
    }

    return res;
}

// DIVIDE LA INSTRUCCION

void tokenizar(
    const string& instruccion,
    string& izquierda,
    string& operador,
    string& derecha
) {
    size_t posDoble = instruccion.find("==");

    if (posDoble != string::npos) {
        izquierda = trim(instruccion.substr(0, posDoble));
        operador = "==";
        derecha = trim(instruccion.substr(posDoble + 2));
        return;
    }

    size_t posIgual = instruccion.find('=');

    if (posIgual == string::npos) {
        izquierda = instruccion;
        operador = "";
        derecha = "";
        return;
    }

    izquierda = trim(instruccion.substr(0, posIgual));
    operador = "=";
    derecha = trim(instruccion.substr(posIgual + 1));
}

// ANALIZA LA INSTRUCCION COMPLETA

ResultadoAnalisis analizarInstruccion(const string& instruccion) {
    ResultadoAnalisis res;
    res.valido = false;

    string trimmed = trim(instruccion);

    if (trimmed.empty()) {
        res.errores.push_back("Instruccion vacia");
        return res;
    }

    string izquierda, operador, derecha;
    tokenizar(trimmed, izquierda, operador, derecha);

    if (operador.empty()) {
        res.errores.push_back("Falta el operador de asignacion \"=\"");
        return res;
    }

    if (operador == "==") {
        res.errores.push_back("Operador invalido \"==\"");
        return res;
    }

    res.operador = "=";

    if (izquierda.empty()) {
        res.errores.push_back("Falta el identificador");
    } else {
        ResultadoToken resId = validarIdentificador(izquierda);

        if (!resId.valido) {
            res.errores.push_back(
                "Identificador invalido \"" +
                izquierda +
                "\": " +
                resId.razon
            );
        } else {
            res.identificador = izquierda;
        }
    }

    if (!terminaCon(derecha, ';')) {
        if (derecha.empty()) {
            res.errores.push_back("Falta el valor y el terminador");
        } else {
            res.errores.push_back("Falta el terminador ;");
        }
    } else {
        res.terminador = ";";
    }

    string valorRaw = derecha;

    if (terminaCon(valorRaw, ';')) {
        valorRaw = trim(valorRaw.substr(0, valorRaw.size() - 1));
    }

    if (valorRaw.empty()) {
        res.errores.push_back("Falta el valor de la asignacion");
    } else {
        ResultadoToken resVal = validarValor(valorRaw);

        if (!resVal.valido) {
            res.errores.push_back(
                "Valor invalido \"" +
                valorRaw +
                "\": " +
                resVal.razon
            );
        } else {
            res.valor = valorRaw;
            res.tipoValor = resVal.tipo;
        }
    }

    res.valido = res.errores.empty();
    return res;
}

// MUESTRA RESULTADOS

bool generarReporte(const string& instruccion) {
    ResultadoAnalisis r = analizarInstruccion(instruccion);

    cout << "Entrada: " << instruccion << "\n";

    if (r.valido) {
        cout << "[VALIDO]\n";
        cout << "  - Identificador : " << r.identificador << "\n";
        cout << "  - Operador      : " << r.operador << "\n";
        cout << "  - Valor         : " << r.valor << " (" << r.tipoValor << ")\n";
        cout << "  - Terminador    : " << r.terminador << "\n";
    } else {
        cout << "[INVALIDO]\n";

        for (size_t i = 0; i < r.errores.size(); ++i) {
            cout << "  Error " << (i + 1) << ": " << r.errores[i] << "\n";
        }

        const string& e0 = r.errores[0];

        if (e0.find("comenzar con numeros") != string::npos) {
            cout << "  Sugerencia: Empieza con letra o _\n";
        } else if (
            e0.find("Falta") != string::npos ||
            e0.find("falta") != string::npos
        ) {
            cout << "  Sugerencia: Usa identificador = valor;\n";
        }
    }

    cout << "\n";
    return r.valido;
}

// MODO INTERACTIVO

void modoInteractivo() {
    cout << string(60, '=') << "\n";
    cout << "Analizador Lexico\n";
    cout << string(60, '=') << "\n";
    cout << "Escribe una instruccion.\n";
    cout << "Escribe \"salir\" para terminar.\n\n";

    string linea;

    while (true) {
        cout << ">>> ";

        if (!getline(cin, linea)) {
            break;
        }

        if (trim(linea) == "salir") {
            cout << "\nHasta luego.\n";
            break;
        }

        cout << "\n";
        generarReporte(linea);
    }
}

// FUNCION PRINCIPAL

int main(int argc, char* argv[]) {
    if (argc == 1) {
        modoInteractivo();

    } else if (
        argc == 3 &&
        string(argv[1]) == "--instruccion"
    ) {
        generarReporte(argv[2]);

    } else {
        cout << "Uso:\n";
        cout << "./analizador\n";
        cout << "./analizador --instruccion \"x = 25;\"\n";
    }

    return 0;
}

