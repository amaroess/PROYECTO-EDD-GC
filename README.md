## PROYECYO : Gestor de contraseñas

Este proyecto implementa registro de perfiles, inicio de sesión mediante una clave maestra y la administración
de credenciales de servicios o páginas garantizando un rendimiento óptimo y almacenamiento local seguro.

## Compilación y ejecución
    Desarrollado en lenguaje C

Requisitos previos para ejecutar:
    Tener un compilador de C (como gcc)
    Visual Studio Code (opcional) 

 # Pasos: 
    Abre la terminal en la carpeta donde está el archivo "main.c" y ejecuta el siguiente comando para compliar el programa:

    gcc main.c tdas/*.c -o programa

# Ejecutar: 
    En windows: ./programa.exe
    En mac: ./programa

## Funciones implementadas
* Funcionando correctamente:
1. crearUsuario y eliminarUsuario
2. iniciarSesion: inicio de sesión con seguridad de máximo 3 intento, si se falla repetidamente se congela la cuenta por 60 segundos
3. leerArchivo: al iniciar el programa, se lee el archivo.txt y descifra las claves maestras
4. guardarArchivo: al cerrar sesión o salir, se guarda la información de la memoria en archivo.txt, aplicando el cifrado
5. encriptar: encripta las claves en base a operaciones XOR, convirtiendo el resultado a formato hexadecimal
6. desencriptar: lee los hexadecimales y los convierte de vuelta a bytes, se revierte el XOR para recuperar la contraseña original
7. estructura de datos Trie (addTrie, searchTrie, borrarTrie): indexación por prefijos de los servicios de cada usuario
8. robustez: verifica la seguridad de las contraseñas comparando con una tabla con contraseñas comunes y en base a su longitud y variedad de carácteres
9. agregarPassword: solicita el nombre y contraseña de la página a guardar
10. eliminarPassword: solicita el nombre y contraseña de la página a eliminar 

* Opciones no funcionando: Ninguna.

## Ejemplo de uso:
1. inicio del programa, primero se registra un nuevo usuario.
    Se selecciona la opción 2) Crear Usuario 
    
    -----BIENVENIDO A TU GESTOR DE CONTRASEÑAS-----

    1) Iniciar Sesion.
    2) Crear Usuario
    3) Eliminar Usuario
    4) Salir.
I   Ingrese su opcion:
    2
    Ingrese su nombre de usuario: pepe
    Ingrese su contraseña maestra: clave123
    Usuario registrado correctamente

2. Seguridad y bloqueo de la cuenta
    Al intentar ingresar con datos que no coinciden en la opción 1) Iniciar Sesion, el programa descuenta intentos y se bloquea la cuenta temporalmente

    Ingrese su opcion:
    1
    Ingrese su nombre de usuario: pepe
    Ingrese su contraseña maestra: jeje
    Contraseña incorrecta.
    Ingrese su contraseña maestra: clave
    Contraseña incorrecta.
    Ingrese su contraseña maestra: j
    Contraseña incorrecta.
    El usuario ha sido bloqueado temporalmente por 60 segundos.
    
    * Intentando volver a iniciar sesión:
    Ingrese su opcion:
    1
    Ingrese su nombre de usuario: pepe
    Tu cuenta se encuentra bloqueada temporalmente!
    Tiempo restante : 45 segundos

3. Inicio de sesión existoso, se ingresa al submenú de credenciales
    Después de ingresar la clave maestra correctamente, se abre el submenú y se agrega una contraseña con la opción 1) Añadir una contraseña. Se añaden servicios y se evalua la robustez poblando la estructura Trie
    
    Bienvenido a su gestor de contraseñas.
    1) Anadir una Contrasena.
    2) Eliminar una Contrasena.
    3) Ver todas las paginas registradas.
    4) Cerrar Sesion.

    Ingrese su opcion:
    1
    Contraseña a agregar: jejeje123
    Página o servicio al que corresponde la contraseña: netflix
    Tu contraseña es muy débil!
    Intenta añadir símbolos o números en la contraseña!
    Intenta alargar la contraseña!

4. Visualización y desencriptación 
    Se selecciona la opción 3 para revisar nuestras contraseñas. Se usa el arbol trie para rastrear la pagina y la muestra en texto.

    Bienvenido a su gestor de contrasenas.
    1) Anadir una Contrasena.
    2) Eliminar una Contrasena.
    3) Ver todos las paginas registradas.
    4) Cerrar Sesion.

    Ingrese su opcion:
    3
    Imprimiendo lista de paginas con contraseñas registradas:
    netflix

    Ingrese la pagina de la cual quiere obtener su contraseña:
    netflix
    Tu contraseña de netflix es: jejeje123

5. Cierre de sesión
    Al elegir la opción 4, el programa consulta si se desea salir, se vacia de manera segura las variables en memoria y se escribe en el .txt con las claves cifradas correspondientes:

    Bienvenido a su gestor de contrasenas.
    1) Anadir una Contrasena.
    2) Eliminar una Contrasena.
    3) Ver todos las paginas registradas.
    4) Cerrar Sesion.
    Ingrese su opcion:
    4
    Estas seguro de cerrar sesion? (s/n): s
    Sesion cerrada exitosamente.

## Contribuciones: 

1. Amaro Escobar: 
    - Implementación de struct Usuario, struct contraseña, función eliminar usuario, función agregar password, función cerrar sesión
    - Auto-evaluación: 3 (aporte excelente)
2. Sebastián Levicoy:
    - Implementación de función iniciar sesión, función encriptar, función robustez, función gestionar password, implementación Trie. Documentación y comentarios en el código.
    - Auto-evaluación: 3 (aporte excelente)
3. Camila Vásquez:
    -Implementación de función leer archivo, función crear usuario, función eliminar password. Diseño y redacción del README.
    - Auto-evaluación: 3 (aporte excelente)