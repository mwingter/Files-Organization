#ifndef ORGANIZAARQ_H

	#define ORGANIZAARQ_H

	#include "rdados.h"
	#include "escrevenatela.h"
	#include "manipulaReg.h"
	#include "merge_sort.h"

	//============================ FUNÇÕES AUXILIARES ===========================
	int int_compare_id(const void *A, const void *B);
	int recalcula_tam_registro(REGDADOS *rd);
	void avancaProximo(REGDADOS *rd1, REGDADOS *rd2, FILE *bin_in1, FILE *bin_in2, int *tam_pagina1, int *tam_pagina2);
	void check_file_status(FILE *fp);

	//============================= ORDENAÇÃO INTERNA ===========================
	void ordena_por_id(char* nomeBin_in, char* nomeBin_out);

	//========================== MERGING DE DOIS ARQUIVOS =======================
	void read_andMerge(char *nomeBin_in1, char *nomeBin_in2, char *nomeBin_out);

	//========================== MATCHING DE DOIS ARQUIVOS ======================	
	void read_andMatch(char *nomeBin_in1, char *nomeBin_in2, char *nomeBin_out);


#endif