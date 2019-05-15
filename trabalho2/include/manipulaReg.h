#ifndef MANIPULAREG_H
	
	#define MANIPULAREG_H

	#include "rdados.h"
	#include "escrevenatela.h"

	//nó de um registro removido
	typedef struct reg_removido_ REG_REM;
	struct reg_removido_{		
		long int pos;
		int tam;
		long int encadeamentoLista;
	};

	void novo_no_removido(REG_REM* reg_vazio, long int pos, int tam, long int proxRem);
	REG_REM* lista_removidos(FILE* bin, REG_REM* lista, int* numero_registros_removidos);
	int int_compare(const void *A, const void *B);
	void busca_RemoveReg(char* nomeBin, char* nomeCampo, char* valorCampo);
	void removeUmRegistro(FILE* bin, REGDADOS* rd);
	void atualiza_topoLista(char* nomeBin, long int topo);
	void atualiza_encadLista(char* nomeBin, REG_REM* lista, int n_rem);
	void remove_id (FILE* bin, int tam_bin, int id, int *achei);
	void remove_salario (FILE* bin, int tam_bin, double sal, int *achei);
	void remove_telefone (FILE* bin, int tam_bin, char* tel, int *achei);
	void remove_nome (FILE* bin, int tam_bin, char* nome, int *achei);
	void remove_cargo (FILE* bin, int tam_bin, char* cargo, int *achei);
	void printaUmReg(REGDADOS* rd);
	void volta_le_Eprinta_umRegistro(FILE*bin, int tamReg);
	void bestFit_insere(char* nomeBin, REGDADOS* rd);
	void insere_umReg_naPos(FILE* bin, REGDADOS* rd, long int pos, int tam_reg_noBin);
	int tamanho_BestFit(int* tamanhos_ordenado, int tam_reg, int n_reg);
	long int achaBestFit(FILE* bin, int tam_reg, int tam_bin, int*tam_reg_noBin);

#endif