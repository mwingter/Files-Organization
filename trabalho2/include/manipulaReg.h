#ifndef MANIPULAREG_H
	
	#define MANIPULAREG_H

	#include "rdados.h"
	#include "escrevenatela.h"

	void busca_RemoveReg(char* nomeBin, char* nomeCampo, char* valorCampo);
	void removeUmRegistro(FILE* bin, REGDADOS* rd);
	void remove_id (FILE* bin, int tam_bin, int id, int *achei);

#endif