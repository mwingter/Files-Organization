#ifndef ARVOREB_H
	
	#define ARVOREB_H

	#define TAM_CAB_ARVORE 5
	#define TAM_PAGINA_ARVB 105
	#define ORDEM_ARVORE 7 		// 'm' = 7

	//====== struct do nó da arvore-B =======
	typedef struct no_arvoreB_ REG_ARVOREB;
	struct no_arvoreB_ {
		char eFolha; //indica se o nó é um nó folha ou não, representado por uma string de 1 byte 
						//(0 indica que o nó não é folha e 1 indica que o nó é folha)
		int n;				// 'n' - indica o número de chaves do nó, representado por um inteiro de 4 bytes.
		int ponteiroSubarvore[ORDEM_ARVORE+1];	// 'P'  Foi adicionada uma posição a mais em
		int chaveBusca[ORDEM_ARVORE];		// 'C'  cada vetor apenas para a implementa-
		long int ponteiroDados[ORDEM_ARVORE];		// 'Pr' ção do split, mas estas nunca serão
	};											//	mostradas no arquivo de índices.


	//====================== [15] CRIAR ARVORE B =========================
	void cria_arvoreB(char* nomeBin_in, char* nomeBin_indice);

	//================= [16] RECUPERAR DADOS NA ARVORE B =================
	long int busca_e_recupera_arvoreB(char* nomeBin_indice, int valor, int* nivel);
	
	//======================== FUNÇÕES AUXILIARES ========================
	int byte_offset_arvore(int RRN);

	REG_ARVOREB* LeRegistroArvore(FILE* bin_indice, int RRN);

	long int BuscaArvoreB(FILE* bin_indice, REG_ARVOREB *reg, int chave, int* nivel);

	void nova_arvoreB(FILE* bin_indice); 

	REG_ARVOREB* CriaRegistroArvore();
	
	void InsereReg_naArvore(FILE* bin_indice, REG_ARVOREB* registro);

	
#endif