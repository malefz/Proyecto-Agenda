#include <iostream>  // Para operaciones de entrada/salida (cout y cin)
#include <vector>    // Para usar el contenedor vector (una lista dinámica)
#include <string>    // Para trabajar con cadenas de texto (tipo string)
#include <fstream>   // Para operaciones de archivo
#include <limits>    // Para manejar los límites de los tipos de datos (como el valor máximo de un tipo int)
#include <stdexcept> //Para el manejo de excepciones (errores que pueden ser capturados por try-catch)
#include <algorithm> // Para usar funciones algoritmicas (ejemplo "sort")

using namespace std;
// Clase que representa un contacto en la agenda
class Agenda
{
    // Atributo privado
private:
    string nombre;
    string apellido;
    string numeroDeCelular;
    string email;

    // Interfaz pública para interactuar con los atributos
public:
    // Constructor e inicializacion de las variables
    Agenda(string nombre, string apellido, string numeroDeCelular, string email)
        : nombre(nombre), apellido(apellido), numeroDeCelular(numeroDeCelular), email(email) {}

    // Método para mostrar la información del contacto en la consola
    void mostrarContacto()
    {
        cout << "Nombre: " << nombre << endl;                     // Muestra el contacto
        cout << "Apellido: " << apellido << endl;                 // Muestra el apellido
        cout << "Numero de Celular: " << numeroDeCelular << endl; // Muestra el número de celular
        cout << "Email: " << email << endl;                       // Muestra el email
    }

    // Método para guardar un contacto en un archivo - toma una referencia a objetos de tipo ofstream como parámetro
    void contactoArchivado(ofstream &archivo)
    {
        // Guarda el nombre, apellido, número de celular y email en el archivo
        archivo << nombre << " " << apellido << " " << numeroDeCelular << " " << email << endl;
    }

    // Getters: Métodos para obtener el nombre y número de celular del contacto
    string getNombre() const { return nombre; }
    string getNumeroDeCelular() const { return numeroDeCelular; }
};

// Función para validar el número de celular (10 dígitos)
bool validarNumeroCelular(const string &numero)
{
    // Se comprueba que el número tenga exactamente 10 caracteres
    if (numero.size() != 10)
    {
        return false;
    }

    // Se recorre cada caracter en el número
    for (char c : numero)
    {
        //  Se compueba si el carácter no es un dígito (debe estar entre '0' y '9')
        if (c < '0' || c > '9')
        {
            return false;
        }
    }
    // Retorna 'true' si todos los caracteres son dígitos  y la longitud es correcta
    return true;
}

// Función para pedir el número de celular
// Solicita un número de celular al usuario, lo valida asegurandose de tener exactamente 10 dígitos
string pedirNumeroCelular()
{
    string numeroDeCelular;

    // Bucle para seguir pidiendo el número hasta que sea válido
    while (true)
    {
        cout << "Ingrese el numero de celular: ";
        getline(cin, numeroDeCelular);

        // Verifica si el número de celular es válido, si no es, muestra el mensaje de error
        if (!validarNumeroCelular(numeroDeCelular))
        {
            cout << "Numero de celular no valido. " << endl;
        }
        else
        {
            // El número de celular es válido
            return numeroDeCelular;
        }
    }
}

// Función para validar un email
// Verifica si el correo electrónico contiene los símbolos '@' y '.', y que el '@' aparece antes del '.'.
bool validarEmail(const string &email)
{
    // Varibale para verificar si se encuentra el símbolo '@'.
    bool tieneArroba = false;
    // Variable para verificar si se encuentra el símbolo '.'.
    bool tienePunto = false;
    // Verifica si '@' aparece antes de '.'
    bool arrobaAntesDePunto = false;

    // Se recorre cada caracter del email
    for (size_t i = 0; i < email.size(); i++)
    {
        // Si se encuentra el símbolo '@', retorna true.
        if (email[i] == '@')
        {
            tieneArroba = true;
            // Si ya se encontró un punto antes, se marca que '@' está antes de '.'
            if (tienePunto)
                arrobaAntesDePunto = true;
        }

        // Si se encuentra el punto, se marca que se encontró y la posible relación con el '@'
        if (email[i] == '.')
        {
            tienePunto = true;
            // Si ya se encontró un '@' antes, se marca que '@' está antes de '.'
            if (tieneArroba)
                arrobaAntesDePunto = true;
        }
    }

    // Se verifica que estén ambos símbolos y que '@' aparezca antes de '.'
    return tieneArroba && tienePunto && arrobaAntesDePunto;
}

// Función de búsqueda binaria
// Realiza una búsqueda binaria en el vector de punteros a contactos (Agenda) para encontrar un contacto por su nombre.
// Si encuentra el nombre, devuelve 'true' y guarda el índice del contacto en la variable 'index'.
// Si no encuentra el nombre, devuelve 'false'
// La búsqueda binaria asume que el vector de contactos está previamente ordenado por nombre.
bool busquedaBinaria(const vector<Agenda *> &contactos, const string &nombre, int &index)
{
    // Inicializa los índices de búsqueda
    int inicioRango = 0, finRango = contactos.size() - 1;

    // Mientras el rango de búsqueda no esté vacío
    while (inicioRango <= finRango)
    {
        // Calcula el índice del punto medio del rango de búsqueda
        int indiceMedio = inicioRango + (finRango - inicioRango) / 2;

        // Si el contacto en el punto medio tiene el nombre que buscamos
        if (contactos[indiceMedio]->getNombre() == nombre)
        {
            // Guarda el índice donde se encuentra el contacto
            index = indiceMedio;
            // Retorna que se encontró el contacto
            return true;
        }
        // Si el nombre que buscamos es mayor que el nombre del contacto en el medio, busca en la mitad derecha
        else if (contactos[indiceMedio]->getNombre() < nombre)
        {
            // Actualiza el límite izquierdo para buscar en la mitad derecha
            inicioRango = indiceMedio + 1;
        }
        // Si el nombre que buscamos es menor, busca en la mitad izquierda
        else
        {
            // Actualiza el límite derecho para buscar en la mitad izquierda
            finRango = indiceMedio - 1;
        }
    }
    // Si no se encuentra el contacto, retorna false
    return false;
}

// Función para ordenar los contactos por nombre dentro de cada grupo de letras (A-Z).
// Recibe un arreglo de vectores de punteros a Agenda ('contactosPorLetra'), donde cada
// índice representa una letra del alfabeto (0 = 'A',...,25 = 'Z')
// La función ordena los contactos en cada grupo alfabético por su nombre en orden ascendente
void ordenarContactosPorLetra(vector<Agenda *> contactosPorLetra[26])
{
    // Recorre cada grupo de letras (de A a la Z)
    for (int i = 0; i < 26; ++i)
    {
        // Ordena el vector de contactos en el índice i (que corresponde a una letra del alfabeto)
        sort(contactosPorLetra[i].begin(), contactosPorLetra[i].end(),
             // Función lambda para comparar los nombres de los contactos de forma ascendente
             [](Agenda *a, Agenda *b)
             {
                 // Compara los nombres de los contactos (a y b) en orden alfabético ascendente
                 // Las flechitas permiten acceder a los miembros del objeto clase Agenda
                 return a->getNombre() < b->getNombre();
             });
    }
}

// Función para agregar un contacto
// Recibe un arreglo de vectores 'contactosPorLetra' que organiza los contactos por la primera letra del nombre
void agregarContacto(vector<Agenda *> contactosPorLetra[26])
{
    string nombre, apellido, email, numeroDeCelular;

    // Solicita al usuario el nombre del contacto
    cout << "Ingrese el nombre: ";
    getline(cin, nombre);

    // Solicita al usuario el apellido del contacto
    cout << "Ingrese el apellido: ";
    getline(cin, apellido);

    // Solicita el número de celular usando la función 'pedirNumeroCelular'
    numeroDeCelular = pedirNumeroCelular();

    // Solicita al usuario el email del contacto
    cout << "Ingrese el EMAIL: ";
    getline(cin, email);

    // LLama a la función 'validarEmail' para verificar si es válido
    if (!validarEmail(email))
    {
        cout << "Email no válido." << endl;
        return; // Si el email no es válido, termina la función
    }

    // Verifica si el nombre comienza con una letra (mayúscula o minuscula)
    if ((nombre[0] >= 'A' && nombre[0] <= 'Z') || (nombre[0] >= 'a' && nombre[0] <= 'z'))
    {
        // Si el nombre comienza con una letra válida, crea un nuevo contacto.
        Agenda *nuevoContacto = new Agenda(nombre, apellido, numeroDeCelular, email);

        // Calcula la letra inicial del nombre para determinar para determinar en qué grupo de contacto se almacenará
        int letraInicial = toupper(nombre[0]) - 'A';

        // Verifica si la letra inicial está dentro del rango A-Z
        if (letraInicial >= 0 && letraInicial < 26)
        {
            // Agrega el nuevo contacto al grupo correspondiente según la letra inicial
            contactosPorLetra[letraInicial].push_back(nuevoContacto);

            // Ordena los contactos dentro de cada grupo de letras por nombre
            ordenarContactosPorLetra(contactosPorLetra);
        }
        else
        {
            // Si la letra inicial no es válida (A-Z)
            cout << "El nombre no comienza con una letra válida." << endl;
        }
    }
    else
    {
        // Si el nombre no comienza con una letra, muestra mensaje de error.
        cout << " El nombre debe comenzar con una letra." << endl;
    }
}

// Función para buscar un contacto en la agenda
// Recibe un arreglo de vectores 'ContactosPorLetra' que organiza los contactos por la primera letra del nombre.
void buscarContacto(vector<Agenda *> contactosPorLetra[26])
{
    string nombre;

    // Solicita al usuario el nombre del contacto a buscar
    cout << "Ingrese el nombre: ";
    getline(cin, nombre);

    // Verifica que el nombre ingrese por el usuario comience con una letra del alfabeto
    if (!(nombre[0] >= 'A' && nombre[0] <= 'Z') && !(nombre[0] >= 'a' && nombre[0] <= 'z'))
    {
        cout << "El nombre debe comenzar con una letra del alfabeto." << endl;
        return; // Si no comienza con una letra, termina la función
    }

    // Calcula el índice basado en la letra inicial del nombre para determinar el grupo alfabético
    int letraInicial = toupper(nombre[0]) - 'A';

    // Verifica si la letra inicial es válida (en el rango de A-Z)
    if (letraInicial >= 0 && letraInicial < 26)
    {
        // Accede al grupo de contactos correspondientes a la letra inicial del nombre
        vector<Agenda *> &listaContactos = contactosPorLetra[letraInicial];

        // Variable para almacenar el índice del contacto encontrado
        int index;

        // Realiza una búsqueda binaria en la lista de contactos para encontrar el nombre
        if (busquedaBinaria(listaContactos, nombre, index))
        {
            // Si encuentra el contacto, muestra la información
            listaContactos[index]->mostrarContacto();
        }
        else
        {
            // Si no encuentra el contacto, muestra un mensaje
            cout << "El nombre no se encontró." << endl;
        }
    }
    else
    {
        // Si la letra inicial no es válida, muestra un mensaje de error
        cout << "Nombre no válido para la búsqueda." << endl;
    }
}

// Función para editar un contacto en la agenda
// Recibe un arreglo de vectores 'contactosPorLetra' que organiza los contactos por la primera letra del nombre.
void editarContacto(vector<Agenda *> contactosPorLetra[26])
{
    string nombre;

    // Solicita al usuario el nombre del contacto que desea editar
    cout << "Ingrese el nombre del contacto que quiere modificar: ";
    getline(cin, nombre);

    // Verifica que el nombre comience con una letra del alfabeto (mayúscula o minúscula)
    if (!(nombre[0] >= 'A' && nombre[0] <= 'Z') && !(nombre[0] >= 'a' && nombre[0] <= 'z'))
    {
        cout << "El nombre debe comenzar con una letra del alfabeto." << endl;
        return; // Si no comienza con una letra, termina la función
    }

    // Calcula el índice basado en la letra inicial del nombre para determinar el grupo alfabético
    int letraInicial = toupper(nombre[0]) - 'A';

    // Verifica si la letra inicial es válida (en el rango de A-Z)
    if (letraInicial >= 0 && letraInicial < 26)
    {
        // Accede al grupo de contactos correspondientes a la letra inicial del nombre
        vector<Agenda *> &listaContactos = contactosPorLetra[letraInicial];

        // Variable para almacenar el índice del contacto encontrado
        int index;

        // Realiza una búsqueda binaria en la lista de contactos para encontrar el nombre
        if (busquedaBinaria(listaContactos, nombre, index))
        {
            // Si se encuentra el contacto, permite modificar sus datos
            string nuevoNombre, nuevoApellido, nuevoNumero, nuevoEmail;
            cout << "Ingrese el nuevo nombre: ";
            getline(cin, nuevoNombre);
            cout << "Ingrese el nuevo apellido: ";
            getline(cin, nuevoApellido);
            cout << "Ingrese el nuevo numero: ";
            getline(cin, nuevoNumero);
            cout << "Ingrese el nuevo email: ";
            getline(cin, nuevoEmail);

            // Elimina el contacto anterior (previa liberación de memoria) y crea uno nuevo con los datos actualizados
            delete listaContactos[index];
            listaContactos[index] = new Agenda(nuevoNombre, nuevoApellido, nuevoNumero, nuevoEmail);

            // Muestra un mensaje indicando que el contacto fue actualizado con éxito
            cout << "Contacto actualizado exitosamente." << endl;
        }
        else
        {
            // Si no se encuentra el contacto, muestra un mensaje de error
            cout << "El nombre no se encontró para ser editado." << endl;
        }
    }
    else
    {
        // Si la letra inicial no es válida (no está en el rango A-Z), muestra un mensaje de error
        cout << "Nombre no válido para editar." << endl;
    }
}

// Función para eliminar un contacto de la agenda
// Recibe un arreglo de vectores `contactosPorLetra` que organiza los contactos por la primera letra del nombre.
void eliminarContacto(vector<Agenda *> contactosPorLetra[26])
{
    string nombre;

    // Solicita al usuario el nombre del contacto que desea eliminar
    cout << "Ingrese el nombre para eliminar contacto: ";
    getline(cin, nombre);

    // Calcula el índice basado en la letra inicial del nombre para determinar el grupo alfabético
    int letraInicial = toupper(nombre[0]) - 'A';

    // Verifica que el nombre no esté vacío
    if (nombre.size() == 0)
    {
        cout << "El nombre no puede estar vacío." << endl;
        return; // Si el nombre está vacío, termina la función
    }

    // Verifica si la letra inicial es válida (en el rango de A-Z)
    if (letraInicial >= 0 && letraInicial < 26)
    {
        // Accede al grupo de contactos correspondientes a la letra inicial del nombre
        vector<Agenda *> &listaContactos = contactosPorLetra[letraInicial];

        // Verifica si hay contactos en el grupo correspondiente
        if (listaContactos.size() == 0)
        {
            cout << "No hay contactos disponibles para eliminar en esta categoría." << endl;
            return; // Si no hay contactos en la categoría, termina la función
        }

        // Variable para almacenar el índice del contacto encontrado
        int index;

        // Realiza una búsqueda binaria para encontrar el contacto por su nombre
        if (busquedaBinaria(listaContactos, nombre, index))
        {
            // Si el contacto es encontrado, lo elimina
            // Elimina el contacto de la memoria
            delete listaContactos[index];
            // Reemplaza el contacto eliminado con el último de la lista
            listaContactos[index] = listaContactos.back();
            // Elimina el último contacto de la lista
            listaContactos.pop_back();

            cout << "Contacto eliminado exitosamente." << endl;
        }
        else
        {
            // Si el contacto no es encontrado, muestra un mensaje de error
            cout << "No se encontró el contacto para eliminar." << endl;
        }
    }
    else
    {
        // Si la letra inicial no es válida, muestra un mensaje de error
        cout << "Nombre no válido para eliminar." << endl;
    }
}

// Función para guardar los contactos en un archivo
void guardarContacto(vector<Agenda *> contactosPorLetra[26])
{
    try
    {
        // Intenta abrir el archivo "contactos.txt" en modo de escritura
        ofstream archivoDeContactos("contactos.txt", ios::out);

        // Si no se pudo abrir el archivo, lanza una excepción
        if (!archivoDeContactos.is_open())
        {
            throw runtime_error("Error al abrir el archivo para guardar los contactos.");
        }

        // Itera sobre cada letra (A-Z) para guardar los contactos correspondientes
        for (int i = 0; i < 26; i++)
        {
            for (size_t j = 0; j < contactosPorLetra[i].size(); ++j)
            {
                // Guarda cada contacto en el archivo
                contactosPorLetra[i][j]->contactoArchivado(archivoDeContactos);
            }
        }

        // Informa que los contactos se guardaron correctamente
        cout << "Contactos guardados en el archivo exitosamente." << endl;

        // Cierra el archivo
        archivoDeContactos.close();
    }
    catch (const runtime_error &errorArchivo)
    {
        // Si ocurre un error al abrir el archivo, muestra un mensaje de excepción
        cout << "Excepción al guardar el archivo: " << errorArchivo.what() << endl;
    }
    catch (const exception &errorGeneral)
    {
        // Captura cualquier otra excepción general y muestra un mensaje
        cout << "Excepción general: " << errorGeneral.what() << endl;
    }
}

// Función principal
int main()
{
    // Array de vectores para almacenar los contactos, agrupados por la primera letra del nombre
    vector<Agenda *> contactosPorLetra[26];
    int opcion; // Variable que almacena la opción seleccionada por el usuario

    // try-catch para manejar errores, como problemas al guardar el archivo o errores inesperados
    try
    {
        // El ciclo se repite hasta que el usuario seleccione la opción 6 (Salir)
        do
        {
            // Muestra el menú de opciones para el usuario
            cout << endl;
            cout << "MENU AGENDA DE CONTACTOS " << endl;
            cout << "1. Agregar contacto. " << endl;
            cout << "2. Buscar contacto. " << endl;
            cout << "3. Editar contacto. " << endl;
            cout << "4. Eliminar contacto. " << endl;
            cout << "5. Guardar contacto. " << endl;
            cout << "6. Salir. " << endl;

            // Solicita al usuario que elija una opción
            cout << "Elegir una opcion: ";
            cin >> opcion;

            // VALIDACIÓN DE ENTRADA AL USUARIO
            // Verifica si la entrada es válida. Si no, muestra un mensaje de error y repite el ciclo
            if (cin.fail())
            {
                cin.clear();                                         // Limpia el estado de error y vuelve a funcionar
                cin.ignore(numeric_limits<streamsize>::max(), '\n'); // Descarta la entrada incorrecta
                cout << endl;
                cout << "Opcion no valida. Por favor, ingrese un número entero entre 1 y 6." << endl;
                continue; // Vuelve a mostrar el menú si la entrada es incorrecta
            }

            // Limpia el buffer de entrada para evitar problemas con el getline posterior
            cin.ignore();

            // Ejecuta la opción correspondiente según la elección del usuario
            switch (opcion)
            {
            case 1: // Agregar un nuevo contacto
                agregarContacto(contactosPorLetra);
                break;
            case 2: // Buscar un contacto por nombre
                buscarContacto(contactosPorLetra);
                break;
            case 3: // Editar un contacto existente
                editarContacto(contactosPorLetra);
                break;
            case 4: // Eliminar un contacto
                eliminarContacto(contactosPorLetra);
                break;
            case 5: // Guardar los contactos en un archivo
                guardarContacto(contactosPorLetra);
                break;
            case 6: // Salir del programa
                cout << "Gracias por usar la agenda de contactos. ¡Hasta pronto!" << endl;
                break;
            default: // Si la opción no es válida
                cout << "Opción no válida. " << endl;
            }
        } while (opcion != 6); // Repite el ciclo hasta que el usuario elija salir (opción 6)

        // Liberación de memoria para los objetos Agenda creados dinámicamente
        for (int i = 0; i < 26; ++i)
        {
            for (size_t j = 0; j < contactosPorLetra[i].size(); ++j)
            {
                delete contactosPorLetra[i][j]; // Elimina cada contacto para liberar la memoria
            }
        }
    }
    catch (const runtime_error &errorArchivo) // Captura errores relacionados con la apertura o manejo de archivos
    {
        // Muestra el mensaje de error si hubo un problema con el archivo
        cout << "Excepción al guardar el archivo: " << errorArchivo.what() << endl;
    }
    catch (const exception &errorGeneral) // Captura cualquier otra excepción general
    {
        // Muestra el mensaje de error para cualquier otra excepción no específica
        cout << "Error inesperado: " << errorGeneral.what() << endl;
    }

    return 0; // Retorna 0 para indicar que el programa terminó correctamente
}
