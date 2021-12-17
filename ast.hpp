#pragma once

#include <iostream>
#include <string>
#include <sstream>
#include <unordered_map>
#include "ast.hpp"

using namespace std;

enum class eType{  
    ROOT,
    FUNCTION,
    RET,
    VARIABLE,
    /* operator types */
    FUNCTIONCALL,
    // binary
    ADD,
    SUB,
    MUL,
    DIV,
    ASSN,
    // unary
    NEG,
    // constant
    INT,
};

class Node;
class FuncNode;
class VarNode;
class FunccallNode;
class BiOpNode;
class UnaryOpNode;
class IntNode;
class ReturnNode;

void appendSon      ( Node *parent, Node *son );
void appendBro      ( Node *node, Node *bro );
string generateCode ( Node *node, FuncNode *curFunc );

class Node {
public:
    Node(eType t, Node *s, Node *b)
        : type(t)
        , son(s)
        , bro(b)
    {}

    string generateCode(void) const
    {
        stringstream ss;

        ss << "mst" << '\n'
           << "cup 0 main" << '\n'
           << "stp"<<'\n';
        
        return ss.str();
    }

    eType type;
    Node *son;
    Node *bro;
};

class FuncNode: public Node {
public:
    FuncNode( string &_id, Node *paramList );

    string generateCode( void ) const
    {
        stringstream ss;        

        ss << id << ": " << "ssp " << varCount + 3 << '\n'; 

        return ss.str();
    }

    string getID( void ) const
    {
        return id;
    }

    void initVar( string id )
    {
        varCount ++;
        varList.emplace( id, varCount+2 );
    }

    bool isFunction( void ) const
    {
        return true;
    }

    int getVarIdx( string id )
    {
        int res = -1;
        if( varList.find(id) != varList.end() ) {
            res = varList[id];
        }
        return res;
    }
private:
    string id;
    int varCount;
    unordered_map<string, int> varList;
};

class VarNode: public Node {
public:
    VarNode( string &_id )
        : id(_id)
        , Node(eType::VARIABLE, NULL, NULL)
    {}

    string generateCode( FuncNode *funcNode, bool isStore ) const
    {
        stringstream ss;
        int idx;

        idx = funcNode->getVarIdx( id );
        if( idx == -1 ) {
            cerr << "variable \"" << id << "\" is not declared" << endl;
            exit(0);
        }

        if( isStore )
            ss << "lda " << idx << '\n';
        else
            ss << "lda " << idx << '\n'
               << "ind" << '\n';

        return ss.str();
    }

    string getID( void ) const
    {
        return id;
    }

    void setIdx( int index )
    {
        idx = index;
    }

private:
    string id;
    int idx;
};

class FunccallNode: public Node {
public:
    FunccallNode( string &_id, Node *argList )
        : id( _id )
        , Node( eType::FUNCTIONCALL, argList, NULL )
    {}

    string generateCode( void ) const;

private:
    string id;
};

class BiOpNode: public Node {
public:
    BiOpNode( eType type, Node *operand1, Node *operand2 )
        : Node( type, operand1, NULL )
    {
        operand1->bro = operand2;
    }

    string generateCode( void ) const
    {
        stringstream ss;

        switch( type )
        {
        case eType::ADD :
            ss << "add\n";
            break;
        case eType::SUB :
            ss << "sub\n";
            break;
        case eType::MUL :
            ss << "mul\n";
            break;
        case eType::DIV :
            ss << "div\n";
            break;
        case eType::ASSN :
            ss << "sto\n";
            break;
        default:
            cout << "ERR in Biop GenerateCode" << endl;
            break;
        }

        return ss.str();
    }
private:
};

class UnaryOpNode: public Node {
public:
    UnaryOpNode( eType type, Node *operand1 )
        : Node( type, operand1, NULL )
    {}

    string generateCode( void ) const
    {
        stringstream ss;
        switch( type )
        {
        case eType::NEG :
            ss << "neg\n";
            break;
        default:
            cout << "ERR in Unary op GenerateCode" << endl;
            break;
        }
        return ss.str();
    }
private:
};

class IntNode: public Node {
public:
    IntNode( int _val )
        : val( _val )
        , Node( eType::INT, NULL, NULL )
    {}

    string generateCode( void ) const
    {
        stringstream ss;
        ss << "ldc " << val << '\n';
        return ss.str();
    }
private:
    int val;
};

class ReturnNode: public Node {
public:
    ReturnNode( bool isFunc )
        : bIsFunc( isFunc )
        , Node( eType::RET, NULL, NULL )
    {}

    string generateCode( void ) const
    {
        stringstream ss;

        if( bIsFunc )
            ss << "str 0" << '\n' 
               << "retf" << '\n';
        else
            ss << "retp" << '\n';
        return ss.str();
    }
private:
    bool bIsFunc;
};