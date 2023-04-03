#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include<stdbool.h>
#define MAX 100

typedef struct node 
{
    int id; //ID of user
    int numfren; //number of friends of user
    char name[MAX]; //name of user
    int* friends; //friends of user as an array
    struct node* right;  //user to the right
    struct node* left; //user to the left
} node;

struct node* retUser(char str[MAX])
{
    char name[MAX];
    char ID[MAX];
    char strfriends[MAX];

    //copied ID
    char*token=strtok(str,",");
    strcpy(ID,token);

    //copied Name
    token=strtok(NULL,",");
    strcpy(name,token);

    //copied friends' ID
    token=strtok(NULL,",");
    strcpy(strfriends,token);
    //printf("%s\n",(strfriends));

    //creating user nodes
    int id=atoi(ID);
    struct node *user = malloc(sizeof(struct node));
    user->id=id;
    user->friends=(int*)malloc(MAX * sizeof(int));
    strcpy(user->name,name);


    //adding user's friend's IDs
    token = strtok(strfriends,"|");  
    int i=0;
    while( token != NULL ) 
    {

        int temp=atoi(token);
        user->friends[i]=temp;
        i++;
        token = strtok(NULL,"|");
    }
    user->numfren=i;
    if(i==0){user->friends[i]=-1;}
    return user;
}

//search for user with id=key
struct node* search(int key, struct node *users)
{
    //CODE HERE
    if(users==NULL ||users->id==key)
		return users;
	if(key<users->id)
		return(search(key,users->left));
	return(search(key,users->right));
}

//see document for explanattion
struct node*refineUser(struct node*user, struct node *users)
{  
    //CODE HERE 
   user->left=user->right=NULL;
   while(search(user->id,users)!=NULL)
   {
	   user->id=user->id+1;
   }
  if(user->numfren!=0)
  {
    int num=user->numfren;
    int i=0; 
    while(i<num)
    {
        node *curr= search(user->friends[i],users);
        if (curr!=NULL)
        {
            
            if (curr->numfren!=0){ 
            curr->friends[curr->numfren]=user->id;
            curr->numfren++;
            }
            else {
                curr->friends[0]=user->id;
                curr->numfren++;
            }
            i++;
        }
        else
        {
            //deleting that element from array
            for (int j=i;j<num-1;j++)
            {
                user->friends[j]=user->friends[j+1];    
            }
		 user->numfren--;
		 num--;
		
	    }
	 
    }

 }
 
  if (user->numfren==0)
  {
	  user->friends[0]=-1;
  }
return user;
}

//insert user with id
struct node* insertUser(struct node*root,int id,struct node*user)
{  
    //CODE HERE
   user->left=user->right=NULL;
   if(root==NULL)
	{   
		root=user;
		return root;
	}
	node *curr=root;
	node *pre=NULL;
	while(curr!=NULL)
	{
		pre=curr;
		if(id<curr->id)
			curr=curr->left;
		else
			curr=curr->right;
	}
	if(id<pre->id)
		pre->left=user;
	else
		pre->right=user;
	return root;
}

//prints friends list
void friends(int id, struct node *users) 
{
    //CODE HERE
    node * curr=search(id,users);
    if(curr!=NULL)
    {
      if (curr->numfren!=0)
      {
	   for (int i=0;i<curr->numfren;i++)
       {
		   printf("%d\n",curr->friends[i]);
	    }
       }
       else printf("-1\n");
    }
}

//find child node with minimum value (inorder suc) - helper to delete node
struct node *minValueNode(struct node *node) 
{
    //CODE HERE
   node=node->right;
    while(node->left!=NULL)
    {
        node=node->left;
    }
    return node;
}

//deletes itself from its friend's nodes
struct node*deleteFriends(int key, struct node*users)
{   
    //CODE HERE
    if (users!=NULL)
    {
        struct node* found=search(key,users);
        if (found==NULL)
            return NULL;
        int i;
        for(i=0;i<found->numfren;i++)
        { 
            int place;
            struct node* keyfren=search(found->friends[i],users);
            for(int j=0;j<keyfren->numfren;j++)
            {  
                if(keyfren->friends[j]==key)
                    place=j;
            }
            for(int k=place;k<keyfren->numfren-1;k++)
                keyfren->friends[k]=keyfren->friends[k+1];
            keyfren->numfren--;
        }
	    if(found->numfren==0)
		    found->friends[0]=-1;
    }   
	return users;   
}

struct node *deleteNode(struct node *root, int key) 
{
    //CODE HERE
  	struct node* pre;
	struct node* suc;
	pre=NULL;
	struct node* curr;
	curr=root;
	while(curr!=NULL)
	{
		if(key<(curr)->id)
		{
			pre=curr;
			curr=(curr)->left;
		}
		else if(key>(curr)->id)
		{
			pre=curr;
			curr=(curr)->right;
		}
		else
		{	
			break;
		}
	}
	if(curr==NULL)
		return root;
	else
	{
		if(curr->left==NULL && curr->right==NULL)
		{
			suc=NULL;
		}
		else if(curr->left==NULL && curr->right!=NULL)
		{
			suc=curr->right;
		}
		else if(curr->right==NULL && curr->left!=NULL)
		{
			suc=curr->left;
		}
		else
		{
			struct node*minimum;
			minimum=minValueNode(curr);
			minimum->left=curr->left;
			suc=curr->right;
		}
		if(pre==NULL)
		{
			root=suc;
			free(curr);
		}
		else if(curr->id>pre->id)
		{
			pre->right=suc;
	                free(curr);
		}
		else if(curr->id<pre->id)
		{
			pre->left=suc;
	        	free(curr);
		}
	}
	return root;
}

//Print USER's IDs in ascending order
void printInOrder(node* myusers) 
{   
    //CODE HERE
    if(myusers==NULL)
		return;
	printInOrder(myusers->left);
	printf("%d %s\n",myusers->id,myusers->name);
	printInOrder(myusers->right);
}

int main(int argc, char **argv)
{
    node *users=NULL;   
    while(1)
    {

        int opt, id;
        fflush(stdin);
        scanf("%d",&opt);
        char str[MAX];
        switch (opt)
        {
            case 1:
      
                scanf("%s",str);
                struct node*tbins=retUser(str);
                tbins=refineUser(tbins, users);
                users=insertUser(users,tbins->id,tbins);
                break;

              

            case 2:
           
                scanf("%d", &id);
                deleteFriends(id, users);
                users=deleteNode(users, id);
                break;

            

            case 3:
        
                scanf("%d", &id);
                node* result=search(id,users);
                if(result==NULL) 
                    printf("USER NOT IN TREE.\n");
                else{
                    printf("%d\n",result->id);
                }
                break;

            case 4:
                scanf("%d", &id);
                friends(id, users);
                break;
                

            case 5:
                printInOrder(users);
                break;

            case 6:
                exit(0);
                break;

            default: 
                printf("Wrong input! \n");
                break;


                
        }

    }

    
    return 0;

    
}
