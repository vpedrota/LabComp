%option noyywrap
%option nounput
%{

#include "stdio.h"
#include "globals.h"
#include "string.h"
char tokenString[MAXTOKENLEN + 1];
%}

digit       [0-9]
number      {digit}+
letter      ([a-z]|[A-Z])
identifier  {letter}+
newline     \n
whitespace  [ \t\r]+


%%

"else" {return ELSE;}
"if" {return IF;}
"int" {return INT;}
"return" {return RETURN;}
"void" {return VOID;}
"while" {return WHILE;}
"+" {return MAIS;}
"-" {return MENOS;}
"*" {return VEZES;}
"/" {return SOBRE;}
"<" {return MENORQ;}
"<=" {return MENORI;}
">" {return MAIORQ;}
">=" {return MAIORI;}
"==" {return IGUAL;}
"!=" {return DIF;}
"=" {return RECEBE;}
";" {return PONVIR;}
"," {return VIRG;}
"(" {return EPAREN;}
")" {return DPAREN;}
"[" {return ECOLCH;}
"]" {return DCOLCH;}
"{" {return ECHAVE;}
"}" {return DCHAVE;}
{whitespace}  {}
<<EOF>> {return FIMARQV;}
{newline} {lineno++;}
{identifier} {return ID;}
{number}  {return NUM;}
"/*" {
        char c;
        while(1){
          c = input();
          if(c == '*'){
            c = input();
            if(c == '/') break;
          }
          if(c == '\n') lineno++;
        }
      }
. {return ERROR;}


%%

TokenType getToken(void){
	int token;
	token = yylex();
	strncpy(tokenString,yytext,MAXTOKENLEN);
	return token;
}

int lexic(){

	FILE *saida = fopen("lexico.txt", "w+");
	int Token;
	Token = yylex();
	while(Token != FIMARQV){

		switch(Token){
			case IF: fprintf(saida, "IF "); break;	
			case ELSE: fprintf(saida, "ELSE "); break;
			case INT: fprintf(saida, "INT "); break;
			case RETURN: fprintf(saida, "RETURN "); break;
			case VOID: fprintf(saida, "VOID "); break;	
			case WHILE: fprintf(saida, "WHILE "); break;
			case MAIS: fprintf(saida, "MAIS "); break;
			case MENOS: fprintf(saida, "MENOS "); break;
			case VEZES: fprintf(saida, "VEZES "); break;
			case SOBRE: fprintf(saida, "SOBRE "); break;
			case MENORQ: fprintf(saida, "MENORQ "); break;	
			case MENORI: fprintf(saida, "MENORI "); break;	
			case MAIORQ: fprintf(saida, "MAIORQ "); break;
			case MAIORI: fprintf(saida, "MAIORI "); break;	
			case IGUAL: fprintf(saida, "IGUAL "); break;	
			case DIF: fprintf(saida, "DIF "); break;	
			case RECEBE: fprintf(saida, "RECEBE "); break;	
			case PONVIR: fprintf(saida, "PONVIR "); break;	
			case VIRG: fprintf(saida, "VIRG "); break;	
			case EPAREN: fprintf(saida, "EPAREN "); break;
			case DPAREN: fprintf(saida, "DPAREN "); break;
			case ECOLCH: fprintf(saida, "ECOLCH "); break;
			case DCOLCH: fprintf(saida, "DCOLCH "); break;
			case ECHAVE: fprintf(saida, "ECHAVE "); break;
			case DCHAVE: fprintf(saida, "DCHAVE "); break;	
			case ID: fprintf(saida, "ID "); break;	
			case NUM: fprintf(saida, "NUM "); break;	
			case ERROR: fprintf(saida, "ERROR ");
				printf("ERRO LÉXICO: %s LINHA: %d\n", yytext, lineno);
				return 0;
				break;
			case FIMARQV: fprintf(saida, "FIMARQV "); break;
			default: fprintf(saida, "TOKEN DESCONHECIDO: %s\n", yytext);
		}

		Token = yylex();
	}
	
	fclose(saida);

	return 1;
}
