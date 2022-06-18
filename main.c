#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <locale.h>
#include <math.h>

//maximo de caracteres
#define MAX 50
//modulo 29
#define MOD 29
// a-z = 0-25 | _ = 26 | , = 27 | . = 28


float** alocaMatriz(int linhas);
float** alocaMatrizRect(int linhas, int colunas);
float det_matriz();
float** adj_matriz(float** matriz, int N);
int charToInt(char c);
char intToChar(int i);
float** matrixMulti(float** matrix1, int L1, int C1, float** matrix2, int L2, int C2);
float** inversaMatrixHill(float** matrix_original, int N);

int main(void) {

  setlocale(LC_ALL, "pt-br");
  
  

  //float iKey[MAX][MAX];
  char strMsg[MAX], strKey[MAX];
  int keySize;

  
  printf("Escreva a mensagem a ser criptografada(Até 50 Caracteres): ");
  fgets(strMsg, MAX, stdin);
  strMsg[strlen(strMsg)-1] = '\0';
  //printf("\n");
  printf("Escreva a chave de criptografia: ");
  fgets(strKey, MAX, stdin);
  strKey[strlen(strKey)-1] = '\0';
  printf("\n");
  
  printf("----------------------------------------------------------\n");
  keySize = ceil(sqrt(strlen(strKey)));
  // printf("key size: %d\n", keySize);

  if(strlen(strKey) % keySize != 0){
    printf("***A chave de criptografia não forma uma matriz quadrada, preenchendo o resto dos espaços com '_' !***\n");
    for(int i = strlen(strKey); i < keySize*keySize; i++)
      strKey[i] = '_';
    printf("Está é a chave final: '%s'\n\n", strKey);
  }
  
  float **key = alocaMatriz(keySize);
  
  for(int i = 0; i < keySize; i++)
    for(int j = 0; j < keySize; j++){
      key[i][j] = charToInt(strKey[(i*keySize)+j]);
    }

  printf("Essa é a Matriz Chave: \n");
  for(int i = 0; i < keySize; i++){
    for(int j = 0; j < keySize; j++)
      printf("[%d] ", (int)key[i][j]);
    printf("\n");
  }
  printf("----------------------------------------------------------\n");

  //tratando a mensagem
  float div = (float) strlen(strMsg) / keySize;
  div = ceil(div);
  if(strlen(strMsg) < keySize * (int)div){
    printf("***Mensagem muito curta, preenchendo o resto dos espaços com '_' !***\n");
    for(int i = strlen(strMsg); i < keySize*(int)div; i++)
      strMsg[i] = '_';
    printf("Está é a mensagem final: '%s'\n\n", strMsg);
  }

  
  float** msg = alocaMatrizRect(keySize, (int)div);
  for(int i = 0; i < keySize; i++)
    for(int j = 0; j < (int)div; j++){
      msg[i][j] = charToInt(strMsg[(i*(int)div)+j]);
    }
  
  printf("Essa é a Matriz Mensagem: \n");
  for(int i = 0; i < keySize; i++){
    for(int j = 0; j < (int)div; j++)
      printf("[%d] ", (int)msg[i][j]);
    printf("\n");
  }
  printf("----------------------------------------------------------\n");
  

  float **msgCripto = alocaMatrizRect(keySize, (int)div);
  msgCripto = matrixMulti(key, keySize, keySize, msg, keySize, (int)div);
  
  printf("Essa é a Matriz Criptografada: \n");
  for(int i = 0; i < keySize; i++){
    for(int j = 0; j < (int)div; j++)
      printf("[%d] ", (int)msgCripto[i][j]);
    printf("\n");
  }
  printf("----------------------------------------------------------\n");
  //colocando a matriz criptografada em modulo MOD
  for(int i = 0; i < keySize; i++)
    for(int j = 0; j < (int)div; j++)
      msgCripto[i][j] = (int)msgCripto[i][j] % MOD;
  
  printf("Essa é a Matriz Criptografada em Modulo %d: \n", MOD);
  for(int i = 0; i < keySize; i++){
    for(int j = 0; j < (int)div; j++)
      printf("[%d] ", (int)msgCripto[i][j]);
    printf("\n");
  }
  
  printf("Essa é a Mensagem Criptografada: ");
  for(int i = 0; i < keySize; i++){
    for(int j = 0; j < (int)div; j++)
      printf("%c", intToChar((int)msgCripto[i][j]));
  }
  printf("\n");
  
  printf("----------------------------------------------------------\n");


  float** iKey = alocaMatriz(keySize);
  iKey = inversaMatrixHill(key, keySize);

  
  printf("Essa é a Matriz Chave Inversa:\n");
  for(int i = 0; i < keySize; i++){
    for(int j = 0; j < keySize; j++)
      printf("[%d] ", (int)iKey[i][j]);
    printf("\n");
  }  
  printf("----------------------------------------------------------\n");

  printf("Para descriptografar, basta multiplicar a matriz chave inversa\npela mensagem criptografada:\n");

  float** msgDescripto = alocaMatrizRect(keySize, (int)div);
  msgDescripto = matrixMulti(iKey, keySize, keySize, msgCripto, keySize, (int)div);

  printf("Essa é a Matriz Descriptografada:\n");
  for(int i = 0; i < keySize; i++){
    for(int j = 0; j < (int)div; j++)
      printf("[%d] ", (int)msgDescripto[i][j]);
    printf("\n");
  }  
  printf("----------------------------------------------------------\n");
  printf("Basta fazer o Modulo %d para chegar a mensagem original!\n", MOD);
  
  for(int i = 0; i < keySize; i++)
    for(int j = 0; j < (int)div; j++)
      msgDescripto[i][j] = (int)msgDescripto[i][j] % MOD;
  printf("Chegamos nessa matriz:\n");
  for(int i = 0; i < keySize; i++){
    for(int j = 0; j < (int)div; j++)
      printf("[%d] ", (int)msgDescripto[i][j]);
    printf("\n");
  }  
  printf("Transformando para letras temos: '");
  for(int i = 0; i < keySize; i++)
    for(int j = 0; j < (int)div; j++)
      printf("%c", intToChar((int)msgDescripto[i][j]));
  printf("'\n");
  
  return 0;
}

//funçoes suporte -------------------------------------------------

float** adj_matriz(float** matriz, int N){
  float matriz_original[N][N];
  float** matriz_adj = alocaMatriz(N);
  float** matriz_sm = alocaMatriz(N);
  float** matriz_cof = alocaMatriz(N-1);
  
  //copiando a matriz
  for(int i = 0; i < N; i++)
    for(int j = 0; j < N; j++)
      matriz_original[i][j] = matriz[i][j];
  for(int i = 0; i < N; i++)
    for(int j = 0; j < N; j++)
      matriz_sm[i][j] = -666;
  
  //printf("ok\n");
  //formando a matriz dos cofatores
  for(int i = 0; i < N; i++){ 
    for(int j = 0; j < N; j++){
      int x=0, y=0;
      for(int k = 0; k < N; k++){ 
        for(int l = 0; l < N; l++){
          if(k != i && l != j){
            if(x == N-1){
              x = 0;
              y += 1;
            }
            matriz_cof[x][y] = matriz_original[k][l]; 
            x++;
          }    
        }
      }
      // printf("matriz de cof[%d][%d]: \n",i, j);
      // for(int q = 0; q < N-1; q++){
      //   for(int r = 0; r < N-1; r++)
      //     printf("[%.0f] ", matriz_cof[q][r]);
      //   printf("\n");
      // }
      float det = det_matriz(matriz_cof, N-1);
      // printf("%d\n",i+j);
      int i1 = i+1;
      int j1 = j+1;
      // printf("%d\n",i1+j1);
      matriz_sm[i][j] = pow(-1, (i1+j1)) * det;
      
      // printf("matriz de sm: \n");
      // for(int q = 0; q < N; q++){
      //   for(int r = 0; r < N; r++)
      //     printf("[%.0f] ", matriz_sm[q][r]);
      //   printf("\n");
      // }
      // printf("\n");
    }
  }
  
  // printf("\n\n\n-----matriz de cofatores-----\n");
  // for(int i = 0; i < N; i++){
  //   for(int j = 0; j < N; j++)
  //     printf("[%d]", (int)matriz_sm[i][j]);
  //   printf("\n");
  // }

  // printf("\n\n\n");



      
  // printf("ok\n");
  //transposicao da matriz
  for(int i = 0; i < N; i++)
    for(int j = 0; j < N; j++)
      matriz_adj[j][i] = matriz_sm[i][j];

  return matriz_adj;
  
}


float det_matriz(float **matriz, int N){
	float x[MAX], ratio, det=1;
	float n = (float) N; //conversao de int para float
  
  float matriz_original[N][N];
  for(int i = 0; i < N; i++)
    for(int j = 0; j < N; j++)
      matriz_original[i][j] = matriz[i][j];
    //memcpy(matriz_aux, matriz_malha, MAX*MAX*sizeof(float)); //copia da matriz original para aux
  
  

  
  //Aplicando eliminação de Gauss
	 for(int i=0; i < n; i++)
	 {
    //   if(matriz_original[i][i] == 0.0)
		  // {
			 //   printf("Erro de matriz, fechando programa!");
			 //   exit(0);
		  // }
		  for(int j=i+1; j < n; j++)
		  {
			   ratio = (float) matriz_original[j][i] / matriz_original[i][i];

			   for(int k=0;k < n; k++)
			   {
			  		matriz_original[j][k] = matriz_original[j][k] - ratio*matriz_original[i][k];
			   }
		  }
	 }

	 //Impressão da matriz escalonada
	 // printf("\nMatriz Escalonada: \n");
	 // for(int i = 0; i < n; i++)
	 // {
		//   for(int j = 0; j < n;j++)
		//   {
		// 	   printf("%02.f\t", matriz_original[i][j]);
		//   }
		//   printf("\n");
	 // }

	 // Encontrando o determinante multiplicando a diagonal principal
	 for(int i=0; i < n; i++)
     {
         // printf("%f = %f * %f\n", det, det, matriz_original[i][i]);
         det = det * (float) matriz_original[i][i];
     }

	 // printf("Determinante da matriz: %0.3f\n\n", det);


	 return det;
}


int charToInt(char c){
  if(c >= 65 && c <= 90)
    return c-65;
  else if(c >= 97 && c <= 122)
      return c-97;
  else
    switch(c){
      case '_': return 26;
      case ',': return 27;
      case '.': return 28;
      default: return -1;  //nao deve acontecer!!
    }
}


char intToChar(int i){
// a-z = 0-25 | _ = 26 | , = 27 | . = 28
  if(i <= 25)
    return i + 97;
  else
    switch(i){
      case 26: return '_';
      case 27: return ',';
      case 28: return '.';
      default: return 0; //nao deve acontecer
    }
}


float** matrixMulti(float** matrix1, int L1, int C1, float** matrix2, int L2, int C2){
  if(C1 != L2){
    printf("Matrizes Incompativeis!!\nAbortando Programa...\n");
    exit(0);
  }
  
  float** matrixResultado = alocaMatrizRect(L1, C2);

  for(int i = 0; i < L1; i++)
    for(int j = 0; j < C2; j++)
      matrixResultado[i][j] = 0;

  
  for(int i = 0; i < L1; i++){
    for(int j = 0; j < C2; j++){
      for(int k = 0; k < C1; k++){
        matrixResultado[i][j] += matrix1[i][k] * matrix2[k][j];
      }
    }
  }
  
  //funçao para printar a matriz multiplicada
  // for(int i = 0; i < L1; i++){
  //   for(int j = 0; j < C2; j++){
  //    printf("[%d] ", (int)matrixResultado[i][j]);   
  //   }
  //   printf("\n");
  // }

  return matrixResultado;
}


float** inversaMatrixHill(float** matrix_original, int N){

  float** matrixInversa = alocaMatriz(N);
  

  float det = det_matriz(matrix_original, N);
  det = (int) det % MOD;
  float** adj = alocaMatriz(N);
  adj = adj_matriz(matrix_original, N);

  for(int i = 0; i < N; i++)
    for(int j = 0; j < N; j++)
      adj[i][j] = (int) adj[i][j] % MOD;
  
  int x = 1;
  while(((x * (int)det) % MOD) != 1){
    // printf("x: %d -> ", x);
    // printf("%d\n", ((x * det) % MOD));
    x++;
  }
  det = x;

  
  for(int i = 0; i < N; i++)
    for(int j = 0; j < N; j++)
      matrixInversa[i][j] = det * adj[i][j];

  for(int i = 0; i < N; i++)
    for(int j = 0; j < N; j++){
      matrixInversa[i][j] = (int)matrixInversa[i][j] % MOD;
      if(matrixInversa[i][j] < 0)
        matrixInversa[i][j] += MOD;
    }
  

  
  return matrixInversa;
}


float** alocaMatriz(int linhas){
  //matriz quadrada -> linhas == colunas
    float **mat = (float **) malloc(sizeof(float*) * linhas);
    for (int i = 0; i < linhas; i++)
        mat[i] = (float *) malloc(sizeof(float) * linhas);
    return mat;
}


float** alocaMatrizRect(int linhas, int colunas){
  //matriz quadrada -> linhas == colunas
    float **mat = (float **) malloc(sizeof(float*) * linhas);
    for (int i = 0; i < linhas; i++)
        mat[i] = (float *) malloc(sizeof(float) * colunas);
    return mat;
}
