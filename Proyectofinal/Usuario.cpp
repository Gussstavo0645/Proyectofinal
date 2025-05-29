#include "Usuario.h"
#include "utils.h"

Usuario::Usuario(std::string n, std::string clave) {
    nombre = n;
    claveEncriptada = Encode(clave);
    llavePersonal = Encode(clave + "_llave");
}

bool Usuario::validarAcceso(std::string clave) {
    return claveEncriptada == hashSHA256(clave);
}