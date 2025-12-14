#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// --- CONSTANTES GLOBALES ---
#define DATA_SIZE 4 // Un entero (int) es de 4 bytes en nuestra pila x86 de 32 bits.

// Códigos de salida definidos en el S.O.M. (SimpleStack Official Manual)
#define EXIT_SUCCESS 0
#define EXIT_ERROR_USAGE 1
#define EXIT_ERROR_FILE 2
#define EXIT_ERROR_SYNTAX 3


/**
 * @brief Función Core: Traduce el código SimpleStack a Assembly x86.
 * * Itera sobre el archivo de entrada, tokeniza cada línea y genera el Assembly
 * siguiendo las reglas de la máquina de pila (LIFO).
 */
void traducir_archivo(FILE *input_file, FILE *output_file) {
    char linea[128];
    char comando[20];
    int valor;
    int line_num = 0; // Contador para rastrear dónde ocurren los errores.

    // ¡Empezamos a leer y a compilar!
    while (fgets(linea, sizeof(linea), input_file) != NULL) {
        line_num++;

        // Limpieza: Eliminamos el '\n' y manejamos líneas vacías o en blanco.
        linea[strcspn(linea, "\n")] = 0;
        if (strlen(linea) == 0 || linea[0] == ';') continue; // Ignoramos líneas vacías o comentarios

        // --- Caso 1: Instrucción con Argumento (LOAD <valor>) ---
        // Intentamos leer un comando seguido de un número.
        if (sscanf(linea, "%s %d", comando, &valor) == 2) {
            
            if (strcmp(comando, "LOAD") == 0) {
                // LOAD: PUSH el valor 'valor' a la pila. (Stack: [valor, ...])
                fprintf(output_file, "\n\t; --- LINEA %d: LOAD %d ---\n", line_num, valor);
                fprintf(output_file, "\tmov eax, %d\n", valor); // Cargar el valor en EAX
                fprintf(output_file, "\tpush eax\n");           // Empujar EAX a la pila
            } else {
                // ¡Error de sintaxis! El usuario puso un argumento donde no iba.
                fprintf(stderr, "Error (Código %d): Línea %d: El comando '%s' no acepta un argumento numerico.\n", EXIT_ERROR_SYNTAX, line_num, comando);
                exit(EXIT_ERROR_SYNTAX);
            }
        } 
        
        // --- Caso 2: Instrucción sin Argumento (ADD, SUB, DISCARD, etc.) ---
        else if (sscanf(linea, "%s", comando) == 1) {
            
            fprintf(output_file, "\n\t; --- LINEA %d: %s ---\n", line_num, comando);

            // --- ADD: B + A ---
            if (strcmp(comando, "ADD") == 0) {
                // ADD requiere 4 pasos clave: Pop A, Pop B, Sumar (B+A), Push Resultado.
                fprintf(output_file, "\tpop ebx\n"); // ebx = A (el de la cima)
                fprintf(output_file, "\tpop eax\n"); // eax = B (el segundo)
                fprintf(output_file, "\tadd eax, ebx\n"); // B + A
                fprintf(output_file, "\tpush eax\n"); // Resultado a la pila
            } 
            
            // --- SUB: B - A ---
            else if (strcmp(comando, "SUB") == 0) {
                // SUB: El orden importa! B (el segundo) debe ser el minuendo.
                fprintf(output_file, "\tpop ebx\n"); // ebx = A (Sustraendo)
                fprintf(output_file, "\tpop eax\n"); // eax = B (Minuendo)
                fprintf(output_file, "\tsub eax, ebx\n"); // B - A
                fprintf(output_file, "\tpush eax\n");
            } 
            
            // --- MUL: B * A ---
            else if (strcmp(comando, "MUL") == 0) {
                // MUL (imul) usa EAX implícitamente, así que la operación es limpia.
                fprintf(output_file, "\tpop ebx\n"); // ebx = A
                fprintf(output_file, "\tpop eax\n"); // eax = B
                fprintf(output_file, "\timul ebx\n"); // eax = B * A
                fprintf(output_file, "\tpush eax\n");
            } 
            
            // --- DISCARD: Limpiar la basura de la cima ---
            else if (strcmp(comando, "DISCARD") == 0) {
                // Movemos el puntero de pila (ESP) para ignorar el valor. ¡Súper eficiente!
                fprintf(output_file, "\tadd esp, %d\n", DATA_SIZE); 
            }
            
            // --- DUPLICATE: Copiar el valor superior ---
            else if (strcmp(comando, "DUPLICATE") == 0) {
                // Necesitamos el valor, así que Pop a EAX, y luego Push dos veces.
                fprintf(output_file, "\tpop eax\n");
                fprintf(output_file, "\tpush eax\n");
                fprintf(output_file, "\tpush eax\n");
            }
            
            // --- SWAP: Intercambiar los dos de la cima ---
            else if (strcmp(comando, "SWAP") == 0) {
                // Pop A, Pop B. Push A, Push B. (¡El orden inverso hace el swap!)
                fprintf(output_file, "\tpop ebx\n"); // ebx = A
                fprintf(output_file, "\tpop eax\n"); // eax = B
                fprintf(output_file, "\tpush ebx\n"); // Push A (ahora es el segundo)
                fprintf(output_file, "\tpush eax\n"); // Push B (ahora es la cima)
            }
            
            // --- EXIT: El fin del camino ---
            else if (strcmp(comando, "EXIT") == 0) {
                fprintf(output_file, "\n\t; --- LINEA %d: EXIT (Llamada al Sistema) ---\n", line_num);
                fprintf(output_file, "\t; El valor de la cima se usa como código de salida (EBX).\n");
                fprintf(output_file, "\tpop ebx\n");   // Sacamos el resultado final.
                fprintf(output_file, "\tmov eax, 1\n");  // Syscall 1 (exit)
                fprintf(output_file, "\tint 0x80\n");  // ¡Adios!
                return; 
            }
            
            // --- ¡Comando Desconocido! ---
            else {
                fprintf(stderr, "Error (Código %d): Línea %d: ¡No reconozco el comando '%s'!\n", EXIT_ERROR_SYNTAX, line_num, comando);
                exit(EXIT_ERROR_SYNTAX);
            }
        } 
        
        // --- Error de Formato ---
        else {
             fprintf(stderr, "Error (Código %d): Línea %d: Formato de instrucción no reconocido. ¿Falta un argumento 'LOAD'?\n", EXIT_ERROR_SYNTAX, line_num);
             exit(EXIT_ERROR_SYNTAX);
        }
    }
}


/**
 * @brief Función principal: El punto de entrada del compilador.
 */
int main(int argc, char *argv[]) {
    
    // 1. Verificación de la firma: ¿La gente lo está usando bien?
    if (argc != 5 || strcmp(argv[1], "--compile") != 0 || strcmp(argv[3], "--asm") != 0) {
        fprintf(stderr, "Error (Código %d): Sintaxis de uso incorrecta.\n", EXIT_ERROR_USAGE);
        fprintf(stderr, "Uso: %s --compile <entrada.ss> --asm <salida.asm>\n", argv[0]);
        return EXIT_ERROR_USAGE;
    }

    char *input_filename = argv[2];
    char *output_filename = argv[4];

    // 2. Apertura de Archivos: ¡Necesitamos leer y escribir!
    FILE *input_file = fopen(input_filename, "r");
    if (input_file == NULL) {
        perror("Error (Código 2): No se pudo abrir el archivo de entrada");
        return EXIT_ERROR_FILE;
    }

    FILE *output_file = fopen(output_filename, "w");
    if (output_file == NULL) {
        perror("Error (Código 2): No se pudo crear el archivo de salida");
        fclose(input_file);
        return EXIT_ERROR_FILE;
    }

    // 3. Generación del Encabezado: El boilerplate necesario para NASM
    fprintf(output_file, "; Archivo generado por SimpleStack Compiler (NASM x86 Linux)\n");
    fprintf(output_file, "; ---------------------------------------------------------\n");
    fprintf(output_file, "section .text\n");
    fprintf(output_file, "global _start\n");
    fprintf(output_file, "_start:\n");
    
    // 4. ¡A compilar!
    traducir_archivo(input_file, output_file);

    // 5. Limpieza y Cierre
    fclose(input_file);
    fclose(output_file);
    
    printf("\nSimpleStack Compiler: '%s' -> '%s'\n", input_filename, output_filename);
    printf("¡Compilación de SimpleStack terminada con exito! (Código %d).\n", EXIT_SUCCESS);

    return EXIT_SUCCESS;
}
