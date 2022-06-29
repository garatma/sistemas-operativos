#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "constantes.h"

// el 9° bit de la TP es el de válido.
// el 17° bit del TLB es el de referenciada y el 18° el de válido.
char TP[256][9], TLB[16][18], frames_libres[256];
int peor_caso_TLB = 0;
FILE * arch_dir;

/*
 * si modo es 'r', busca en la TLB con numero_pag para obtener el frame asociado y
 * guardado en numero_frame. si el bit de válido asociado es 1, y la página es la
 * que se está buscando entonces retorna 1, sino 0.
 * si modo es 'w', busca en la TLB con numero_pag para luego escribir el valor de 
 * numero_frame en la TLB. para este modo, el valor retornado no debe considerarse
 */
int entrada_TLB(char numero_pag[9],char numero_frame[9], char modo){
	int i=0, j, hit = 0;
	if ( modo == 'r' ) {
		while(!hit && i<16) {
			j = 0;
			hit = TLB[i][val] == '1';
			while ( hit && j < 8 ) {
				// seguir comparando mientras los bits en i sean iguales
				hit = numero_pag[j] == TLB[i][j];
				j++;
			}
			if ( !hit ) i++;
		}
		if ( hit ) {
			TLB[i][ref] = '1';  //como se encontró la página, se marca como referenciada
			for(j = 8; j < 16; j++) numero_frame[j-8] = TLB[i][j];
		}
	}
	else if ( modo == 'w' ) {
		i = 0;
		int encontre_entrada = 0;
		while ( i < ref && !encontre_entrada ) {
			if ( TLB[i][val] == '0' ) { // buscar entre las no-válidas
				encontre_entrada = 1;
				TLB[i][val] = '1';  //se valida la página.
				for ( j = 0; j < 8; j++ ) {
					TLB[i][j+8] = numero_frame[j];
					TLB[i][j] = numero_pag[j];
				}
			}
			i++;
		}
		if ( !encontre_entrada ) { // buscar entre las válidas, pero no referenciadas
			i = 0;
			while ( i < ref && !encontre_entrada ) {
				if ( TLB[i][ref] == '0' ) {
					encontre_entrada = 1;
					for ( j = 0; j < 8; j++ ) {
						TLB[i][j+8] = numero_frame[j];
						TLB[i][j] = numero_pag[j];
					}
				}
				i++;
			}
		}
		if ( !encontre_entrada ) { // si todas fueron válidas o referenciadas
			for ( i = 0; i < 8; i++ ) {
				TLB[peor_caso_TLB][i+8] = numero_frame[i];
				TLB[peor_caso_TLB][i] = numero_pag[i];
			}
			TLB[peor_caso_TLB][ref] = '0';  // el bit de referenciado vuelve a 0
			peor_caso_TLB++;   // para que no se reemplace siempre la misma página.
			if ( peor_caso_TLB == 16 ) peor_caso_TLB = 0;  // para evitar segmentation fault.
		}
	}
	return hit;
}

/*
 * si modo es 'r', indexa en la TP con numero_pag para obtener el frame asociado y
 * guardado en numero_frame. si el bit de válido asociado es 1, retorna 1, sino 0.
 * si modo es 'w', indexa en la TP con numero_pag para luego escribir el valor de 
 * numero_frame en la TP. para este modo, el valor retornado no debe considerarse
 */
int entrada_TP(char numero_pag[9], char numero_frame[9], char modo) {
	int i, indice_TP = 0, hit, j = 0;
	for ( i = 7; i > -1; i--) indice_TP += (numero_pag[i]-'0')*pot(10,j++);
	binario_decimal(&indice_TP);
	if ( modo == 'r' ) {
		if ( TP[indice_TP][8] == '1' ) { //bit de válido en 1
			hit = 1; // hit de TP
			for ( i = 0; i < 8; i++ ) numero_frame[i] = TP[indice_TP][i];
		}
		else hit = 0; // miss de TP
	}
	else if ( modo == 'w' ) { // colocar el numero_frame en numero_pag
		TP[indice_TP][8] = '1';
		for ( i = 0; i < 8; i++ ) TP[indice_TP][i] = numero_frame[i];
	}
	return hit;
}

/* Se busca un frame que este libre y se lo guarda en numero_frame. 
 * Para ello, se mantiene una lista con todos los frames que estén libres. 
 * Un frame libre se marca con un 0, mientras que uno ocupado se marca con un 1.
*/
void reservar_frame(char numero_frame[9]) {
	int i = 0, frame = -1;
	while ( i < 256 && frame == -1) {
		if ( frames_libres[i] == '0' ) {
			frame = i;
			frames_libres[i] = '1'; 
		}
		i++;
	}
	decimal_binario(frame,numero_frame);
}

/* a partir de direccion_logica, obtiene el numero de frame asociado, que se almacena
 * en numero_frame, accediendo al TLB o a la TP, como sea necesario.
 */
void traducir(char direccion_logica[17], char numero_frame[9]) {
	int i, hit;
	char numero_pag[8];
	for (i=0; i<8; i++) numero_pag[i]=direccion_logica[i];
	hit = entrada_TLB(numero_pag,numero_frame,'r');
	if ( hit ) printf("Hit en el TLB.\n");
	if ( !hit ) { //miss TLB
		printf("Miss en el TLB.\n");
		hit = entrada_TP(numero_pag,numero_frame,'r');
		if ( hit ) printf("Hit en la TP.\n");
		if ( !hit ) { //miss TP
			printf("Miss en la TP.\n");
			reservar_frame(numero_frame);
			entrada_TP(numero_pag,numero_frame,'w');
            printf("TP actualizada.\n");
		}
		entrada_TLB(numero_pag,numero_frame,'w');
		printf("TLB actualizado.\n");
	}
}

// setea los valores de los parámetros en '0'
void inicializar_tablas() {
	int i, j;
	for ( i = 0; i < 256; i++) {
		frames_libres[i] = '0';
		for ( j = 0; j < 9; j++ ) TP[i][j] = '0';
	}
	for ( i = 0; i < 16; i++)
        for ( j = 0; j < 18; j++ ) TLB[i][j] = '0';
}

int main() {
	char offset[9], numero_frame[9], direccion_logica[17]; // 17 y 9 por el NULL al final de los string
	inicializar_tablas();
	arch_dir=fopen("memoria.txt","r");
	int i, j;
	if (arch_dir == NULL){
		printf("Error al abrir el archivo memoria.txt.\n");
		exit(error_archivo);
	}
	while ( fscanf(arch_dir, "%s", direccion_logica) != EOF ) {
		for ( i = 0; i < 16; i++ ) {
			if ( direccion_logica[i] != '0' && direccion_logica[i] != '1' ) {
				printf("Las direcciones ingresadas no son correctas.\n");
				exit(error_direcciones);
			}
		}
        if ( strlen(direccion_logica) != 16 ) {
            printf("Las direcciones ingresadas no son correctas.\n");
            exit(error_direcciones);
        }
        for( i = 0; i < 8; i++ ) offset[i] = direccion_logica[i+8];
        printf("Traducción de la dirección lógica %s:\n",direccion_logica);
		traducir(direccion_logica,numero_frame);
		printf("La dirección física es %s %s\n\n",numero_frame,offset);
	}
	fclose(arch_dir);
	printf("TLB (página marco)\n");
	for ( i = 0; i < 16; i++ ) {
        for ( j = 0; j < 8; j++ ) printf("%c", TLB[i][j]); // imprimir número página
        printf(" ");
        for ( j = 8; j < 16; j++ ) printf("%c", TLB[i][j]); // imprimir número de frame
        printf("\n");
	}
	
	printf("\n");
	printf("TP (marco)\n");
	for ( i = 0; i < 256; i++ ) {
		if ( TP[i][8] == '1' )  {
			for ( j = 0; j < 8; j++ ) printf("%c", TP[i][j]); // imprimir numero de frame
            printf("\n");
        }
	}
	return 0;
}
