/*
	TRABALHO 3 - ORGANIZAÇÃO DE ARQUIVOS

	Nome:	Michelle Wingter da Silva	nUSP:	10783243
			Juliano Fantozzi					xxxxxxxx

*/

#ifndef ORGANIZAARQ_H
	#define ORGANIZAARQ_H

	#include "rdados.h"
	#include "escrevenatela.h"
	#include "manipulaReg.h"
	#include "merge_sort.h"

//============================= ORDENAÇÃO INTERNA ===========================
	void ordena_por_id(char* nomeBin_in, char* nomeBin_out);
	int int_compare_id(const void *A, const void *B);

//========================== MERGING DE DOIS ARQUIVOS ======================
	void read_andMerge(char *nomeBin_in1, char *nomeBin_in2, char *nomeBin_out);

//========================== MATCHING DE DOIS ARQUIVOS ======================
	void read_andMatch(char *nomeBin_in1, char *nomeBin_in2, char *nomeBin_out);


#endif