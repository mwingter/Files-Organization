/*
	Trabalho realizado para a disciplina SCC0215 - Organização de Arquivos,
	ministrada pela professora Cristina Dutra de Aguiar Ciferri.
	Clara Rosa Silveira 	-  9021070
	Óliver Savastano Becker - 10284890
	Raphael Medeiros Vieira	- 10284239
						ICMC | USP | 1º semestre 2018
*/
#include <arvoreb.h>

#define POSICAO_NO_RAIZ 1
#define POSICAO_ALTURA POSICAO_NO_RAIZ + 4
#define POSICAO_ULTIMO_RRN POSICAO_ALTURA + 4

void CriaArvoreB() {
	FILE* fp = fopen(ARQUIVO_ARVORE, "wb+");
	char status = 0;	// Status para indicar a consistência do arquivo.
	int noRaiz = -1;	// RRN do nó raíz.
	int altura = -1;	// Altura máxima da árvore.
	int ultimoRRN = -1;	// Último RRN usado por um nó da árvore.

	fwrite(&status, sizeof(status), 1, fp);
	fwrite(&noRaiz, sizeof(noRaiz), 1, fp);
	fwrite(&altura, sizeof(altura), 1, fp);
	fwrite(&ultimoRRN, sizeof(ultimoRRN), 1, fp);

	fclose(fp);
}

void AlteraRRNdaRaiz(int RRN) {
	if (RRN < 0)
		return;

	FILE* fp = fopen(ARQUIVO_ARVORE, "rb+");
	if (fp == NULL)
		return;

	fseek(fp, POSICAO_NO_RAIZ, SEEK_SET);	// Vai para a posição do noRaiz no cabeçalho.

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

REGISTRO_ARVORE* CriaRegistroArvore() {
	//aloca espaço para a struct que salvará as informações do registro da árvore B
	//iniciando com -1 os ponteiros e a chave de busca
	REGISTRO_ARVORE* registro = (REGISTRO_ARVORE*)malloc(sizeof(REGISTRO_ARVORE));
	registro->quantidadeChaves = 0;
	memset(registro->ponteiroSubarvore, -1, sizeof(int) * (ORDEM_DA_ARVORE + 1));
	memset(registro->chaveBusca, -1, sizeof(int) * (ORDEM_DA_ARVORE));
	memset(registro->ponteiroDados, -1, sizeof(int) * (ORDEM_DA_ARVORE));

	return registro;
}

void InsereRegistroArvore(REGISTRO_ARVORE* registro, int RRN) {
	if (registro == NULL || RRN < 0) //verifica se o registro e o RRN são válidos
		return;

	FILE* fp = fopen(ARQUIVO_ARVORE, "rb+");
	if (fp == NULL) //verifica se o arquivo foi aberto corretamente
		return;

	//vai para a posição do RRN desejado
	fseek(fp, 	(RRN), SEEK_SET);

	//efetua a escrita dos dados do registro que estão salvos na struct
	fwrite(&registro->quantidadeChaves, sizeof(int), 1, fp);
	for(int i = 0; i < ORDEM_DA_ARVORE-1; i++){
		fwrite(&registro->ponteiroSubarvore[i], sizeof(int), 1, fp);
		fwrite(&registro->chaveBusca[i], sizeof(int), 1, fp);
		fwrite(&registro->ponteiroDados[i], sizeof(int), 1, fp);
	}
	fwrite(&registro->ponteiroSubarvore[ORDEM_DA_ARVORE-1], sizeof(int), 1, fp);
	
	fclose(fp);
}

REGISTRO_ARVORE* LeRegistroArvore(int RRN) {

	FILE *fp = fopen(ARQUIVO_ARVORE, "rb");
	if (fp == NULL) //verifica se o arquivo foi aberto corretamente
		return NULL;

	fseek(fp, BYTE_OFFSET_ARVORE(RRN), SEEK_SET); //vai para a posição do RRN desejado

	//aloca espaço para um novo registro na struct REGISTRO_ARVORE
	REGISTRO_ARVORE *reg = (REGISTRO_ARVORE*) malloc(sizeof(REGISTRO_ARVORE));

	
	//salva as informações do registro da arvoreB na struct
	//sendo elas a quantidade de chaves, ponteiros para o arquivo de indíce,
	//chaves de busca e ponteiros para o arquivo de dados 
	fread(&reg->quantidadeChaves, sizeof(int), 1, fp);

	for(int i = 0; i < ORDEM_DA_ARVORE-1; i++){

		fread(&reg->ponteiroSubarvore[i], sizeof(int), 1, fp);
		fread(&reg->chaveBusca[i], sizeof(int), 1, fp);
		fread(&reg->ponteiroDados[i], sizeof(int), 1, fp);
	}

	fread(&reg->ponteiroSubarvore[ORDEM_DA_ARVORE-1], sizeof(int), 1, fp);

	fclose(fp);
	
	return reg; //retorna o endereço da struct que acabou de ser criada
}

void AtualizaRegistroArvore(REGISTRO_ARVORE* registro, int RRNAtual) {

	// atualiza o registro no bufferpool

	InsereRegistroArvore(registro, RRNAtual);
}

// Função responsável por shiftar os índices de um nó para a direita, possibilitando a inserção de
// uma nova chave de busca menor que estas a serem deslocadas, de modo a manter a ordenação do nó.
void DeslocaChavesParaDireita(REGISTRO_ARVORE* registro, int n) {
	if (n < 0)
		return;

	for (int j = registro->quantidadeChaves; j > n; --j) {
		registro->ponteiroSubarvore[j+1] = registro->ponteiroSubarvore[j];
		registro->chaveBusca[j] = registro->chaveBusca[j-1];
		registro->ponteiroDados[j] = registro->ponteiroDados[j-1];
	}
}

// Função responsável por inserir uma nova chave de busca em um nó da árvore B. Não trata dos casos
// de overflow, e só deve ser chamada após conferir se o nó não está cheio.
int InsereChaveNoIndice(REGISTRO_ARVORE* registro, int subarvore, int chaveBusca, 
			int campoReferencia) {
	if (registro == NULL)
		return -1;

	int i = 0;

	// Procura o lugar em que a chave deve ser inserida, de modo a manter o registro ordenado.
	while (i < registro->quantidadeChaves && chaveBusca > registro->chaveBusca[i]) ++i;

	if (chaveBusca == registro->chaveBusca[i]) // Caso a chave a ser inserida já exista.
		return 0;

	DeslocaChavesParaDireita(registro, i);	// Abre espaço para inserir uma nova chave.

	registro->quantidadeChaves++;
	registro->ponteiroSubarvore[i+1] = subarvore;
	registro->chaveBusca[i] = chaveBusca;
	registro->ponteiroDados[i] = campoReferencia;

	return 1;
}

// Transfere 'n' índices de um registro para outro, possibilitando a execução de um split.
void MoveChavesDeRegistro(REGISTRO_ARVORE* origem, REGISTRO_ARVORE* destino, int n) {
	if (origem == NULL || destino == NULL)
		return;

	int i;
	int inicio = destino->quantidadeChaves;
	if ((inicio + n) > (ORDEM_DA_ARVORE - 1)) // Caso não caibam todas as chaves no destino
		return;

	for (i = n; i < origem->quantidadeChaves; ++i) {
		destino->ponteiroSubarvore[inicio + (i-n)] = origem->ponteiroSubarvore[i];
		destino->chaveBusca[inicio + (i-n)] = origem->chaveBusca[i];
		destino->ponteiroDados[inicio + (i-n)] = origem->ponteiroDados[i];

		origem->ponteiroSubarvore[i] = -1;
		origem->chaveBusca[i] = -1;
		origem->ponteiroDados[i] = -1;
	}
	destino->ponteiroSubarvore[inicio + (i-n)] = origem->ponteiroSubarvore[i];
	origem->ponteiroSubarvore[i] = -1;

	destino->quantidadeChaves += i-n;
	origem->quantidadeChaves -= i-n;
}

// Realiza o split 1 pra 2, criando um novo registro que guardará metade dos índices a sofrer split
// e retornando a chave que irá 'subir', utilizando argumentos por referência.
int Split1Pra2(REGISTRO_ARVORE* registro, int* chaveBusca, int* campoReferencia) {
	if (registro == NULL)
		return -1;

	int RRNnovoRegistro = UltimoRRN();	// Descobre qual foi o último RRN utilizado.

	// Confere se o nó atual é uma folha ou não ao conferir se o primeiro ponteiro pra subarvore
	// é -1. Caso seja nó folha, o ponteiro do novo nó será -1, senão será o do último RRN lido.
	InsereChaveNoIndice(registro, (registro->ponteiroSubarvore[0] == -1) ? -1 : RRNnovoRegistro,
				 *chaveBusca, *campoReferencia);

	// Cria o novo registro que armazenará metade das chaves do registro que sofreu o split.
	REGISTRO_ARVORE* novoRegistro = CriaRegistroArvore();

	int meio = ORDEM_DA_ARVORE / 2;

	// Coloca a metade da direita no registro novo.
	MoveChavesDeRegistro(registro, novoRegistro, meio + 1);

	// Atribui o valor da chave do meio para os argumentos, pois ela será promovida.
	*chaveBusca = registro->chaveBusca[meio];
	*campoReferencia = registro->ponteiroDados[meio];

	// Limpa o conteúdo de memória da posição do meio do antigo registro (chave foi promovida).
	registro->chaveBusca[meio] = -1;
	registro->ponteiroDados[meio] = -1;
	registro->quantidadeChaves--;

	AlteraUltimoRRN(++RRNnovoRegistro);	// Escreve o RRN do novo registro no cabeçalho.

	AtualizaRegistroArvore(novoRegistro, RRNnovoRegistro);

	free(novoRegistro);
	return RRNnovoRegistro;
}

// Função para tratar o caso de overflow no nó raíz, realizando um split e alterando o nó raíz do
// registro de cabeçalho.
int SplitNoRaiz(REGISTRO_ARVORE* registro, int chaveBusca, int campoReferencia) {
	if (registro == NULL)
		return -1;

	int RRNnovoRegistro = Split1Pra2(registro, &chaveBusca, &campoReferencia);

	REGISTRO_ARVORE* novaRaiz = CriaRegistroArvore();

	novaRaiz->quantidadeChaves++;
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
int BuscaOndeInserir(REGISTRO_ARVORE* registro, int* chaveBusca, int* campoReferencia,
			int RRNAtual, int alturaAtual) {
	if (registro == NULL)
		return -1;

	int retornoFuncao = 0;

	if (alturaAtual > 0) { // Caso ainda não tenha chego em um nó folha.
		int i = 0;
		while (i < registro->quantidadeChaves && *chaveBusca > registro->chaveBusca[i]) i++;

		int RRNFilho = registro->ponteiroSubarvore[i];

		REGISTRO_ARVORE* filho = LeRegistroArvore(RRNFilho);
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
	if (registro->quantidadeChaves < ORDEM_DA_ARVORE-1) {
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

	REGISTRO_ARVORE* registro = NULL;
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
		registro->quantidadeChaves = 1;
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

//função de busca recursiva
int BuscaArvoreB(REGISTRO_ARVORE *reg, int chave){

	int i = 0;

	//procura pela posição com a chave de busca imediatamente menor que a procurada
	//ou até a ultima chave de busca disponível 
	while(i < reg->quantidadeChaves && chave > reg->chaveBusca[i])
		i++;

	//verifica se já encontrou a chave
	if(chave == reg->chaveBusca[i]) {
		int RRN = reg->ponteiroDados[i];
		free(reg);
		return RRN;
	}

	//se for nó folha e não encontrou a chave a ser buscada é porque ela não existe
	if(reg->ponteiroSubarvore[0] == -1)
		return -1;

	//vai para o filho da posição atual e continua pela busca
	int filho = reg->ponteiroSubarvore[i];
	free(reg);
	return BuscaArvoreB(LeRegistroArvore(filho), chave);
}

//imprime as informações do registro da árvore B tais como a quantidade de chaves,
//os ponteiros da subarvore, as chaves de busca e os ponteiros do arquivo de dados
void ImprimeRegistroArvore(REGISTRO_ARVORE* reg) {
	printf("n=%d  ", reg->quantidadeChaves);

	int i = 0;
	for(i = 0; i < reg->quantidadeChaves; i++){
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

	REGISTRO_ARVORE* reg;
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