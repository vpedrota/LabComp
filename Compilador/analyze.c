#include "globals.h"
#include "symtab.h"
#include "analyze.h"
#include "util.h"

typedef int bool;

static void traverse(TreeNode * t, void (* preProc) (TreeNode *), void (* postProc) (TreeNode *)) {
  if (t != NULL) {
    preProc(t);
    for (int i=0; i < MAXCHILDREN; i++) {
      traverse(t->child[i], preProc, postProc);
    }
    postProc(t);
    traverse(t->sibling, preProc, postProc);
  }
}

static void popAfterInsertProc(TreeNode * t) {

  if (t->nodekind == StmtK) {
    if (t->kind.stmt == CompoundK) {
      popScope();
    }
  }

  if (t==NULL) return;
  else return;
}

static void nullProc(TreeNode * t) {
  if (t==NULL) return;
  else return;
}

static void symbolError(TreeNode * t, char * message) {
  fprintf(listing,"ERRO SEMANTICO: %s LINHA: %d\n", t->attr.name, t->lineno);
  Error = 1;
  exit(-1);
}

static bool isFirstCompoundK = 0;
static int locationCounter = 0;

static void insertNode(TreeNode * t) {
  switch (t->nodekind) {
    case ExpK: {
      switch (t->kind.exp){
        case IdK:
        case NameArrK:
        case FunCallK: {
          if (st_lookup_all_scope(t->attr.name) == NULL){
            symbolError(t, "SÍMBOLO NÃO DEFINIDO");
          } else {
            BucketList list = st_lookup_all_scope(t->attr.name);
            t->type = list->type;
            insertLines(t->attr.name, t->lineno);
          }
        }
        default:
          break;
      }
      break;
    }
    case StmtK: {
      switch (t->kind.stmt) {
        case CompoundK: {
          if (!isFirstCompoundK) {
            Scope scope = (Scope) malloc(sizeof(struct ScopeListRec));
	    scope->name = currScope()->name;
            scope->parent = currScope();
            pushScope(scope);
          }
          isFirstCompoundK = 0;
          break;
        }
        default:
          break;
      }
      break;
    }
    case DeclK: {
      switch (t->kind.decl) {
        case FunK: {
          locationCounter = 0;

          if (st_lookup_scope(t->attr.name) != NULL) {
            symbolError(t, "REDEFINIÇÃO DA FUNÇÃO");
            break;
          }

          if (strcmp(currScope()->name, "global") == 0) {
            st_insert(currScope()->name, t->attr.name, t->child[0]->type, t, locationCounter++);
          }
          Scope scope = (Scope) malloc(sizeof(struct ScopeListRec));
          scope->name = t->attr.name;
          scope-> parent = currScope();
          pushScope(scope);
          isFirstCompoundK = 1;
          break;
        }

        case VarK: {

          if (st_lookup(t->attr.name) != NULL) {
            symbolError(t, "REDEFINIÇÃO DA VARIÁVEL");
            break;
          }

          if (t->child[0]->type == Void) {
            symbolError(t, "VARIÁVEL NÃO DEVERIA SER DO TIPO VOID");
            break;
          }

          st_insert(currScope()->name, t->attr.name, t->child[0]->type, t, locationCounter++);
          break;
        }

        case ArrVarK: {

          if (t->child[0]->type == Void) {
            symbolError(t, "REDEFINIÇÃO DE VETOR");
            break;
          }

          if (st_lookup(t->attr.vetor.nome) != NULL) {
            symbolError(t, "VETOR JÁ DECLARADO");
            break;
          }

          st_insert(currScope()->name, t->attr.vetor.nome, t->child[0]->type, t, locationCounter++);
          break;
        }

        case ArrParamK: {
        
          if (t->child[0]->type == Void) {
            symbolError(t, "PARÂMETRO VETOR NÃO DEVERIA SER DO TIPO VOID");
            break;
          }

          if (st_lookup(t->attr.name) != NULL) {
            symbolError(t, "REDEFINIÇÃO DE PARÂMETRO VETOR");
            break;
          }

          st_insert(currScope()->name, t->attr.name, t->child[0]->type, t, locationCounter++);
          break;
        }

        case ParamK: {

          if (t->attr.name != NULL) {
            if (st_lookup(t->attr.name) != NULL) {
              symbolError(t, "REDEFINIÇÂO DE PARÂMETRO");
              break;
            }

            st_insert(currScope()->name, t->attr.name, t->child[0]->type, t, locationCounter++);
          }
          break;
        }
        default:
          break;
      }
      break;
    }
    default:
      break;
   }
 }

static void typeError(TreeNode * t, char * message) {
  fprintf(listing,"ERRO DE TIPO: %s LINHA: %d\n",message,t->lineno);
  Error = 1;
  exit(-1);
}

static void checkNode(TreeNode * t) {
  switch (t->nodekind) {
    case StmtK:
      switch (t->kind.stmt) {
        case AssignK: {
          if (t->child[0]->attr.vetor.tipo == Array) {
            typeError(t->child[0], "ATRIBUIÇÃO PARA VARIÁVEL VETOR");
          }

          if (t->child[0]->attr.type == Void) {
            typeError(t->child[0], "ATRIBUIÇÃO PARA VARIÁVEL VOID");
          }
          break;
        }
        case ReturnK: {
          const TreeNode * funcDecl;
          const ExpType funcType = funcDecl->type;
          const TreeNode * expr = t->child[0];

          if (funcType == Void && (expr != NULL && expr->type != Void)) {
             typeError(t,"RETORNO DE VALOR NÃO ESPERADO");
           } else if (funcType == Integer && (expr == NULL || expr->type == Void)) {
             typeError(t,"RETORNO DE VALOR ESPERADO");
           }
        }
        default:
          break;
       }
       break;
     default:
       break;
   }
}

void typeCheck(TreeNode * syntaxTree) {
  traverse(syntaxTree, nullProc, checkNode);
}

void buildSymtab(TreeNode * syntaxTree) {

  Scope globalScope = (Scope) malloc(sizeof(struct ScopeListRec));
  globalScope->name = "global";
  pushScope(globalScope);

  TreeNode * fun_declaration = newDeclNode(FunK);
  fun_declaration->type = Integer;

  TreeNode * type_specifier = newExpNode(TypeK);
  type_specifier->attr.type = INT;

  TreeNode * compound_stmt = newStmtNode(CompoundK);
  compound_stmt->child[0] = NULL;
  compound_stmt->child[1] = NULL;

  fun_declaration->lineno = 0;
  fun_declaration->attr.name = "input";
  fun_declaration->child[0] = type_specifier;
  fun_declaration->child[1] = NULL;
  fun_declaration->child[2] = compound_stmt;

  st_insert("global", "input", Integer, fun_declaration, 1);
  

  fun_declaration = newDeclNode(FunK);
  fun_declaration->type = Void;

  type_specifier = newDeclNode(FunK);
  type_specifier->attr.type = VOID;

  TreeNode * params = newDeclNode(ParamK);
  params->attr.name = "arg";
  params->child[0] = newExpNode(TypeK);
  params->child[0]->attr.type = INT;

  compound_stmt = newStmtNode(CompoundK);
  compound_stmt->child[0] = NULL;
  compound_stmt->child[1] = NULL;

  fun_declaration->lineno = 0;
  fun_declaration->attr.name = "output";
  fun_declaration->child[0] = type_specifier;
  fun_declaration->child[1] = params;
  fun_declaration->child[2] = compound_stmt;

  st_insert("global", "output", Void, fun_declaration, 0);

  traverse(syntaxTree, insertNode, popAfterInsertProc);
  popScope();
  
   printSymTab(listing);
  
}

