# include <stdio.h>
# include <stdlib.h>
# include <stdbool.h>
# include <string.h>

# define BUFFER_SIZE 50
# define MAX_ALIAS_LENGTH 20

/* address_t structure */
struct address_t
{
    int octet[4];
    char alias[MAX_ALIAS_LENGTH];
    struct address_t * leftChild, * rightChild, *parent;
    int height, depth;
};


struct address_t * root; // root node

int max(int a,int b); // calculate maximum 
bool add_node_to_tree(struct address_t * node); // adding node to tree
void read_file(char * filename); // read file from CS531_Inet
void display_list_inorder(struct address_t * node); // display list in inorder traversal
void menu(void); // display menu
void add_address_to_tree(void); // adding address to tree
bool check_address_is_valid(int octet[4]); // check if address is valid
bool check_address_exist_inorder(struct address_t * node, int octet[4]); // check if address exist in tree with inorder traversal
void look_up_address_by_alias(void); // look up address in tree
struct address_t * search_alias(char alias[MAX_ALIAS_LENGTH]); // search node by alias in tree
void update_address(void); // update address of alias
void delete_address(void); // delete address in tree
struct address_t * remove_node_in_tree(struct address_t * root, char alias[MAX_ALIAS_LENGTH]); // remove node in tree
void calculate_height(struct address_t * root); // calculate node height
void display_alias_for_location(void); // display alias for location
void search_alias_by_location_inorder(struct address_t * node, int location[2]); // search alias by location in inorder traversal
bool check_location_exist_inorder(struct address_t * node , int location[2]); // check if location exist in tree with inorder traversal
void save_to_file(void); // save tree to file
void save_node_to_file_inorder(FILE * fp,struct address_t * node); // save node to file with inorder traversal


/* calculate maximum */
int max(int a, int b){
    if (a >= b) return a;
    else return b;
}

/* add node to tree */
bool add_node_to_tree(struct address_t * node){
    if (root == NULL) {  // replace root with node if root is NULL
        root = node;
        return true;
    }

    struct address_t * ptr = root;
    int h = 1;
    while (ptr != NULL){
        if (strcmp(ptr->alias,node->alias) > 0){ // if root's alias > node's alias and root has no left child
            if (ptr->leftChild == NULL){
                node->depth ++; // add 1 to node depth
                node->parent = ptr; // set parent to root
                ptr->leftChild = node; // set root's left child as node
                struct address_t * node_ptr = node;
                while (node_ptr->parent != NULL){ // update parent height
                    node_ptr->parent->height = max(h,node_ptr->parent->height);
                    h ++;
                    node_ptr = node_ptr->parent;
                }
                return true;
            }
            else { // set root to its left child
                ptr = ptr->leftChild;
                node->depth ++;
            }
        }
        else if (strcmp(ptr->alias,node->alias) < 0){ // same for root's alias < node's alias
            if (ptr->rightChild == NULL){
                node->depth ++;
                node->parent = ptr;
                ptr->rightChild = node;
                struct address_t * node_ptr = node;
                while (node_ptr->parent != NULL){
                    node_ptr->parent->height = max(h,node_ptr->parent->height);
                    h ++;
                    node_ptr = node_ptr->parent;
                }
                return true;
            }
            else {
                ptr = ptr->rightChild;
                node->depth ++;
            }
        }
        else if (strcmp(ptr->alias,node->alias) == 0){ // if root's alias is equal to node's alias
            printf("Error: %s already exist\n",node->alias);
            return false;
        }

    }
}

/* read file function */
void read_file(char * filename){
    FILE * fp;
    char buffer[BUFFER_SIZE];

    fp = fopen(filename,"r");
    if (fp == NULL){
        printf("File does not exist./n");
        return;
    }

    while (fgets(buffer,BUFFER_SIZE,fp)){
        struct address_t *node = (struct address_t*) malloc (sizeof(struct address_t));
        sscanf(buffer,"%d.%d.%d.%d %s",&node->octet[0],&node->octet[1],&node->octet[2],&node->octet[3],node->alias);
        node->height = 0;
        node->depth = 0;
        node->parent = NULL;
        node->leftChild = NULL;
        node->rightChild = NULL;
        add_node_to_tree(node); // adding node to tree
    }

    fclose(fp);
}

/* display list in inorder traversal */
void display_list_inorder(struct address_t * node){
    if (node == NULL) return;

    display_list_inorder(node->leftChild); // display address in left child

    char address[20];
    sprintf(address,"%d.%d.%d.%d",node->octet[0],node->octet[1],node->octet[2],node->octet[3]);

    char parent[MAX_ALIAS_LENGTH];
    if (node->parent != NULL){
        strcpy(parent,node->parent->alias);
    }
    else {
        strcpy(parent,"NONE");
    }

    printf("%-10s %-15s height:%-3d depth:%-3d parent:%-10s\n",node->alias,address,node->height,node->depth,parent); // display address in root

    display_list_inorder(node->rightChild); // display address in right child
}

/* add address and alias to link list*/
void add_address_to_tree(void){

    /* enter address */
    printf("Enter IPv4 address:");
    int octet[4];
    scanf("%d.%d.%d.%d",&octet[0],&octet[1],&octet[2],&octet[3]);

    /* check if address is valid and check if address exist */
    bool is_valid = check_address_is_valid(octet);
    bool exist = check_address_exist_inorder(root,octet);
    while(!is_valid || exist){
        if (!is_valid){
            printf("Error: %d.%d.%d.%d is an illegal address - please re-enter\n",octet[0],octet[1],octet[2],octet[3]);
        }
        if (exist){
            printf("Error: %d.%d.%d.%d already exist - please re-enter\n",octet[0],octet[1],octet[2],octet[3]);
        }
        printf("Enter Ipv4 address :");
        scanf("%d.%d.%d.%d",&octet[0],&octet[1],&octet[2],&octet[3]);
        is_valid = check_address_is_valid(octet);
        exist = check_address_exist_inorder(root,octet);
    }


    /* enter alias */
    char alias[11];
    printf("Enter alias: ");
    scanf("%s",alias);

    /* add node to tree */
    struct address_t *node = (struct address_t*) malloc (sizeof(struct address_t));
    strcpy(node->alias,alias);
    memcpy(node->octet,octet,sizeof(node->octet));
    node->height = 0;
    node->depth = 0;
    node->leftChild = NULL;
    node->rightChild = NULL;
    node->parent = NULL;

    if (add_node_to_tree(node)) {
        printf("Adding address successfully!\n");
    }
}

/* check address number is valid */
bool check_address_is_valid(int octet[4]){
    for (int i =0;i<4;i++){
        if (octet[i] < 0 || octet[i] > 255){
            return false;
        }
    }
    return true;
}

/* check if address exist */
bool check_address_exist_inorder(struct address_t * node, int octet[4]){
    if (node == NULL) {return false;}

    bool exist_left , exist_root, exist_right;

    exist_left = check_address_exist_inorder(node->leftChild,octet);

    exist_root = true;
    for (int i=0;i<4;i++){
        if (node->octet[i] != octet[i]){
            exist_root = false;
        }
    }

    exist_right = check_address_exist_inorder(node->rightChild,octet);

    return (exist_left || exist_root || exist_right);
}


/* look up address with alias */
void look_up_address_by_alias(void){

    /* enter alias */
    char alias[11];
    printf("Enter alias: ");
    scanf("%s",alias);

    struct address_t * node;

    node = search_alias(alias);

    if (node == NULL){
        printf("Error: %s does not exist.\n",alias);
    }

    else {
        printf("%s: %d.%d.%d.%d\n",node->alias,node->octet[0],node->octet[1],node->octet[2],node->octet[3]);
    }

}

/* search alias in tree */
struct address_t * search_alias(char alias[MAX_ALIAS_LENGTH]){

    struct address_t * ptr = root;

    while (ptr != NULL){
        if (strcmp(ptr->alias,alias) > 0){
            ptr = ptr->leftChild;
        }
        else if (strcmp(ptr->alias,alias) < 0){
            ptr = ptr->rightChild;
        }

        else if (strcmp(ptr->alias,alias) == 0){
            return ptr;
        }
    }

    return NULL;
}

/* update address */
void update_address(void){

    /* enter alias */
    char alias[11];
    printf("Enter alias: ");
    scanf("%s",alias);

    struct address_t * node;

    node = search_alias(alias);

    if (node == NULL) {
        printf("Error: %s does not exist.\n",alias);
    }

    else {
        printf("Update Address for %s: %d.%d.%d.%d\n",node->alias,node->octet[0],node->octet[1],node->octet[2],node->octet[3]);
        int location[4];
        for (int i=0;i<4;i++){
            printf("Enter location value #%d (0-255):",i+1);
            while ( scanf("%d",&location[i]) != 1 || location[i] < 0 || location[i] > 255){ // check if the address number is valid
                printf("Error: %d is an illegal entry - please re-enter.\n",location[i]);
                fflush(stdin);
                printf("Enter location value #%d (0-255):",i+1);
            }
        }
        memcpy(node->octet,location,sizeof(node->octet));
        printf("Update address successfully!\n");
    }
}

/* delete address in tree */
void delete_address(void){

    /* enter alias */
    char alias[11];
    printf("Enter alias: ");
    scanf("%s",alias);

    struct address_t * node;

    node = search_alias(alias);

    if (node == NULL) {
        printf("Error: %s does not exist.\n",alias);
    }

    else {

        /* show option for deletion*/
        char option;
        printf("Dou you sure you want to delete %s %d.%d.%d.%d?(y/n)",node->alias,node->octet[0],node->octet[1],node->octet[2],node->octet[3]);
        scanf(" %c",&option);
        while (option != 'y' && option != 'n'){ // print error if option is not y or n
            printf("Error: please enter y or n.\n");
            printf("Dou you sure you want to delete %s %d.%d.%d.%d?(y/n)",node->alias,node->octet[0],node->octet[1],node->octet[2],node->octet[3]);
            scanf(" %c",&option);
        }

        /* delete */
        if (option == 'y'){
            root = remove_node_in_tree(root,alias); // remove node in tree
            calculate_height(root); // calculate height for all node
            printf("%s deleted!\n");
        }
        
        /* no deletion*/
        else if (option == 'n'){
            printf("No deletion.\n");
        }
    }
}


/* remove node in tree */
struct address_t * remove_node_in_tree(struct address_t * root, char alias[MAX_ALIAS_LENGTH]){

    if (root == NULL){ return NULL;}

    if (strcmp(root->alias,alias) > 0){
        root->leftChild = remove_node_in_tree(root->leftChild,alias);
    }

    else if (strcmp(root->alias,alias) < 0){
        root->rightChild = remove_node_in_tree(root->rightChild,alias);
    }

    else {
        
        // case 1: leaf node
        if (root->leftChild == NULL && root->rightChild == NULL){
            free(root);
            return NULL;
        }
        
        // case 2: has right child
        else if (root->leftChild == NULL){
            struct address_t * temp = root->rightChild;
            free(root);
            return temp;
        }
        
        // case 3: has left child
        else if (root->rightChild == NULL){
            struct address_t * temp = root->leftChild;
            free(root);
            return temp;
        }
        
        // case 4: has both child
        else {
            struct address_t * temp = root->rightChild;
            // get the smallest alias in right subtree
            while (temp->leftChild != NULL){
                temp = temp->leftChild;
            }
            // replace root node value with the temp node value
            strcpy(root->alias,temp->alias);
            memcpy(root->octet,temp->octet,sizeof(temp->octet));
            root->rightChild = remove_node_in_tree(root->rightChild,temp->alias);
        }
    }
    return root;
}

/* calculate height in node */
void calculate_height(struct address_t * root){

    if (root == NULL) {return;}

    // case 1: leaf node
    if (root->leftChild == NULL && root->rightChild == NULL){
        root->height  = 0;
    }

    // case 2: has left child
    else if (root->rightChild == NULL){
        calculate_height(root->leftChild);
        root->height = root->leftChild->height + 1;
    }

    // case 3: has right child
    else if (root->leftChild == NULL){
        calculate_height(root->rightChild);
        root->height = root->rightChild->height + 1;
    }

    // case 4: has both child
    else {
        calculate_height(root->rightChild);
        calculate_height(root->leftChild);
        root->height = 1 + max(root->leftChild->height,root->rightChild->height);
    }
}

/* displat alias for location */
void display_alias_for_location(void){

     /* enter location value */
    int location[2];
    printf("Enter location value #1 (0-255):");
    scanf("%d",&location[0]);
    
    /* check if location value is valid*/
    while (location[0] < 0 || location[0] > 255){
        printf("Error: %d is an illegal entry - please re-enter.\n",location[0]);
        printf("Enter location value #1 (0-255):");
        scanf("%d",&location[0]);
    }
    printf("Enter location value #2 (0-255):");
    scanf("%d",&location[1]);
    while (location[1] < 0 || location[1] > 255){
        printf("Error: %d is an illegal entry - please re-enter.\n",location[1]);
        printf("Enter location value #1 (0-255):");
        scanf("%d",&location[1]);
    }

    printf("Location: %d.%d\n",location[0],location[1]);

    // check if location exist in tree
    if (check_location_exist_inorder(root,location)){
        // search alias by location
        search_alias_by_location_inorder(root,location);
    }
    
    // display error message
    else {
        printf("Error: Location %d.%d does not exist.\n",location[0],location[1]);
    }
}

/* search alias by location */
void search_alias_by_location_inorder(struct address_t * node, int location[2]){

    if (node == NULL) { return;}

    search_alias_by_location_inorder(node->leftChild,location);

    // display alias and address if location match
    if (node->octet[0] == location[0] && node->octet[1] == location[1]){
        printf("%s | %d.%d.%d.%d\n",node->alias,node->octet[0],node->octet[1],node->octet[2],node->octet[3]);
    }

    search_alias_by_location_inorder(node->rightChild,location);
}

/* check if location exist in tree */
bool check_location_exist_inorder(struct address_t * node , int location[2]){

    if (node == NULL) { return false;}

    bool exist_left , exist_root , exist_right;

    exist_left = check_location_exist_inorder(node->leftChild,location);

    exist_root = false;
    if (node->octet[0] == location[0] && node->octet[1] == location[1]){
        exist_root = true;
    }
    

    exist_right = check_location_exist_inorder(node->rightChild,location);

    return (exist_left || exist_root || exist_right);
}

/* save tree to file */
void save_to_file(void){

    // input file name
    char filename[50];
    printf("Enter file name:");
    scanf("%s",filename);
    strcat(filename,".txt");
    FILE * fp = fopen(filename,"w");

    if (fp == NULL){
        printf("Error: can not open file %s.\n",filename);
        return;
    }

    save_node_to_file_inorder(fp,root);

    fclose(fp);

    printf("Successfully save to file %s\n",filename);
}

/* save node to file in inorder traversal */
void save_node_to_file_inorder(FILE * fp,struct address_t * node){

    if (node == NULL) { return;}

    save_node_to_file_inorder(fp,node->leftChild);

    fprintf(fp,"%d.%d.%d.%d %s\n",node->octet[0],node->octet[1],node->octet[2],node->octet[3],node->alias);

    save_node_to_file_inorder(fp,node->rightChild);

}

/* show menu */
void menu(void){

    int option;
    bool quit = false;

    while (quit == false){
        printf("\n1) Add address\n2) Look up address\n3) Update address\n4) Delete address\n5) Display list\n6) Display alias for location\n7) Save to file\n8) Quit\n");

        printf("\nEnter option(1-8):");
        if (scanf(" %d",&option) != 1){
            printf("Error: please enter number 0-8\n");
            fflush(stdin);
            continue;
        }
        printf("\n");
        switch (option)
            {
            case 1:
                add_address_to_tree();
                break;
            case 2:;
                look_up_address_by_alias();
                break;
            case 3:
                update_address();
                break;
            case 4:
                delete_address();
                break;
            case 5:
                display_list_inorder(root);
                break;
            case 6:
                display_alias_for_location();
                break;
            case 7:
                save_to_file();
                break;
            case 8:
                printf("Good Bye!\n");
                quit = true;
                break;
            default:
                printf("Error: please enter number 0-8\n");
                break;
            }
    }
}

int main(){
    read_file("CS531_Inet.txt");
    menu();
    return 0;
}
