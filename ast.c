#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "ast.h"


Node *makeBiOpNode( char operator, Node *leftOperand, Node *rightOperand )
{
    Node *newNode = calloc( sizeof(Node), 1 );

    switch( operator )
    {
    case '+':
        newNode->type = ADD;
        break;
    case '-':
        newNode->type = SUB;
        break;
    case '*':
        newNode->type = MUL;
        break;
    case '/':
        newNode->type = DIV;
        break;
    case '%':
        newNode->type = MOD;
        break;
    default:
        fprintf(stderr,"Make Binary Operator Node Error\n");
        exit(0);
        break;
    }
    
    newNode->son = leftOperand;
    newNode->son->bro = rightOperand;

    return newNode;
}

Node *makeUnaryOpNode( char operator, Node *operand )
{
    Node *newNode = calloc( sizeof(Node), 1 );

    switch( operator )
    {
    case '+':
        free(newNode);
        newNode = operand;
        break;
    case '-':
        newNode->type = NEG;
        newNode->son = operand;
        break;
    default:
        fprintf(stderr,"Make Unary Operator Node Error\n");
        exit(0);
        break;
    }
    
    

    return newNode;
}

Node *makeConstNumNode( int num )
{
    Node *newNode = calloc( sizeof(Node), 1 );

    newNode->type = INT;
    newNode->val = num;

    return newNode;
}

void generateCode(Node *node)
{
    Node *nodeIter;
    if( node->type == INT ) {
        printf( "ldc %d\n", node->val );
    }
    else {
        nodeIter = node->son;
        while( nodeIter != NULL ) {
            generateCode( nodeIter );
            nodeIter = nodeIter->bro;
        }
        
        switch( node->type )
        {
        case ADD:
            printf("add\n");
            break;
        case SUB:
            printf("sub\n");
            break;
        case MUL:
            printf("mul\n");
            break;
        case DIV:
            printf("div\n");
            break;
        case NEG:
            printf("neg\n");
            break;
        default:
            fprintf(stderr, "generate code: Operator Node Type Error\n");
            exit(0);
            break;
        }
        
    }
}