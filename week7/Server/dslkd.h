typedef struct elementtype{                     // Create structure
  char username[25];
  char password[25];
  int status;
  int TimeFalse;
}elementtype;
struct node_el{                                 //Create linked-list
  elementtype element;
  struct node_el *next;
};
typedef struct node_el node;
node *root=NULL;
node *cur=NULL;
node *prev=NULL;
node *makenewnode(elementtype element){         //làm mới node
  node *new=(node*)malloc(sizeof(node));
  new->element=element;
  new->next=NULL;
  return new;
}
void insert_first(elementtype element){         //thêm vào đầu
  node *new=(node*)malloc(sizeof(node));
  new=makenewnode(element);
  new->next=root;
  root=new;
  cur=root;
}
void inserafftercur(elementtype element){         //Trong bài không dùng
  node *new=(node*)malloc(sizeof(node));
  new=makenewnode(element);
  new->next=cur->next;
  cur->next=new;
  cur=cur->next;
  prev=cur;
}
void insertbeforecur(elementtype element){        //Trong bài không dùng
  node *new=(node*)malloc(sizeof(node));
  new=makenewnode(element);
  if(root==NULL){
    root=new;
    cur=root;
    prev=NULL;
  }else{
  new->next=cur;
  if(cur==root){
    root=new;
  }
  else
   new->next=cur;
   prev->next=new;
   cur=new;
  }
}
void displaynode(node *p){                          //In danh sách liên kết ra mh
  while(p!=NULL){
    elementtype tmp=p->element;
    printf("%-30s%-20s%-10d%-10d\n",tmp.username,tmp.password,tmp.status,tmp.TimeFalse);
  p=p->next;
  }
}
node* reverse(node *root){                          //Trong bài không dùng
  prev=NULL;
  cur=root;
  while(cur!=NULL){
    root=root->next;
    cur->next=prev;
    prev=cur;
    cur=root;
  }
  return prev;
}
void del_first(){                                 //Trong bài không dùng
  root=root->next;
}
void del_k(char username[]){                            //Trong bài không dùng
  node *p=root;
  node *prev=p, *del;
    if(strcmp(root->element.username,username)==0) del_first();
    else{
      while(strcmp(p->element.username,username)!=0&&p->next!=NULL){
        prev=p;
        p=p->next;
      }
      if(strcmp(p->element.username,username)==0){
        prev=p->next;
      }
    }
}