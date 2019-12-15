#include <stdio.h>
#include <string.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <sys/types.h>
#include <pthread.h>
#include <unistd.h>

// Filas de mensagens
// (RIPPED STRAIGHT FROM TEACHER)
#define REQ_QUEUE 	10010
#define RESP_QUEUE 	10020

#define TO_UPPERCASE 1
#define TO_LOWERCASE 2

#define MAX_TEXT_SIZE 	1000

struct reqmsg {
 	long	cli_id;
   	int	conv_type;
	char	textbuffer[MAX_TEXT_SIZE+1];
};

struct respmsg {
	long	cli_id;
	char	textbuffer[MAX_TEXT_SIZE+1];
};

// Rotina dos processos clientes
// Executar dentro dos processos filhos somente
void cliente(){
  int req_mq;
	int resp_mq;
	struct reqmsg cli_reqmsg;
	struct respmsg serv_respmsg;
	long cli_id;
	int optconv;

	cli_id = getpid();
	req_mq = msgget(REQ_QUEUE, 0777);
	if (req_mq == -1) {
   		printf("msgget falhou no cliente\n"); 
		exit(1); 
	} 
	resp_mq = msgget(RESP_QUEUE, 0777);
	if (resp_mq == -1) {
   		printf("msgget falhou no cliente\n"); 
		exit(1); 
	} 
	do {
		fflush(stdin);
		fflush(stdout);
		
    printf("\n>> Entre com o texto a ser convertido:\n");
		scanf("%[^\n]s",cli_reqmsg.textbuffer);
		printf("\n>> Qual opcao? \n[1] Maiuscula \n[2] Minuscula \n[9] Finalizar programa\n");
    printf("Sua opcao: ");
		scanf("%d",&optconv);
		scanf("%*c");
    printf("\n");


    while(optconv != 1 && optconv != 2 && optconv != 9){
      printf("\n>> Opcao invalida\n\n");
		  printf("\n>> Qual opcao? \n[1] Maiuscula \n[2] Minuscula \n[9] Finalizar programa\n");
      printf("Sua opcao: ");
		  scanf("%d",&optconv);
		  scanf("%*c");
      printf("\n");
    }

		cli_reqmsg.conv_type=TO_UPPERCASE;
		
    if (optconv==1)
			cli_reqmsg.conv_type=TO_UPPERCASE;
		
    else if (optconv==2)
			cli_reqmsg.conv_type=TO_LOWERCASE;
		
    if (optconv!=9) {
			// Preenche o tipo da mensagem com o identificador (PID) do cliente
			cli_reqmsg.cli_id = cli_id;
			// Envia requisicao ao servidor
			msgsnd(req_mq,&cli_reqmsg,sizeof(struct reqmsg),0);
			// Espera pela mensagem de resposta especifica para este cliente
			// usando o PID do processo cliente como tipo de mensagem
		
    	if (msgrcv(resp_mq,&serv_respmsg,sizeof(struct respmsg),cli_id,0) < 0) {
				printf("msgrcv falhou no cliente\n");
				exit(1);
			}
		
    	// Apresenta o texto convertido
			printf(">> O texto convertido e':\n");
			printf("%s\n",serv_respmsg.textbuffer);
		}
	} while(optconv!=9);
	exit(0);
}

// Threads de comandos
void *run(/*filename*/){

}

void *mail(/*id_usuario, mensagem*/){

}

void *showmail(){

}

void *sendmail(mailid,message) {
  char cmd[100];  // to hold the command.
  char to[] = "sample@example.com"; // email id of the recepient.
  char body[] = "SO rocks";    // email body.
  char tempFile[100];     // name of tempfile.

  strcpy(tempFile,tempnam("/tmp","sendmail")); // generate temp file name.

  FILE *fp = fopen(tempFile,"w"); // open it for writing.
  fprintf(fp,"%s\n",body);        // write body to it.
  fclose(fp);             // close it.

  sprintf(cmd,"sendmail %s < %s",to,tempFile); // prepare command.
  system(cmd);     // execute it.

  return 0;
}

void *send(/*id_usuario, id_mensagem, texto_mensagem*/){

}

void *recieve(/*id_usuario*/){

}

void *users(){

}

void *myid(){

}

void *dirlist(){

}

void *godir(/*dir*/){

}

void *exit(){
  
}

void stroupper(char * str){
  // Convert string to upper case
  while (*str) {
    *str = toupper((unsigned char) *str);
    str++;
  }
}

void strolower(char * str){
  // Convert string to upper case
  while (*str) {
    *str = tolower((unsigned char) *str);
    str++;
  }
}

// Processo principal e servidor
int main(void) {
  pid_t pid;
  pid = fork();
  if(pid == -1){
    printf("Algo de errado não está certo");

  }else if(pid == 0){
    cliente();

  }else{
    int req_mq;
	  int resp_mq;
	  struct reqmsg cli_reqmsg;
	  struct respmsg serv_respmsg;

	  req_mq = msgget(REQ_QUEUE, IPC_CREAT | 0777);
	  if (req_mq == -1) {
   	  printf("msgget falhou no servidor\n"); 
		  exit(1); 
	  } 
	  resp_mq = msgget(RESP_QUEUE, IPC_CREAT | 0777);
	  if (resp_mq == -1) {
   		printf("msgget falhou no servidor\n"); 
		  exit(1); 
	  } 
	  printf("servidor: iniciou execucao\n");
	  for (;;) {
		  // Espera pela requisicao de qualquer cliente (message type=0)
		  if (msgrcv(req_mq,&cli_reqmsg,sizeof(struct reqmsg),0,0)<0) {
			  printf("msgrcv falhou no servidor\n");
			  exit(1);
		  }
		  printf("servidor: recebeu requisicao do cliente %ld\n",cli_reqmsg.cli_id);
		  // Faz a conversao requerida
		  if (cli_reqmsg.conv_type==TO_UPPERCASE)
			  stroupper(cli_reqmsg.textbuffer);
		  else
			  strolower(cli_reqmsg.textbuffer);
		  // Copia o identificador do cliente para a mensagem de resposta
		  serv_respmsg.cli_id = cli_reqmsg.cli_id;
		  // Copia o buffer convertido para a mensagem de resposta
		  strcpy(serv_respmsg.textbuffer,cli_reqmsg.textbuffer);
		  // Envia a resposta ao cliente
		  msgsnd(resp_mq,&serv_respmsg,sizeof(struct respmsg),0);
		  printf("servidor: enviou resposta ao cliente %ld\n",serv_respmsg.cli_id);
	  }
	  exit(0);

  }
}