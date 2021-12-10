#define ERR -1
#define INT 0
#define ADD 1
#define SUB 2
#define MUL 3
#define DIV 4
#define MOD 5
#define NEG 6

typedef struct Node Node;

struct Node;

struct Node {
    int val;
    int type;
    struct Node *son;
    struct Node *bro;
};

Node *makeBiOpNode          ( char operator, Node *leftOperand, Node *rightOperand );
Node *makeUnaryOpNode       ( char operator, Node *operand );
Node *makeConstNumNode      ( int num );
void generateCode           (Node *node);