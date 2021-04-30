#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int deletar_dados(char* filename){

  FILE *aquivo_banco, *arquivo_temp;
  char ch, *codigo=NULL, *delete_code=NULL, conf;
  int linha_deletar, cont = 1, sair=0;
  
  codigo = malloc(sizeof(char)*7);
  delete_code = malloc(sizeof(char)*7);

  //Abre o banco de dados no modo de leitura
  aquivo_banco = fopen(filename, "r");

  //Printa os registros atuais do banco de dados
  printar_banco_func(aquivo_banco);

  //Retorna ao começo do banco e procura a linha correspondendo ao código do produto desejado pelo usuário, caso o produto nao exita, o processo se repete
  do{
    rewind(aquivo_banco);
    ch='O';
    printf("\nInsira o código do produto para ser deletado:\n");
    scanf("%s", delete_code);

    do{
      fscanf(aquivo_banco,"%s",codigo);

      if(strcmp(codigo, delete_code)!=0){
        fscanf(aquivo_banco,"%*[^\n]");
        cont++;
      }
      ch = getc(aquivo_banco);
    }while((strcmp(codigo, delete_code)!=0) && ch!=EOF);

    if((strcmp(codigo, delete_code)!=0)){
      printf("\nCódigo de Produto não encontrado!");
      cont=1;
    }
  }while(strcmp(codigo, delete_code)!=0);

  //Reseta o contador
  linha_deletar = cont;
  cont=1;

  //Verificação da ação do usuário
  printf("\nVocê tem certeza que deseja deletar o Produto %s ? Digite s/n\n",delete_code);

  do{
    scanf(" %c", &conf);
    if(conf == 's'){
    printf("\nDeletando dados...\n");
    sair=1;
    }
    else if(conf == 'n'){
    printf("\nAção de Deleção cancelada!");
    sair=1;
    return(0);
    }
    else{
    printf("\nDigite um valor válido!\n"); 
    }
  }while(sair==0);

  //Cria um novo arquivo no modo de escrita para colocar apenas as linhas diferentes do produto que deve ser deletado

  rewind(aquivo_banco);
  arquivo_temp = fopen("temp.txt", "w");
  ch = 'O';
  while (ch!=EOF){
      ch = getc(aquivo_banco);

      if (cont!=linha_deletar){
          putc(ch, arquivo_temp);
      }
      if (ch == '\n'){
          cont++;}
  }
  //Fecha os arquivos e Renomeia o arquivo temporário para o nome original do banco de dados
  fclose(aquivo_banco);
  fclose(arquivo_temp);
  remove(filename);
  rename("temp.txt", filename);

  //Printando as Alterações realizadas
  printf("\n Alterações Realizadas com sucesso! Banco de dados atualizado:\n");
  aquivo_banco = fopen(filename, "r");

  printar_banco_func(aquivo_banco);
  fclose(aquivo_banco);
  return 0;
}

int printar_banco_func(FILE *aquivo_banco){
  printf("\n|----------------- Banco de Dados Estoque - Acesso Funcionário -----------------|\n");
  char ch;
  while (ch!=EOF){
      printf("%c", ch);
      ch = getc(aquivo_banco);
  }
  return 0;
}

struct produto_estoque{
  char *codigo;
  char *nome;
  float preco;
  int estoque;
};

int main(){
//deletar_dados("banco_produtos.txt");

FILE *aquivo_banco, *arquivo_temp;
char* filename = "banco_produtos.txt"; 

aquivo_banco = fopen(filename, "r");
int tam=1;

char ch;
while (ch!=EOF){
    ch = getc(aquivo_banco);
    if (ch == '\n'){
        tam++;}
}

struct produto_estoque *produtos_cliente = malloc(sizeof(struct produto_estoque)*tam);

int produtos_validos=0;
rewind(aquivo_banco);
for(int i=0;i<tam;i++){

  ch = getc(aquivo_banco);
  char *cod = malloc(sizeof(char)*(ch+1));
  char *nome = malloc(sizeof(char)*(ch+1));
  float preco;
  int qtd;

  fscanf(aquivo_banco, "%s %s %f %d", cod, nome, &preco, &qtd);
    if (qtd>0){
    produtos_cliente[produtos_validos].codigo = cod;
    produtos_cliente[produtos_validos].nome = nome;
    produtos_cliente[produtos_validos].preco = preco;
    produtos_cliente[produtos_validos].estoque = qtd;
    produtos_validos++;
  }
}

for (int i=0;i<tam;i++){
printf("\n (%d) Produto: %s | Preço R$ %.2f", i,produtos_cliente[i].nome,produtos_cliente[i].preco);
}

int produto_escolhido, quantidade;
float total;

do{
  printf("\n Escolha o Índice do Produto de interesse:\n ");
  scanf("%d", &produto_escolhido);
}while(produto_escolhido>tam-1||produto_escolhido<0);

do{
  printf("\n Insira a Quantidade Total de Compra:\n ");
  scanf("%d", &quantidade);
  if(quantidade > produtos_cliente[produto_escolhido].estoque){
    printf("\n Quantidade Indiponível no Estoque! Insira uma quantidade inferior");
  }
}while(quantidade > produtos_cliente[produto_escolhido].estoque || quantidade<0);

total = quantidade*produtos_cliente[produto_escolhido].preco;
printf("\n Pedido de número #%d realizado com Sucesso!\n |---------------------------------------------|\n Resumo da Compra\n Total da Compra: R$ %.2f",rand() % 8000,total);

return 0;
}