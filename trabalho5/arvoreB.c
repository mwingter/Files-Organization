/*
	TRABALHO 5 - ORGANIZAÇÃO DE ARQUIVOS

	Nome:	Michelle Wingter da Silva	nUSP:	10783243
			Juliano Fantozzi					9791218

*/


#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "arvoreB.h"
#include "rdados.h"
#include "manipulaReg.h"
#include "organizaArq.h"
#include "indice.h"

#define POSICAO_NO_RAIZ 1
#define POSICAO_ALTURA POSICAO_NO_RAIZ + 4
#define POSICAO_ULTIMO_RRN POSICAO_ALTURA + 4


//============================= [15] CRIAR ARVORE B ===============================
/*
 * cria_arvoreB
 * Função: Lê um arquivo de dados e constrói um arquivo de indice arvore B.
 * Parâmetros: 	
 				nomeBin_in: Nome do arquivo de dados.
 				nomeBin_indice: Nome do arquivo de indice de arvore B.
*/
void cria_arvoreB(char* nomeBin_in, char* nomeBin_indice){
	FILE* bin_in = fopen(nomeBin_in, "rb");
	check_file_status(bin_in);

	FILE* bin_indice = fopen(nomeBin_indice, "wb+");
	//check_file_status(bin_indice);


	//ler os arquivo de dados e salvar
	REGCAB *rc = calloc(1, sizeof(REGCAB));
	REGDADOS *rd = calloc(1, sizeof(REGDADOS));
	/*REG_ARVOREB* reg_arvore = CriaRegistroArvore();
	int n_reg = 0;
	int n_paginas = 1;
	int pos = 0;
	int tam_pagina = 0;
	int subarvore = -1;
	*/
	rewind(bin_in);
	//int tam_bin_in = tamArquivo(bin_in);
	leCabecalho(bin_in, rc);
	//escrevendo o cabeçalho no arquivo de indice de arvore B
	nova_arvoreB(bin_indice); 

	//printf("tamanho do arq %d\n", tam_bin_in);
/*
	while(ftell(bin_in) < tam_bin_in){
		//printf("Entrei no loop do arq dados. Chamando limpa_registro_dados\n");
		limpa_registro_dados(rd);
		pos = ftell(bin_in);
		//rd->nomeServidor[0] = '\0';
		//printf("Chamando leUmRegistroBin\n");
		leUmRegistroBin(bin_in, rd, &tam_pagina);
		if(rd->removido == '-'){
			n_reg++;
			reg_arvore[n_paginas-1].n++;
			reg_arvore[n_paginas-1].eFolha = 1;
			//printf("Chamando InsereChaveNoIndiceArvore\n");
			InsereChaveNoIndiceArvore(reg_arvore, subarvore, rd->idServidor, pos);
			if(n_reg == 7){
				n_paginas++;
				n_reg = 0;
				//reg_arvore = realloc(reg_arvore, n_paginas*sizeof(REG_ARVOREB));
			}
			//reg_arvore[n_reg-1] = CriaRegistroArvore();
			//reg_arvore[n_reg-1].chaveBusca = rd->idServidor;
			//reg_arvore[n_reg-1].ponteiroDados = pos;
		}
	}
*/

/*
	//passando os registros de indice de arvoreB para o arquivo de arvoreB
	//for (int i = 0; i < n_paginas; ++i)
	//{
		InsereReg_naArvore(bin_indice, reg_arvore);
	//}

*/

	//atualizando status do arquivo de indice arvore B
	rewind(bin_indice);
	char status = '1';	// Status para indicar a consistência do arquivo.

	fwrite(&status, sizeof(status), 1, bin_indice);

	fclose(bin_in); fclose(bin_indice);
	free(rc); free(rd);


}


//======================= [16] RECUPERAR DADOS NA ARVORE B =========================
/*
 * busca_e_recupera_arvoreB
 * Função: Realiza a busca de um registro de determinado valor de id no arquivo de arvore B.
 * Parâmetros: 	
 				nomeBin_indice: Nome do arquivo de indice de arvore B.
 				valor: Numero do id que se deseja buscar.
 				nivel: Ponteiro para guardar a quantidade de níveis acessados na árvore durante a busca.

 * Retorno:		Posição do registro no arquivo de dados.

*/
long int busca_e_recupera_arvoreB(char* nomeBin_indice, int valor, int* nivel){
	FILE* bin_indice = fopen(nomeBin_indice, "rb");
	check_file_status(bin_indice);

	rewind(bin_indice);

	//carregando o indice para a memoria
	char status;
	int noRaiz;
	//faz a leitura do status e noRaiz
	fread(&status, sizeof(char), 1, bin_indice);
	if (status == '0'){
		printf("Falha no processamento do arquivo.\n");
		exit(0);
	}

	fread(&noRaiz, sizeof(int), 1, bin_indice);

	fseek(bin_indice, byte_offset_arvore(noRaiz), SEEK_SET); //indo pro nó raiz

	long int ind = -1;
	ind = BuscaArvoreB(bin_indice, LeRegistroArvore(bin_indice, noRaiz), valor, nivel); //fazendo busca na arvore pra achar o id

	rewind(bin_indice);
	check_file_status(bin_indice);
	

	fclose(bin_indice);
	return ind;
}

//======================== FUNÇÕES AUXILIARES ========================
int byte_offset_arvore(int RRN){
	int bo = (TAMANHO_PAGINA * (RRN)) + TAMANHO_PAGINA;
	return bo;
}

/*
 * LeRegistroArvore
 * Função: Lê um registro de indice arvore B no arquivo de indice arvore B.
 * Parâmetros: 	
 				bin_indice: Arquivo de indice de arvore B.
 				RRN: valor do RRN do registro na árvore B (ou seja, posição do registro que desejo ler).

 * Retorno:		Registro lido na árvore B.
*/
REG_ARVOREB* LeRegistroArvore(FILE *bin_indice, int RRN) {

	fseek(bin_indice, BYTE_OFFSET_ARVORE(RRN), SEEK_SET); //vai para a posição do RRN desejado

	//aloca espaço para um novo registro na struct REG_ARVOREB
	REG_ARVOREB *reg = (REG_ARVOREB*) malloc(sizeof(REG_ARVOREB));

	
	//salva as informações do registro da arvoreB na struct
	//sendo elas a quantidade de chaves, ponteiros para o arquivo de indíce,
	//chaves de busca e ponteiros para o arquivo de dados 
	fread(&reg->eFolha, sizeof(char), 1, bin_indice);
	//printf("Folha = %c\n", reg->eFolha);
	fread(&reg->n, sizeof(int), 1, bin_indice);
	//printf("n = %d\n", reg->n);

	for(int i = 0; i < ORDEM_DA_ARVORE-1; i++){

		fread(&reg->ponteiroSubarvore[i], sizeof(int), 1, bin_indice);
		//printf("subarvore = %d\n", reg->ponteiroSubarvore[i]);
		fread(&reg->chaveBusca[i], sizeof(int), 1, bin_indice);
		//printf("chave = %d\n", reg->chaveBusca[i]);
		fread(&reg->ponteiroDados[i], sizeof(long int), 1, bin_indice);
		//printf("ponteiroDados = %ld\n", reg->ponteiroDados[i]);
	}

	fread(&reg->ponteiroSubarvore[ORDEM_DA_ARVORE-1], sizeof(int), 1, bin_indice);
	//printf("ponteiroSubarvore = %d\n", reg->ponteiroSubarvore[ORDEM_DA_ARVORE-1]);
	
	return reg; //retorna o endereço da struct que acabou de ser criada
}

/*
 * BuscaArvoreB
 * Função: Busca um registro de determinada chave (id) em um arquivo de indice em um arquivo de indice árvore B.
 * Parâmetros: 	
 				bin_indice: Arquivo de indice de arvore B.
 				reg: Nó de registro de arvore B onde será feita a busca.
 				chave: Chave (id) que se deseja buscar.
 				nivel: Ponteiro que guarda o numero de niveis acessados durante a busca.

 * Retorno:		Posição do registro no arquivo de dados.

*/
long int BuscaArvoreB(FILE* bin_indice, REG_ARVOREB *reg, int chave, int* nivel){
	(*nivel)++;
	int i = 0;

	//procura pela posição com a chave de busca imediatamente menor que a procurada
	//ou até a ultima chave de busca disponível 
	while(i < reg->n && chave > reg->chaveBusca[i])
		i++;

	//verifica se já encontrou a chave
	if(chave == reg->chaveBusca[i]) {
		int pos = reg->ponteiroDados[i];
		free(reg);
		return pos;
	}

	//se for nó folha e não encontrou a chave a ser buscada é porque ela não existe
	if(reg->ponteiroSubarvore[0] == -1)
		return -1;

	//vai para o filho da posição atual e continua pela busca
	int filho = reg->ponteiroSubarvore[i];
	free(reg);
	return BuscaArvoreB(bin_indice, LeRegistroArvore(bin_indice, filho), chave, nivel);
}

/*
 * nova_arvoreB
 * Função: Recebe um arquivo de indice de arvore B e escreve a pagina de cabeçalho, com status 0 e no Raiz -1.
 * Parâmetros: 	
 				bin_indice: Arquivo de indice de arvore B.
*/
void nova_arvoreB(FILE* bin_indice) {
	char status = '0';	// Status que indica a consistência do arquivo.
	int noRaiz = -1;	// RRN do nó raíz.

	fwrite(&status, sizeof(status), 1, bin_indice);
	fwrite(&noRaiz, sizeof(noRaiz), 1, bin_indice); 

	int restoPagina = TAMANHO_PAGINA - CABECALHO_ARVORE;
	char arroba = '@';
	// Preenchendo com @ o resto da pagina
	for(int i = 0; i < restoPagina; i++){ 
		fwrite(&arroba, sizeof(char), 1, bin_indice);
	}
}


/*void InsereReg_naArvore(FILE* bin_indice, REG_ARVOREB* registro) {
	if (registro == NULL) //verifica se o registro e o RRN são válidos
		return;

	//efetua a escrita dos dados do registro que estão salvos na struct
	fwrite(&registro->n, sizeof(int), 1, bin_indice);
	for(int i = 0; i < ORDEM_DA_ARVORE-1; i++){
		fwrite(&registro->ponteiroSubarvore[i], sizeof(int), 1, bin_indice);
		fwrite(&registro->chaveBusca[i], sizeof(int), 1, bin_indice);
		fwrite(&registro->ponteiroDados[i], sizeof(int), 1, bin_indice);
	}
	fwrite(&registro->ponteiroSubarvore[ORDEM_DA_ARVORE-1], sizeof(int), 1, bin_indice);
}

REG_ARVOREB* CriaRegistroArvore() {
	//aloca espaço para a struct que salvará as informações do registro da árvore B
	//iniciando com -1 os ponteiros e a chave de busca
	REG_ARVOREB* registro = (REG_ARVOREB*)malloc(sizeof(REG_ARVOREB));
	registro->n = 0;
	memset(registro->ponteiroSubarvore, -1, sizeof(int) * (ORDEM_DA_ARVORE + 1));
	memset(registro->chaveBusca, -1, sizeof(int) * (ORDEM_DA_ARVORE));
	memset(registro->ponteiroDados, -1, sizeof(int) * (ORDEM_DA_ARVORE));

	return registro;
}



// Função responsável por inserir uma nova chave de busca em um nó da árvore B. Não trata dos casos
// de overflow, e só deve ser chamada após conferir se o nó não está cheio.
int InsereChaveNoIndiceArvore(REG_ARVOREB* registro, int subarvore, int chaveBusca, int campoReferencia) {
	if (registro == NULL)
		return -1;

	int i = 0;

	// Procura o lugar em que a chave deve ser inserida, de modo a manter o registro ordenado.
	while (i < registro->n && chaveBusca > registro->chaveBusca[i]) ++i;

	if (chaveBusca == registro->chaveBusca[i]) // Caso a chave a ser inserida já exista.
		return 0;

	DeslocaChavesParaDireita(registro, i);	// Abre espaço para inserir uma nova chave.

	registro->n++;
	registro->ponteiroSubarvore[i+1] = subarvore;
	registro->chaveBusca[i] = chaveBusca;
	registro->ponteiroDados[i] = campoReferencia;

	return 1;
}

void AlteraRRNdaRaiz(int RRN) {
	if (RRN < 0)
		return;

	FILE* fp = fopen(ARQUIVO_ARVORE, "rb+");
	if (fp == NULL)
		return;

	fseek(fp, 1, SEEK_SET);	// Vai para a posição do noRaiz no cabeçalho.

	fwrite(&RRN, sizeof(RRN), 1, fp);
	fclose(fp);
}

int RRNdaRaiz() {
	FILE* fp = fopen(ARQUIVO_ARVORE, "rb");
	if (fp == NULL)
		return -2;

	fseek(fp, POSICAO_NO_RAIZ, SEEK_SET);	// Vai para a posição do noRaiz no cabeçalho.

	int RRN;
	fread(&RRN, sizeof(RRN), 1, fp);
	fclose(fp);

	return RRN; //retorna o RRN da raiz
}

void AlteraAlturaDaArvore(int altura) {
	if (altura < 0)
		return;

	FILE* fp = fopen(ARQUIVO_ARVORE, "rb+");
	if (fp == NULL)
		return;

	fseek(fp, POSICAO_ALTURA, SEEK_SET);	// Vai para a posição da altura no cabeçalho.

	fwrite(&altura, sizeof(altura), 1, fp); // Atualiza a altura da árvore
	fclose(fp);
}

int AlturaDaArvore() {
	FILE* fp = fopen(ARQUIVO_ARVORE, "rb");
	if (fp == NULL)
		return -2;

	fseek(fp, POSICAO_ALTURA, SEEK_SET);	// Vai para a posição da altura no cabeçalho.

	int altura;
	fread(&altura, sizeof(altura), 1, fp);
	fclose(fp);

	return altura; //retorna a altura da árvore
}

void AlteraUltimoRRN(int RRN) {
	if (RRN < 0)
		return;

	FILE* fp = fopen(ARQUIVO_ARVORE, "rb+");
	if (fp == NULL)
		return;

	fseek(fp, POSICAO_ULTIMO_RRN, SEEK_SET);// Vai para a posição do ultimoRRN no cabeçalho.

	fwrite(&RRN, sizeof(RRN), 1, fp); //altera o ultimo RRN
	fclose(fp);
}

int UltimoRRN() {
	FILE* fp = fopen(ARQUIVO_ARVORE, "rb");
	if (fp == NULL)
		return -2;

	fseek(fp, POSICAO_ULTIMO_RRN, SEEK_SET);// Vai para a posição do ultimoRRN no cabeçalho.

	int RRN;
	fread(&RRN, sizeof(RRN), 1, fp);
	fclose(fp);

	return RRN; //retorna o ultimo RRN
}



void InsereRegistroArvore(REG_ARVOREB* registro, int RRN) {
	if (registro == NULL || RRN < 0) //verifica se o registro e o RRN são válidos
		return;

	FILE* fp = fopen(ARQUIVO_ARVORE, "rb+");
	if (fp == NULL) //verifica se o arquivo foi aberto corretamente
		return;

	//vai para a posição do RRN desejado
	fseek(fp, BYTE_OFFSET_ARVORE(RRN), SEEK_SET);

	//efetua a escrita dos dados do registro que estão salvos na struct
	fwrite(&registro->n, sizeof(int), 1, fp);
	for(int i = 0; i < ORDEM_DA_ARVORE-1; i++){
		fwrite(&registro->ponteiroSubarvore[i], sizeof(int), 1, fp);
		fwrite(&registro->chaveBusca[i], sizeof(int), 1, fp);
		fwrite(&registro->ponteiroDados[i], sizeof(int), 1, fp);
	}
	fwrite(&registro->ponteiroSubarvore[ORDEM_DA_ARVORE-1], sizeof(int), 1, fp);
	
	fclose(fp);
}
*/


/*
void AtualizaRegistroArvore(REG_ARVOREB* registro, int RRNAtual) {

	// atualiza o registro no bufferpool

	InsereRegistroArvore(registro, RRNAtual);
}

// Função responsável por shiftar os índices de um nó para a direita, possibilitando a inserção de
// uma nova chave de busca menor que estas a serem deslocadas, de modo a manter a ordenação do nó.
void DeslocaChavesParaDireita(REG_ARVOREB* registro, int n) {
	if (n < 0)
		return;

	for (int j = registro->n; j > n; --j) {
		registro->ponteiroSubarvore[j+1] = registro->ponteiroSubarvore[j];
		registro->chaveBusca[j] = registro->chaveBusca[j-1];
		registro->ponteiroDados[j] = registro->ponteiroDados[j-1];
	}
}

// Função responsável por inserir uma nova chave de busca em um nó da árvore B. Não trata dos casos
// de overflow, e só deve ser chamada após conferir se o nó não está cheio.
int InsereChaveNoIndice(REG_ARVOREB* registro, int subarvore, int chaveBusca, int campoReferencia) {
	if (registro == NULL)
		return -1;

	int i = 0;

	// Procura o lugar em que a chave deve ser inserida, de modo a manter o registro ordenado.
	while (i < registro->n && chaveBusca > registro->chaveBusca[i]) ++i;

	if (chaveBusca == registro->chaveBusca[i]) // Caso a chave a ser inserida já exista.
		return 0;

	DeslocaChavesParaDireita(registro, i);	// Abre espaço para inserir uma nova chave.

	registro->n++;
	registro->ponteiroSubarvore[i+1] = subarvore;
	registro->chaveBusca[i] = chaveBusca;
	registro->ponteiroDados[i] = campoReferencia;

	return 1;
}

// Transfere 'n' índices de um registro para outro, possibilitando a execução de um split.
void MoveChavesDeRegistro(REG_ARVOREB* origem, REG_ARVOREB* destino, int n) {
	if (origem == NULL || destino == NULL)
		return;

	int i;
	int inicio = destino->n;
	if ((inicio + n) > (ORDEM_DA_ARVORE - 1)) // Caso não caibam todas as chaves no destino
		return;

	for (i = n; i < origem->n; ++i) {
		destino->ponteiroSubarvore[inicio + (i-n)] = origem->ponteiroSubarvore[i];
		destino->chaveBusca[inicio + (i-n)] = origem->chaveBusca[i];
		destino->ponteiroDados[inicio + (i-n)] = origem->ponteiroDados[i];

		origem->ponteiroSubarvore[i] = -1;
		origem->chaveBusca[i] = -1;
		origem->ponteiroDados[i] = -1;
	}
	destino->ponteiroSubarvore[inicio + (i-n)] = origem->ponteiroSubarvore[i];
	origem->ponteiroSubarvore[i] = -1;

	destino->n += i-n;
	origem->n -= i-n;
}

// Realiza o split 1 pra 2, criando um novo registro que guardará metade dos índices a sofrer split
// e retornando a chave que irá 'subir', utilizando argumentos por referência.
int Split1Pra2(REG_ARVOREB* registro, int* chaveBusca, int* campoReferencia) {
	if (registro == NULL)
		return -1;

	int RRNnovoRegistro = UltimoRRN();	// Descobre qual foi o último RRN utilizado.

	// Confere se o nó atual é uma folha ou não ao conferir se o primeiro ponteiro pra subarvore
	// é -1. Caso seja nó folha, o ponteiro do novo nó será -1, senão será o do último RRN lido.
	InsereChaveNoIndice(registro, (registro->ponteiroSubarvore[0] == -1) ? -1 : RRNnovoRegistro,
				 *chaveBusca, *campoReferencia);

	// Cria o novo registro que armazenará metade das chaves do registro que sofreu o split.
	REG_ARVOREB* novoRegistro = CriaRegistroArvore();

	int meio = ORDEM_DA_ARVORE / 2;

	// Coloca a metade da direita no registro novo.
	MoveChavesDeRegistro(registro, novoRegistro, meio + 1);

	// Atribui o valor da chave do meio para os argumentos, pois ela será promovida.
	*chaveBusca = registro->chaveBusca[meio];
	*campoReferencia = registro->ponteiroDados[meio];

	// Limpa o conteúdo de memória da posição do meio do antigo registro (chave foi promovida).
	registro->chaveBusca[meio] = -1;
	registro->ponteiroDados[meio] = -1;
	registro->n--;

	AlteraUltimoRRN(++RRNnovoRegistro);	// Escreve o RRN do novo registro no cabeçalho.

	AtualizaRegistroArvore(novoRegistro, RRNnovoRegistro);

	free(novoRegistro);
	return RRNnovoRegistro;
}

// Função para tratar o caso de overflow no nó raíz, realizando um split e alterando o nó raíz do
// registro de cabeçalho.
int SplitNoRaiz(REG_ARVOREB* registro, int chaveBusca, int campoReferencia) {
	if (registro == NULL)
		return -1;

	int RRNnovoRegistro = Split1Pra2(registro, &chaveBusca, &campoReferencia);

	REG_ARVOREB* novaRaiz = CriaRegistroArvore();

	novaRaiz->n++;
	novaRaiz->ponteiroSubarvore[0] = RRNdaRaiz();
	novaRaiz->chaveBusca[0] = chaveBusca;
	novaRaiz->ponteiroDados[0] = campoReferencia;
	novaRaiz->ponteiroSubarvore[1] = RRNnovoRegistro;

	int RRNraiz = RRNnovoRegistro + 1;

	AlteraRRNdaRaiz(RRNraiz);
	AlteraUltimoRRN(RRNraiz);
	AlteraAlturaDaArvore(AlturaDaArvore()+1);

	AtualizaRegistroArvore(novaRaiz, RRNraiz);
	free(novaRaiz);

	return 1;
}

// Função recursiva para encontrar o nó folha que a chave deve ser inserida.
int BuscaOndeInserir(REG_ARVOREB* registro, int* chaveBusca, int* campoReferencia,
			int RRNAtual, int alturaAtual) {
	if (registro == NULL)
		return -1;

	int retornoFuncao = 0;

	if (alturaAtual > 0) { // Caso ainda não tenha chego em um nó folha.
		int i = 0;
		while (i < registro->n && *chaveBusca > registro->chaveBusca[i]) i++;

		int RRNFilho = registro->ponteiroSubarvore[i];

		REG_ARVOREB* filho = LeRegistroArvore(RRNFilho);
		retornoFuncao = BuscaOndeInserir(filho, chaveBusca, campoReferencia, RRNFilho,
							alturaAtual-1);

		if (retornoFuncao != 1) { // Caso não tenha acontecido um split, encerra a função.
			free(registro);
			return 0;
		}

		// Se a função retornou 1, é porque houve split e deve-se inserir uma chave no 
		// registro atual.	
	}

	//if (alturaAtual == 0) {	// Quando chegar a um nó folha, pode inserir.
		// Caso ainda tenha espaço para mais chaves no nó atual.
	if (registro->n < ORDEM_DA_ARVORE-1) {
		int ponteiro = alturaAtual ? UltimoRRN() : -1;

		retornoFuncao = InsereChaveNoIndice(registro, ponteiro, *chaveBusca,
				*campoReferencia);

		AtualizaRegistroArvore(registro, RRNAtual);

		free(registro);
		return 0;
	}

	// Senão, faz um split.
	if (RRNAtual == RRNdaRaiz()) {	// Caso haja um overflow na raíz.
		retornoFuncao = SplitNoRaiz(registro, *chaveBusca, *campoReferencia);

		AtualizaRegistroArvore(registro, RRNAtual);

		free(registro);
		return 0;
	}

	// Se o overflow não foi na raiz, então é o caso geral de split.
	retornoFuncao = Split1Pra2(registro, chaveBusca, campoReferencia);
	AtualizaRegistroArvore(registro, RRNAtual);

	free(registro);

	return 1;
}

int InsereIndice(int chaveBusca, int campoReferencia) {

	REG_ARVOREB* registro = NULL;
	int raiz = RRNdaRaiz();

	if (raiz == -2) {	// Caso a função retorne erro.
		return -1;
	}
	if (raiz < 0) {		// Caso ainda não haja nenhum registro de índice no arquivo.
		registro = CriaRegistroArvore();

		// Muda o cabeçalho, já que será inserido o primeiro registro de índice.
		AlteraRRNdaRaiz(0);
		AlteraUltimoRRN(0);
		AlteraAlturaDaArvore(0);

		// Insere o índice no registro (como é um nó folha, não terá ponteiros de filho).
		registro->n = 1;
		registro->chaveBusca[0] = chaveBusca;
		registro->ponteiroDados[0] = campoReferencia;

		// Por fim, escreve o novo registro no arquivo de índice.
		InsereRegistroArvore(registro, 0);
		
		free(registro);
		return 0;	// Finaliza a inserção, sem erros.
	}

	registro = LeRegistroArvore(raiz);
	BuscaOndeInserir(registro, &chaveBusca, &campoReferencia, raiz, AlturaDaArvore());

	return 0;
}
*/


/*
//imprime as informações do registro da árvore B tais como a quantidade de chaves,
//os ponteiros da subarvore, as chaves de busca e os ponteiros do arquivo de dados
void ImprimeRegistroArvore(REG_ARVOREB* reg) {
	printf("n=%d  ", reg->n);

	int i = 0;
	for(i = 0; i < reg->n; i++){
		printf("|%d| ", reg->ponteiroSubarvore[i]);
		printf("|%d|", reg->chaveBusca[i]);
		printf("%d| ", reg->ponteiroDados[i]);
	}

	printf("|%d|\n", reg->ponteiroSubarvore[i]);
}

void ImprimeArquivoArvoreB() {

	printf("\n==========Arquivo árvore B.==========\n");
	FILE* fp = fopen(ARQUIVO_ARVORE, "rb");
	if (fp == NULL) { //verifica se o arquivo da árvore B existe
		printf("ERRO! Arquivo de árvore B não existe.\n");
		return;
	}

	char status;
	int noRaiz, altura, ultimoRRN;

	//faz a leitura do status, noRaiz, altura e o ultimoRRN para efetuar a impressão
	fread(&status, sizeof(char), 1, fp);
	fread(&noRaiz, sizeof(int), 1, fp);
	fread(&altura, sizeof(int), 1, fp);
	fread(&ultimoRRN, sizeof(int), 1, fp);

	printf("Registro de cabeçalho:\tstatus: %d, noRaiz: %d, altura: %d, ultimoRRN: %d\n", 
		status, noRaiz, altura, ultimoRRN);

	fclose(fp);

	REG_ARVOREB* reg;
	int i = 0;
	//repete a impressão enquanto não for o último RRN
	while (i <= ultimoRRN) {
		printf("Registro %d: ", i);
		//cria uma struct temporária com a informações do registro da árvore B
		//para efetua a impressão
		reg = LeRegistroArvore(i);
		ImprimeRegistroArvore(reg);
		free(reg); //libera o espaço da struct temporária
		i++;
	}

	printf("\n");
}
*/