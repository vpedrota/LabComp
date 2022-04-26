#ifndef _UTIL_H_
#define _UTIL_H_

void printToken(TokenType , const char* );


void printTypes(TreeNode* tree);

TreeNode * newStmtNode(StmtKind);

TreeNode * newDeclNode(DeclKind);

char * copyString( char * );

void printTree( TreeNode * );


#endif
