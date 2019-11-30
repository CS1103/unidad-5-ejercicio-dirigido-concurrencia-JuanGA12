//
// Created by Juan Galvez on 2019-11-29.
//

#ifndef PD4_MATRIZ_H
#define PD4_MATRIZ_H

#include <iostream>
#include <sys/time.h>
#include <vector>
#define NUM_HILOS 2

using namespace std;

struct datos {
    long thread_id;
    int num_filas1;
    int num_filas2;
    int num_columnas1;
    int num_columnas2;
};

struct datos datos_pasar[NUM_HILOS];

template<typename Data_Type>
vector<vector<Data_Type>> matriz1;
template<typename Data_Type>
vector<vector<Data_Type>> matriz2;
template<typename Data_Type>
vector<vector<Data_Type>> matriz3;

template<typename Data_Type>
void* multiplicar_paralelo(void* parametros) {

    int filas1, columnas1,columnas2;
    long lim_inf, lim_sup;
    long thread;
    struct datos* mis_datos;
    mis_datos=(struct datos*)parametros;
    thread = mis_datos->thread_id;
    filas1 = mis_datos->num_filas1;
    columnas1 = mis_datos->num_columnas1;
    columnas2 = mis_datos->num_columnas2;

    if(filas1%NUM_HILOS != 0 && thread==NUM_HILOS-1 ){
        lim_inf = thread * (filas1 / NUM_HILOS);
        lim_sup = (thread + 1) * (filas1 / NUM_HILOS) +1;
    }
    else{
        lim_inf = thread * (filas1 / NUM_HILOS);
        lim_sup = (thread + 1) * (filas1 / NUM_HILOS);
    }

    Data_Type sum=0;

    cout<<"Hilo "<<thread<<", LIM inf "<<lim_inf<<" LIM sup "<<lim_sup<<endl;
    for (long i = lim_inf; i < lim_sup; i++) {
        for (int j = 0; j < columnas2; j++) {
            for (int z = 0; z < columnas1; z++) {
                sum +=matriz1<Data_Type>[i][z]*matriz2<Data_Type>[z][j];
            }
            matriz3<Data_Type>[i][j]=sum;
            sum=0;
        }
    }

    pthread_exit(nullptr);
}



template<typename Data_Type>
class Matriz {
public:
    int Filas;
    int Columnas;
    vector<vector<Data_Type>> matriz;

    Matriz() : Filas{0}, Columnas{0} {}

    Matriz(int Filas_, int Columnas_) : Filas{Filas_}, Columnas{Columnas_} {

        for (int i = 0; i < Filas; i++) {
            vector<Data_Type> a;
            for (int j = 0; j < Columnas; j++) {
                a.push_back(0);
            }
            matriz.push_back(a);//llenado la matriz con 0´s
        }
    }

    virtual ~Matriz() = default;

    void set_filas(int Filas_) { Filas = Filas_; }

    void set_columnas(int columas_) { Columnas = columas_; }

    void Llenar_Matriz(const vector<Data_Type> &c);

    friend ostream &operator<<(ostream &o, const Matriz<Data_Type> &matrix) {
        for (int i = 0; i < matrix.Filas; i++) {
            for (int j = 0; j < matrix.Columnas; j++) {
                o << matrix.matriz.at(i).at(j) << " ";
            }
            o << endl;
        }
        return o;
    };

    Matriz<Data_Type> operator*(Matriz<Data_Type> matrix);
};

template<typename Data_Type>
void Matriz<Data_Type>::Llenar_Matriz(const vector<Data_Type> &c) {

    auto it = begin(c);
    auto it2 = end(c);

    for(;it<it2;){
        for(auto i=0;i<Filas;i++){
            vector<Data_Type> vec;
            for(int j=0;j<Columnas;j++){
                vec.push_back(*it);
                advance(it,1);
            }
            matriz.push_back(vec);
        }

    }
}

template<typename Data_Type>
Matriz<Data_Type> Matriz<Data_Type>::operator*(Matriz<Data_Type> matrix) {

    try {

        if (Columnas == matrix.Filas) {
            Matriz<Data_Type> Resultado = Matriz<Data_Type>(Filas, matrix.Columnas);
            for (int i = 0; i < Filas; i++) {
                vector<Data_Type> a;
                for (int j = 0; j < matrix.Columnas; j++) {
                    a.push_back(0);
                }
                matriz3<Data_Type>.push_back(a);
            }

            matriz1<Data_Type> = matriz;
            matriz2<Data_Type> = matrix.matriz;

            auto rc = 0;
            long thread;
            void *status;

            struct timeval start;
            struct timeval finish;
            long compiletime;
            double Time;
            pthread_t hilos[NUM_HILOS];
            pthread_attr_t attr;
            pthread_attr_init(&attr);
            pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_JOINABLE);

            gettimeofday(&start, nullptr);
            for (thread = 0; thread < NUM_HILOS; thread++) {
                datos_pasar[thread].thread_id = thread;
                datos_pasar[thread].num_filas1 = Filas;
                datos_pasar[thread].num_columnas1 = Columnas;
                datos_pasar[thread].num_filas2 = matrix.Filas;
                datos_pasar[thread].num_columnas2 = matrix.Columnas;
                cout << "Creando el hilo " << thread << endl;
                rc = pthread_create(&hilos[thread], &attr, multiplicar_paralelo<Data_Type>,
                                    (void *) &datos_pasar[thread]);
                if (rc) {
                    cout << "ERROR código " << rc << endl;
                    exit(-1);
                }
            }

            pthread_attr_destroy(&attr);

            for (thread = 0; thread < NUM_HILOS; thread++) {
                rc = pthread_join(hilos[thread], &status);
                if (rc) {
                    cout << "ERROR código " << rc << endl;
                    exit(-1);
                }
                cout << "En la funcion main, ya acabo el hilo " << thread << " con status " << (long) status << endl;
            }
            gettimeofday(&finish, nullptr);
            compiletime = (finish.tv_sec - start.tv_sec) * 1000000;
            compiletime = compiletime + (finish.tv_usec - start.tv_usec);
            Time = (double) compiletime;
            cout << endl;
            cout << "Despues de multiplicar con hilos:" << endl;
            cout << "El programa se demoró " << (double) Time / 1000000.0 << " segundos." << endl;
            Resultado.matriz = matriz3<Data_Type>;
            cout << "Resultado de la multiplicación: " << endl;
            cout << Resultado;
            cout << "Fin del programa" << endl;
            pthread_exit(nullptr);

        } else {
            throw runtime_error("No se puede hacer la multiplicación");
        }
    }
    catch (std::exception& e) {
            cout << e.what() << endl;
    }
}



#endif //PD4_MATRIZ_H
