#include <stdio.h>
#include <string.h>
#include <time.h>
#include "tdas/list.h" // TDA LISTA
#include "tdas/map.h" // TDA MAPA
#include "tdas/extra.h"
#include <ctype.h>
 
#define claveHex 0x6C
#define TIEMPO_BLOQUEO 60

typedef struct Usuario // hecho por amaro, el 11/06/2024
{
    char* usuario; // nombre de usuario
    char* contrasena; // contraseña del usuario
    time_t momento_bloqueo;
} Usuario;
typedef struct Contrasena // hecho por amaro, el 11/06/2024
{
    char* contrasenaCifrada; // contraseña cifrada
    char* pagina; // página o servicio al que corresponde la contraseña.
} Contrasena;

typedef struct{
    Map* hijos;
    char palabra;
    List* contraseñas;
} NodoTrie;

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
    nuevoUsuario->momento_bloqueo = 0;
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

char encriptar(char* contrasena){
    int i = 0;
    char* contrasenaCifrada = malloc(strlen(contrasena) + 1); // se reserva memoria para la contraseña cifrada, el tamaño es el mismo que la contraseña original más uno para el caracter nulo.
    while(contrasena[i] != '\0'){
        char caracterCif = contrasena[i] ^ claveHex; 
        sprintf(&contrasenaCifrada[i], "%02X", caracterCif);
        i++;
    }
    contrasenaCifrada[i*2] = '\0';
    return contrasenaCifrada;
}

char desencriptar(char* contrasenaCifrada){
    char* contrasena = malloc(strlen(contrasenaCifrada) / 2 + 1); 
    if(contrasena == NULL) return NULL;
    int tamano = strlen(contrasenaCifrada);
    int k = 0;
    for(int i = 0; i < tamano; i += 2){
        char carCif[3];
        carCif[0] = contrasenaCifrada[i];
        carCif[1] = contrasenaCifrada[i + 1];
        carCif[2] = '\0';

        unsigned int valorCif;
        sscanf(carCif, "%X", &valorCif);

        contrasena[k] = (char)(valorCif ^ claveHex);
        k++;
    }
    contrasena[k] = '\0';
    return contrasena;
}

List* iniciarSesion(List * listaUsuario, Map* mapa){
    char usuario[50];
    printf("Ingrese su nombre de usuario: ");
    scanf("%49s", usuario);

    Usuario* usuarioActual = list_first(listaUsuario);
    while(usuarioActual != NULL){
        if(strcmp(usuarioActual->usuario, usuario) == 0){
            int intentos = 3;
            char clave[50];
            if(usuarioActual->momento_bloqueo > 0){
                time_t tiempo_actual = time(NULL);
                double tiempoPasado = difftime(tiempo_actual, usuarioActual->momento_bloqueo);
                if(tiempoPasado < TIEMPO_BLOQUEO){
                    int segundosRestantes = TIEMPO_BLOQUEO - (int)tiempoPasado;
                    printf("Tu cuenta se encuentra bloqueada temporalmente!\n Tiempo restante : %d segundos\n", segundosRestantes);
                    return NULL;
                }
                else{
                    usuarioActual->momento_bloqueo = 0;
                }
            }
            printf("Ingrese su contraseña maestra: ");
            scanf("%49s", clave);
            while(intentos > 0){
                if(strcmp(usuarioActual->contrasena, clave) == 0){
                    printf("Inicio de sesión exitoso.\n");
                    MapPair* parUsuario = map_search(mapa, usuarioActual->usuario);
                    return parUsuario->value;
                } 
                else{
                    intentos--;
                    printf("Contraseña incorrecta.\n");
                }
            }
            if(intentos == 0){
                printf("El usuario ha sido bloqueado temporalmente por %d segundos. \nIntente iniciar sesion mas tarde.");
                usuarioActual->momento_bloqueo = time(NULL);
                return NULL;
            }
            
        }
        usuarioActual = list_next(listaUsuario);
    }
    printf("Usuario no encontrado.\n");
}

int is_equal_str(char dato1, char dato2){
    return strcmp(dato1, dato2);
}

NodoTrie crearTrie(){
    NodoTrie* raiz;
    raiz = (NodoTrie *)malloc(sizeof(NodoTrie));
    raiz->hijos = map_create(is_equal_str);
    raiz->contraseñas = NULL;
    return *raiz;
}

void addTrie(NodoTrie *raiz, char *palabra, List* listaContrasena){
    int i = 0;
    NodoTrie* actual = raiz;
    while(palabra[i] != '\0'){
        if(map_search(actual->hijos, palabra[i]) == NULL){
            NodoTrie * nodo;
            if(palabra[i+1] == '\0'){
                nodo->palabra = palabra;
                nodo->contraseñas = listaContrasena;
            }
            else{
                nodo->contraseñas = NULL;
                nodo->palabra = NULL;
            }
            nodo->hijos = map_create(is_equal_str);
            map_insert(actual->hijos, palabra[i], nodo);
            return;
        }
        actual = map_search(actual->hijos, palabra[i]);
        i++;
    }
    return;
}

List* searchTrie(NodoTrie* raiz, char *palabra){ //falta hacer una para eliminar las palabras
    int i = 0;
    NodoTrie* actual  = raiz;
    while(palabra[i] != '\0'){
        if(strcmp(actual->palabra, palabra) == 0) return actual->contraseñas;
        if(map_search(actual->hijos, palabra[i]) == NULL) return NULL;
        actual = map_search(actual->hijos, palabra[i]);
        i++;
    }
    return NULL;
}

void mostrarPaginas(List* contrasenas){
    Contrasena* aux = list_first(contrasenas);
    printf("Imprimiendo lista de paginas con contrasenas registradas: \n");
    while(aux != NULL){
        printf("%s \n", aux->pagina);
    }
    return;
}

Map* cargarPalabras(){
    Map* palabrasFiltradas = map_create(is_equal_str);
    FILE *archivo = fopen("contrasenas.csv", "r");
    if(archivo == NULL){
        printf("No se encontró el archivo.");
        return;
    }

    char* campo;
    while((campo = leer_linea_csv(archivo, ",")) != NULL){
        if(campo == NULL) continue;
        char* mensaje = "Esta contrasena es muy comun! Se recomiendo cambiarla.";
        map_insert(palabrasFiltradas, campo, mensaje);
    }
    return palabrasFiltradas;
}

void robustez(Map* palabrasF, char *contrasena){
    if(map_search(palabrasF, contrasena) != NULL) printf("%s", map_search(palabrasF, contrasena)->value);
    else{
        int puntaje = 0;
        int mayus = 0;
        int simbolos = 0;
        int minus = 0;
        int numero = 0;
        int largo = strlen(contrasena);
        for(unsigned int i = 0; i < largo; i++){
            if(isupper(contrasena[i])) mayus = 1;
            else if(islower(contrasena[i])) minus = 1;
            else if(isdigit(contrasena[i])) numero = 1;
            else if(ispunct(contrasena[i])) simbolos = 1;
        }

        if(minus) puntaje++;
        if(mayus) puntaje +=2;
        if(numero) puntaje +=2;
        if(simbolos) puntaje +=3;
        if(largo >= 10) puntaje +=2;

        if(puntaje >= 8) printf("Felicidades! Tu contraseña es muy segura.\n");
        else if(puntaje >= 5) {
            printf("Tu contrasena podria mejorar\n");
            if(simbolos == 0 || numero == 0) printf("Intenta anadir simbolos o numeros en la contrasena!\n");
        }
        else{
            printf("Tu contrasena es mu debil!\n");
            if(simbolos == 0 || numero == 0) printf("Intenta anadir simbolos o numeros en la contrasena!\n");
            if(largo <= 10) printf("Intenta alargar la contrasena!");
        }
    }
}

void eliminarPassword(List* listaPassword, NodoTrie* raizTrie){
    char pagina[21];
    printf("Ingrese el nombre de la página o servicio del que desea eliminar su contraseña: ");
    scanf("%20s", pagina);

    List * listaTrie = searchTrie(raizTrie, pagina);
    if(listaTrie == NULL){
        printf("La página o servicio '%s' no ha sido registrada\n", pagina);
        return;
    }
    Contrasena* aux = list_first(listaPassword);
    while(aux != NULL){
        if(strcmp(aux->pagina, pagina) == 0){
            if(aux->pagina != NULL) free(aux->pagina);
            if(aux->contrasenaCifrada != NULL) free(aux->contrasenaCifrada);
            free(aux);

            list_popCurrent(listaPassword);
            printf("La contraseña de la página '%s' ha sido eliminada", pagina);
            return;
        }
        aux = list_next(listaPassword);
    }
}



void sesionIniciada(List* contrasenasUsuario, NodoTrie* raizTrie){
    printf("Bienvenido a su gestor de contrasenas.\n");
    printf("1) Anadir una Contrasena.\n");
    printf("2) Eliminar una Contrasena.\n");
    printf("3) Ver todas las paginas registradas.\n");
    printf("4) Cerrar Sesion.\n");
    char opcion;
    do{
        scanf("%s", opcion);
        switch(opcion){
            case '1':
                AgregarPassword(contrasenasUsuario);
                break;
            case '2':
                eliminarPassword(raizTrie);
                break;
            case '3':
                mostrarPaginas(contrasenasUsuario);
                break;
            case '4':
                CerrarSesion();
                break;
            default:
                printf("Ingrese un caracter valido.\n");
                break;

        }
    } while (opcion != '4');
}

int main()
{
    puts("-----BIENVENIDO A TU GESTOR DE CONTRASEÑAS-----\n");
    Map* palabrasFiltradas = map_create(is_equal_str);
    palabrasFiltradas = cargarPalabras();
    List* listaUsuarios;
    Map* mapaContrasenas;
    LeerArchivo(listaUsuarios, mapaContrasenas);
    char dato;
    printf("1) Iniciar Sesion.\n");
    printf("2) Crear Usuario\n");
    printf("3) Eliminar Usuario\n");
    printf("4) Salir.\n");

    do{
        printf("Ingrese su opcion:\n");
        scanf("%s", dato);
        switch(dato){
            case '1':
                List* contrasenasUsuario;
                contrasenasUsuario = iniciarSesion(listaUsuarios, mapaContrasenas);
                if(contrasenasUsuario != NULL){
                    NodoTrie raizUsuario = crearTrie();
                    Contrasena* aux = list_first(contrasenasUsuario);
                    while(aux != NULL){
                        addTrie(&raizUsuario, aux->pagina, contrasenasUsuario);
                        aux = list_next(contrasenasUsuario);
                    }
                    sesionIniciada(contrasenasUsuario, &raizUsuario);
                }
                break;
            case '2':
                crearUsuario(listaUsuarios, mapaContrasenas);
                break;
            case '3':
                EliminarUsuario(listaUsuarios);
                break;
            case '4':
                printf("Saliendo del programa...");
                break;
            default:
                printf("Ingrese un caracter valido.");
                break;
        }
    } while(dato != '4');
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
