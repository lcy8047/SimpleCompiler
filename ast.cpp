#include <cstdio>
#include <cstdlib>
#include <cstring>

#include <iostream>
#include <unordered_map>

#include "ast.hpp"

using namespace std;

unordered_map<string, int> funcList;

void appendSon( Node *parent, Node *son )
{
    Node *nodeIter;
    
    if( parent->son == NULL ) {
        parent->son = son;
    }
    else {
        nodeIter = parent->son;
        while( nodeIter->bro != NULL ) {
            nodeIter = nodeIter->bro;
        }
        nodeIter->bro = son;
    }
}

void appendBro( Node *node, Node *bro )
{
    Node *nodeIter;
    
    nodeIter = node;
    while( nodeIter->bro != NULL ) {
        nodeIter = nodeIter->bro;
    }
    nodeIter->bro = bro;
}

string generateCode( Node *node, FuncNode *curFunc )
{
    Node *nodeIter;
    string code;

    if( node == NULL )
        return code;

    if ( node->type == eType::ROOT ) {
        bool bFindMainFunc = false;

        code.append("mst\n");
        code.append("cup 0 main\n");
        code.append("stp\n");

        nodeIter = node->son;
        while( nodeIter != NULL ) {
            if( ((FuncNode *)nodeIter)->getID().compare("main") == 0 )
                bFindMainFunc = true;
            nodeIter = nodeIter->bro;
        }
        if( !bFindMainFunc ) {
            cerr << "not found main function" << endl;
            exit(0);
        }
        nodeIter = node->son;
        while( nodeIter != NULL ) {
            code.append( generateCode(nodeIter, NULL) );
            nodeIter = nodeIter->bro;
        }
    }
    else if( node->type == eType::FUNCTION ) {
        FuncNode *funcNode = (FuncNode *)node;
        int varCount = 0;

        nodeIter = funcNode->son;
        while( nodeIter != NULL && nodeIter->type == eType::VARIABLE ) {
            funcNode->initVar( ((VarNode *)nodeIter)->getID() );
            nodeIter = nodeIter->bro;
        }

        code.append( funcNode->generateCode() );
        while( nodeIter != NULL ) {
            code.append( generateCode(nodeIter, funcNode) );
            nodeIter = nodeIter->bro;
        }
    }
    else if( node->type == eType::FUNCTIONCALL ) {
        FunccallNode *funccallNode = (FunccallNode *)node;

        code.append("mst\n");

        nodeIter = funccallNode->son;
        while( nodeIter != NULL ) {
            if( nodeIter->type == eType::VARIABLE ) {
                code.append( ((VarNode *)nodeIter)->generateCode(curFunc, false) );
            }
            else if( nodeIter->type == eType::INT ) {
                code.append( ((IntNode *)nodeIter)->generateCode() );
            }
            else {
                code.append( generateCode(nodeIter, curFunc) );
            }
            nodeIter = nodeIter->bro;
        }

        code.append( funccallNode->generateCode() );
    }
    else if( node->type == eType::RET ) {
        ReturnNode *returnNode = (ReturnNode *)node;

        nodeIter = returnNode->son;
        while( nodeIter != NULL ) {
            if( nodeIter->type == eType::VARIABLE ) {
                code.append( ((VarNode *)nodeIter)->generateCode(curFunc, false) );
            }
            else if( nodeIter->type == eType::INT ) {
                code.append( ((IntNode *)nodeIter)->generateCode() );
            }
            else {
                code.append( generateCode(nodeIter, curFunc) );
            }
            nodeIter = nodeIter->bro;
        }

        code.append( returnNode->generateCode() );
    }
    else {
        nodeIter = node->son;
        while( nodeIter != NULL ) {
            if( nodeIter->type == eType::VARIABLE && 
                node->type == eType::ASSN ) {
                code.append( ((VarNode *)nodeIter)->generateCode(curFunc, true) );
            }
            else if( nodeIter->type == eType::VARIABLE && 
                     node->type != eType::ASSN ) {
                code.append( ((VarNode *)nodeIter)->generateCode(curFunc, false) );
            }
            else if( nodeIter->type == eType::INT ) {
                code.append( ((IntNode *)nodeIter)->generateCode() );
            }
            else {
                code.append( generateCode(nodeIter, curFunc) );
            }
            nodeIter = nodeIter->bro;
        }
        switch( node->type )
        {
        // binary
        case eType::ADD :
        case eType::SUB :
        case eType::MUL :
        case eType::DIV :
        case eType::ASSN :
        {
            BiOpNode *biOpNode = (BiOpNode *)node;
            code.append( biOpNode->generateCode() );
            break;
        }
        // unary
        case eType::NEG :
        {
            UnaryOpNode *unaryOpNode = (UnaryOpNode *)node;
            code.append( unaryOpNode->generateCode() );
            break;
        }
        default:
            cout << "ERR in generateCode func : " << curFunc->getID() << endl;
            cout << "node type : " << (int) node->type << endl;
            code.append("ERR in genCode\n");
            break;
        }
    }
    return code;
}

string FunccallNode::generateCode( void ) const
{
    stringstream ss;
    if( funcList.find( id ) != funcList.end() )
        ss << "cup " << funcList[id] << ' ' << id << '\n';
    else {
        cerr << "not valid function : " << id << endl;
    }
    return ss.str();
}

FuncNode::FuncNode( string &_id, Node *paramList )
        : id(_id)
        , Node(eType::FUNCTION, paramList, NULL)
{
    int paramCount = 0;
    Node *nodeIter = son;
    while( nodeIter != NULL && nodeIter->type == eType::VARIABLE ) {
        paramCount ++;
        nodeIter = nodeIter->bro;
    }
    funcList.emplace(id, paramCount);
}