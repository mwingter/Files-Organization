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
	int bo = (TAM_PAGINA_ARVB * (RRN)) + TAM_PAGINA_ARVB;
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

	fseek(bin_indice, byte_offset_arvore(RRN), SEEK_SET); //vai para a posição do RRN desejado

	//aloca espaço para um novo registro na struct REG_ARVOREB
	REG_ARVOREB *reg = (REG_ARVOREB*) malloc(sizeof(REG_ARVOREB));

	
	//salva as informações do registro da arvoreB na struct
	//sendo elas a quantidade de chaves, ponteiros para o arquivo de indíce,
	//chaves de busca e ponteiros para o arquivo de dados 
	fread(&reg->eFolha, sizeof(char), 1, bin_indice);
	//printf("Folha = %c\n", reg->eFolha);
	fread(&reg->n, sizeof(int), 1, bin_indice);
	//printf("n = %d\n", reg->n);

	for(int i = 0; i < ORDEM_ARVORE-1; i++){

		fread(&reg->ponteiroSubarvore[i], sizeof(int), 1, bin_indice);
		//printf("subarvore = %d\n", reg->ponteiroSubarvore[i]);
		fread(&reg->chaveBusca[i], sizeof(int), 1, bin_indice);
		//printf("chave = %d\n", reg->chaveBusca[i]);
		fread(&reg->ponteiroDados[i], sizeof(long int), 1, bin_indice);
		//printf("ponteiroDados = %ld\n", reg->ponteiroDados[i]);
	}

	fread(&reg->ponteiroSubarvore[ORDEM_ARVORE-1], sizeof(int), 1, bin_indice);
	//printf("ponteiroSubarvore = %d\n", reg->ponteiroSubarvore[ORDEM_ARVORE-1]);
	
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

	int restoPagina = TAM_PAGINA_ARVB - TAM_CAB_ARVORE;
	char arroba = '@';
	// Preenchendo com @ o resto da pagina
	for(int i = 0; i < restoPagina; i++){ 
		fwrite(&arroba, sizeof(char), 1, bin_indice);
	}
}

/*
 * CriaRegistroArvore
 * Função: Aloca espaço para a struct que salvará as informações do registro da árvore B, iniciando os ponteiros e a chave de busca com -1.
 * Retorno: O registro de árvore B alocado.
 			
*/
REG_ARVOREB* CriaRegistroArvore() {
	REG_ARVOREB* reg = (REG_ARVOREB*)malloc(sizeof(REG_ARVOREB));
	reg->eFolha = 1; //inicia o registro marcado como nó folha
	reg->n = 0;
	memset(reg->ponteiroSubarvore, -1, sizeof(int) * (ORDEM_ARVORE + 1));
	memset(reg->chaveBusca, -1, sizeof(int) * (ORDEM_ARVORE));
	memset(reg->ponteiroDados, -1, sizeof(int) * (ORDEM_ARVORE));

	return reg;
}

/*
 * InsereReg_naArvore
 * Função: Escreve um registro de arvore B em um arquivo de arvore B.
 * Retorno: O registro de árvore B alocado.
 			
*/
void InsereReg_naArvore(FILE* bin_indice, REG_ARVOREB* reg) {
	if (reg == NULL){ //verifica se o reg e o RRN são válidos
		return;
	}
	//escrevendo os dados do registro que estão salvos na struct
	fwrite(&reg->eFolha, sizeof(char), 1, bin_indice);
	fwrite(&reg->n, sizeof(int), 1, bin_indice);
	for(int i = 0; i < ORDEM_ARVORE-1; i++){
		fwrite(&reg->ponteiroSubarvore[i], sizeof(int), 1, bin_indice);
		fwrite(&reg->chaveBusca[i], sizeof(int), 1, bin_indice);
		fwrite(&reg->ponteiroDados[i], sizeof(int), 1, bin_indice);
	}
	fwrite(&reg->ponteiroSubarvore[ORDEM_ARVORE-1], sizeof(int), 1, bin_indice);
}