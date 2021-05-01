#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int deletar_dados(char* filename){
  //Função para deletar dados do Produto (Funcionário)
  FILE *aquivo_banco, *arquivo_temp;
  char ch, *codigo=NULL, *delete_code=NULL, conf;
  int linha_deletar, cont = 1, sair=0;
  
  codigo = malloc(sizeof(char)*7);
  delete_code = malloc(sizeof(char)*7);

  //Abre o banco de dados no modo de leitura
  aquivo_banco = fopen(filename, "r");

  //Printa os registros atuais do banco de dados
  printar_banco_func(aquivo_banco,0);

  //Retorna ao começo do banco e procura a linha correspondendo ao código do produto desejado pelo usuário, caso o produto nao exista, o processo se repete
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

  printar_banco_func(aquivo_banco,1);
  fclose(aquivo_banco);
  return 0;
}

int printar_banco_func(FILE *aquivo_banco,int back_menu){
  //Função de Consultar o estoque de produtos (Funcionário)
  //Abre o banco de dados de produtos e printa todos os registros encontrados para o funcionário
  printf("\n|-------------- Banco de Dados Estoque - Acesso Funcionário --------------|\n");
  char ch;
  while (ch!=EOF){
      printf("%c", ch);
      ch = getc(aquivo_banco);
  }

  //Parametro que indice se deve mostrar a tela de voltar ao menu de ação após printar ou não
  if(back_menu==1){
    menu_acao_func();
  }
  else{
   return 0; 
  }
  
  return 0;
}

struct produto_estoque{
  //Estrutura para armazenar os registroes do banco de dados de produtos
  char *codigo;
  char *nome;
  float preco;
  int estoque;
};

int comprar_cliente(char* filename){
  //Função de Compra do Cliente (Cliente)
  FILE *aquivo_banco, *arquivo_temp;

  //Abre o banco de produtos no modo de leitura
  aquivo_banco = fopen(filename, "r");
  int tam=1;

  //Varre todas as linhas do banco e conta as quebrar de linhas para saber o total de registros no banco 
  char ch;
  while (ch!=EOF){
      ch = getc(aquivo_banco);
      if (ch == '\n'){
          tam++;}
  }

  //Usa a Struct do produto para armazenar os dados referentes ao produto, obtidos do banco de dados
  struct produto_estoque *produtos_cliente = malloc(sizeof(struct produto_estoque)*tam);

  int produtos_validos=0;
  rewind(aquivo_banco);
  for(int i=0;i<tam;i++){

    ch = getc(aquivo_banco);
    char *cod = malloc(sizeof(char)*(ch+1));
    char *nome = malloc(sizeof(char)*(ch+1));
    float preco;
    int qtd;

    //Lê as informações do produto do banco de dados
    fscanf(aquivo_banco, "%s %s %f %d", cod, nome, &preco, &qtd);

    //Seleciona apenas os produtos com estoque não nulo e armazena num vetor da struct!
    if (qtd>0){
      produtos_cliente[produtos_validos].codigo = cod;
      produtos_cliente[produtos_validos].nome = nome;
      produtos_cliente[produtos_validos].preco = preco;
      produtos_cliente[produtos_validos].estoque = qtd;
      produtos_validos++;
    }
  }
  //Printa todos os produtos disponiveis para o cliente, armazenados na struct
  printf("\n|--------- |Magazine-UFABC| Produtos Disponíveis a Pronta-entrega! ---------|\n");

  for (int i=0;i<tam-1;i++){
  printf("\n (%d) Produto: %s | Preço R$ %.2f", i,produtos_cliente[i].nome,produtos_cliente[i].preco);
  }

  int produto_escolhido, quantidade;
  float total;
  //Recebe input do produto desejado pelo cliente
  do{
    printf("\n\n Escolha o Número do Produto de Desejado:\n ");
    scanf("%d", &produto_escolhido);
  }while(produto_escolhido>tam-1||produto_escolhido<0);

  //Recebe input da qtd desejada pelo cliente
  do{
    printf("\n Insira a Quantidade Total de Compra:\n ");
    scanf("%d", &quantidade);
    if(quantidade > produtos_cliente[produto_escolhido].estoque){
      //Verfica a disponibilidade da quantia desejada no estoque
      printf("\n Quantidade Indiponível no Estoque! Insira uma quantidade inferior");
    }
  }while(quantidade > produtos_cliente[produto_escolhido].estoque || quantidade<0);

  //Calcula o total da compra e Mostra um resumo do pedido ao cliente
  total = quantidade*produtos_cliente[produto_escolhido].preco;
  printf("\n Pedido de número #%d realizado com Sucesso!\n |---------------------------------------------|\n Resumo da Compra \n Items Ordenados: %d unidade(s), %s \n Total da Compra: R$ %.2f",rand() % 8000,quantidade,produtos_cliente[produto_escolhido].nome,total);

  return 0;
}

int login_func(char* filename){
  //Função de login do funcionário (Funcionário)

  FILE *Banco;

  //Alocando as string de login e senha inputadas e do banco de dados
  char ch, *login=NULL,*login_digitado=NULL, *senha=NULL,*senha_digitada=NULL;

  login = malloc(sizeof(char)*20);
  login_digitado = malloc(sizeof(char)*20);
  senha = malloc(sizeof(char)*20);
  senha_digitada = malloc(sizeof(char)*20);

  //Procura pelo login inputado no banco de dados
  Banco = fopen(filename ,"r");
  do{
      rewind(Banco);
      printf("\nInsira o seu Usuário: \n");
      scanf("%s", login_digitado);

      do{
          fscanf(Banco,"%s %s", login, senha);
          ch=fgetc(Banco);
      }while((strcmp(login, login_digitado)!=0) && ch!=EOF);

      if(strcmp(login, login_digitado)!=0){
          printf("\nLogin incorreto\n");
      }
  }while(strcmp(login, login_digitado)!=0);

  //Procura pela senha inputada no banco de dados, o funcionario tem 3 chances para acertar
  for(int i=3;i>0;i--){
      printf("\nInsira a sua Senha: \n");
      scanf("%s",senha_digitada);
      //printf("\n");
      if(strcmp(senha, senha_digitada)==0){
          fclose(Banco);
          return(1);
          break;      
      }
      else{
          printf("\nSenha incorreta\n");
          printf("Voce tem mais %i tentativas\n", i-1);
          if(i==1){
              fclose(Banco);
              return(0);
              exit(0);
          }
      } 
    }
    return (0);  
}

int menu_acao_func(){
  //Menu de ações disponiveis para o funcionário, chama as demais funcoes
  int acao;
  FILE *aquivo_banco;

  do{
  printf("\nQual ação você deseja realizar?");
  printf("\n (0) Deletar Produto do Estoque\n (1) Consultar Estoque\n (2) Registar um novo Produto\n (3) Sair\n "); 
  scanf("%d",&acao);

  switch(acao){
    case 0:
      deletar_dados("banco_produtos.txt");
      return acao;
    case 1:
      aquivo_banco = fopen("banco_produtos.txt", "r");
      printar_banco_func(aquivo_banco,1);
      return acao;
    case 3:
      return acao;
    default:
      printf("\nRecurso em fase de Desenvolvimento!");
  }

  }while(acao!=0||acao!=1||acao!=2||acao!=3);
  return (acao);
}

int main(){

int escolha, login;
//Escolhe se é cliente ou funcionário
do{
  printf("\n| ******** |Magazine-UFABC| Seja bem-vindo ao Sistema de Controle de Estoque e Interface de Compra da Magazine-UFABC! ********|\n\n Por favor, escolha o seu tipo de Acesso:");

  printf("\n (0) Acesso Funcionário | Controle Estoque\n (1) Acesso Cliente | Interface Compra\n ");

  scanf("%d", &escolha);
}while(escolha!=0 && escolha!=1);

if (escolha==1){
  //Cliente
 comprar_cliente("banco_produtos.txt"); 
}
else{
  //Funcionario
  login = login_func("Banco.txt");
  if (login==1){
    menu_acao_func();
  }
  else{
    printf("\n Acesso não autorizado!");
  }
}

return 0;
}