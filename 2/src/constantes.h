#define error_parametros -1
#define error_modo -2
#define error_asignacion -3
#define error_archivo -4
#define error_fork -5
#define error_send -6
#define error_receive -7
#define error_malloc -8
#define error_cola -9
#define error_creacion_memoria -10
#define error_pipe -11
#define error_leer_pipe -12
#define error_escribir_pipe -13
#define error_close_pipe -14
#define error_vinculacion_memoria -15
#define error_attach -16
#define error_detach -17
#define error_direcciones -18

#define ref 16
#define val 17
#define capacidad_tarro 10
#define cantidad_abejas 3
#define EA 1
#define AC 2
#define CD 3
#define DE 4
#define EB 5
#define BC 6
#define AB 7
#define EABC 8
#define ABCABC 9
#define ABCD 10

/*
 * conversión de numero_pag de binario a decimal
 */
void binario_decimal(int * numero) {
	int  bin = *numero, base = 1, resto;
	*numero = 0;
    while (bin > 0) {
        resto = bin % 10;
        *numero += resto * base;
        bin = bin / 10 ;
        base = base * 2;
    }
}

// calcula x elevado a la n
int pot(int x, int n) {
    int i, numero = 1;
    for (i = 0; i < n; i++) numero *= x;
    return numero;
}

/*
 * conversión de decimal a binario de la variable "decimal". el resultado, en binario, es guardado en la   * variable "binario".
 */
void decimal_binario(int decimal, char binario[9]) {
	int i = 7;
	if ( decimal != 0 ) {
		while (decimal != 1) {
			binario[i] = decimal % 2 + '0';
			decimal /= 2;
			i--;
		}
		if ( decimal == 1 ) binario[i--] = '1';
	}
	while ( i > -1 ) binario[i--] = '0';
}