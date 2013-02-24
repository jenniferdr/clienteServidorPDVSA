
#include "calculadora.h"

#include <iostream>

using namespace std;

int main(int argc, char* argv[]) {
    if (argc != 4) {
        cerr << "Uso: cliente_calculadora   " << endl;
        return 1;
    }
    char *server = argv[1];

    CLIENT *clnt = clnt_create(server, CALCULADORAPROG, CALCULADORAVERS, "tcp");
    if (clnt == NULL) {
        cerr << "Error al crear un cliente de rpc" << endl;
        clnt_pcreateerror(server);
        return 1;
    }

    Operandos op;
    op.operando1 = atoi(argv[2]);
    op.operando2 = atoi(argv[3]);

    int* resultadoEntero;
    double* resultadoFloat;
    char** comparacion;

    cout << "Procesando la suma de " << op.operando1 << " y " << op.operando2 << endl;
    resultadoEntero = sumar_1(&op, clnt);
    if (resultadoEntero != NULL) {
        cout << *resultadoEntero << endl;
    } else {
        cerr << "Error!" << endl;
        clnt_perror(clnt, server);
    }

    cout << "Procesando la resta de " << op.operando1 << " y " << op.operando2 << endl;
    resultadoEntero = restar_1(&op, clnt);
    if (resultadoEntero != NULL) {
        cout << *resultadoEntero << endl;
    } else {
        cerr << "Error!" << endl;
        clnt_perror(clnt, server);
    }

    cout << "Procesando la multiplicación de " << op.operando1 << " y " << op.operando2 << endl;
    resultadoEntero = multiplicar_1(&op, clnt);
    if (resultadoEntero != NULL) {
        cout << *resultadoEntero << endl;
    } else {
        cerr << "Error!" << endl;
        clnt_perror(clnt, server);
    }

    cout << "Procesando la división de " << op.operando1 << " y " << op.operando2 << endl;
    resultadoFloat = dividir_1(&op, clnt);
    if (resultadoFloat != NULL) {
        cout << *resultadoFloat << endl;
    } else {
        cerr << "Error!" << endl;
        clnt_perror(clnt, server);
    }

    cout << "Procesando la comparación de " << op.operando1 << " y " << op.operando2 << endl;
    comparacion = son_iguales_1(&op, clnt);
    if (comparacion != NULL) {
        cout << *comparacion << endl;
    } else {
        cerr << "Error!" << endl;
        clnt_perror(clnt, server);
    }

    cout << endl << "Visita http://sigloxxi.fcie.uam.es!" << endl;

    return 0;
}
