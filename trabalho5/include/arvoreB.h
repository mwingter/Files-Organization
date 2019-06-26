#ifndef ARVOREB_H
	
	#define ARVOREB_H

	#define CABECALHO_ARVORE 5
	#define TAMANHO_PAGINA 105
	#define BYTE_OFFSET_ARVORE(RRN) (TAMANHO_PAGINA * (RRN)) + CABECALHO_ARVORE
	#define ORDEM_DA_ARVORE 7 				// Ordem 'm' = 10.
	#define ARQUIVO_ARVORE "indice.bin"

	//====== struct do nó da arvore-B =======
	/*typedef struct no_arvoreB_ arvoreB;;
	struct no_arvoreB_ {
		int quantidadeChaves;						// 'n'
		int ponteiroSubarvore[ORDEM_DA_ARVORE+1];	// 'P'  
		int chaveBusca[ORDEM_DA_ARVORE];			// 'C'  
		int ponteiroDados[ORDEM_DA_ARVORE];			// 'Pr' 
	};

	typedef struct {
		int quantidadeChaves;				// 'n'
		int ponteiroSubarvore[ORDEM_DA_ARVORE+1];	// 'P'  Foi adicionada uma posição a mais em
		int chaveBusca[ORDEM_DA_ARVORE];		// 'C'  cada vetor apenas para a implementa-
		int ponteiroDados[ORDEM_DA_ARVORE];		// 'Pr' ção do split, mas estas nunca serão
	} REGISTRO_ARVORE;					//	mostradas no arquivo de índices.



	//=========================================

	void nova_arvoreB();

	void AlteraRRNdaRaiz(int RRN);

	int RRNdaRaiz();

	void AlteraAlturaDaArvore(int altura);

	int AlturaDaArvore();

	void AlteraUltimoRRN(int RRN);

	int UltimoRRN();

	REGISTRO_ARVORE* CriaRegistroArvore();

	void InsereRegistroArvore(REGISTRO_ARVORE* registro, int RRN);

	REGISTRO_ARVORE* LeRegistroArvore(int RRN);

	void AtualizaRegistroArvore(REGISTRO_ARVORE* registro, int RRNAtual);

	void DeslocaChavesParaDireita(REGISTRO_ARVORE* registro, int n);

	int InsereChaveNoIndice(REGISTRO_ARVORE* registro, int subarvore, int chaveBusca, int campoReferencia);

	void MoveChavesDeRegistro(REGISTRO_ARVORE* origem, REGISTRO_ARVORE* destino, int n);

	int Split1Pra2(REGISTRO_ARVORE* registro, int* chaveBusca, int* campoReferencia);

	int SplitNoRaiz(REGISTRO_ARVORE* registro, int chaveBusca, int campoReferencia);

	int BuscaOndeInserir(REGISTRO_ARVORE* registro, int* chaveBusca, int* campoReferencia, int RRNAtual, int alturaAtual);

	int InsereIndice(int chaveBusca, int campoReferencia);
		
	int BuscaArvoreB(REGISTRO_ARVORE *reg, int chave);

	void ImprimeRegistroArvore(REGISTRO_ARVORE* reg);

	void ImprimeArquivoArvoreB();


#endif