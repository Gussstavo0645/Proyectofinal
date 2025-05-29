#pragma once
#ifndef UTILS_H
#define UTILS_H

#include <string>

std::string hashSHA256(const std::string& input);
std::string Encode(const std::string& input);
std::string Decode(const std::string& input);
std::string codificarTexto(const std::string& texto);
std::string generarFirma(const std::string& contenido);
bool verificarIntegridad(const std::string& contenido, const std::string& firma);

#endif