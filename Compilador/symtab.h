#ifndef _SYMTAB_H_
#define _SYMTAB_H_

#include "globals.h"

#define SIZE 211

typedef struct LineListRec {
  int lineno;
  struct LineListRec * next;
} * LineList;

typedef struct BucketListRec {
  char * name;
  TreeNode * treeNode;
  LineList lines;
  int memloc;
  struct BucketListRec * next;
  ExpType type;
} * BucketList;

typedef struct ScopeListRec {
  char * name;
  int nestedLevel;
  struct ScopeListRec *parent;
  BucketList hashTable[SIZE];
} * Scope;

Scope globalScope;
static Scope scopeList[SIZE];
static int sizeOfList = 0;

static Scope scopeStack[SIZE];
static int topScope = 0;

Scope newScope(char * scopeName);
void popScope(void);
void pushScope(Scope scope);
void insertScopeToList(Scope scope);

void st_insert(char * scopeName, char * name, ExpType type, TreeNode * treeNode, int loc);

Scope currScope();

BucketList st_lookup(char * name);

Scope st_lookup_scope(char * scopeName);
BucketList st_lookup_all_scope(char * name);
void insertLines(char* name, int lineno);

void printSymTab(FILE * listing);

#endif

