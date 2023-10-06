#include <iostream>
#include <vector>
#include <string>

const std::string digito = "digito";
const std::string letra = "letra";
const std::string simbolo = "simbolo";
const std::string eop = "eop"; // end of page

struct Simbolo {
    char simbolo;
    std::string tipo = "";
};

struct Estado {
    int id;
    bool esFinal;
    std::string valor = "";
};

struct Transicion {
    int estadoInicial;
    Simbolo simbolo;
    int estadoDestino;
};

std::string Reconocer(char letra) {
    char letraMinuscula = tolower(letra);
    char vocabulario[] = "abcdefghijklmnopqrstuvwxyz";
    char digito[] = "0123456789";
    char simbolo[] = "*=+_.,;/&%$#\"\'";
    int i;
    for (i = 0; vocabulario[i]; i++)
    {
        if (letraMinuscula == vocabulario[i])
        {
            return "letra";
        }
    }

    for (i = 0; digito[i]; i++)
    {
        if (letraMinuscula == digito[i])
        {
            return "digito";
        }
    }

    for (i = 0; simbolo[i]; i++)
    {
        if (letraMinuscula == simbolo[i])
        {
            return "simbolo";
        }
    }
    return "";
}

//F: Estados finales
//Q: Conjunto de estados
//Sigma: vocabulario
//Delta: funcion de transicion
//x: entrada
//q0: estado inicial
void ReconoceAFD(const std::string& x, const std::vector<Estado>& Q, const std::vector<char>& Sigma,
                 const std::vector<Transicion>& Delta, int q0, const std::vector<int>& F) 
                 {
    int q = q0; 
    int i = 0; 
    int y = 0;
    bool encontro = false;
    //bool first_flag = false;
    Simbolo simbolos;
    while (i < x.length()) 
    {
        char xi = x[i];
        bool transicionEncontrada = false;
        encontro = false;  
        for (const Transicion& t : Delta) 
        {   
            transicionEncontrada = false;
            if(t.simbolo.simbolo == '-'){
                simbolos = { '-', Reconocer(xi) };
            } else {
                simbolos = { xi, Reconocer(xi) };
            }

            if (t.estadoInicial == q && simbolos.simbolo == t.simbolo.simbolo && simbolos.tipo == t.simbolo.tipo)
            {
                q = t.estadoDestino; 
                for (Estado estadoFinal : Q) 
                { 
                    if (q == estadoFinal.id && estadoFinal.esFinal) 
                    {
                        q = 0;
                        std::cout<<estadoFinal.valor<<std::endl;
                        transicionEncontrada = true;
                        encontro = true;
                        break;
                    }
                }
            }     

            if(transicionEncontrada)
            {
                break;
            }
        }
        if(!encontro)
        {
            i++;
        }
        
        //first_flag = true;
        //i++;
    }
    simbolos = { '-', eop }; //end of page
    for (const Transicion& t : Delta) 
    {   
        if (t.estadoInicial == q && simbolos.simbolo == t.simbolo.simbolo && simbolos.tipo == t.simbolo.tipo)
        {
            q = t.estadoDestino; 
            for (Estado estadoFinal : Q) 
            { 
                if (q == estadoFinal.id && estadoFinal.esFinal) 
                {
                    i+=2;
                    y++;
                    q = 0;
                    std::cout<<estadoFinal.valor<<std::endl;
                    encontro = true;
                    break;
                }
            }
        }     
    }

    if(!encontro)
    {
        std::cout<<"Error"<<std::endl;
    }
   // {8,{"-", simbolo},8},{8,{"-", digito},8}, {8,{'\"',simbolo},9} , {9,{"-", letra},10}, {9,{"-", simbolo},10}, {9,{"-", digito},10}

}

int main() 
{
    std::vector<Estado> Q = {{0, false}, {1, false}, {2, false},{3, true,"Identificador"}, 
    {4, true, "Entero"}, {5, false},{6, false}, {7, true, "Real" }, {8, false},{9, false}, {10, true,"Cadena"}, 
    {11, false},{12, false}, {13, false}, {14, true, "Operador_multiplicacion"}, {15,true,"Operador_asociacion"}};
    std::vector<char> Sigma = {'0', '1'}; //vocabulario
    std::vector<Transicion> Delta = 
    {
    {0, {'-',letra}, 1}, {1, {'-',letra}, 1}, {1, {'-',digito}, 1},{1, {'-',simbolo}, 3}, {1, {'-',eop}, 3},// identificador
    {0, {'-',digito}, 2}, {2, {'-',digito}, 2},  {2, {'-',letra}, 4}, {2, {'.',simbolo}, 5}, {2, {'-',eop}, 4}, //entero
                          {2, {'-',simbolo}, 4},  {5, {'-',digito}, 6}, {6, {'-',digito}, 6} ,{6, {'-',simbolo}, 7}, {6, {'-',eop}, 7}, {6, {'-',letra}, 7}, // real
    {0,{'*', simbolo},12}, {12,{'-',letra},14}, {12,{'-',digito},14},  {12,{'-',eop},14}, // OPERADOR_MULTIPLICACION
    {0,{'=', simbolo},11}, {11,{'=', simbolo},13}, {13,{'-', digito},15}, {13,{'-', letra},15},  {11,{'-', eop},13}, //{13,{'-', simbolo},15},// OPERADOR_RELACIONAL
    {0, {'\"',simbolo}, 8}, {8,{'-', simbolo},8},{8,{'-', digito},8},  {8,{'-', letra},8}, {8,{'\"',simbolo},9} , {9,{'-', letra},10}, {9,{'-', simbolo},10}, {9,{'-', digito},10}, {9,{'-', eop},10} //CADENA
    }; 
    int q0 = 0; // estado_Inicial
    std::vector<int> F = {3,4,7,14,15,10}; // estados finales
    std::string cadena = "Adb*fgg==34afs";

    ReconoceAFD(cadena, Q, Sigma, Delta, q0, F);
    return 0;
}