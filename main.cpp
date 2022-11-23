#include <iostream>
#include <bits/stdc++.h>

using namespace std;

#define separador cout<< "\n------------------------------------------------\n\n\r";

int main()
{
    srand(time(NULL));
    double t, Q, a, B, pev, p, pt = 0.0, r, H_ij;
    int iteras = 1, i = 0, hormigas = 1;
    int ar = 11;
    int  nodoActual, key, arista, costoT, nodoAlimento = 3;
    vector<pair<int, double>> probabilidad;
    vector<vector<pair<int, int>>> recorrido(hormigas); //Nodo, arista tomada
    map<int, int> costos; //Recorrido_i, costo Total


    //Solicitud de parametros iniciales
    cout<< char(175) << "Ingrese los siguientes valores: " << endl;
    cout<< "\tCantidad de feromona inicial: ";
    cin>> t;
    cout<< "\tConstante Q: ";
    cin>> Q;
    cout<< "\tImportancia de la feromona: ";
    cin>> a;
    cout<< "\tImportancia del factor de visibilidad: ";
    cin>> B;
    cout<< "\tIngrese el coeficiente de evaporacion ";
    cin>> pev;

    separador;

    //Construccion de matriz de datos por medio de aristas
    double datos[][3] =
    {
        {5, 1/5.0, t}, //A0
        {3, 1/3.0, t}, //A1
        {6, 1/6.0, t}, //A2
        {5, 1/5.0, t}, //A3
        {2, 1/2.0, t}, //A4
        {3, 1/3.0, t}, //A5
        {5, 1/5.0, t}, //A6
        {3, 1/3.0, t}, //A7
        {6, 1/6.0, t}, //A8
        {4, 1/4.0, t}, //A9
        {5, 1/5.0, t} //A10
    };
    /*
    for(int j  = 0; j < 3; j++)
    {
        for(int l = 0; l < 11; l++)
            cout<<datos[j][l] << " ";
        cout<< endl;
    }*/
    //Mapa de nodos y aristas
    vector<map<int,int>> grafo =   //{nodo, arista}
    {
        { {1,0}, {2,1}, {5,2} }, //1 //n0
        { {0,0}, {2,4}, {6,3} }, //2 //n1
        { {0,1}, {1,4}, {4,8}, {5,5}, {6,7} }, //3 //n2
        { {4,10}, {6,9} }, //4 //n3
        { {2,8}, {3,10}, {5,6} }, //5 //n4
        { {0,2}, {2,5}, {4,6} }, //6 //n5
        { {1,3}, {2,7}, {3,9} } //7 //n6
    };

    vector<bool> nodosVisitados;

    while(i < iteras)
    {
        //Hacer camino para cada hormiga
        int h_i = 0;


        while(h_i < hormigas)
        {
            nodoActual = 0;
            costoT = 0;
            recorrido[h_i].clear(); //Limpiamos por si la hormiga h_i tuvo que volver a empezar
            nodosVisitados.assign(7,false);
            cout<< char(175) << "Recorrido de la hormiga " << h_i + 1 << ", empezando en el nodo: " << nodoActual << endl;

            //Calculando probabilidad
            while((nodoActual != nodoAlimento) && (!nodosVisitados[nodoActual]))
            {

                probabilidad.clear(); //Calcula la probabilidad <nodo, probabilidad>
                pt = 0.0;
                nodosVisitados[nodoActual] = true;
                //recorrido[h_i].push_back(nodoActual);

                //Primero calcula la probabilidad individual
                //Calculo de las probabilidades para obetener probabilidad total
                for(auto it : grafo[nodoActual])
                {
                    if(nodosVisitados[it.first] == false)
                    {
                        key = it.first;
                        arista = it.second;
                        p = (datos[arista][1] * datos[arista][2]);
                        pt += p;
                        probabilidad.push_back({key,p});
                    }
                }

                //Calculo de las probabilidades acumuladas
                (probabilidad.begin())->second = (probabilidad.begin())->second / pt;
                for(int it = 1; it < probabilidad.size(); it++)
                {
                    if(it != (probabilidad.size() - 1))
                        probabilidad[it].second = (probabilidad[it].second / pt) + probabilidad[it-1].second;
                    else
                        probabilidad[it].second = 1.0;
                }

                /* Impresion de probabilidades para cada camino posible
                for(auto it : probabilidad)
                {
                    cout<< it.second << " ";
                }
                cout<< endl; */

                //Elegir el nuevo nodo
                r = (rand() % 100) / 100.0;
                for(auto it : probabilidad)
                {
                    if(it.second > r)
                    {
                        recorrido[h_i].push_back({nodoActual,grafo[nodoActual][it.first]});
                        nodoActual = it.first;
                        cout<< "\t>La hormiga " << h_i + 1 << " camino al nodo:" << nodoActual + 1 << endl;
                        break;
                    }
                }
            }
            recorrido[h_i].push_back({nodoActual, -1});

            /* Impresion de los nodos y la arista tomada como lista
            for (auto itr = recorrido[h_i].begin(); itr != recorrido[h_i].end(); ++itr)
            {
                cout << '\t' << itr->first << '\t' << itr->second
                     << '\n';
            }
            cout << endl;*/

            //Se acepto solucion
            if(nodoActual == 3)
            {
                //Imprimir recorrido y calculo del costo del recorrido
                cout<< "\n" << char(175) <<"Recorrido de la hormiga " << h_i + 1 << ": ";
                for(int j = 0; j < recorrido[h_i].size(); j++)
                {
                    if( recorrido[h_i][j].second != -1 )
                    {
                        arista = recorrido[h_i][j].second;
                        costoT += datos[arista][0];
                    }
                    cout<< recorrido[h_i][j].first + 1 << " ";
                }
                costos[h_i] = costoT;
                cout<< "\n\tCosto del recorrido: " <<costos[h_i] << endl;

                h_i++;
            }
            else
            {
                cout<< "\n>>>>>>>>>>>>>>>>>>>>Choque con nodo ya visitado, empezando de nuevo con la hormiga " << h_i << ".<<<<<<<<<<<<<<<<<<<<\n" <<endl;
            }


        }

        //Aqui se actualiza la feromona

        //P1 hacer la multiplicacion
        for(int j = 0; j < ar; j++)
        {
            datos[j][2] = (1-pev)*datos[j][2];
        }
        //P2 hacer las sumas
        for(int j = 0; j < recorrido.size(); j++) //Recorrido_j
        {
            H_ij = Q / costos[j];
            for(int l = 0; l < recorrido[j].size(); l++) //Arista por la que paso j
            {
                arista = recorrido[j][l].second;
                if(arista != -1)
                {
                    datos[arista][2] += H_ij;
                }
            }
        }

        //Imprimir la tabla de datos
        separador
        cout<< char(175) << "Actualizacion de la tabla de datos de los caminos\n" << endl;

        //Linea superior
        cout<< char(201);
        for(int j = 0; j < 63; j++) cout<< char(205);
        cout<< char(187) << endl;

        //Cabezera
        cout<< char(204) << "\tArista\tDistancia\tVisibilidad\tFeromona\t" << char(185) << endl;

        for(int j = 0; j < ar; j++)
        {
            cout<< char(204) << "\t" << j << "\t";
            for(int l = 0; l < 3; l++)
            {
                cout<< fixed <<setprecision(5) <<datos[j][l] << "\t\t";
            }
            cout<< char(185) << endl;
        }

        //Liena inferior
        cout<< char(200);
        for(int j = 0; j < 63; j++) cout<< char(205);
        cout<< char(188) << endl;

        cout<< endl << char(175) << "Fin de la iteracion: " << i + 1 << endl;
        separador;

        i++;
    }

    return 0;
}
