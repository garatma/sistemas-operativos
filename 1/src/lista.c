#include "lista.h"
#include <stdlib.h>

TLista crear_lista() {
    TLista l = (TLista) malloc(sizeof(struct lista));
    l->cantidad_elementos = 0;
    l->primer_celda = POS_NULA;
    return l;
}

int l_insertar(TLista lista, TPosicion pos, int elem) {
    int resultado = TRUE;
    if ( lista == NULL ) resultado = LST_NO_INI;
    else {
        TCelda nueva = (TCelda) malloc(sizeof(struct celda));
        if ( nueva == NULL ) resultado = FALSE;
        else {
            nueva->elemento = elem;
            if ( pos == POS_NULA || pos == l_primera(lista) ) {
				//	Hay que insertar en la primera posici�n.
                nueva->proxima_celda = lista->primer_celda;
                lista->primer_celda = nueva;
            }
            else {
				//	Hay que obtener el anterior a pos e insertarlo como su siguiente.
                TPosicion anterior = l_anterior(lista,pos);
                anterior->proxima_celda = nueva;
                nueva->proxima_celda = pos;
            }
            lista->cantidad_elementos++;
        }
    }
    return resultado;
}

int l_eliminar(TLista lista, TPosicion pos){
    int resultado = FALSE;
    if ( lista == NULL ) resultado = LST_NO_INI;
    else {
        if ( pos == POS_NULA ) resultado = LST_POS_INV;
        else {
            // Se elimina la posicion.
            resultado = TRUE;
            TPosicion ant = l_anterior(lista,pos);
            TPosicion sig = l_siguiente(lista,pos);
            if ( ant != POS_NULA ) {
                if ( sig != POS_NULA )
                    // pos no es ni la primera ni la �ltima.
                    ant->proxima_celda = sig;
                else
                    // pos es la �ltima.
                    ant->proxima_celda = POS_NULA;
            }
            else
                // pos es la primera.
                lista->primer_celda = sig;
			//	Eliminaci�n de pos y su liberaci�n de memoria.
            resultado = TRUE;
            pos->proxima_celda = POS_NULA;
            pos->elemento = 0;
            free(pos);
            lista->cantidad_elementos--;
        }
    }
    return resultado;
}

TPosicion l_primera(TLista lista) {
    TPosicion primera;
    if ( lista == NULL ) primera = (TPosicion) LST_NO_INI;
    else {
        if ( lista->cantidad_elementos == 0 ) primera = POS_NULA;
        else primera = lista->primer_celda;
    }
    return primera;
}

TPosicion l_ultima(TLista lista) {
    TPosicion ultima;
    if ( lista == NULL ) ultima = (TPosicion) LST_NO_INI;
    else {
        if ( (*lista).cantidad_elementos == 0 ) ultima = POS_NULA;
        else {
			//	Hay que recorrer la lista y obtener el �ltimo.
            ultima = l_primera(lista);
            while ( l_siguiente(lista,ultima) != POS_NULA )
                ultima = l_siguiente(lista,ultima);
        }
    }
    return ultima;
}

TPosicion l_anterior(TLista lista, TPosicion pos) {
    TPosicion movil, sig;
    if ( lista == NULL ) movil = (TPosicion) LST_NO_INI;
    else {
		//	Hay que recorrer la lista y obtener el anterior a pos.
        movil = l_primera(lista);
        sig = l_siguiente(lista,movil);
        if ( movil != pos && pos != POS_NULA )
            while ( sig != pos ) {
                movil = sig;
                sig = l_siguiente(lista,movil);
                }
        else movil = POS_NULA;
    }
    return movil;
}

TPosicion l_siguiente(TLista lista, TPosicion pos) {
    TCelda movil;
    if ( lista == NULL ) movil = (TCelda) LST_NO_INI;
    else {
        if ( pos == POS_NULA || pos->proxima_celda == POS_NULA ) movil = POS_NULA;
        else movil = pos->proxima_celda;
    }
    return movil;
}

int l_recuperar(TLista lista, TPosicion pos){
    int e = 0;
    if ( lista == NULL ) e = LST_NO_INI;
    else
        if (pos != POS_NULA)
            e = pos->elemento;
    return e;
}

int l_size(TLista lista){
    int s = ( lista == NULL ) ? LST_NO_INI : lista->cantidad_elementos;
    return s;

}
