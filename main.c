#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define MAX_NAME_LENGTH 50

struct boardNode
{
    char name[MAX_NAME_LENGTH];
    struct itemNode *itemList; //this will point to the first item in the list of items of the boardNode
    struct boardNode *next;
    struct boardNode *prev;
};

struct itemNode
{
    char data[MAX_NAME_LENGTH];
    struct itemNode *next;
    struct itemNode *prev;
};



void displayMenu(void);

void ascii(); // function to print ascii art at the end of the program

void displayBoard(struct boardNode *boardStart); // function prototype for displaying the board

void editList(struct boardNode *boardStart); // function prototype for editing a list on the board

void freeBoard(struct boardNode *currentNode); //prototype for freeing the memory allocated for a board

void freeList(struct boardNode *currentNode); //prototype for freeing the memory allocated to a single list

struct boardNode * loadBoard(struct boardNode *boardStart); //prototype for loading a board from a file

struct boardNode *findBoard(struct boardNode *boardStart, char *name); //prototype for finding a list on the board

struct itemNode *findItem(struct itemNode *itemList, char *name); //prototype for finding an item in a list

void addItem(struct itemNode **itemList, char *name); //prototype for adding an item to a list

void deleteItem(struct itemNode **itemList, char *name); //prototype for deleting an item from a list

void editBoard(struct boardNode **boardStart); //prototype for editing the board

void saveBoard(struct boardNode *boardStart); //prototype for saving the board to a file

int main()
{
    //set up board with default names and items
    struct boardNode *boardStart;

    struct boardNode *list1 = malloc(sizeof(struct boardNode));
    struct boardNode *list2 = malloc(sizeof(struct boardNode));
    struct boardNode *list3 = malloc(sizeof(struct boardNode));
    struct boardNode *list4 = malloc(sizeof(struct boardNode));

    boardStart = list1; //store the head of the board in boardStart
    strcpy(list1->name, "Nick");
    list1->prev = NULL;
    list1->next = list2;
    struct itemNode *list1_item1 = malloc(sizeof(struct itemNode));
    strcpy(list1_item1->data, "3070 RTX");
    list1_item1->next = NULL;
    list1_item1->prev = NULL;
    list1->itemList = list1_item1; //we store the head of the item list in a member of the boardNode

    strcpy(list2->name, "Tim");
    list2->prev = list1;
    list2->next = list3;
    struct itemNode *list2_item1 = malloc(sizeof(struct itemNode));
    strcpy(list2_item1->data, "Oculus Quest 2");
    list2_item1->next = NULL;
    list2_item1->prev = NULL;
    list2->itemList = list2_item1;

    strcpy(list3->name, "Dante");
    list3->prev = list2;
    list3->next = list4;
    struct itemNode *list3_item1 = malloc(sizeof(struct itemNode));
    struct itemNode *list3_item2 = malloc(sizeof(struct itemNode));
    strcpy(list3_item1->data, "Oculus Quest 1");
    list3_item1->prev = NULL;
    list3_item1->next = list3_item2;
    strcpy(list3_item2->data, "3070 RTX");
    list3_item2->prev = list3_item1;
    list3_item2->next = NULL;
    list3->itemList = list3_item1;

    strcpy(list4->name, "Abey");
    list4->prev = list3;
    list4->next = NULL;
    struct itemNode *list4_item1 = malloc(sizeof(struct itemNode));
    struct itemNode *list4_item2 = malloc(sizeof(struct itemNode));
    strcpy(list4_item1->data, "Oculus Pro");
    list4_item1->prev = NULL;
    list4_item1->next = list4_item2;
    strcpy(list4_item2->data, "Oculus Quest 1");
    list4_item2->prev = list4_item1;
    list4_item2->next = NULL;
    list4->itemList = list4_item1;

    int user_choice;
    do
    {
        displayMenu();
        printf("Enter your choice: ");
        scanf("%d", &user_choice);

        if(user_choice==1)
        {
            displayBoard(boardStart);
        }
        else if(user_choice==2)
        {
            struct boardNode *newboardStart = loadBoard(boardStart); //save the start of the new loaded board in newBoardStart
            if(newboardStart!=boardStart) //if we have a new start we free the old board and save the new start in boardStart
            {
                freeBoard(boardStart); //free the old board
                boardStart = newboardStart;
            } //else the old board start remains the same
        }
        else if(user_choice==3)
        {
            editList(boardStart);
        }
        else if(user_choice==4)
        {
            editBoard(&boardStart); //passing in the address of boardStart because it might change
        }
        else if(user_choice==5)
        {
            saveBoard(boardStart);
        }
        else if(user_choice!=6)
        {
            puts("\nPlease enter a valid choice\n");
        }
    }while(user_choice!=6);
    ascii();
    return 0;
}

void displayMenu(void) //displays options to choose from
{
    puts("Menu:");
    puts("1. Display board");
    puts("2. Load board from a file");
    puts("3. Edit list");
    puts("4. Edit Board");
    puts("5. Save board to a file");
    puts("6. Quit");
}


void displayBoard(struct boardNode *boardStart)
/* this function takes a pointer to the head of a linked list
of 'boardNode' structs, and iterates through each board in the list.
for each board, it prints the board's name, and then iterates
through each item in the board's 'itemList' linked list and prints
the item's data. Finally, it moves on to the next board in the list,
until all boards have been processed. */
{
    struct boardNode *currentBoard = boardStart; // declare a pointer to the head of the board
    struct itemNode *currentItem;
    /* as the function iterates through each boardNode, the
    currentItem pointer will be used to access each item
    in the item list associated with that boardNode */

    // Loop through each board in the linked list until currentBoard is NULL
    printf("\n");
    if(currentBoard==NULL) //if the currentBoard pointer points to NULL at first the board is empty
    {
        printf("Board is empty\n");
    }
    else
    {
        while (currentBoard != NULL)
        {
            printf("%s:\n", currentBoard->name);
            // set currentItem to the head of the current board's item list
            currentItem = currentBoard->itemList;

            // loop through each item in the current board's item list until currentItem is NULL
            while (currentItem != NULL)
            {
                printf("   %s\n", currentItem->data);
                // set currentItem to the next item in the list
                currentItem = currentItem->next;
            }
            // set currentBoard to the next board in the list
            currentBoard = currentBoard->next;
        }
    }
    printf("\n");
}

void editList(struct boardNode *boardStart)
{
    char boardName[MAX_NAME_LENGTH], itemName[MAX_NAME_LENGTH], newName[MAX_NAME_LENGTH];
    int option;

    printf("\nEnter the name of the list to edit: ");
    fflush(stdin);
    fgets(boardName, MAX_NAME_LENGTH, stdin);
    boardName[strcspn(boardName, "\n")] = '\0';  // remove newline character

    struct boardNode *currentBoard = findBoard(boardStart, boardName);
    if (currentBoard == NULL)
    {
        printf("Can't find list: %s\n\n", boardName);
        return;
    }

    while (1)
    {
        printf("\nOptions:\n");
        printf("1. Edit an item\n");
        printf("2. Add a new item\n");
        printf("3. Delete an item\n");
        printf("4. Return to main menu\n");
        printf("Enter your option: ");
        scanf("%d", &option);
        getchar();  // consume newline character

        if (option == 1)
        {
            printf("Enter the name of the item to edit: ");
            fgets(itemName, MAX_NAME_LENGTH, stdin);
            itemName[strcspn(itemName, "\n")] = '\0';  // remove newline character

            struct itemNode *currentItem = findItem(currentBoard->itemList, itemName);
            if (currentItem == NULL)
            {
                printf("Can't find item: %s\n", itemName);
                continue;
            }

            printf("Enter new name for item '%s': ", itemName);
            fgets(newName, MAX_NAME_LENGTH, stdin);
            newName[strcspn(newName, "\n")] = '\0';  // remove newline character

            // copy the newName string into the data field of the currentItem struct
            strcpy(currentItem->data, newName);

            printf("'%s' renamed to '%s'\n", itemName, newName);
        }
        else if (option == 2)
        {
            printf("Enter the name of the new item: ");
            fgets(newName, MAX_NAME_LENGTH, stdin);
            newName[strcspn(newName, "\n")] = '\0';  // remove newline character

            // call addItem function
            addItem(&currentBoard->itemList, newName);
        }
        else if (option == 3)
        {
            printf("Enter the name of the item to delete: ");
            fgets(itemName, MAX_NAME_LENGTH, stdin);
            itemName[strcspn(itemName, "\n")] = '\0';  // remove newline character

            // call deleteItem function
            deleteItem(&currentBoard->itemList, itemName);
        }
        else if (option == 4)
        {
            break;
        }

        else
        {
            printf("\nInvalid option\n");
        }
    }

    displayBoard(boardStart);
}

// function to locate board to be edited
struct boardNode *findBoard(struct boardNode *boardStart, char *name)
{
    // declare a pointer to the head of the board
    struct boardNode *currentBoard = boardStart;

    // traverse the list of boards until either the board with the given name is found or the end of the list is reached
    while (currentBoard != NULL) {
        // if the current board's name matches the given name, return a pointer to the current board
        if (strcmp(currentBoard->name, name) == 0) {
            return currentBoard;
        }
        // otherwise, move on to the next board in the list
        currentBoard = currentBoard->next;
    }

    // if the board with the given name was not found, return NULL
    return NULL;
}


// this function takes a pointer to the head of the item list and a name of an item to search for
struct itemNode *findItem(struct itemNode *itemList, char *name)
{
    // declare a pointer to the current item and set it to the head of the list
    struct itemNode *currentItem = itemList;

    // loop through each item in the list
    while (currentItem != NULL) {
        // compare the name of the current item to the search name
        if (strcmp(currentItem->data, name) == 0) {
            // if the names match, return a pointer to the current item
            return currentItem;
        }
        // move on to the next item in the list
        currentItem = currentItem->next;
    }

    // if the item is not found in the list, return NULL
    return NULL;
}


// function to add an item to the board
void addItem(struct itemNode **itemList, char *name) //always adds new item to the beginning of the list
{
    // allocate memory for new item
    struct itemNode *newItem = (struct itemNode *)malloc(sizeof(struct itemNode));

    // copy name of item to data field of the new item
    strncpy(newItem->data, name, MAX_NAME_LENGTH);
    // establish link to next item in the list
    newItem->next = *itemList;
    // establish link to previous item in the list
    newItem->prev = NULL;
    if((*itemList) != NULL) //if the itemList was not empty before establish link in the other direction between first item and new item
    {
        (*itemList)->prev = newItem;
    }
    // update the start of the list to point to new item
    *itemList = newItem;

    printf("Added new item: %s\n", name);
}

// function to delete an item from the board
void deleteItem(struct itemNode **itemList, char *name)
{
    // call findItem function to locate the item to be deleted
    struct itemNode *currentItem = findItem(*itemList, name);

    if(currentItem == NULL) // if item not in list
    {
        printf("Can't find item: %s\n", name);
        return;
    }
    else //item to delete was found and is now pointed to by currentItem
    {
        if (currentItem->prev == NULL) { //if we're on the first item
            *itemList = currentItem->next; //point start of the list to next item
        } else {
            currentItem->prev->next = currentItem->next; //point previous item to next item
        }

        if(currentItem->next != NULL) //if not the last item establish link in the other direction
        {
            currentItem->next->prev = currentItem->prev;
        }
        free(currentItem); //then free memory of the deleted item

        printf("Deleted item: %s\n", name);
    }
}

//function for editing the board
void editBoard(struct boardNode **boardStart)
{
    int option;
    do
    {
        start: printf("\nOptions:\n");
        printf("1. Edit the name of a list\n");
        printf("2. Add a new list\n");
        printf("3. Delete a list\n");
        printf("4. Return to main menu\n");
        printf("Enter your option: ");
        scanf("%d", &option);
        fflush(stdin);

        if(option==1)
        {
            char listName[MAX_NAME_LENGTH];

            printf("Enter the name of the list to edit: ");
            fgets(listName, MAX_NAME_LENGTH, stdin);
            listName[strlen(listName)-1] = '\0'; //removing newline

            struct boardNode *currentList = findBoard(*boardStart, listName);
            if(currentList == NULL)
            {
                printf("Can't find list: %s\n", listName);
                continue;
            }
            else
            {
                char newName[MAX_NAME_LENGTH];
                printf("Enter new name for list '%s': ", listName);
                fgets(newName, MAX_NAME_LENGTH, stdin);
                newName[strlen(newName)-1] = '\0'; //removing newline

                strcpy(currentList->name, newName); //replace the name of the list with the new name

                printf("'%s' renamed to '%s'\n", listName, newName);
            }
        }
        else if(option==2)
        {
            char newName[MAX_NAME_LENGTH];
            printf("Enter the name of the new list: ");
            fgets(newName, MAX_NAME_LENGTH, stdin);
            newName[strlen(newName)-1] = '\0'; //removing newline

            struct boardNode *newList = malloc(sizeof(struct boardNode)); //allocating space for the new list

            strcpy(newList->name, newName);
            newList->itemList = NULL; //initialize an empty item list for the new list
            newList->next = *boardStart; //introducing a new list at the start of the board
            newList->prev = NULL;
            if((*boardStart)!=NULL) //if the board wasn't empty link in the other direction
            {
                (*boardStart)->prev = newList;
            }
            *boardStart = newList; //the new boardStart becomes the new list

            printf("Added new list: %s\n", newName);
        }
        else if(option==3)
        {
            char deleteName[MAX_NAME_LENGTH];
            printf("Enter the name of the list to delete: ");
            fgets(deleteName, MAX_NAME_LENGTH, stdin);
            deleteName[strlen(deleteName)-1] = '\0';

            struct boardNode *currentNode = findBoard(*boardStart, deleteName);

            if(currentNode==NULL)
            {
                printf("Can't find list: %s\n", deleteName);
                continue;
            }
            else //list to delete was found and is pointed to by currentNode
            {
                if(currentNode->prev==NULL) //if we're at the start of the board
                {
                    *boardStart = currentNode->next;
                }
                else //not the first list on the board
                {
                    currentNode->prev->next = currentNode->next;
                }

                if(currentNode->next != NULL) //if not at the end of the list we link in the other direction
                {
                    currentNode->next->prev = currentNode->prev;
                }

                freeList(currentNode);
                printf("Deleted list: %s\n", deleteName);
            }

        }
        else if(option!=4)
        {
            puts("\nInvalid option.");
        }
    }while(option!=4);

    displayBoard(*boardStart); //display the board to see any changes before returning
}

//a function to free the memory of a single list on the board, including its list of items
void freeList(struct boardNode *currentNode)
{
    //first free the items
    struct itemNode *currentItem = currentNode->itemList;
    while(currentItem!=NULL)
    {
        struct itemNode *tempPtr = currentItem;
        currentItem = currentItem->next; //move on to the next item
        free(tempPtr);
    }

    //then free the board node
    free(currentNode);
}

//a function to free the memory allocated for the entire board
void freeBoard(struct boardNode *currentNode)
{
    while(currentNode!=NULL)
    {
        struct boardNode *freeThis = currentNode; //save a reference of the currentNode to free it later
        currentNode = currentNode->next; //move on to the next board node
        freeList(freeThis); //free the memory using the reference and by calling freeList on it
    }
}


//can load files with the following structure:
//      -- names and items can contain spaces
//      -- each of the list names and items on a separate line
//      -- a list always has its name first, followed by the items
//      -- different lists are separated by a single "-" character on its own line
//      -- file has to end with a newline
struct boardNode * loadBoard(struct boardNode *boardStart) //takes in a pointer to the start of the current board
{
    puts("");
    printf("Enter filename: ");
    char filename[30];

    fflush(stdin);
    fgets(filename, 30, stdin);
    filename[strlen(filename)-1] = '\0'; //removing newline at the end

    FILE *fp;
    if((fp = fopen(filename, "r")) == NULL) //error handling
    {
        puts("File could not be opened\n");
        return boardStart; //return the start of the current board in case the file couldn't be opened
    }
    else
    {
        char input[30];

        //allocate the new start of the board in case the file was opened successfully
        struct boardNode *newBoardStart = malloc(sizeof(struct boardNode));
        struct boardNode *currentNode = newBoardStart;
        currentNode->next = NULL;
        currentNode->prev = NULL;

        int startFilled = 0; //an int to determine if the start node is filled

        do
        {
            fgets(input, 30, fp); //this reads a line from the file
            input[strlen(input)-1] = '\0'; //and then removes the newline at the end

            if(startFilled == 0) //if we haven't filled the start node yet
            {
                strcpy(currentNode->name, input);
                startFilled = 1;
            }
            else //already filled start node so we need to allocate a new node
            {
                struct boardNode *newNode = malloc(sizeof(struct boardNode));
                strcpy(newNode->name, input);
                newNode->prev = currentNode; //linking between current node and new one
                newNode->next = NULL;
                currentNode->next = newNode;
                currentNode = newNode; //the current node becomes the new node
            }

            currentNode->itemList = NULL; //set the itemList of the current node to be empty at first

            fgets(input, 30, fp);
            input[strlen(input)-1] = '\0';

            struct itemNode *currentItem;

            while(!feof(fp) && strcmp(input, "-")!=0) //the item list ends either at the end of file or at the "-" marker
            {

                if(currentNode->itemList == NULL) //if the item list is empty
                {
                    currentItem = malloc(sizeof(struct itemNode));
                    currentNode->itemList = currentItem; //set the item list to point to the first item
                    strcpy(currentItem->data, input);
                    currentItem->next = NULL;
                    currentItem->prev = NULL;
                }
                else //list is not empty, we need to link a new item after the current one
                {
                    struct itemNode *newItem = malloc(sizeof(struct itemNode));
                    strcpy(newItem->data, input);
                    newItem->prev = currentItem; //linking between current item and new one
                    newItem->next = NULL;
                    currentItem->next = newItem;
                    currentItem = newItem; //current item becomes the new item
                }

                fgets(input, 30, fp);
                input[strlen(input)-1] = '\0';
            }

        }while(!feof(fp));

        printf("Successfully loaded file %s\n\n", filename);

        fclose(fp);
        return newBoardStart;
    }

}

void saveBoard(struct boardNode *boardStart)
{
    printf("\nEnter filename: ");
    char filename[30];
    fflush(stdin);
    fgets(filename, 30, stdin); //taking in the filename
    filename[strlen(filename)-1] = '\0'; //removing newline at the end

    FILE *fp;
    if((fp = fopen(filename, "w")) == NULL) //checking if the file was opened successfully
    {
        puts("File could not be opened\n");
        return;
    }
    else
    {
        struct boardNode *currentNode = boardStart; //declare a currentNode to iterate over the board and initialize it to the boardStart

        while(currentNode!=NULL) //while we're not at the end of the board
        {
            fprintf(fp, "%s\n", currentNode->name); //writing the name of the list to the file
            struct itemNode *currentItem = currentNode->itemList; //declare a currentItem to iterate over the itemList and initialize to the start
            while(currentItem!=NULL) //while not at the end of the itemList
            {
                fprintf(fp, "%s\n", currentItem->data); //writing the name of the item to the file
                currentItem = currentItem->next;
            }

            if(currentNode->next != NULL) //unless this is the last list we print a '-' to separate this list from the next one
            {
                fprintf(fp, "-\n");
            }

            currentNode = currentNode->next;
        }

        printf("Successfully saved to file %s\n\n", filename);
        fclose(fp);
    }
}


// function to print the ascii art
void ascii()
{
    printf("        .. .     .........   .              .                                                                                                \n");
    printf("  (###&%%%%%%#%%(/***(####((((/*//***,*/***////(##(.                                                                                             \n");
    printf("  (%%#(%%%%%%%%##(/*/*(####((((////*****/**/////((#(                                                                                              \n");
    printf("  (%%#(%%&#(#((%%###(/(//((((////(####/////(((****                                                                                              \n");
    printf("  /#(#%%%%%%%%#((###((########(///(##((#######%%/,,,                                                                                              \n");
    printf("  /#((%%%%%%%%%%((((((%%&&&%%%%%%%%%%(***((((#&&&&%%%%%%&/,,,                                                                                              \n");
    printf("  *#(##(((((/(((/%%&&&&@@@@(///((((%%@@@@&&&&(//*                                                                                              \n");
    printf("  ,#(##(((((((((/%%&&&&@@@@(///((((%%@@@@&&&&(///                                                                                              \n");
    printf("  ,#(##((###(////(####((//%%&&&&&&&%%//*/####(///                                                                                              \n");
    printf("  .#(##((###(///*/((((***/&&%%&&&&&%%***/####(/(/                  #@@@@@@@@@@@@@@@@@@@@@@@@@.                                                     \n");
    printf("  .#(%%#((##((****((((#%%%%%%%%@@@@@@@@&&%%%%%%(//(***,                #@@@@@@@@@@@@@@@@@@@@@@@@@@@@@,                                                   \n");
    printf("   %%(#%%#(##((****(##(#%%%%%%%%@@@@@@@@&%%&&%%////***,               .@@@@@@@&@&&&&&&&&&&&&&&&@@@@@@#   .&&&&&&%%          ,&&&&&&.          %%&&&&&@     \n");
    printf("   %%(#%%%%%%%%/((***,/(((%%@@@@@@@@@@@@@@@@&((((#((/               .@@@@@@(                 @@@@@@#     @@@@@@@        ,@@@@@@@@*        @@@@@@@      \n");
    printf("   #(#(((#(((*,*,/(((%%@@@@@@@@@@@@@@@@&(((((((*               .@@@@@@(                 @@@@@@#     .@@@@@@@      (@@@@@@@@@@#      @@@@@@@       \n");
    printf("    .%%#(##(#%%///////*(&&&&(//*****#&&&&####///,               .@@@@@@(                 @@@@@@#      .@@@@@@@    %%@@@@@@@@@@@@%%    %%@@@@@@,       \n");
    printf("     #######%%/////**/(&&&&//(((#(#%%%%%%%%%%%%(/*                   .@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@#       .@@@@@@@  &@@@@@@@&@@@@@@&  &@@@@@@,        \n");
    printf("     %%%%%%######%%%%%%%%%%&&%%%%####&&&&%%%%%%%%##(*((((                   .@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@#        .@@@@@@@%%@@@@@@%%  &@@@@@@&&@@@@@@.         \n");
    printf("     #&&&%%&%%%%&#((((#(##***,%%%%%%%%#%%#%%/***###(                   .@@@@@@#,,,,,,,,,,,,,,,,,@@@@@@#         ,@@@@@@@@@@@@#    %%@@@@@@@@@@@@/          \n");
    printf("     #%%%%%%%%%%%%%%%%#((((####****%%%%%%%%####%%%%%%%%/**,                   .@@@@@@(                 @@@@@@#          .@@@@@@@@@@%%      #@@@@@@@@@@*           \n");
    printf("     #%%##%%%%%%%%%%%%((((((((((((#%%######%%%%%%%%***,                   .@@@@@@(                 @@@@@@#           .@@@@@@@@%%        /@@@@@@@@/            \n");
    printf("     (###%%%%%%%%%%#########((((((((#######%%%%%%%%(                   .@@@@@@(                 @@@@@@#            ,@@@@@@(          ,@@@@@@%%             \n");
    printf("     /####(///#%%%%%%%%##%%#((((((((########%%%%%%/                    .  .                    .    ..               ...              . ..               \n");
    printf("     /%%%%##((/(((#(#%%#%%%%%%%%%%%%########((((###*                                                                                                  \n");
    printf("     *%%%%##(((/(####%%%%%%%%%%%%%%#%%#######((((///,                                                                                                  \n");
    printf("     *%%%%#%%(((((****((((////%%%%#%%####((((///.                                                                                                  \n");
    printf("     *%%##%%#(((/***/(((#////((((####((((///.                                                                                                  \n");
    printf("     ,%%###//*/(#######%%////((((%%###((((((#.                                                                                                  \n");
    printf("     ,%%%%%%%%(/((((////*******((((((((((####%%.              *@@@@@@@&                 .@@@@@@@@.                                                        \n");
    printf("     .%%%%%%%%##(((*,******/**/((((((((//*/%%%%%%               *@@@@@@@@@&             .@@@@@@@@@@.                                                        \n");
    printf("     .%%%%%%%%#########(/((/***////((((***(#%%#               *@@@@@@@@@@@@         ,@@@@@@@@@@@@.    #@@@@@@@@@@@@@@@@@@@@&.       %%@@@@@@@@@@@@@@@@@@@@&.\n");
    printf("     .%%&%%##########(###////////####////(((               *@@@@@@@@@@@@@@.    *@@@@@@@@@@@@@@.    #@@@@@@@@@@@@@@@@@@@@@@@%%     &@@@@@@@@@@@@@@@@@@@@@@@# \n");
    printf("      %%%%%%%%#///(%%%%%%%%%%%%%%%%///////(###((((((((               *@@@@@@ #@@@@@@@@,/@@@@@@@@,/@@@@@@.    %%@@@@@@@@@@@@@@@@@@@@@@@@#    &@@@@@@@@@@@@@@@@@@@@@@@@#\n");
    printf(" ,##(#%%&#%%#((((############***/#(#######(#               *@@@@@@   *@@@@@@@@@@@@@@.  /@@@@@@.                       @@@@@@#    &@@@@@@           .@@@@@@#\n");
    printf(" .(####(#%%%%######(####(####////((((###(///               *@@@@@@     /@@@@@@@@@@     /@@@@@@.    #@@@@@@@@@@@@@@@@@@@@@@@@#    &@@@@@@           .@@@@@@#\n");
    printf(" .####&%%##(////(((#////%%%%%%%%(//((((#%%%%##(((,              *@@@@@@       .@@@@@@       /@@@@@@.    #@@@@@@@@@@@@@@@@@@@@@@@@#    &@@@@@@           .@@@@@@#\n");
    printf(" .%%%%##&&&%%((((#####(((((((((((//(%%%%%%%%#(((#(//*           *@@@@@@         .@&         /@@@@@@.    #@@@@@@            @@@@@@#    &@@@@@@           .@@@@@@#\n");
    printf(" .%%###&&&&%%(/***/%%%%#######(////###((((((##(##(//*        *@@@@@@                     /@@@@@@.    %%@@@@@@,,,,,,,,,,,*@@@@@@#    &@@@@@@           .@@@@@@#\n");
    printf(" .##(%%&&&%%#(###((/*/((((#####/***############(***        *@@@@@@                     /@@@@@@.    /@@@@@@@@@@@@@@@@@@@@@@@@#    &@@@@@@           .@@@@@@#\n");
    printf(" .##(####%%%%##################****############/,*,        *@@@@@@                     /@@@@@@.      %%@@@@@@@@@@@@@@@@@@@@@@#    %%@@@@@@           .@@@@@@#\n");
    printf("  ##(#%%%%%%%%&#%%%%#(#############***,########(((#/,,,                                                                                                \n");
    printf("  %%%%(%%&@@@@%%##############(##*,**###%%((/*////////                                                                                                \n");
    printf("   .(@@@@@&##(#%%#####///*///////(###%%/**,////////                                                                                                \n");
    printf("     ,@@&#(##(#%%####(****///////(////(((#///////*                                                                                                \n");
    printf("          .##((#(///(((((///////(/***(##%%#%%%%&@@@#                                                                                                \n");
    printf("           %%#((#///*(((###%%%%&@@@@%%&&&@@@@&%%&&@@@#                                                                                                \n");
    printf("           (%%#(#%%&&&@@@@@&&&&@@@@&&@@&&&&@@@@&&&(                                                                                                \n");
    printf("            .(#&%%&&&&&&&&@@@@&&&&@@@@&&&&&#(*.                                                                                                   \n");
    printf("              ,@@@@@&&&&@@@%%/,..        \n");
}
