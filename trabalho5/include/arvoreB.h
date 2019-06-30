#ifndef ARVOREB_H
	
	#define ARVOREB_H

	#define CABECALHO_ARVORE 5
	#define TAMANHO_PAGINA 105
	#define BYTE_OFFSET_ARVORE(RRN) (TAMANHO_PAGINA * (RRN)) + TAMANHO_PAGINA
	#define ORDEM_DA_ARVORE 7 				// Ordem 'm' = 7
	#define ARQUIVO_ARVORE "indice.bin"

	//====== struct do nó da arvore-B =======
	typedef struct no_arvoreB_ REG_ARVOREB;
	struct no_arvoreB_ {
		char eFolha; //indica se o nó é um nó folha ou não, representado por uma string de 1 byte 
						//(0 indica que o nó não é folha e 1 indica que o nó é folha)
		int n;				// 'n' - indica o número de chaves do nó, representado por um inteiro de 4 bytes.
		int ponteiroSubarvore[ORDEM_DA_ARVORE+1];	// 'P'  Foi adicionada uma posição a mais em
		int chaveBusca[ORDEM_DA_ARVORE];		// 'C'  cada vetor apenas para a implementa-
		long int ponteiroDados[ORDEM_DA_ARVORE];		// 'Pr' ção do split, mas estas nunca serão
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

	void InsereReg_naArvore(FILE* bin_indice, REG_ARVOREB* registro);

	REG_ARVOREB* CriaRegistroArvore();

	int InsereChaveNoIndiceArvore(REG_ARVOREB* registro, int subarvore, int chaveBusca, int campoReferencia);

	void AlteraRRNdaRaiz(int RRN);

	int RRNdaRaiz();

	void AlteraAlturaDaArvore(int altura);

	int AlturaDaArvore();

	void AlteraUltimoRRN(int RRN);

	int UltimoRRN();

	REG_ARVOREB* CriaRegistroArvore();

	void InsereRegistroArvore(REG_ARVOREB* registro, int RRN);


	void AtualizaRegistroArvore(REG_ARVOREB* registro, int RRNAtual);

	void DeslocaChavesParaDireita(REG_ARVOREB* registro, int n);

	int InsereChaveNoIndice(REG_ARVOREB* registro, int subarvore, int chaveBusca, int campoReferencia);

	void MoveChavesDeRegistro(REG_ARVOREB* origem, REG_ARVOREB* destino, int n);

	int Split1Pra2(REG_ARVOREB* registro, int* chaveBusca, int* campoReferencia);

	int SplitNoRaiz(REG_ARVOREB* registro, int chaveBusca, int campoReferencia);

	int BuscaOndeInserir(REG_ARVOREB* registro, int* chaveBusca, int* campoReferencia, int RRNAtual, int alturaAtual);

	int InsereIndice(int chaveBusca, int campoReferencia);
		

	void ImprimeRegistroArvore(REG_ARVOREB* reg);

	void ImprimeArquivoArvoreB();


#endif