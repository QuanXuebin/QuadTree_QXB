#include<stdio.h>
#include<stdlib.h>
#include<string.h>

typedef struct point_t{
	long x;
	long y;
}Point;

/*矩形结构*/
typedef struct quadrect_t{
    long left, right, top, bottom;
}QuadRect;

/*四叉树节点的结构*/
typedef struct quadnode_t {
   QuadRect rect;//节点所代表的矩形区
   Point    point;
   int      isEmpty;
   int      isLeaf;
   int      index;
   int      child_index[4];//用来保存四叉树的子节点的索引,这个索引在最后查找四叉树的时候能发挥巨大的作用
   struct   quadnode_t *child[4];
}QuadNode;


void quad_tree_build (QuadNode *tree);
void ergodic_tree(QuadNode *tree);
void FindPoint(Point point,QuadNode *qnode);
void save_tree(QuadNode *tree, char *file);
void destroy_quadtree(QuadNode *tree);
void find_point_from_text(char *file, Point point);
