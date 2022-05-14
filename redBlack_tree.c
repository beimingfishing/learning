
//----------------------------------所有的注释都是解释下方的代码！！！！！！！


#include<stdio.h>
#include<malloc.h>

//定义红黑树节点结构 结构体
//红黑树的内容包括 它的值、两个子节点以及它的父节点 和描述树的特性的节点颜色
//定义红色和黑色
#define black 0
#define red 1

typedef struct Node
{
    struct Node * parent; //父节点
    struct Node * left_N;   //左子节点
    struct Node * right_N;  //右子节点
    int value;            //值
    int color;            //颜色， 0 代表黑色， 1 代表红色
}Node;

//-----------------------------所有函数都在这个地方定义
//定义左右
#define right 1
#define left  0

void remove(Node* mode);//删除目标节点
int revolve(Node* node);//定义旋转函数
int changeColor_upORnot(Node* node);//定义改变颜色函数

//插入新节点的函数，同时承担创建树的功能
Node* insert_Node();
//备注： 所有的节点都通过insert创建，向上层程序隐藏实现，上层程序最多只传入指针作为头结点
//和将值传递进入作为插入的值


//-----------------------------------------------------------------------------
//接下来时实现函数的区域
Node* insert_Node(Node* T, int value){

    //如果T是空指针，则创建头结点，并且初始化头结点。
    if(T == NULL){
        T = (Node*)malloc(sizeof(Node));
        T->parent = NULL;
        T->left_N = NULL;
        T->right_N = NULL;
        T->color = black;
        T->value = value;
    }
    //头结点不为空 --> 在树里面插入节点
    else{
        //设置一个标志用来标志查询的方向
        int direction ;
        Node* point = T;     //创建指针用于在树里面遍历
        do{
            // 如果这个数已经在树中出现，则不进行操作
            if(value == point->value) return T;   
            
            //与当前节点进行比较，如果大于当前节点的值则向右， 如果小于当前节点则向左,并且记录下跳转的方向
            if(value > point->value) { 
                direction = right;
                if(point->right_N == NULL) break;
                else point = point->right_N;
            }
            else{
                direction = left;
                if(point->left_N == NULL) break;
                else point = point->left_N;
            }
        }while(1);

        //如果这时候point指向的节点是黑色的(point所指向的节点是插入值的父节点)，那么直接插入红色的节点对整体树的结构
        //没有造成破坏，则直接插入
        if(point->color ==black){
            if(direction == right) {
                point->right_N = (Node*)malloc(sizeof(Node));
                (point->right_N)->parent =NULL;
                (point->right_N)->right_N =NULL;
                (point->right_N)->left_N =NULL;
                (point->right_N)->color = red;
                (point->right_N)->value =value;
            }
            else{
                point->left_N = (Node*)malloc(sizeof(Node));
                (point->left_N)->parent =NULL;
                (point->left_N)->right_N =NULL;
                (point->left_N)->left_N =NULL;
                (point->left_N)->color = red;
                (point->left_N)->value =value;
            }
            //插入结束，返回树
            return T;
            
        }
        //如果将要插入节点的父节点是红色得，则需要处理树，使得其符合红黑树的特性
        else{
            //初始化新节点
            Node* newNode = (Node*)malloc(sizeof(Node));
            newNode->parent = NULL;
            newNode->right_N = NULL;
            newNode->left_N = NULL;
            newNode->color = red;
            newNode->value =value;
            
            //将节点放入树中，随后再做调整
            if(direction == right) point->right_N = newNode;
            else point->left_N =newNode;

            //因为出现了两个红色的节点，所以接下来对树进行调整，大致的可以划分为两种情况
            
            //第一种情况,父节点的兄弟节点（叔叔节点是黑色）， 操作是只进行一次变色并且旋转

            Node* parent = point->parent;
            //如果父节点是左节点
            if(point = parent->left_N && (parent->right_N == NULL || parent->right_N->color == black)){
                changeColor_upORnot(parent);
            }
            //如果父节点是右节点
            else if(point = parent->right_N && (parent->left_N == NULL || parent->left_N->color == black)){
                changeColor_upORnot(parent);
            }
            
            //第二种情况，父节点的兄弟节点（叔叔节点是红色），则一直递归的调整直到根节点



        }



    }
}

//改变颜色的函数，节点及兄弟节点两个都是红色的时候两个以及父节点都改变颜色，随后判断改变颜色后
//的节点与父节点是否冲突，冲突时递归调用  ;  当出现第二种情况，兄弟节点是黑色或者为NULL的时候改变
//自己和父节点的颜色，随后利用旋转将树调整回来
int changeColor_upORnot(Node* node){
    //要调整的是父节点，所以是考虑 父节点和 父节点的父节点 以及 父节点的兄弟节点 之间的关系
    node = node->parent;
    Node* parent = node->parent;
    //对父节点和兄弟节点做检查
    if(node == parent->left_N && (parent->right_N == NULL || parent->right_N->color == black) ){
        node->color = black;
        parent->color = red;
        revolve(node);
        return 1;
    }
    else if(node == parent->right_N && (parent->left_N == NULL || parent->left_N->color == black) ){
        node->color =black;
        parent->color =red;
        revolve(node);
        return 1;
    }
    //如果父节点和其兄弟节点的颜色都是红色，则三个节点都改变颜色，然后向上检查。
    if(node == parent->left_N && parent->right_N->color == red){
        parent->color = red;
        parent->left_N->color = black;
        parent->right_N->color = black;
        if(parent->parent->color == red) changeColor_upORnot(parent);
        return 1;
    }
    else if(node == parent->right_N && parent->left_N->color == red){
        parent->color = red;
        parent->left_N->color = black;
        parent->right_N->color = black;
        if(parent->parent->color == red) changeColor_upORnot(parent);
        return 1;
    }
    return 0;
}

int revolve(Node* node){
    //创建一个指针来确定父节点，方便确定位置
    Node* parent = node ->parent;
    //创建一个指针确定祖父节点，用来旋转时连接
    Node* grandpaParent = parent->parent;
    //如果node是左子树，则这个树需要
    if(node == parent->left_N){
        //父节点是左右节点时分别的旋转

        //旋转时祖父节点的子节点应该变成当前节点 （下列条件代码块中三行代码的解释）

        //父节点的子节点在旋转后变成了当前节点的其中一个子节点 （判断是哪一个需要看方向）

        //父节点变成当前节点的子节点

        if(parent == grandpaParent->left_N){
            grandpaParent->left_N = node;
            if(parent != NULL)parent->left_N = node->right_N;
            node->right_N = parent;
        }
        else if(parent == grandpaParent->right_N){
            grandpaParent->right_N = node;
            if(parent != NULL)parent->left_N = node->right_N;
            node->right_N = parent;
        }
    }
    else if(node == parent->right_N){
        if(parent == grandpaParent->left_N){
            grandpaParent->left_N = node;
            if(parent != NULL)parent->right_N = node ->left_N;
            node->left_N = parent;
        }
        else if(parent == grandpaParent->right_N){
            grandpaParent->right_N = node;
            if(parent != NULL)parent->right_N = node->left_N;
            node->left_N = parent;
        }
    }
}


//remove函数实现
void remove(Node* node){

}