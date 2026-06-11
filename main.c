#include <stdio.h>
#include "tdas/list.h" // TDA LISTA
#include "tdas/map.h" // TDA MAPA
#include "d"
int main()
{
    puts("-----BIENVENIDO A TU GESTOR DE CONTRASEÑAS-----\n");
    return 0;
    // struct Usuario (usuario, contrasena), struct contrasena (contrasena cifrada, pagina) *---
    // void CrearUsuario(List* listaUsuario) *---
    // void EliinarUsuario(List* listaUsuario) *---
    // void IniciarSesion(List* listaUsuario) *---
    // void CerrarSesion() *---
    // char encriptar(char str) *---
    // void LeerArchivo() *---
    // void AgregarPassword(List* ListaPassword) *---
    // void EliminarPassword(List* ListaPassword) *---
    // void RobustezPassword(char* password) *---
    // void GestionarPassword(List* ListaPassword) *---
}
/*
 seba: viernes 12: (iniciar sesion), (encriptar), (robustez), (gestionar password).
 camila: sabado 13: (leer archivo), (crear usuario), (eliminar password).
 amaro: sabado 13: (struct Usuario), (struct contrasena), (eliminar usuario), (agregar password), (cerrar sesion).
 
 FLUJO:
 
 ListaUsuario -----> usuario            Mapa -----> usuario
              |                              |
              |                              |
               -----> contrasena              ----> ListaPassword -----> servicio
                                                                   |
                                                                   |
                                                                    ----> contrasena
 */
