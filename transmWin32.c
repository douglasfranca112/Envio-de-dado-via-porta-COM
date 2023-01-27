#include<winbase.h>
#include<stdio.h>
#include <string.h>

int main()
{
    HANDLE hPorta; //handle da porta 
    BOOL Status; //bool de status
    DCB confporta = { 0 };; //inicia estrutura de DCB
    COMMTIMEOUTS timeouts = { 0 }; //inicia estrutura de timeouts
    char SerialBuffer[64] = { 0 }; //char que vamos armazenar o dado
    DWORD BytesWritten = 0; //byte escrito
    hPorta = CreateFileA("\\\\.\\COM3",                //Porta utilizada (alterar de acordo com a porta utilizada no dispositivo)
                      GENERIC_READ | GENERIC_WRITE,// escreve dados e ler dados 
                      0,            // Impede que outros processos abram um arquivo ou dispositivo se solicitarem acesso de exclusão, leitura ou gravação
                      NULL,         // Sem descritor de segurança
                      OPEN_EXISTING,// Abre uma porta somente se existir
                      0,            // argumento nao utilizado
                      NULL);        // sem uso

    if (hPorta == INVALID_HANDLE_VALUE)//caso a porta não esteja disponível
        printf("Erro ao tentar abrir a porta serial\n");
    else{
        printf("Porta serial pronta para o uso\n");
       
       //Setting the Parameters for the SerialPort
        confporta.DCBlength = sizeof(confporta);
        Status = GetCommState(hPorta, &confporta); //retreives  the current settings
        if (Status == FALSE)
        {
            printf_s("\nError to Get the Com state\n\n");
            goto Exit1;
        }
        //configurando DCB
        confporta.DCBlength = sizeof(confporta); //definindo tamanho
        confporta.BaudRate = CBR_9600; ///9600 bits por segundo
        confporta.ByteSize = 8; //8 bits de dados
        confporta.Parity = NOPARITY; //sem paridade
        confporta.StopBits = ONESTOPBIT; //1 bit de parada

        Status = SetCommState(hPorta, &confporta);
        if (Status == FALSE)
        {
            printf_s("\nError to Setting DCB Structure\n\n");
            goto Exit1;
        }
            //Setting Timeouts
        timeouts.ReadIntervalTimeout = 50;
        timeouts.ReadTotalTimeoutConstant = 50;
        timeouts.ReadTotalTimeoutMultiplier = 10;
        timeouts.WriteTotalTimeoutConstant = 50;
        timeouts.WriteTotalTimeoutMultiplier = 10;
        if (SetCommTimeouts(hPorta, &timeouts) == FALSE)
        {
            printf_s("\nError to Setting Time outs");
            goto Exit1;
        }

        printf("Insira o dado a ser transmitidos: \n"); 
        scanf("%s", SerialBuffer);  //dado que será escrito

        Status = WriteFile(hPorta,// handle da porta serial
                       SerialBuffer,            // Data que vai ser lido
                       sizeof(SerialBuffer),   // numeros de bytes que a porta vai ler
                       &BytesWritten,  // numeros de bytes escritos
                       NULL);   //sem uso
        if (Status == FALSE){ //caso não seja possível escrever na porta por algum erro
        printf("\n\n   Erro %d na tentativa de escrita da porta",GetLastError());
        }
        else {
            printf("Dado enviado com sucesso\n");
            
            //print numbers of byte written to the serial port
            printf_s("\nNumber of bytes written to the serail port = %d\n\n", BytesWritten);
        }
    }
    Exit1:
        CloseHandle(hPorta); //fechando porta serial
    
    return 0;
}