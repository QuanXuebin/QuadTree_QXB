#include"QuadTree.h"

/*  
   矩形分割示意图(rect split diagrammatic sketch)
    _________________     
   |        |        |
   |   TL   |   TR   |
   |--------|--------|
   |   BL   |   BR   |
   |________|________|
   
*/

int equalPoint(Point p1,Point p2){
	if(p1.x == p2.x && p1.y == p2.y){
		return 1;
	}
	return 0;
}

/*打印坐标*/
void printfPoint(Point point){
	printf("(%ld,%ld)",point.x,point.y);
}


/**在建树的过程中分割矩形区域，并且返回相应的子矩形**/
QuadRect splitRect(int i,QuadNode *qnode) {
	QuadRect rect;
	long length  = qnode->rect.right - qnode->rect.left;
	long width   = qnode->rect.top   - qnode->rect.bottom;
	length       = length /2;
	width        = width  /2;
    switch(i) {
		case 0:
		   rect.right  = qnode->rect.right;
		   rect.left   = qnode->rect.left + length;
		   rect.top    = qnode->rect.top;
		   rect.bottom = qnode->rect.bottom + width;
		break;
		
		case 1:
		   rect.right  = qnode->rect.right - length;
		   rect.left   = qnode->rect.left;
		   rect.top    = qnode->rect.top;
		   rect.bottom = qnode->rect.bottom + width;
		break;
		
		case 2:
		   rect.right  = qnode->rect.right - length;
		   rect.left   = qnode->rect.left;
		   rect.top    = qnode->rect.top - width;
		   rect.bottom = qnode->rect.bottom;
		break;
		
		case 3:
		   rect.right  = qnode->rect.right;
		   rect.left   = qnode->rect.left + length;
		   rect.top    = qnode->rect.top - width;
		   rect.bottom = qnode->rect.bottom;
		break;
	}
	return rect;
}

/**划分矩形区域,判断点该分配到哪个区域 0 表示分配到第一象限  1 表示分配到第二象限 2 表示分配到第三象限 3表示分配到第四象限**/
int determine_rect(QuadRect rect,long x,long y) {
	long length  = rect.right - rect.left;
	long width   = rect.top   - rect.bottom;
	if(           x <  rect.right && x >= (rect.left + length/2) && y <   rect.top && y >= (rect.bottom + width/2)) {
		return 0;
	} else if (   x < (rect.right - length/2) && x >=  rect.left && y <   rect.top && y >= (rect.bottom + width/2)) {
		return 1;
	} else if (   x < (rect.right - length/2) && x >=  rect.left && y <  (rect.top - width/2) && y >=  rect.bottom) {
		return 2;
	} else if (   x <  rect.right && x >= (rect.left + length/2) && y <  (rect.top - width/2) && y >=  rect.bottom) {
		return 3;	
	}
	return -1;
}

void QuadInsert(Point *point, QuadNode *qnode) {
	
	int i = 0;
	if(qnode->isLeaf == 1) {
		if (qnode->isEmpty == 1) {
			qnode->point   = *point;
			qnode->isEmpty = 0;
		} else {
			if(point->x == qnode->point.x && point->y == qnode->point.y){
				printf("return\n");
				return;
			}
			qnode->isLeaf = 0;
			for(i=0;i<4;i++) {
				qnode->child[i]          = (QuadNode *)calloc(1, sizeof(QuadNode));
				qnode->child[i]->rect    = splitRect(i,qnode);
				qnode->child[i]->isEmpty = 1;
				qnode->child[i]->isLeaf  = 1;
	
				if(   qnode->child[i]->rect.left   <= qnode->point.x 
				   && qnode->child[i]->rect.right  >  qnode->point.x  
				   && qnode->child[i]->rect.bottom <= qnode->point.y
				   && qnode->child[i]->rect.top    >  qnode->point.y) {
					qnode->child[i]->point = qnode->point;
					qnode->child[i]->isEmpty = 0;
					memset(&qnode->point,0,sizeof(Point));
				}
				if(   qnode->child[i]->rect.left   <= point->x 
				   && qnode->child[i]->rect.right  >  point->x 
				   && qnode->child[i]->rect.bottom <= point->y
				   && qnode->child[i]->rect.top    >  point->y) {
				   QuadInsert(point,qnode->child[i]);
				}
			}
		}
	} else {
		for(i=0; i<4; i++) {
			if(   qnode->child[i]->rect.left   <= point->x 
			   && qnode->child[i]->rect.right  >  point->x 
			   && qnode->child[i]->rect.bottom <= point->y
			   && qnode->child[i]->rect.top    >  point->y) {
				  QuadInsert(point,qnode->child[i]);
			}
		}
	}
}

QuadNode *creatRootQuad() {
	QuadNode *tree = NULL;
	tree           = (QuadNode *)malloc(sizeof(QuadNode));
	if(tree) {
		QuadRect rect  = {-180 * 1000000, 180 * 1000000, 90 * 1000000, -90 * 1000000};
		tree->rect     = rect;
	    tree->isEmpty  = 1;
		tree->isLeaf   = 1;
	}
	return tree;
}

void quad_tree_build (QuadNode *tree) {
	int       ret = 0;
	double    posx, posy;
	FILE     *fp    = NULL;
	Point    *point = NULL;
	point = (Point *)malloc(sizeof(Point));

    fp = fopen("edog.txt", "rb");
	if(fp) {
		while (1) {
			ret = fscanf(fp, "%lf %lf",&posy, &posx);
			if (ret != 2) break;
			point->x = (long)(posx * 1000000);
			point->y = (long)(posy * 1000000);
			QuadInsert(point,tree);
 		}
		fclose(fp);
	}
}

void destroy_quadtree(QuadNode *tree) {
	int i;
    if (tree) {
        for (i=0; i<4; i++) {
            if (tree->child[i]) {
                destroy_quadtree(tree->child[i]);
            }
        }
        free(tree);
    }
}

//遍历四叉树
void ergodic_tree(QuadNode *tree){
	int i;
	if(tree->isLeaf) {
		if(!tree->isEmpty){
			printfPoint(tree->point);
		    printf("\n");
		} 
	} else {
		for(i=0;i<4;i++) {
			ergodic_tree(tree->child[i]);
		}
	}
}

void FindPoint(Point point,QuadNode *qnode) {
	int i;
	if(qnode->isLeaf){
		if(qnode->isEmpty) {
			printf("safe! There aren't High-speed camera nearby\n");
		} else {
			printf("There are speed traps nearby\n");
			printfPoint(qnode->point);
			printf("\n");
		}
	} else {
		for(i=0;i<4;i++){
			if(    qnode->child[i]->rect.left   <= point.x 
			    && qnode->child[i]->rect.right  >  point.x 
				&& qnode->child[i]->rect.bottom <= point.y
				&& qnode->child[i]->rect.top    >  point.y) {
				   FindPoint(point,qnode->child[i]);
				   break;
			}
		}
	}
}

#define QUEUE_SIZE  (2 * 1024 * 1024)

/**为四叉树的各个节点设置索引**/
void set_tree_index (QuadNode *tree) {
	QuadNode **queue = malloc(sizeof(QuadNode *) * QUEUE_SIZE);
	QuadNode  *node  = NULL;
	if (!queue) return;
	int tail = 0, head = 0, qnum = 0, index = 0, i;
	
	//put first point in queue
	queue[tail & (QUEUE_SIZE - 1)] = tree;
	tail++ , qnum++;
	while (qnum > 0) {
		node = queue[head & (QUEUE_SIZE - 1)];
        head++; qnum--;
		//如果子节点是叶子节点,并且该节点未保存任何数据，则该节点不设置索引
		if(node->isLeaf) {
			if(node->isEmpty) {
				continue;
			}
		}
		node->index = index++;
		
		for(i=0;i<4;i++) {
			if(node->child[i]) {
				queue[tail & (QUEUE_SIZE - 1)] = node->child[i];
                tail++; qnum++;
                if (qnum > QUEUE_SIZE) printf("queue size overflow !\n");
			}
		}
	}
	free(queue);
}


void push_tree_in_text(QuadNode *tree, FILE *fp) {
	int tail = 0, head = 0, qnum = 0, index = 0, i;
	QuadNode **queue = malloc(sizeof(QuadNode *) * QUEUE_SIZE);
	QuadNode  *node  = NULL;
	if (!queue) return;
	//put first point in queue
	queue[tail & (QUEUE_SIZE - 1)] = tree;
	tail++ , qnum++;
	while (qnum > 0) {
		node = queue[head & (QUEUE_SIZE - 1)];
        head++; qnum--;
		//如果子节点是叶子节点,并且该节点未保存任何数据，则该节点不写入文件
		if(node->isLeaf) {
			if(node->isEmpty) {
				continue;
			}
		}
		for (i=0; i<4; i++) {
            if (!node->child[i]) continue;
            node->child_index[i] = node->child[i]->index;//把孩子节点的index赋值给父亲节点的数组中，这样就建立了父节点与子节点的关联
        }
		fprintf(fp,"%7d (%10d %10d %10d %10d) (%10d %10d) (%7d %7d %7d %7d) isLeaf: %d\r\n",
		        node->index,
		        node->rect.right, node->rect.left, node->rect.top, node->rect.bottom,
                node->point.x, node->point.y,
				node->child_index[0], node->child_index[1], node->child_index[2], node->child_index[3],
				node->isLeaf);
		
		for(i=0;i<4;i++) {
			if(node->child[i]) {
				queue[tail & (QUEUE_SIZE - 1)] = node->child[i];
                tail++; qnum++;
                if (qnum > QUEUE_SIZE) printf("queue size overflow !\n");
			}
		}
	}
	free(queue);
}

void save_tree(QuadNode *tree, char *file) {
	FILE *fp = fopen(file,"wb");
	set_tree_index(tree);
	if(fp) {
		fprintf(fp, "edog %s file v1.0.0 %10d %10d %10d %10d\r\n", "edt", tree->rect.right, tree->rect.left, tree->rect.top, tree->rect.bottom);
		push_tree_in_text(tree,fp);
		fclose(fp);
	}
}

void find_point_from_text(char *file, Point point){
	FILE *fp = fopen(file,"rb");
	char str[4][10] = {};
	long right, left, top, bottom, x = 0, y = 0;
	int  tr, tl, bl, br, isleaf, idx, select, rec, i, index = 0;
	QuadRect rect;
	if(fp) {
		fscanf(fp,"%8s %8s %8s %8s",str[0],str[1],str[2],str[3]);
		
		while (1) {
			fseek(fp,66 + index * 123,SEEK_SET);
			rec = fscanf(fp,"%d (%ld %ld %ld %ld) (%ld %ld) (%d %d %d %d) isLeaf: %d",&idx,&right,&left,&top,&bottom,&x,&y,&tr,&tl,&bl,&br,&isleaf);
			rect.right  = right;
			rect.left   = left;
			rect.top    = top;
			rect.bottom = bottom;
			select      = determine_rect(rect,point.x,point.y);//根据点和当前节点的矩形，判断该点会落在哪个区域，然后根据索引继续搜索
			switch(select){
			   case 0:  index = tr; break;
		       case 1:  index = tl; break;
		       case 2:  index = bl; break;
		       case 3:  index = br; break; 
               default: index++;    break;			   
			}
			if(point.x < right && left < point.x && point.y < top && point.y > bottom) {
				if(isleaf) {
					printf("(%ld,%ld)\n",x,y);	
					break;
				}
			}
			if(rec < 11) {
				printf("there is no point that you want to search !\n");
				break;
			}
			printf("index = %d \n",index);
		}	
	}
}

int main() {
	Point point;
	double x,y;
	QuadNode *tree = NULL;
	QuadRect *rect = NULL;
	rect = (QuadRect *)malloc(sizeof(QuadRect));
	tree  = creatRootQuad();
	quad_tree_build (tree);
	save_tree(tree,"etest.edt");
	destroy_quadtree(tree);	
	point.x = (long)(40.6561759 * 1000000);
	point.y = (long)(55.6055057 * 1000000);
	//printf("please input point in this format: xxx yyy\n");
	//scanf("%lf %lf",&x,&y);
	//point.x = (long)(x * 1000000);
	//point.y = (long)(y * 1000000);
	find_point_from_text("etest.edt",point);
	return 0;
}

