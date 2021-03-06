#include "registro.h"
#include "telas.h"

//Tela inicial onde é perguntado se operador deseja ou não inserir produtos ao "banco de dados".
void inicio(){
	system("clear");
	
	//Verifica se é possível abrir o arquivo para leitura, se não for possível é dada a opção ao operador de inserir ou não os produtos.
	
	if((db = fopen("db.txt", "r"))==NULL){
		printf("ERRO: Impossível abrir arquivo.\n");
		exit(1);
	}
	
	fseek(db, 0, SEEK_END);
	
	if((ftell(db) - 1) <= 0){
		printf("----------------------------------------------------------------------------------------------\n");
		printf("\n		SISTEMA DE GERENCIAMENTO DE COMPRAS, VENDAS E ESTOQUE\n\n");
		printf("----------------------------------------------------------------------------------------------\n");
		printf("\n\n\n");
		printf("Nenhum produto inserido no sistema, deseja inserir produtos agora? (S/N)");
		printf("\n\n\n");
		while(op != 's' && op != 'n'){
			scanf("%c", &op);
		}
		if(op == 's'){
			addProduto();
		}else if(op == 'n'){
			telaInicial();
		}
	}else{
		telaInicial();
	}
}



void addProduto(){
	int add = 0;
	
	op = 's';
	
	/*
		Salvando produtos
		
	*/
	
	//Verifica se o arquivo pode ser aberto. Se não, imprime mensagem de erro e fecha o programa.
	if((db = fopen("db.txt", "a")) == NULL){
		printf("ERRO: Impossível abrir arquivo.");
		exit(1);
	}
	
	while((add < 100) && (op == 's')){
		system("clear");
		printf("	INSIRA OS DADOS DO PRODUTO\n\n\n");
		printf("Categoria: ");
		scanf(" %[^\n]", tProdutos[add].categoria);
		printf("Nome: ");
		scanf(" %[^\n]", tProdutos[add].nome);
		printf("Preço: ");
		scanf("%lf", &tProdutos[add].preco);
		printf("Descrição: ");
		scanf(" %[^\n]", tProdutos[add].descricao);
		printf("Fabricante: ");
		scanf(" %[^\n]", tProdutos[add].fabricante);
		printf("Data de validade: [dd mm aaaa] ");
		scanf("%d %d %d", &tProdutos[add].val.dia, &tProdutos[add].val.mes, &tProdutos[add].val.ano);
		printf("Localização: [cc pp]");
		scanf("%d %d", &tProdutos[add].loc.corredor, &tProdutos[add].loc.prateleira);
		printf("Quantidade em estoque: ");
		scanf("%d", &tProdutos[add].estoque);
		srand(time(NULL));
		tProdutos[add].codigo = 1001 + ( rand() % 4000 );
		
		fwrite(&tProdutos[add], sizeof(Produto), 1, db);
		add++;
		op = valida();
		//qntd++;
	}
		fclose(db);
		
		if(op == 'n'){
			telaFuncionario();
		}
		
}

char valida(){
	char op;
	while (op != 's' && op != 'n'){
		printf(" \n Inserir novo produto? [S ou N] ? " );
		scanf("%c", &op);
	}
	return op;
}

void listar(){
	int indice = 0, leitura;
	int opcao;
	
	system("clear");
	
	//printf("CODIGO    |NOME    |PREÇO    |CATEGORIA    |DESCRIÇÃO    |FABRICANTE    |VALIDADE    |    LOCALIZAÇÃO    |ESTOQUE\n\n");
	
	if((db = fopen("db.txt", "r"))==NULL){
		printf("ERRO: Impossível abrir arquivo.");
		exit(1);
	}
	
	leitura = fread(&tProdutos[indice], sizeof(Produto), 1, db);
	
	while(leitura == 1){
		printf("Código : %d | Nome: %s | Descrição: %s | Preço: R$%.2lf | Descrição: %s | Fabricante: %s | Validade: %d/%d/%d Localização: Corredor %d - Pratelira %d | Qnt estoque: %d\n\n", tProdutos[indice].codigo, tProdutos[indice].nome, tProdutos[indice].categoria, tProdutos[indice].preco, tProdutos[indice].descricao, tProdutos[indice].fabricante, tProdutos[indice].val.dia, tProdutos[indice].val.mes, tProdutos[indice].val.ano, tProdutos[indice].loc.corredor, tProdutos[indice].loc.prateleira, tProdutos[indice].estoque);
		indice++;
		leitura = fread(&tProdutos[indice], sizeof(Produto), 1, db);
	}
	
	fclose(db);
	
	printf("\n\n Digite o número opção desejada: \n");
	printf("\n 1- Ordenar por preço\n 2 - Ordenar por nome	\n 0- Voltar\n\n");
	
	scanf("%d", &opcao);
	switch(opcao){
		case 1: ordenarPreco();
			break;
		case 2: ordenarNome();
			break;
		case 0: telaFuncionario();
			break;
	}
}

void ordenarPreco(){
	
	Produto aux;
	int i, j, k, esc;
	
	if((db = fopen("db.txt", "r"))==NULL){
		printf("ERRO: Impossível abrir arquivo.\n");
		exit(1);
	}
	
	for(i = 0; i < QTD; i++){
		for(j = 0; j < QTD; j++){
		
			if(tProdutos[i].preco < tProdutos[j].preco){
				aux = tProdutos[i];
				tProdutos[i] = tProdutos[j];
				tProdutos[j] = aux;
				
			}
		}
	}
	
	remove("db.txt");
	if((db = fopen("db.txt", "w"))==NULL){
		printf("ERRO: Impossível abrir arquivo.\n");
		exit(1);
	}
	
	
	for(k = 0; k < QTD; k++){
		if(tProdutos[k].preco != 0.0){	
			esc = fwrite(&tProdutos[k], sizeof(Produto), 1, db);
			//Se o valor retornado por fwrite for diferente de "1" ocorreu algum erro na reescrita do arquivo.
			if(esc != 1){
				printf("Erro!");
			}
		
		}
	}
	
	fclose(db);
	
	listar();
	
}

void ordenarNome(){

	Produto aux;
	int i, j, k, esc;
	
	if((db = fopen("db.txt", "r"))==NULL){
		printf("ERRO: Impossível abrir arquivo.\n");
		exit(1);
	}
	
	for(i = 0; i < QTD; i++){
		for(j = 0; j < QTD; j++){
		
			if(strcmp(tProdutos[i].nome, tProdutos[j].nome)<0){
				aux = tProdutos[i];
				tProdutos[i] = tProdutos[j];
				tProdutos[j] = aux;
				
			}
		}
	}
	
	remove("db.txt");
	if((db = fopen("db.txt", "w"))==NULL){
		printf("ERRO: Impossível abrir arquivo.\n");
		exit(1);
	}
	
	
	for(k = 0; k < QTD; k++){
		if(tProdutos[k].preco != 0.0){	
			esc = fwrite(&tProdutos[k], sizeof(Produto), 1, db);
			//Se o valor retornado por fwrite for diferente de "1" ocorreu algum erro na reescrita do arquivo.
			if(esc != 1){
				printf("Erro!");
			}
		
		}
	}
	
	fclose(db);
	
	listar();
}

void rmProduto(){

}
