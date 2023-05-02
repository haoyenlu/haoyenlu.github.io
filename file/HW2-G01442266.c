# include <stdio.h>
# include <stdlib.h>
# include <stdbool.h>
# include <string.h>

# define INPUT_BUFFER 50

/* create address_t structure*/
struct address_t
{
    int octet[4];
    char alias[11];
    struct address_t *next;
};

/* head pointer to the link list*/
struct address_t* head;


void read_file(char* file_name); // read address and alias to link list
void add_node_to_last(struct address_t * node); // add node to last of link list
void add_node_to_first(struct address_t * node); // add node to first of link list
void search_address_by_alias(void); // search address in link list by alias name
bool check_address_valid(int octet[4]); // check if address is valid
void add_address(void); // add address to link list
void display_alias_for_location(void); // display alias using address 
void delete_address(void); // delete address using alias
void update_address(void); // update alias's address
void save_to_file(void); // save new link list to file
void print_list(); // print out the link list

void menu(); // show menu

void enter_alias(char alias[]); // function for user enter alias


/* read address and alias from the file to link list*/
void read_file(char* file_name){
    /* open file*/
    FILE *fp;
    char buffer[INPUT_BUFFER];

    fp = fopen(file_name,"r");
    if (fp == NULL){
        printf("File does not exist./n");
        return;
    }

    
    struct address_t * current = head;
    
    /* read file to link list*/
    while (fgets(buffer,INPUT_BUFFER,fp)){
        struct address_t *temp = (struct address_t*) malloc (sizeof(struct address_t));
        sscanf(buffer,"%d.%d.%d.%d %s",&temp->octet[0],&temp->octet[1],&temp->octet[2],&temp->octet[3],temp->alias);
        //printf("Address: %d.%d.%d.%d | Alias: %s\n",temp->octet[0],temp->octet[1],temp->octet[2],temp->octet[3],temp->alias);
        temp->next = NULL;
        add_node_to_last(temp);
    }
}

/* add node to the last of link list*/
void add_node_to_last(struct address_t *node){
    if (head == NULL){ head = node; }
    else {
        struct address_t *ptr = head;
        while (ptr->next != NULL){
            ptr = ptr->next;
        }
        ptr->next = node;
    }
}

/* add node to first of link list*/
void add_node_to_first(struct address_t * node){
    node->next = head;
    head = node;
}

/* search address in link list by alias name */
void search_address_by_alias(void){

    /* enter alias*/
    char alias[11];
    enter_alias(alias);

    /* check if alias exist*/
    struct address_t * ptr = head;
    bool exist = false;
    while (ptr != NULL){
        if (strcmp(ptr->alias,alias) == 0){
            /* show address of alias*/
            printf("Address for %s: %d.%d.%d.%d\n",alias,ptr->octet[0],ptr->octet[1],ptr->octet[2],ptr->octet[3]);
            exist = true;
        }
        ptr = ptr->next;
    }

    /* show error if alias doesn't exist */
    if (exist == false){
        printf("ERROR: %s does not exist.\n",alias);
    }
}


/* add address and alias to link list*/
void add_address(void){

    /* enter alias */
    char alias[11];
    enter_alias(alias);

    /* check if alias exist */
    struct address_t * ptr = head;
    bool exist = false;
    while(ptr != NULL){
        if (strcmp(ptr->alias,alias) == 0){
            printf("ERROR: %s already exists\n",alias);
            exist = true;
        }
        ptr = ptr->next;
    }

    /* if alias exist then return*/
    if (exist == true){
        return;
    }

    else{

        /* enter address */
        printf("Enter address for %s :",alias);
        int octet[4];
        scanf("%d.%d.%d.%d",&octet[0],&octet[1],&octet[2],&octet[3]);

        /* check if address is valid */
        while(check_address_valid(octet) == false){
            printf("Error: %d.%d.%d.%d is an illegal address - please re-enter\n",octet[0],octet[1],octet[2],octet[3]);
            printf("Enter address for %s :",alias);
            scanf("%d.%d.%d.%d",&octet[0],&octet[1],&octet[2],&octet[3]);
        }

        /* add address and alias to link list*/
        struct address_t *temp = (struct address_t*) malloc (sizeof(struct address_t));
        strcpy(temp->alias,alias);
        memcpy(temp->octet,octet,sizeof(temp->octet));
        temp->next = NULL;
        add_node_to_last(temp);
        printf("Adding address successfully\n");
    }
    
}

/* show alias and address with location value*/
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

    /* search address */
    struct address_t * ptr = head;
    while (ptr != NULL){
        if (ptr->octet[0] == location[0] && ptr->octet[1] == location[1]){
            printf("%d.%d : %s\n",ptr->octet[2],ptr->octet[3],ptr->alias);
        }
        ptr = ptr->next;
    }
}

/* delete address from link list*/
void delete_address(void){

    /* enter alias */
    char alias[11];
    enter_alias(alias);

    /* search list */
    struct address_t * ptr = head;
    bool exist = false;
    int count = 0;
    while (ptr != NULL){
        if (strcmp(ptr->alias,alias) == 0){
            exist = true;
            break;
        }
        count ++; // count the position of the alias
        ptr = ptr->next;
    }
    if (exist == false){
        printf("Error: %s does not exist\n",alias);
    }
    else {
        
        /* show option for deletion*/
        char option;
        printf("Dou you sure you want to delete %s %d.%d.%d.%d?(y/n)",ptr->alias,ptr->octet[0],ptr->octet[1],ptr->octet[2],ptr->octet[3]);
        scanf(" %c",&option);
        while (option != 'y' && option != 'n'){ // print error if option is not y or n
            printf("Error: please enter y or n.\n");
            printf("Dou you sure you want to delete %s %d.%d.%d.%d?(y/n)",ptr->alias,ptr->octet[0],ptr->octet[1],ptr->octet[2],ptr->octet[3]);
            scanf(" %c",&option);
        }

        /* delete */
        if (option == 'y'){

            struct address_t * prev = head;
            for (int i=0;i<count-1;i++){
                prev = prev->next;
            }
            prev->next = ptr->next;

            if (ptr == head){
                head = ptr->next;
            }
            printf("%s deleted.\n",alias);
        }
        
        /* no deletion*/
        else if (option == 'n'){
            printf("No deletion.\n");
        }
    }
}

/* update address by alias */
void update_address(void){
    char alias[11];
    enter_alias(alias);

    /* search address by alias */
    struct address_t * ptr = head;
    bool exist = false;
    while(ptr != NULL){
        if (strcmp(ptr->alias,alias) == 0){
            exist = true;
            break;
        }
        ptr = ptr->next;
    }
    if (exist == false){ // print error if the alias does not exist
        printf("Error: %s does not exist.\n",alias);
        return;
    }

    /* update address */ 
    else {
        printf("Update Address for %s: %d.%d.%d.%d\n",ptr->alias,ptr->octet[0],ptr->octet[1],ptr->octet[2],ptr->octet[3]);
        int location[4];
        for (int i=0;i<4;i++){
            printf("Enter location value #%d (0-255):",i+1);
            while ( scanf("%d",&location[i]) != 1 || location[i] < 0 || location[i] > 255){ // check if the address number is valid
                printf("Error: %d is an illegal entry - please re-enter.\n",location[i]);
                fflush(stdin);
                printf("Enter location value #%d (0-255):",i+1);
            }
        }
        /* use memcpy to assign value to link list node */
        memcpy(ptr->octet,location,sizeof(ptr->octet));
    }
}

/* check address number is valid */
bool check_address_valid(int octet[4]){
    for (int i =0;i<4;i++){
        if (octet[i] < 0 || octet[i] > 255){
            return false;
        }
    }
    return true;
}

/* save the new address and alias list to text file */
void save_to_file(void){

    // input file name
    char file_name[50];
    printf("Enter file name:");
    scanf("%s",file_name);
    strcat(file_name,".txt");
    FILE *f = fopen(file_name,"w");

    // print error if cannot correctly open file
    if (f == NULL) { printf("Error opening file.\n"); return;} 

    struct address_t * ptr = head;
    // loop through all the list and write to file using fprintf
    while (ptr != NULL){ 
        fprintf(f,"%d.%d.%d.%d %s\n",ptr->octet[0],ptr->octet[1],ptr->octet[2],ptr->octet[3],ptr->alias);
        ptr = ptr->next;
    }
    fclose(f);
    printf("save to %s.\n",file_name);
    
}

/* print out the link list on console */
void print_list(void){
    int count = 0;
    struct address_t *ptr = head;
    while (ptr != NULL){
        printf("Address: %d.%d.%d.%d | Alias: %s\n",ptr->octet[0],ptr->octet[1],ptr->octet[2],ptr->octet[3],ptr->alias);
        ptr = ptr->next;
        count ++;
    }
    printf("Total node count:%d\n",count);
}

/* function for enter alias */
void enter_alias(char alias[]){
    printf("Enter alias:");
    scanf("%s",alias);
}

/* show menu */
void menu(void){

    int option;
    bool quit = false;

    while (quit == false){
        printf("\n1) Add address\n");
        printf("2) Look up address\n");
        printf("3) Update address\n");
        printf("4) Delete address\n");
        printf("5) Display list\n");
        printf("6) Display alias for location\n");
        printf("7) Save to file\n");
        printf("8) Quit\n");

        printf("\nEnter option(1-8):");
        if (scanf(" %d",&option) != 1){
            printf("Error: please enter number 0-8\n");
            fflush(stdin);
            continue;
        }

        switch (option)
            {
            case 1:
                add_address();
                break;
            case 2:;
                search_address_by_alias();
                break;
            case 3:
                update_address();
                break;
            case 4:
                delete_address();
                break;
            case 5:
                print_list();
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
    read_file("CS531_Inet.txt");  // first read in the text file
    menu(); // show menu
    return 0;
}