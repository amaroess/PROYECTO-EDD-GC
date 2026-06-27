#include <stdio.h>
#include <string.h>
#include <time.h>
#include "tdas/list.h" // TDA LISTA
#include "tdas/map.h" // TDA MAPA
#include "tdas/extra.h"
#include <ctype.h>
 
#define claveHex 0x6C // constante hexadecimal para cifrar
#define TIEMPO_BLOQUEO 60 // contaste de tiempo para bloqueo temporal de cuentas

typedef struct Usuario // hecho por amaro, el 11/06/2024
{
    char* usuario; // nombre de usuario
    char* contrasena; // contraseña del usuario
    time_t momento_bloqueo; // marca de tiempo para bloqueo
} Usuario;

typedef struct Contrasena // hecho por amaro, el 11/06/2024
{
    char* contrasenaCifrada; // contraseña cifrada
    char* pagina; // página o servicio al que corresponde la contraseña.
} Contrasena;

typedef struct{
    Map* hijos; // mapa de hijos
    char *palabra; // palabra para ubicar fin del string
    char* contrasena; //contrasena cifrada
} NodoTrie;

int is_equal_str(void* dato1, void* dato2){
    return strcmp((char*)dato1, (char*)dato2); // funcion para comparar strings para el mapa
}

NodoTrie* crearTrie(){ // se crea un Trie utilizando la raiz
    NodoTrie* raiz;
    raiz = (NodoTrie *)malloc(sizeof(NodoTrie)); // se reserva memoria
    raiz->hijos = map_create(is_equal_str); // se crea el mapa de hijos
    raiz->contrasena = NULL; // se establece la contrasena en NULL
    raiz->palabra = NULL;
    return raiz;
}

void addTrie(NodoTrie *raiz, char *palabra, char* contrasenaCif){ // con esta funcion se añaden palabras al Trie
    int i = 0; // se usa un contador para recorrer la palabra
    NodoTrie* actual = raiz; // se inicia desde la raiz
    while(palabra[i] != '\0'){ // hasta encontrar el caracter nulo
        if(map_search(actual->hijos, &palabra[i]) == NULL){ // si la letra que se busca no está en el mapa de hijos del nodo actual
            NodoTrie * nodo = (NodoTrie *)malloc(sizeof(NodoTrie)); // se reserva memoria para el nodo
            if(palabra[i+1] == '\0'){ // si el siguiente caracter es el fin del string
                nodo->palabra = palabra; // se asigna la palabra para marcar la letra final
                nodo->contrasena = contrasenaCif; // se guarda la contraseña cifrada en el caracter final
            }
            else{ // si no es el caracter final
                nodo->contrasena = NULL; //la contraseña y palabra quedan como nulos
                nodo->palabra = NULL;
            }
            nodo->hijos = map_create(is_equal_str); // se crea el mapa de hijos
            map_insert(actual->hijos, &palabra[i], nodo); // en los hijos del actual se ingresa el nodo que acabamos de crear
        }
        actual = map_search(actual->hijos, &palabra[i])->value; // si la letra se encontró entre los hijos del actual, actual se traslada ahi
        i++; // se aumenta el contador
    }
    return;
}

char* searchTrie(NodoTrie* raiz, char *palabra){
    int i = 0; // se inicia el contador
    NodoTrie* actual  = raiz;
    while(palabra[i] != '\0'){ // se recorre la palabra
        if(strcmp(actual->palabra, palabra) == 0) return actual->contrasena; // si el caracter actual tiene la palabra del caracter final y coincide con la que se busca, se retorna la contrasena
        if(map_search(actual->hijos, &palabra[i]) == NULL) return NULL; // si alguna de las letras no está en el Trie, la palabra entera no está
        actual = map_search(actual->hijos, &palabra[i])->value; // se traslada al hijo que coincida con la letra actual
        i++; // se aumenta el contador
    }
    return NULL; // si se recorre la palabra entera sin terminar la ejecucion se retorna nulo
}

void borrarTrie(NodoTrie* nodo){ // esta funcion borra todo el Trie
    if(nodo == NULL) return; // si el nodo ya es nulo se retorna la funcion
    if(nodo->hijos != NULL){ //mientras aun haya hijos
        MapPair* par = map_first(nodo->hijos); // se recorre la lista de hijos
        while(par != NULL){ //se recorren los hijos
            NodoTrie* aux = (NodoTrie*)par->value;
            borrarTrie(aux); // se usa la funcion recursivamente para eliminar los hijos desde las hojas hasta la raiz

            par = map_next(nodo->hijos);
        }
        
        map_clean(nodo->hijos); // con el mapa de hijos vacio se ejecuta un clean en el mapa
        free(nodo->hijos); // y se libera la memoria del mapa
    }

    if(nodo->contrasena != NULL) free(nodo->contrasena); // si el nodo tiene contraseña se libera la memoria dinamica

    free(nodo); // finalmente se libera la memoria total del nodo.

}

char* encriptar(char* contrasena){ //esta funcion recibe una contraseña y la retorna cifrada
    int i = 0; //se incia un contador
    char* contrasenaCifrada = malloc(strlen(contrasena) + 1); // se reserva memoria para la contraseña cifrada, el tamaño es el mismo que la contraseña original más uno para el caracter nulo
    while(contrasena[i] != '\0'){ // mientras no se llegue al fin del string
        char caracterCif = contrasena[i] ^ claveHex; // se hace un desplazamiento de bytes con una constante hexadecimal 
        sprintf(&contrasenaCifrada[i], "%02X", caracterCif); // el resultado del desplazamiento se guarda en formato hexadecimal de 2 caracteres
        i++; // se aumenta el indice en 1 para revisar el siguiente caracter
    }
    contrasenaCifrada[i*2] = '\0'; // al final del string se añade el caracter nulo
    return contrasenaCifrada; // se retorna la contraseña cifrada
}

char* desencriptar(char* contrasenaCifrada){ // esta funcion recibe una contraseña cifrada y devuelve la contraseña original
    char* contrasena = malloc(strlen(contrasenaCifrada) / 2 + 1); //se reserva memoria para la mitad del tamaño del str + el caracter nulo
    if(contrasena == NULL) return NULL; // si falla la reserva de memoria se termina la funcion
    int tamano = strlen(contrasenaCifrada); // se toma el tamaño del str cifrado
    int k = 0; // y se inicia un indice en 0 para armar la cadena original
    for(int i = 0; i < tamano; i += 2){ //se recorre la cadena cifrada de 2 en 2
        char carCif[3]; // se crea una cadena de 3 caracteres
        carCif[0] = contrasenaCifrada[i]; //los primeros 2 son los caracteres en hexadecimal y el tercero es el caracter nulo
        carCif[1] = contrasenaCifrada[i + 1];
        carCif[2] = '\0';

        unsigned int valorCif; // se crea una variable para guardar los valores resultantes del desplazamiento de bytes original
        sscanf(carCif, "%X", &valorCif);

        contrasena[k] = (char)(valorCif ^ claveHex); // usando la clave hexadecimal se revierten al caracter original y se ingresa el caracter en un string
        k++; // se aumenta este indice por cada caracter añadido
    }
    contrasena[k] = '\0'; //se añade el caracter nulo al final del string
    return contrasena; // se retorna la contrasena original
}

void crearUsuario(List* listaUsuario, Map* mapa){
    char nombre[50]; // nombre de usuario
    char clave[50]; //contraseña maestra

    printf("Ingrese su nombre de usuario: ");
    scanf("%49s", nombre);

    MapPair* usuario_existente = map_search(mapa, nombre); //se comprueba con el mapa si el nombre de usuario ya está en uso
    if(usuario_existente != NULL){
        printf("El usuario %s ya está registrado\n");
        return;
    }

    printf("Ingrese su contraseña maestra: ");
    scanf("%49s", clave);

    Usuario* nuevoUsuario = malloc(sizeof(Usuario)); // se reserva memoria para guardar un nuevo usuario
    if(nuevoUsuario == NULL) return; // si falla la reserva termina la ejecucion de la funcion
    
    nuevoUsuario->usuario = strdup(nombre); //se copian los datos para el nuevo usuario
    nuevoUsuario->contrasena = strdup(clave);
    nuevoUsuario->momento_bloqueo = 0; // la marca del bloqueo temporal se inicializa en 0
    list_pushBack(listaUsuario, nuevoUsuario); //se añade el usuario en la lista de usuarios
    List* listaPassword = list_create(); // se crea su lista de contrasenas
    map_insert(mapa, nuevoUsuario->usuario, listaPassword); //se añade en el mapa el usuario nuevo y su lista de contraseñas
    printf("Usuario registrado correctamente\n");
    
}


void EliminarUsuario(List* listaUsuario, Map* mapaUsuarios, NodoTrie* raiz) // hecho por amaro, el 11/06/2024
{
    char UsuarioE[21]; // variable para guardar el nombre del usuario a eliminar, 21 caracteres una cantidad aceptable.
    printf("Usuario a Eliminar: ");
    scanf("%20s", UsuarioE); // se lee el nombre del usuario a eliminar
    Usuario* usuarioAEliminar = list_first(listaUsuario); // se obtiene el primer usuario de la lista
    while (usuarioAEliminar != NULL) // se recorre la lista de usuarios hasta encontrar el usuario a eliminar
    {
        if (strcmp(usuarioAEliminar->usuario, UsuarioE) == 0) // se compara el nombre del usuario a eliminar con el nombre del usuario actual de la lista
        {
            list_popCurrent(listaUsuario); // se elimina el usuario de la lista, del mapa y se elimina su trie
            map_remove(mapaUsuarios, UsuarioE); 
            borrarTrie(raiz);
            printf("Usuario eliminado exitosamente.\n");
            return; // se termina la función después de eliminar el usuario
        }
        usuarioAEliminar = list_next(listaUsuario); // se avanza en la lista
    }
    printf("Usuario no encontrado.\n");
}

void AgregarPassword(List* ListaPassword, NodoTrie* raiz) // hecho por amaro, el 11/06/2024
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
    
    addTrie(raiz, pagina, contrasenaFinal); // se añade la pagina al Trie del usuario 
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

    FILE *archivo = fopen("archivo.txt", "r"); //se abre el archivo de usuarios en modo lectura
    if(archivo == NULL){ // si el archivo no está se termina la ejecucion de la funcion
        printf("No se encontró el archivo.");
        return;
    }

    char** campos;
    while((campos = leer_linea_csv(archivo, ',')) != NULL){ // por cada linea en el csv
        if(campos[0] == NULL || campos[1] == NULL) continue; // Si la linea no tiene al usuario o la clave maestra, se salta

        char* usuario = campos[0]; // el primer campo es el usuario
        char* clave_maestra = campos[1]; // el segundo la contraseña maestra
        char* pagina_servicio = campos[2]; //el tercero la pagina
        char* clave_cifrada = campos[3]; // el cuarto la contraseña de la pagina

        List* listaPassword = NULL; // se inicializa la lista de contraseñas
        MapPair* parUsuario = map_search(mapa, usuario); // se busca el usuario en el mapa
        if(parUsuario != NULL){ // si está se copia su lista de contraseñas
            listaPassword = parUsuario->value;
        }
        if(parUsuario == NULL){ // si no está entonces se añade el usuario y se inicializa su lista de contraseñas
            Usuario* nuevoUsuario = malloc(sizeof(usuario));
            if(nuevoUsuario == NULL) return;

            nuevoUsuario->usuario = strdup(usuario);
            nuevoUsuario->contrasena = strdup(clave_maestra);

            list_pushBack(listaUsuario, nuevoUsuario);
            listaPassword = list_create();
            map_insert(mapa, nuevoUsuario->usuario, listaPassword);
        }
        if(pagina_servicio != NULL && clave_cifrada != NULL){ // si hay un servicio y contraseña validos 
            Contrasena *nuevaC = malloc(sizeof(Contrasena)); //se reserva memoria para la contraseña
            if(nuevaC == NULL) return;

            nuevaC->pagina = strdup(pagina_servicio); //se duplican los datos de la contraseña
            nuevaC->contrasenaCifrada = strdup(clave_cifrada);

            list_pushBack(listaPassword, nuevaC); // y se añaden en la lista de contraseñas del usuario
        }

    }
    fclose(archivo); // tras inspeccionar todo el csv de datos se cierra el archivo
}

void guardarArchivo(List* listaUsuario, Map* mapa){
    FILE *archivo = fopen("archivo.txt", "w"); // se abre el archivo de datos en modo escritura
    if(archivo == NULL) return;

    Usuario * usuario = list_first(listaUsuario); 
    while(usuario != NULL){ // se recorre la lista de usuarios
        MapPair* par = map_search(mapa, usuario->usuario);
        List* listaPassword = NULL;
        if(par != NULL){
            listaPassword = par->value;
        } else{
            listaPassword = NULL;
        }

        if(listaPassword != NULL && list_first(listaPassword) != NULL){
            Contrasena* contrasena = list_first(listaPassword);
            while(contrasena != NULL){
                fprintf(archivo, "%s,%s,%s,%s\n", usuario->usuario, usuario->contrasena, contrasena->pagina, contrasena->contrasenaCifrada);
                contrasena = list_next(listaPassword);
            }
        }
        else{
            fprintf(archivo, "%s,%s,,\n", usuario->usuario, usuario->contrasena);
        }
        usuario = list_next(listaUsuario);


    }

    fclose(archivo);

}

List* iniciarSesion(List * listaUsuario, Map* mapaContras){ // esta funcion recibe la lista de usuarios y el mapa de contrasenas
    char usuario[50]; 
    printf("Ingrese su nombre de usuario: ");
    scanf("%49s", usuario);

    Usuario* usuarioActual = list_first(listaUsuario); // se busca al usuario que quiere iniciar sesion
    while(usuarioActual != NULL){
        if(strcmp(usuarioActual->usuario, usuario) == 0){ // cuando se encuentra
            int intentos = 3; // se le dan 3 intentos
            char clave[50]; 
            if(usuarioActual->momento_bloqueo > 0){ // si el usuario tiene una marca de tiempo distinta a 0
                time_t tiempo_actual = time(NULL);
                double tiempoPasado = difftime(tiempo_actual, usuarioActual->momento_bloqueo);
                if(tiempoPasado < TIEMPO_BLOQUEO){ // si la diferencia del tiempo actual y la marca es menor a 60 segundos la cuenta aun se ecuentra bloqueada
                    int segundosRestantes = TIEMPO_BLOQUEO - (int)tiempoPasado;
                    printf("Tu cuenta se encuentra bloqueada temporalmente!\n Tiempo restante : %d segundos\n", segundosRestantes);
                    return NULL; // si la cuenta se encuentra bloqueada el usuario no podrá acceder a sus contraseñas
                }
                else{ // si la diferencia de tiempo actual y la marca es mayor a 0, ya terminó el bloqueo
                    usuarioActual->momento_bloqueo = 0; // por lo tanto se resetea la marca de tiempo a 0 y se permite ingresar la contraseña
                }
            }
            printf("Ingrese su contraseña maestra: ");
            scanf("%49s", clave); // el usuario ingresa su contraseña
            char *contraCifrada = encriptar(clave); //se encripta y se compara con la contraseña que ya se encuentra encriptada
            while(intentos > 0){ // si el usuario no ha agotado sus intentos
                if(strcmp(usuarioActual->contrasena, contraCifrada) == 0){ // y las contraseñas coinciden
                    printf("Inicio de sesión exitoso.\n");
                    MapPair* parUsuario = map_search(mapaContras, usuarioActual->usuario);
                    return parUsuario->value; // la función retornará la lista de contraseñas del usuario
                } 
                else{
                    intentos--; // si el usuario se equivoca se disminuye la cantidad de intentos
                    printf("Contraseña incorrecta.\n");
                }
            }
            if(intentos == 0){ // si los intentos llegan a 0
                printf("El usuario ha sido bloqueado temporalmente por %d segundos. \nIntente iniciar sesion mas tarde.");
                usuarioActual->momento_bloqueo = time(NULL); // se toma el tiempo en el que ocurre para el bloqueo de contraseña
                return NULL; // y la funcion no retorna nada
            }
            
        }
        usuarioActual = list_next(listaUsuario); // se explora la lista de usuarios buscando al usuario
    }
    printf("Usuario no encontrado.\n"); // si el usuario nunca se encontró entonces se le avisa al usuario
}




void mostrarPaginas(List* contrasenas){
    Contrasena* aux = list_first(contrasenas); // se recorre la lista de paginas/contraseñas
    printf("Imprimiendo lista de paginas con contrasenas registradas: \n");
    while(aux != NULL){ // se imprimen todas las paginas registradas del usuario
        printf("%s \n", aux->pagina);
    }
    return;
}

void mostrarContrasena(NodoTrie* trieContrasena){
    char pagina[50]; // se le pide al usuario la pagina de la que quiere la contraseña
    printf("Ingrese la pagina de la cual quiere obtener su contrasena:\n");
    scanf("%49s", pagina);

    char* contraCif = searchTrie(trieContrasena, pagina); // se busca la contrasena usando el Trie
    if(contraCif == NULL) printf("No se encontró una contrasena para ese servicio.\n"); // se le dice al usuario si no se encontró la contraseña
    char* contraDesCif = desencriptar(contraCif); // la contraseña retornada está cifrada, por lo que se revierte el cifrado
    printf("Tu contraseña de %s es: %s.", pagina, contraDesCif); // se le meuestra al usuario su contraseña y termina la funcion
    return;
}

Map* cargarPalabras(){ // esta funcion carga el archivo con las 100 contraseñas mas comunes y lo guarda en una tabla hash
    Map* palabrasFiltradas = map_create(is_equal_str);
    FILE *archivo = fopen("contrasenas.csv", "r");
    if(archivo == NULL){
        printf("No se encontró el archivo.");
        return NULL;
    }

    char** campo;
    while((campo = leer_linea_csv(archivo, ',')) != NULL){ // se lee cada linea hasta encontrar un NULL
        char* mensaje = "Esta contrasena es muy comun! Se recomiendo cambiarla.";
        map_insert(palabrasFiltradas, campo, mensaje); // se inserta cada contraseña en el mapa con la contra como key y un mensaje como data
    }
    return palabrasFiltradas; // se retorna el mapa de contraseñas comunes
}

void robustez(Map* palabrasF, char *contrasena){ // esta funcion evalua la robustez de cada contraseña
    if(map_search(palabrasF, contrasena) != NULL) printf("%s", map_search(palabrasF, contrasena)->value); // si la contraseña del usuario está en el mapa de contras comunes se rechaza inmediatamente
    else{ // si pasa el filtro del mapa
        int puntaje = 0;
        int mayus = 0;
        int simbolos = 0;
        int minus = 0;
        int numero = 0;
        int largo = strlen(contrasena);
        for(unsigned int i = 0; i < largo; i++){ // se explora la contraseña
            if(isupper(contrasena[i])) mayus = 1;
            else if(islower(contrasena[i])) minus = 1;
            else if(isdigit(contrasena[i])) numero = 1;
            else if(ispunct(contrasena[i])) simbolos = 1;
        }

        if(minus) puntaje++; // ya bajo ciertas condiciones se le da un puntaje a la contraseña
        if(mayus) puntaje +=2;
        if(numero) puntaje +=2;
        if(simbolos) puntaje +=3;
        if(largo >= 10) puntaje +=2;

        if(puntaje >= 8) printf("Felicidades! Tu contraseña es muy segura.\n"); // dependiendo del puntaje se le dirá al usuario que tan fuerte es su contraseña
        else if(puntaje >= 5) {
            printf("Tu contrasena podria mejorar\n");
            if(simbolos == 0 || numero == 0) printf("Intenta anadir simbolos o numeros en la contrasena!\n");
        }
        else{
            printf("Tu contrasena es muy debil!\n");
            if(simbolos == 0 || numero == 0) printf("Intenta anadir simbolos o numeros en la contrasena!\n");
            if(largo <= 10) printf("Intenta alargar la contrasena!\n");
        }
    }
}

void eliminarPassword(List* listaPassword, NodoTrie* raizTrie){
    char pagina[21];
    printf("Ingrese el nombre de la página o servicio del que desea eliminar su contraseña:\n");
    scanf("%20s", pagina);

    char* contrasenaCif = searchTrie(raizTrie, pagina);
    if(contrasenaCif == NULL){
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



void sesionIniciada(List* contrasenasUsuario, NodoTrie* raizTrie){ // esta funcion le muestra al usuario el menú despues de iniciar sesion
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
                AgregarPassword(contrasenasUsuario, raizTrie);
                break;
            case '2':
                eliminarPassword(contrasenasUsuario, raizTrie);
                break;
            case '3':
                mostrarPaginas(contrasenasUsuario);
                mostrarContrasena(raizTrie);
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
    Map* palabrasFiltradas = map_create(is_equal_str); // se crea y carga el mapa de contrasenas comunes
    palabrasFiltradas = cargarPalabras();
    List* listaUsuarios = list_create(); // se crean la lista de usuarios y mapa de contraseñas
    Map* mapaContrasenas = map_create(is_equal_str);
    LeerArchivo(listaUsuarios, mapaContrasenas); // se llenan con los datos del archivo
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
                List* contrasenasUsuario = list_create(); // se obtiene la lista de contraseñas del usuario 
                contrasenasUsuario = iniciarSesion(listaUsuarios, mapaContrasenas);
                if(contrasenasUsuario != NULL){ // se añade cada contraseña en el trie
                    NodoTrie* raizUsuario = crearTrie();
                    Contrasena* aux = list_first(contrasenasUsuario);
                    while(aux != NULL){
                        addTrie(raizUsuario, aux->pagina, aux->contrasenaCifrada);
                        aux = list_next(contrasenasUsuario);
                    }
                    sesionIniciada(contrasenasUsuario, raizUsuario); //se inicia sesion
                    guardarArchivo(listaUsuarios, mapaContrasenas); // una vez finalizada la sesion se guardan los cambios en el archivo de datos
                }
                break;
            case '2':
                crearUsuario(listaUsuarios, mapaContrasenas); 
                guardarArchivo(listaUsuarios, mapaContrasenas); // cuando finaliza la creacion de usuario este se añade en el archivo
                break;
            case '3':
                List* contrasenasUsuario = list_create();
                contrasenasUsuario = iniciarSesion(listaUsuarios, mapaContrasenas);
                if(contrasenasUsuario != NULL){
                    NodoTrie* raizUsuario = crearTrie();
                    Contrasena* aux = list_first(contrasenasUsuario);
                    while(aux != NULL){
                        addTrie(raizUsuario, aux->pagina, aux->contrasenaCifrada);
                        aux = list_next(contrasenasUsuario);
                    }
                    EliminarUsuario(listaUsuarios, mapaContrasenas, raizUsuario); // cuando se quiere eliminar un usuario tambien se elimina su arbol Trie para asegurar que no haya datos basura
                }
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
}