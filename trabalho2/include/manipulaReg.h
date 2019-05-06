#ifndef MANIPULAREG_H
	
	#define MANIPULAREG_H

	#include "rdados.h"
	#include "escrevenatela.h"

	void busca_RemoveReg(char* nomeBin, char* nomeCampo, char* valorCampo);
	void removeUmRegistro(FILE* bin, REGDADOS* rd);
	void remove_id (FILE* bin, int tam_bin, int id, int *achei);
	void remove_salario (FILE* bin, int tam_bin, double sal, int *achei);
	void remove_telefone (FILE* bin, int tam_bin, char* tel, int *achei);
	void remove_nome (FILE* bin, int tam_bin, char* nome, int *achei);
	void remove_cargo (FILE* bin, int tam_bin, char* cargo, int *achei);
	void printaUmReg(REGDADOS* rd);
	void volta_le_Eprinta_umRegistro(FILE*bin, int tamReg);

#endif