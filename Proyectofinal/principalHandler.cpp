#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>

using namespace std;

struct Nodo {
    string espanol, italiano, frances, aleman, ingles;
    Nodo* izquierda;
    Nodo* derecha;

    Nodo(string esp, string ita, string fra, string ale, string ing)
        : espanol(esp), italiano(ita), frances(fra), aleman(ale), ingles(ing),
        izquierda(nullptr), derecha(nullptr) {}
};

class ArbolTraducciones {
private:
    Nodo* raiz;

    Nodo* insertar(Nodo* nodo, string esp, string ita, string fra, string ale, string ing) {
        if (nodo == nullptr) return new Nodo(esp, ita, fra, ale, ing);
        if (esp < nodo->espanol) nodo->izquierda = insertar(nodo->izquierda, esp, ita, fra, ale, ing);
        else nodo->derecha = insertar(nodo->derecha, esp, ita, fra, ale, ing);
        return nodo;
    }

    void inOrden(Nodo* nodo) {
        if (nodo != nullptr) {
            inOrden(nodo->izquierda);
            cout << nodo->espanol << " | " << nodo->italiano << " | " << nodo->frances
                << " | " << nodo->aleman << " | " << nodo->ingles << endl;
            inOrden(nodo->derecha);
        }
    }

    Nodo* buscar(Nodo* nodo, const string& palabra) {
        cout << "llega aqui";
        if (nodo == nullptr || nodo->espanol == palabra) return nodo;
        if (palabra < nodo->espanol) return buscar(nodo->izquierda, palabra);
        return buscar(nodo->derecha, palabra);
    }

public:
    ArbolTraducciones() : raiz(nullptr) {}

    void agregarPalabra(string esp, string ita, string fra, string ale, string ing) {
        raiz = insertar(raiz, esp, ita, fra, ale, ing);
    }

    void mostrar() {
        inOrden(raiz);
    }

    void buscarTraduccion(const string& palabra, int idioma) {
        Nodo* resultado = buscar(raiz, palabra);
        if (resultado) {
            string traduccion;
            switch (idioma) {
            case 1: traduccion = resultado->italiano; break;
            case 2: traduccion = resultado->frances; break;
            case 3: traduccion = resultado->aleman; break;
            case 4: traduccion = resultado->ingles; break;
            default: traduccion = "Idioma no valido"; break;
            }
            cout << "Traduccion: " << traduccion << endl;
        }
        else {
            cout << "Palabra no encontrada.\n";
            return;
        }
    }
};

void leerArchivo(const string& nombreArchivo, ArbolTraducciones& arbol) {
    ifstream archivo(nombreArchivo);
    if (!archivo.is_open()) {
        cerr << "Error al abrir el archivo: " << nombreArchivo << endl;
        return;
    }

    cout << "Archivo abierto correctamente: " << nombreArchivo << endl;

    string linea;
    while (getline(archivo, linea)) {
        cout << "Leyendo línea: " << linea << endl;
        stringstream ss(linea);
        vector<string> palabras;
        string palabra;

        while (getline(ss, palabra, ',')) palabras.push_back(palabra);
        if (palabras.size() == 5) {
            arbol.agregarPalabra(palabras[0], palabras[1], palabras[2], palabras[3], palabras[4]);
        }
        else {
            cout << "Línea con formato incorrecto: " << linea << endl;
        }
    }
    archivo.close();
}


int main() {
    ArbolTraducciones arbol;

    leerArchivo("Traducciones.txt", arbol);

    int opcion;
    do {
        cout << "\n--- MENU ---\n";
        cout << "1. Mostrar palabras\n";
        cout << "2. Buscar traduccion\n";
        cout << "3. Salir\n";
        cout << "Seleccione una opcion: ";
        cin >> opcion;

        switch (opcion) {
        case 1:
            arbol.mostrar();
            break;
        case 2: {
            string palabra;
            int idioma;
            cout << "Ingrese la palabra en espanol: ";
            cin >> palabra;
            cout << "Seleccione idioma (1: Italiano, 2: Frances, 3: Aleman, 4: Ingles): ";
            cin >> idioma;
            arbol.buscarTraduccion(palabra, idioma);
            break;
        }
        case 3:
            cout << "Saliendo.\n";
            break;
        default:
            cout << "Opcion no valida.\n";
        }
    } while (opcion != 3);

    return 0;
}

