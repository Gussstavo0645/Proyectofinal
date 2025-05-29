#pragma once
#ifndef USUARIO_H
#define USUARIO_H

#include <string>

class Usuario {
public:
    std::string nombre;
    std::string claveEncriptada;
    std::string llavePersonal;

    Usuario(std::string nombre, std::string clave);
    bool validarAcceso(std::string clave);
};

#endif