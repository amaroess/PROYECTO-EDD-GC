#include <stdio.h>
#include "tdas/list.h" // TDA LISTA
#include "tdas/map.h" // TDA MAPA
#include "tdas/extra.h"

typedef struct Usuario // hecho por amaro, el 11/06/2024
{
    char* usuario; // nombre de usuario
    char* contrasena; // contraseña del usuario
} Usuario;
typedef struct Contrasena // hecho por amaro, el 11/06/2024
{
    char* contrasenaCifrada; // contraseña cifrada
    char* pagina; // página o servicio al que corresponde la contraseña.
} Contrasena;

void crearUsuario(List* listaUsuario, Map* mapa){
    char nombre[50];
    char clave[50];

    printf("Ingrese su nombre de usuario: ");
    scanf("%49s", nombre);

    MapPair* usuario_existente = map_search(mapa, nombre);
    if(usuario_existente != NULL){
        printf("El usuario %s ya está registrado\n");
        return;
    }

    printf("Ingrese su contraseña maestra: ");
    scanf("%49s", clave);

    Usuario* nuevoUsuario = malloc(sizeof(Usuario));
    if(nuevoUsuario == NULL) return;
    
    nuevoUsuario->usuario = strdup(nombre);
    nuevoUsuario->contrasena = strdup(clave);
    list_pushBack(listaUsuario, nuevoUsuario);
    List* listaPassword = list_create();
    map_insert(mapa, nuevoUsuario->usuario, listaPassword);
    printf("Usuario registrado correctamente\n");
    
}


void EliminarUsuario(List* listaUsuario) // hecho por amaro, el 11/06/2024
{
    char UsuarioE[21]; // variable para guardar el nombre del usuario a eliminar, 21 caracteres una cantidad aceptable.
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

void AgregarPassword(List* ListaPassword) // hecho por amaro, el 11/06/2024
{
    printf("Contraseña a agregar: ");
    char contrasena[21]; // variable para guardar la contraseña a agregar
    scanf("%20s", contrasena); // se lee la contraseña a agregar
    printf("\nPágina o servicio al que corresponde la contraseña: ");
    char pagina[21]; // variable para guardar la página o servicio al que corresponde la contraseña
    scanf("%20s", pagina); // se lee la página o servicio al que corresponde la contraseña
    Contrasena* nuevaContrasena = malloc(sizeof(Contrasena)); // se crea una nueva contraseña
    if (nuevaContrasena == NULL) return; // se verifica que se haya creado la nueva contraseña correctamente
    char* contrasenaFinal = encriptar(contrasena); // variable para guardar la contraseña cifrada.
    nuevaContrasena->contrasenaCifrada = contrasenaFinal; // se asigna la contraseña cifrada a la nueva contraseña, funcion encriptar deberia reservar la memoria.
    strcpy(nuevaContrasena->pagina, pagina); //  se asigna la página o servicio a la nueva contraseña
    list_pushBack(ListaPassword, nuevaContrasena); // se agrega la nueva contraseña a la lista de contraseñas
}
void CerrarSesion() // hecho por amaro, el 11/06/2024
{
    char opcion;
    printf("Estas seguro de cerrar sesion? (s/n):  "); // se pregunta al usuario si está seguro de cerrar sesión 's' para sí, 'n' para no.
    scanf("%c", &opcion); // se escanea opcion 
    if (opcion == 's' || opcion == 'S')
    {
        printf("Sesion cerrada exitosamente.\n");
        exit(0); // se cierra el programa
    }
    return; // se regresa al menú principal si el usuario no quiere cerrar sesión
}
void LeerArchivo(List* listaUsuario, Map* mapa){

    FILE *archivo = fopen("archivo.txt", "r");
    if(archivo == NULL){
        printf("No se encontró el archivo.");
        return;
    }

    char** campos;
    while((campos = leer_linea_csv(archivo, ",")) != NULL){
        if(campos[0] == NULL || campos[1] == NULL) continue; // Si la linea no tiene al usuario o la clave maestra, se salta

        char* usuario = campos[0];
        char* clave_maestra = campos[1];
        char* pagina_servicio = campos[2];
        char* clave_cifrada = campos[3];

        List* listaPassword = NULL;
        MapPair* parUsuario = map_search(mapa, usuario);
        if(parUsuario != NULL){
            listaPassword = parUsuario->value;
        }
        if(parUsuario == NULL){
            Usuario* nuevoUsuario = malloc(sizeof(usuario));
            if(nuevoUsuario == NULL) return;

            nuevoUsuario->usuario = strdup(usuario);
            nuevoUsuario->contrasena = strdup(clave_maestra);

            list_pushBack(listaUsuario, nuevoUsuario);
            listaPassword = list_create();
            map_insert(mapa, nuevoUsuario->usuario, listaPassword);
        }
        if(pagina_servicio != NULL && clave_cifrada != NULL){
            Contrasena *nuevaC = malloc(sizeof(Contrasena));
            if(nuevaC == NULL) return;

            nuevaC->pagina = strdup(pagina_servicio);
            nuevaC->contrasenaCifrada = strdup(clave_cifrada);

            list_pushBack(listaPassword, nuevaC);

        }

    }
    fclose(archivo);
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
