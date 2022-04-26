#include "globals.h"
#include "util.h"


 void printToken( TokenType token, const char* tokenString )
 { switch (token)
   { case ELSE:
     case IF:
     case INT:
     case RETURN:
     case VOID:
     case WHILE:
       fprintf(listing, "reserved word: %s\n",tokenString);
       break;
     case MAIS:     fprintf(listing, "+\n");  break;
     case MENOS:    fprintf(listing, "-\n");  break;
     case VEZES:    fprintf(listing, "*\n");  break;
     case SOBRE:     fprintf(listing, "/\n");  break;
     case MENORQ:       fprintf(listing, "<\n");  break;
     case MENORI:       fprintf(listing, "<=\n"); break;
     case MAIORQ:       fprintf(listing, ">\n");  break;
     case MAIORI:       fprintf(listing, ">=\n"); break;
     case IGUAL:       fprintf(listing, "==\n"); break;
     case DIF:       fprintf(listing, "!=\n"); break;
     case RECEBE:   fprintf(listing, "=\n");  break;
     case PONVIR:     fprintf(listing, ";\n");  break;
     case VIRG:    fprintf(listing, ",\n");  break;
     case EPAREN:   fprintf(listing, "(\n");  break;
     case DPAREN:   fprintf(listing, ")\n");  break;
     case ECOLCH: fprintf(listing, "[\n");  break;
     case DCOLCH: fprintf(listing, "]\n");  break;
     case ECHAVE:   fprintf(listing, "{\n");  break;
     case DCHAVE:   fprintf(listing, "}\n");  break;
     case FIMARQV:  fprintf(listing,"%s %s\n",  "FIMARQV", "EOF"); break;
     case NUM: fprintf(listing, "NUM, val = %s\n",tokenString); break;
     case ID: fprintf(listing, "ID, name = %s\n",tokenString); break;
     case ERROR: fprintf(listing, "ERROR: %s\n",tokenString); break;
     default:
       fprintf(listing,"Unknown token: %d\n",token);
   }
 }
 
TreeNode * newStmtNode(StmtKind kind) {
  TreeNode * t = (TreeNode *) malloc(sizeof(TreeNode));
  int i;
  if (t==NULL) fprintf(listing,"Out of memory error at line %d\n",lineno);
  else {
    for (i=0;i<MAXCHILDREN;i++) t->child[i] = NULL;
    t->sibling = NULL;
    t->nodekind = StmtK;
    t->kind.stmt = kind;
    t->lineno = lineno;
  }
  return t;
}

TreeNode * newExpNode(ExpKind kind) {
  TreeNode * t = (TreeNode *) malloc(sizeof(TreeNode));
  int i;
  if (t==NULL) fprintf(listing,"Out of memory error at line %d\n",lineno);
  else {
    for (i=0;i<MAXCHILDREN;i++) t->child[i] = NULL;
    t->sibling = NULL;
    t->nodekind = ExpK;
    t->kind.exp = kind;
    t->lineno = lineno;
    t->type = Void;
  }
  return t;
}

TreeNode * newDeclNode(DeclKind kind) {
  TreeNode * t = (TreeNode *) malloc(sizeof(TreeNode));
  int i;
  if (t==NULL)
    fprintf(listing,"Out of memory error at line %d\n",lineno);
  else {
    for (i=0;i<MAXCHILDREN;i++) t->child[i] = NULL;
    t->sibling = NULL;
    t->nodekind = DeclK;
    t->kind.decl = kind;
    t->lineno = lineno;
    t->type = Void;
  }
  return t;
}

char * copyString(char * s)
{ int n;
  char * t;
  if (s==NULL) return NULL;
  n = strlen(s)+1;
  t = malloc(n);
  if (t==NULL)
    fprintf(listing,"Out of memory error at line %d\n",lineno);
  else strcpy(t,s);
  return t;
}

static int indentno = 0;

#define INDENT indentno+=2
#define UNINDENT indentno-=2

static void printSpaces(void) {
  int i;
  for (i=0;i<indentno;i++)
    fprintf(listing," ");
}

void printTypes(TreeNode* tree) {
  if (tree->child[0] != NULL) {
    switch (tree->child[0]->type) {
      case Integer:
        fprintf(listing,"int");
        break;
      case Void:
        fprintf(listing,"void\n");
        break;
      case Array:
        fprintf(listing,"int array");
        break;
      default: return;
    }
  } else {
    switch (tree->type) {
      case Integer:
        fprintf(listing,"int");
        break;
      case Void:
        fprintf(listing,"void\n");
        break;
      case Array:
        fprintf(listing,"int array");
        break;
      default: return;
    }
  }
}

void printTree( TreeNode * tree )
{ int i;
  INDENT;
  while (tree != NULL) {

    if (tree->nodekind==StmtK) {
      printSpaces();
      switch (tree->kind.stmt) {
        case IfK:
          fprintf(listing,"If\n");
          break;
        case WhileK:
          fprintf(listing, "While\n");
          break;
        case CompoundK:
          fprintf(listing, "-----------------\n");
          break;
        case ReturnK:
          fprintf(listing, "Retorno\n");
          break;
        case AssignK:
          fprintf(listing, "Atribuição\n");
          break;
        default:
          fprintf(listing, "TIPO DESCONHECIDO\n");
          break;
        }
    }
    else if (tree->nodekind==ExpK) {
      if (tree->kind.exp != TypeK) printSpaces();
      switch (tree->kind.exp) {
        case OpK:
          fprintf(listing,"Operador: ");
          printToken(tree->attr.op, "\0");
          break;
        case ConstK:
          fprintf(listing,"Constante: %d\n",tree->attr.val);
          break;
        case IdK:
          fprintf(listing,"Id: %s\n",tree->attr.name);
          break;
        case TypeK: break;
        case NameArrK:
          fprintf(listing,"Nome Vetor: %s\n", tree->attr.name);
          break;
        case FunCallK:
          fprintf(listing, "Chamada Função: %s\n", tree->attr.name);
          break;
        case CalcK:
          fprintf(listing, "Operação:\n");
          //printToken(tree->child[1]->attr.op, "\0");
          break;
        default:
          fprintf(listing,"TIPO DESCONHECIDO\n");
          break;
      }
    }
    else if (tree->nodekind==DeclK) {
      printSpaces();
      switch (tree->kind.decl) {
        case FunK :
          fprintf(listing, "Função: ");
          printTypes(tree);
          fprintf(listing, " %s()\n", tree->attr.name);
          break;
        case VarK :
          fprintf(listing, "Variável: ");
          printTypes(tree);
          fprintf(listing, " %s\n", tree->attr.name);
          break;
        case ArrVarK :
          fprintf(listing, "Vetor: ");
          printTypes(tree);
          fprintf(listing," %s[%d]\n", tree->attr.vetor.nome, tree->attr.vetor.tamanho);
          break;
        case ArrParamK :
          fprintf(listing, "Parâmetro Vetor: %s\n", tree->attr.name);
          break;
        case ParamK :
          fprintf(listing, "Parâmetro: ");
          printTypes(tree);
          if (tree->attr.name != NULL) {
            fprintf(listing, " %s\n", tree->attr.name);
            
          }
          break;
        default:
          fprintf(listing, "DECLARAÇÃO DESCONHECIDA\n");
          break;
        }
      }
    else fprintf(listing,"TIPO INVÁLIDO\n");
    for (i=0;i<MAXCHILDREN;i++) if (tree->child[i] != NULL) printTree(tree->child[i]);
    tree = tree->sibling;
  }
  UNINDENT;
}
