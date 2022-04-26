%{

#define YYPARSER

#include "globals.h"
#include "util.h"

#define YYSTYPE TreeNode *
static int NumSalvo;
static char * NomeSalvo;
static int LinhaSalva;
static TreeNode * ArvSalva;
static int yylex(void);

int yyerror(char *s);

%}

%token ELSE IF INT RETURN VOID WHILE 
%left MAIS MENOS VEZES SOBRE
%left MENORQ MENORI MAIORQ MAIORI IGUAL DIF
%token PONVIR VIRG EPAREN DPAREN ECOLCH DCOLCH ECHAVE DCHAVE RECEBE
%token ID NUM
%token ERROR


%%

	programa	: decl_lista{ ArvSalva = $1; }
			;
	decl_lista	: decl_lista decl 
				{
				YYSTYPE t = $1;
                               if (t != NULL)
                               { while (t->sibling != NULL) { t = t->sibling; }
                               t->sibling = $2;
                               $$ = $1;
                               } else { $$ = $2; }
			  	}
			| decl { $$ = $1; }
			;
	decl		: var_decl{ $$ = $1; }
                	| fun_decl{ $$ = $1; }
               	;
	id		: ID
        			{
        			NomeSalvo = copyString(tokenString);
        			LinhaSalva = lineno;
        			}
        		;
	num		: NUM
        			{
        			NumSalvo = atoi(tokenString);
        			LinhaSalva = lineno;
        			}
        		;
        var_decl	: tipo_espec id PONVIR
        			{
        			$$ = newDeclNode(VarK);
                            	$$->child[0] = $1;
                            	$$->attr.name = NomeSalvo;
                            	$$->lineno = lineno;
                            	}
                       | tipo_espec id ECOLCH num DCOLCH PONVIR
                       	{
                       	$$ = newDeclNode(ArrVarK);
                       	$$->child[0] = $1;
                       	$$->attr.vetor.nome = NomeSalvo;
                       	$$->attr.vetor.tamanho = NumSalvo;
                       	$$->lineno = lineno;
                       	$$->type = Array;
                       	}
                       ;
	tipo_espec	: INT
				{
				$$ = newExpNode(TypeK);
				$$->type = Integer;
				}
			| VOID
				{
				$$ = newExpNode(TypeK);
				$$->type = Void;
				}
			;
	fun_decl	: tipo_espec id
				{
				$$ = newDeclNode(FunK);
				$$->attr.name = NomeSalvo;
				$$->lineno = lineno;
				}
			  EPAREN params DPAREN comp_decl
				{
				$$ = $3;
				$$->child[0] = $1;
				$$->child[1] = $5;
       			$$->child[2] = $7;
       			}
       		;
       params		: param_lista { $$ = $1; }
       		| VOID
       			{
       			$$ = newDeclNode(ParamK);
       			$$->type = Void;
       			}
       		;
       param_lista	: param_lista VIRG param
       			{
				YYSTYPE t = $1;
                               if (t != NULL)
                               { while (t->sibling != NULL) { t = t->sibling; }
                               t->sibling = $3;
                               $$ = $1;
                               } else { $$ = $2; }
			  	}
			| param { $$ = $1; }
			;
	param		: tipo_espec id
				{
				$$ = newDeclNode(ParamK);
				$$->child[0] = $1;
				$$->attr.name = NomeSalvo;
				}
			| tipo_espec id ECOLCH DCOLCH
				{
				$$ = newDeclNode(ArrParamK);
				$$->child[0] = $1;
				$$->attr.name = copyString(NomeSalvo);
				}
			;
	comp_decl	: ECHAVE local_decl stmt_lista DCHAVE
				{
				$$ = newStmtNode(CompoundK);
				$$->child[0] = $2;
				$$->child[1] = $3;
				}
			;
	local_decl	: local_decl var_decl
				{
				YYSTYPE t = $1;
                               if (t != NULL)
                               { while (t->sibling != NULL) { t = t->sibling; }
                               t->sibling = $2;
                               $$ = $1;
                               } else { $$ = $2; }
			  	}
			| { $$ = NULL; }
			;
	stmt_lista	: stmt_lista stmt
				{
				YYSTYPE t = $1;
                               if (t != NULL)
                               { while (t->sibling != NULL) { t = t->sibling; }
                               t->sibling = $2;
                               $$ = $1;
                               } else { $$ = $2; }
			  	}
			| { $$ = NULL; }
			;
	stmt		: exp_decl { $$ = $1; }
			| comp_decl { $$ = $1; }
			| selec_decl { $$ = $1; }
			| it_decl { $$ = $1; }
			| retorno_decl { $$ = $1; }
			;
	exp_decl	: exp PONVIR { $$ = $1; }
			| PONVIR { $$ = NULL;}
			;
	selec_decl	: IF EPAREN exp DPAREN stmt
				{
				$$ = newStmtNode(IfK);
				$$->child[0] = $3;
				$$->child[1] = $5;
				}
			| IF EPAREN exp DPAREN stmt ELSE stmt
				{
				$$ = newStmtNode(IfK);
				$$->child[0] = $3;
				$$->child[1] = $5;
				$$->child[2] = $7;
				}
			;
	it_decl	: WHILE EPAREN exp DPAREN stmt
				{
				$$ = newStmtNode(WhileK);
				$$->child[0] = $3;
				$$->child[1] = $5;
				}
			;
	retorno_decl	: RETURN PONVIR
				{
				$$ = newStmtNode(ReturnK);
				$$->type = Void;
				}
			| RETURN exp PONVIR
				{
				$$ = newStmtNode(ReturnK);
				$$->child[0] = $2;
				}
			;
	exp		: var RECEBE exp
				{
				$$ = newStmtNode(AssignK);
				$$->child[0] = $1;
				$$->child[1] = $3;
				}
			| simp_exp { $$ = $1; }
			;
	var		: id
				{
				$$ = newExpNode(IdK);
				$$->attr.name = NomeSalvo;
				}
			| id 
				{
				$$ = newExpNode(NameArrK);
				$$->attr.name = NomeSalvo;
				}
			  ECOLCH exp DCOLCH
				{
				$$ = $2;
				$$->child[0] = $4;
				}
			;
	simp_exp	: soma_exp relacional soma_exp
				{
				$$ = newExpNode(CalcK);
				$$->child[0] = $1;
				$$->child[1] = $2;
				$$->child[2] = $3;
				}
			| soma_exp { $$ = $1; }
			;
	relacional	: MENORI
				{
				$$ = newExpNode(OpK);
				$$->attr.op = MENORI;
				}
			| MENORQ
				{
				$$ = newExpNode(OpK);
				$$->attr.op = MENORQ;
				}
			| MAIORQ
				{
				$$ = newExpNode(OpK);
				$$->attr.op = MAIORQ;
				}
			| MAIORI
				{
				$$ = newExpNode(OpK);
				$$->attr.op = MAIORI;
				}
			| IGUAL
				{
				$$ = newExpNode(OpK);
				$$->attr.op = IGUAL;
				}
			| DIF
				{
				$$ = newExpNode(OpK);
				$$->attr.op = DIF;
				}
			;
	soma_exp	: soma_exp soma termo
				{
				$$ = newExpNode(CalcK);
				$$->child[0] = $1;
				$$->child[1] = $2;
				$$->child[2] = $3;
				}
			| termo { $$ = $1; }
			;
	soma		: MAIS
        			{ $$ = newExpNode(OpK);
          			$$->attr.op = MAIS;
        			}
      			| MENOS
        			{ $$ = newExpNode(OpK);
          			$$->attr.op = MENOS;
        			}
      			;
      	termo		: termo mult fator
      				{
				$$ = newExpNode(CalcK);
				$$->child[0] = $1;
				$$->child[1] = $2;
				$$->child[2] = $3;
				}
			| fator { $$ = $1; }
			;
	mult		: VEZES
        			{ $$ = newExpNode(OpK);
          			$$->attr.op = VEZES;
        			}
      			| SOBRE
        			{ $$ = newExpNode(OpK);
          			$$->attr.op = SOBRE;
        			}
      			;
      	fator		: EPAREN exp DPAREN { $$ = $2; }
      			| var { $$ = $1; }
      			| ativ { $$ = $1; }
      			| num
      				{
      				$$ = newExpNode(ConstK);
      				$$->type = Integer;
      				$$->attr.val = atoi(tokenString);
      				}
      			;
      	ativ		: id
      				{
      				$$ = newExpNode(FunCallK);
      				$$->attr.name = NomeSalvo;
      				}
      			  EPAREN args DPAREN
      				{
      				$$ = $2;
      				$$->child[0] = $4;
      				}
      			;
      	args		: arg_lista { $$ = $1; }
      			| { $$ = NULL; }
      			;
      	arg_lista	: arg_lista VIRG exp
      				{
				YYSTYPE t = $1;
                               if (t != NULL)
                               { while (t->sibling != NULL) { t = t->sibling; }
                               t->sibling = $3;
                               $$ = $1;
                               } else { $$ = $3; }
			  	}
			| exp { $$ = $1; }
      			; 
%%


int yyerror(char * message){ 
  extern char* yytext;
  printf("ERRO SINTÁTICO: %s LINHA: %d\n", yytext, lineno);
  Error = 1;
  return 0;
}

static int yylex(void){ 
  return getToken(); 
}

TreeNode * parse(void){ 
  if(yyparse()) return NULL;
  return ArvSalva;
}
