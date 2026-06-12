#include <stdio.h>
#include "tdas/list.h" // TDA LISTA
#include "tdas/map.h" // TDA MAPA
typedef struct Usuario // hecho por amaro, el 11/06/2024
{
    char usuario[20]; // nombre de usuario
    char contrasena[20]; // contraseña del usuario
} Usuario;
typedef struct Contrasena // hecho por amaro, el 11/06/2024
{
    char contrasenaCifrada[50]; // contraseña cifrada
    char pagina[20]; // página o servicio al que corresponde la contraseña.
} Contrasena;

void EliminarUsuario(List* listaUsuario) // hecho por amaro, el 11/06/2024
{
    char UsuarioE[20]; // variable para guardar el nombre del usuario a eliminar
    printf("Usuario a Eliminar: ");
    scanf("%20s", UsuarioE); // se lee el nombre del usuario a eliminar
    Usuario* usuarioAEliminar = list_first(listaUsuario); // se obtiene el primer usuario de la lista
    while (usuarioAEliminar != NULL) // se recorre la lista de usuarios hasta encontrar el usuario a eliminar
    {
        if (strcmp(usuarioAEliminar->usuario, UsuarioE) == 0) // se compara el nombre del usuario a eliminar con el nombre del usuario actual de la lista
        {
            list_popCurrent(listaUsuario); // se elimina el usuario de la lista
            printf("Usuario eliminado exitosamente.\n");
            return; // se termina la función después de eliminar el usuario
        }
        usuarioAEliminar = list_next(listaUsuario); // se avanza en la lista
    }
    printf("Usuario no encontrado.\n");
}

int main()
{
    puts("-----BIENVENIDO A TU GESTOR DE CONTRASEÑAS-----\n");
    return 0;
    // struct Usuario (usuario, contrasena), struct contrasena (contrasena cifrada, pagina) *---
    // void CrearUsuario(List* listaUsuario) *---
    // void EliminarUsuario(List* listaUsuario) *---
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
