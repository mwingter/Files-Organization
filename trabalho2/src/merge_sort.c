#include <stdlib.h>
#include <string.h>



/*
* == Merge Sort Genérico ==
*
* Matheus Carvalho Raimundo
* mcarvalhor@usp.br
*
*/



/*
* = MS_sort() =
* Essa função ordena um conjunto de itens sequencialmente alocados na memória RAM (vetor).
* Para usar, é necessário passar como parâmetros o endereço do primeiro elemento do vetor (ponteiro),
*   o número de itens no vetor (n), o tamanho em bytes de cada item (sizeof) e uma função de comparação dos itens.
* Tal chamada é semelhante à função "qsort" do C Padrão. Mas este é um algoritmo de ordenação estável e rápido.
*
* A função de comparação fcmp(const void *A, const void *B) deve retornar um valor menor que 0, igual a 0 ou maior que 0
*   respectivamente caso A>B, A=B e A<B.
* Por exemplo: fcmp(A, B) deve retornar algo do tipo -1 se A>B, 0 se A=B e +1 se A<B.
*
* Exemplo de uso da função para comparar inteiros:
*  #include "merge_sort.h"
*  int int_compare(const void *A, const void *B) { // 1, 2, 2, 3, 4
*      int *pA, *pB;
*      pA = (int *) A;
*      pB = (int *) B;
*      return *pB - *pA;
*  }
*  int int_compare_reverse(const void *A, const void *B) { // 4, 3, 2, 2, 1
*      int *pA, *pB;
*      pA = (int *) A;
*      pB = (int *) B;
*      return *pA - *pB;
*  }
*  int main(int argv, char *argv[]) {
*      int vector[] = { 9, 2, 5, 0, -4, 100, 1000, 826, 46, 826, 7, 62 }; // 12 itens
*      MS_sort(vector, 12, sizeof(int), int_compare);
*      // Agora "vector" está estavelmente ordenado.
*      MS_sort(vector, 12, sizeof(int), int_compare_reverse);
*      // Agora "vector" está estavelmente ordenado ao contrário.
*      return 0;
*  }
*
* vector: ponteiro para o primeiro elemento do vetor;
* n: número de elementos no vetor;
* memsize: tamanho em bytes de cada elemento (sizeof(elemento));
* fcmp: função de comparação do elemento.
* Complexidade de tempo: O(nlogn)    Complexidade de espaço: O(n)
* Thread-safe: não
*/

void MS_sort(void *vector, unsigned long n, size_t memsize, int (*fcmp)(const void *, const void *)) {
	unsigned long middle, rN, j, k;
	void *aux, *r;

	if(n < 2) {
		return;
	}

	middle = (n / 2);
	r = vector + middle * memsize;
	rN = n - middle;
	MS_sort(vector, middle, memsize, fcmp);
	MS_sort(r, rN, memsize, fcmp);

	aux = (void *) malloc(memsize * n);
	j = k = 0;
	while(j + k < n) {
		if(k >= rN || (j < middle && fcmp(vector + memsize * j, r + memsize * k) >= 0)) {
			memcpy(aux + memsize * (j + k), vector + memsize * j, memsize);
			j++;
		} else {
			memcpy(aux + memsize * (j + k), r + memsize * k, memsize);
			k++;
		}
	}
	memcpy(vector, aux, memsize * n);
	free(aux);
}


