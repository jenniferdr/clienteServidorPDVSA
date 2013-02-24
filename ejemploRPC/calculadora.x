struct Operandos {
    int operando1;
    int operando2;
};

program CALCULADORAPROG {
    version CALCULADORAVERS {
        int SUMAR(Operandos) = 1;

        int RESTAR(Operandos) = 2;

        int MULTIPLICAR(Operandos) = 3;

        double DIVIDIR(Operandos) = 4;

        string SON_IGUALES(Operandos) = 5;
    } = 1;
} = 0x20000001;
