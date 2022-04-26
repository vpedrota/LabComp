#ifndef _GLOBALS_H_
#define _GLOBALS_H_

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>

#define MAXTOKENLEN 40

#ifndef YYPARSER
#include "cminus.tab.h"
#define FIMARQV 0
#endif

int lineno;

extern char tokenString[MAXTOKENLEN+1];
typedef int TokenType;
TokenType getToken(void);

FILE* listing;

typedef enum { StmtK, ExpK, DeclK } NodeKind;
typedef enum { IfK, WhileK, AssignK, CompoundK, ReturnK } StmtKind;
typedef enum { OpK, ConstK, IdK, TypeK, NameArrK, FunCallK, CalcK } ExpKind;
typedef enum { VarK, FunK, ArrVarK, ArrParamK, ParamK } DeclKind;


typedef enum {Void,Integer,Array} ExpType;

#define MAXCHILDREN 3

struct Escopo;

typedef struct AtributoVetor {
  TokenType tipo;
  char * nome;
  int tamanho;
} Vetor;

typedef struct treeNode
   { struct treeNode * child[MAXCHILDREN];
     struct treeNode * sibling;
     int lineno;
     NodeKind nodekind;
     union { StmtKind stmt; ExpKind exp; DeclKind decl;} kind;
     
     union { TokenType op;
             TokenType type;
             int val;
             char * name;
             Vetor vetor;
             struct Escopo * escopo; } attr;

     ExpType type;
   } TreeNode;
   
TreeNode * parse(void);   

int Error;
#endif
