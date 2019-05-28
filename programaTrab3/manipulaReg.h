/*
	TRABALHO 3 - ORGANIZAÇÃO DE ARQUIVOS

	Nome:	Michelle Wingter da Silva	nUSP:	10783243
			Juliano Fantozzi					xxxxxxxx

*/

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
		int tempo_descoberta;
	};

	//========================== REMOÇÃO ======================
	void novo_no_removido(REG_REM* reg_vazio, long int pos, int tam, long int proxRem);
	REG_REM* lista_removidos(FILE* bin, REG_REM* lista, int* numero_registros_removidos);
	int int_compare_tam(const void *A, const void *B);
	int int_compare_pos_reverse(const void *A, const void *B);

	void busca_RemoveReg(char* nomeBin, char* nomeCampo, char* valorCampo);
	void ordenaLista_atualizaEncAndTopoLista(char* nomeBin, REG_REM* lista, int n_rem);
	void atualiza_topoLista(char* nomeBin, long int topo);
	void atualiza_encadLista(char* nomeBin, REG_REM* lista, int n_rem);
	void remove_id (FILE* bin, int tam_bin, int id, int *achei);
	void remove_salario (FILE* bin, int tam_bin, double sal, int *achei);
	void remove_telefone (FILE* bin, int tam_bin, char* tel, int *achei);
	void remove_nome (FILE* bin, int tam_bin, char* nome, int *achei);
	void remove_cargo (FILE* bin, int tam_bin, char* cargo, int *achei);
	void remove_registro_generico(FILE *bin, int tamanhoRegistro);
	void volta_le_Eprinta_umRegistro(FILE*bin, int tamReg);
	void printaUmReg(REGDADOS* rd);	
	//void removeUmRegistro(FILE* bin, REGDADOS* rd);
	
	//========================== INSERÇÃO ====================
	void firstFit_insere(char* nomeBin, REGDADOS* rd, long int* ultimo_reg);
	void achaFirstFit_eInsereReg(FILE* bin, REGDADOS* rd, long int* ultimo_reg);
	void atualizaTopoEncadLista_aposInserir(FILE* bin, long int encadLista_anterior, long int encadLista_atual, int indice_lista);
	void insere_umReg_naPos(FILE* bin, REGDADOS* rd, long int pos/*, int tam_reg_noBin*/);
	void insere_umReg_noFim(FILE* bin, REGDADOS* rd, long int* ultimo_reg);

	//========================= ATUALIZAÇÃO ===================
	void busca_atualiza_campo(char* nomeBin, char nomeCampo[MAX], char valor[MAX], char nomeCampoAtualiza[MAX], char valorCampoAtualiza[MAX], long int* ultimo_reg);
	void atualiza_UmRegistro(REGDADOS* rd, char nomeCampoAtualiza[MAX], char valorCampoAtualiza[MAX]);
	void atualiza_id (FILE* bin, int tam_bin, int id, int *achei, char nomeCampoAtualiza[MAX], char valorCampoAtualiza[MAX], long int* ultimo_reg);
	void atualiza_salario (FILE* bin, int tam_bin, double sal, int *achei, char nomeCampoAtualiza[MAX], char valorCampoAtualiza[MAX], long int* ultimo_reg);
	void atualiza_telefone (FILE* bin, int tam_bin, char* tel, int *achei, char nomeCampoAtualiza[MAX], char valorCampoAtualiza[MAX], long int* ultimo_reg);
	void atualiza_nome (FILE* bin, int tam_bin, char* nome, int *achei, char nomeCampoAtualiza[MAX], char valorCampoAtualiza[MAX], long int* ultimo_reg);
	void atualiza_cargo (FILE* bin, int tam_bin, char* cargo, int *achei, char nomeCampoAtualiza[MAX], char valorCampoAtualiza[MAX], long int* ultimo_reg);

#endif