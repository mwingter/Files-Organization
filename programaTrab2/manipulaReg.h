#ifndef MANIPULAREG_H
	
	#define MANIPULAREG_H

	#include "rdados.h"
	#include "escrevenatela.h"

	void busca_RemoveReg(FILE* bin, char* nomeCampo, char* valorCampo, REGCAB* rc);
	void removeUmRegistro(FILE* bin, REGDADOS* rd);
	void remove_id (FILE* bin, int tam_bin, int id, REGCAB* rc, int *achei);

#endif