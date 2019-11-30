//
// Created by Juan Galvez on 2019-11-29.
//

#ifndef PD4_LECTURA_H
#define PD4_LECTURA_H

#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <iterator>
#include "Matriz.h"

template <typename Data_Type>
void leer(const string &archivo, Matriz<Data_Type> & Matriz1, Matriz<Data_Type> & Matriz2) {
    
    int filas1=0, columnas1=0;
    int filas2=0, columnas2=0;
    Data_Type a=0;
    int contador = 0;

    vector<Data_Type> Valores_Matriz1;
    vector<Data_Type> Valores_Matriz2;

    ifstream inFile(archivo);
    try {
        if (inFile.is_open()) {

            string line;

            //Obtiene la primera linea del archivo y lo guarda en line.
            while (getline(inFile, line)) {

                //guarda en iss los datos.
                istringstream iss(line);

                vector<string> parts((istream_iterator<string>(iss)), istream_iterator<string>());

                if (contador == 0) {
                    filas1 = stoi(parts[0]);
                    columnas1 = stoi(parts[1]);
                    Matriz1.set_filas(filas1);
                    Matriz1.set_columnas(columnas1);

                }

                if (0 < contador && contador <= filas1) {
                    for (auto &part : parts) {
                        a = stod(part);
                        Valores_Matriz1.push_back(a);

                    }
                }

                if (contador == (filas1 + 1)) {
                    filas2 = stoi(parts[0]);
                    columnas2 = stoi(parts[1]);
                    Matriz2.set_filas(filas2);
                    Matriz2.set_columnas(columnas2);
                }

                if ((contador > (filas1 + 1))) {
                    for (auto &part : parts) {
                        a = stod(part);
                        Valores_Matriz2.push_back(a);

                    }

                }

                contador++;
            }
            inFile.close();
            Matriz1.Llenar_Matriz(Valores_Matriz1);
            Matriz2.Llenar_Matriz(Valores_Matriz2);


        } else {
            throw logic_error("No se pudo leer archivo.txt");

        }
    }
    catch (std::exception& e){
        cout<<e.what()<<endl;
    }
}
#endif //PD4_LECTURA_H
