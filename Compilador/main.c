#include "stdio.h"
#include "globals.h"
#include "util.h"
#include "analyze.h"
#include "cminus.tab.h"

FILE *listing;
int lineno = 1;
extern FILE *yyin;
int lexic();
TreeNode * parse(void);

int main(){

    FILE *input = fopen("entrada.txt", "r");
    listing = stdout;
    yyin = input;

    if(!lexic()){
        printf("Erro ao criar arquivo de saída da Análise léxica\n" );
       return 0;
    }
    
    printf("Analise lexica feita com sucesso!\n");
    
    lineno = 1;
    rewind(yyin);
    
    TreeNode *root = parse();
    if(root == NULL){
    	printf("Analise Sintatica invalida!\n");
    	return 0;
    }
    
    printf("Analise sintatica feita com sucesso!\n");
    printf("Imprimindo arvore sintatica:\n");
    
    printTree(root);
    
    buildSymtab(root);
    typeCheck(root);
    
    return 0;
}
