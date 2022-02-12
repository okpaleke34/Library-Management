#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <errno.h>
#include <time.h>
#include <ctype.h>

#define DATALEN 1024 //Data length
#define CLEN 20 //Command length
#define BOOK_LIST "storage/book_list.csv"
#define BORROW_LIST "storage/borrow_list.csv"



/* Link list node */
struct Node
{
    char data[DATALEN];     //Data of the node
    struct Node* next;      //Address of the next node
};

/* Prototypes for the functions */
void mainMenu();
int back2Menu(char*);
const char* strtoupper(char*);
const char* getField(char*, int);
char* randstring(size_t, int);
char* searchDB(char*, int, char*);
//char* fetchCell(char*, int, char*, int);
void addBook(char* title, char* author, char* publishedYear);
void printBookList();
void editBook(char* book_id, char* title, char* author, char* publishedYear);
void deleteBook(char*);
void borrowBook(char* book_id, char* borrower_name);
char* updateCell(char* file_path, int keywordIndex, char* keyword, int seedIndex, char* seed);
void printBorrowList(int);

void push2Node(struct Node**, char*);
void printNode(struct Node*); // function to display the list
void node2File(char*, struct Node*);
void persistData(char*, struct Node*);



int main() {
    mainMenu();
    return 0;
}



int back2Menu(char* newCommand) {
    char cpycmd[CLEN];
    strcpy_s(cpycmd,CLEN, newCommand);
    strtoupper(cpycmd);
    if (strcmp(cpycmd, "MAIN_MENU") == 0) {
        mainMenu();
        return 1;
    }
    return 0;
}

//covert lower case to uppercase
const char* strtoupper(char str[CLEN]) {
    int i = 0;
    while (str[i] != '\0') {
        str[i] = toupper(str[i]);
        i++;
    }
    return str;
}


void mainMenu() {
    srand(time(NULL));   // Initialization, should only be called once.
    char newCommand[CLEN], prevCommand[CLEN];
    int continueProgram = 1;
    while (continueProgram) {
        printf("\nEnter a new command, or type help for list of commands: "); 
        scanf_s("%[^\n]%*c", newCommand,CLEN);
        strtoupper(newCommand);
        printf("\nNew Command: %s \n", newCommand);

        if (strcmp(newCommand, "HELP") == 0) {
            printf("Here are the list of commands: \n");
            printf("%*s To go to program main menu\n", -CLEN, "MAIN_MENU");
            printf("%*s To quit the program\n", -CLEN, "QUIT_PROGRAM");
            printf("%*s To add book to the database\n", -CLEN, "ADD_BOOK");
            printf("%*s To edit a book details eg  name, author etc\n", -CLEN, "EDIT_BOOK");
            printf("%*s To list all the books in database\n", -CLEN, "LIST_BOOK");
            printf("%*s To delete a book from the database\n", -CLEN, "DELETE_BOOK");
            printf("%*s To borrow a book to a student\n", -CLEN, "BORROW_BOOK");
            printf("%*s To return a borrowed book\n", -CLEN, "RETURN_BOOK");
            printf("%*s To update the name of the person that the book was borrowed to\n", -CLEN, "EDIT_BORROWED_BOOK");
            printf("%*s To list all the books that have not been returned\n", -CLEN, "BORROWED_BOOKS");
        }
        //shut down the program
        if (strcmp(newCommand, "QUIT_PROGRAM") == 0) {
            char quitDecision = 'Z';
            while (quitDecision != 'N' && quitDecision != 'Y') {
                printf("Are you sure you want to quit this program? \nNote that when you quit the program you have to run it again to start.\n\nEnter Y to quit or N to continue with the program:");
                quitDecision = getchar();
                getchar();
                quitDecision = toupper(quitDecision);
                if (quitDecision == 'Y') {
                    continueProgram = 0;
                }
                else {
                    if (quitDecision != 'N') {
                        printf("%c is not a valid command\n", quitDecision);
                    }
                }
            }
        }
        else if (strcmp(newCommand, "ADD_BOOK") == 0) {
            char title[50], publishedYear[5], author[50];
            printf("\nEnter the title of the book: ");
            scanf_s("%[^\n]%*c", title, 50);
            back2Menu(title);
            printf("\nEnter the author of the book: ");
            scanf_s("%[^\n]%*c", author, 50);
            back2Menu(author);
            printf("\nEnter the book publication year: ");
            scanf_s("%[^\n]%*c", publishedYear, 5);
            back2Menu(publishedYear);
            addBook(title, author, publishedYear);
        }
        else if (strcmp(newCommand, "EDIT_BOOK") == 0) {
            char title[50], publishedYear[5], author[50], book_id[15];
            printBookList();
            int trial = 0;
            while (searchDB(BOOK_LIST, 1, book_id) == NULL) {
                (trial)? printf("\nBook ID does not exist. Enter a new book id to edit: ") :printf("\nEnter the book id to edit: ");
                 scanf_s("%[^\n]%*c", book_id, 15);
                 back2Menu(book_id);
                 trial++;
            }

            printf("\nEnter the title of the book: ");
            scanf_s("%[^\n]%*c", title, 50);
            back2Menu(title);
            printf("\nEnter the author of the book: ");
            scanf_s("%[^\n]%*c", author, 50);
            back2Menu(author);
            printf("\nEnter the book publication year: ");
            scanf_s("%[^\n]%*c", publishedYear, 5);
            back2Menu(publishedYear);
            editBook(book_id,title, author, publishedYear);
        }
        else if (strcmp(newCommand, "LIST_BOOK") == 0) {
            printf("List of books in the library\n\n");
            printBookList();
        }
        else if (strcmp(newCommand, "DELETE_BOOK") == 0) {
            char book_id[15];
            printBookList();
            int trial = 0;
            while (searchDB(BOOK_LIST, 1, book_id) == NULL) {
                (trial) ? printf("\nBook ID does not exist. Enter a new book id to delete: ") : printf("\nEnter the book id to delete: ");
                scanf_s("%[^\n]%*c", book_id, 15);
                back2Menu(book_id);
                trial++;
            }
            deleteBook(book_id);
        }
        else if (strcmp(newCommand, "BORROW_BOOK") == 0) {
            char borrow_name[50], book_id[15];
            printBookList();
            int trial = 0;
            while (searchDB(BOOK_LIST, 1, book_id) == NULL) {
                (trial) ? printf("\nBook ID does not exist. Enter a new book id to you want to borrow: ") : printf("\nEnter the book id to you want to borrow: ");
                scanf_s("%[^\n]%*c", book_id, 15);
                back2Menu(book_id);
                trial++;
            }
            if (strlen(getField(searchDB(BOOK_LIST, 1, book_id), 5)) > 9) {
                printf("This book is held by %s", getField(searchDB(BORROW_LIST, 3, book_id), 2));
            }
            else {
                printf("\nEnter the name of the borrower: ");
                scanf_s("%[^\n]%*c", borrow_name, 50);
                back2Menu(borrow_name);
                borrowBook(book_id, borrow_name);
            }
        }
        else if (strcmp(newCommand, "RETURN_BOOK") == 0) {
            char borrow_id[15], book_id[15];
            printBorrowList(0);
            int trial = 0;
            while (searchDB(BORROW_LIST, 1, borrow_id) == NULL) {
                (trial) ? printf("\nBorrow ID does not exist. Enter a new borrow id of the book you want to return: ") : printf("\nEnter the borrow id to you want to return: ");
                scanf_s("%[^\n]%*c", borrow_id, 15);
                back2Menu(borrow_id);
                trial++;
            }
            strcpy_s(book_id,15,getField(searchDB(BORROW_LIST, 1, borrow_id),3));
            updateCell(BOOK_LIST, 1, book_id, 5, "_");

            time_t t = time(NULL); 
            char time_str[26];
            ctime_s(time_str, sizeof time_str, &t);
            time_str[strlen(time_str) - 1] = '\0';
            updateCell(BORROW_LIST, 1, borrow_id, 5, time_str);
        }
        else if (strcmp(newCommand, "EDIT_BORROWED_BOOK") == 0){
            char borrow_id[15], borrower_name[50];
            printBorrowList(1);
            int trial = 0;
            while (searchDB(BORROW_LIST, 1, borrow_id) == NULL) {
                (trial) ? printf("\nBorrow ID does not exist. Enter a new borrow id of the book you want to return: ") : printf("\nEnter the borrow id to you want to return: ");
                scanf_s("%[^\n]%*c", borrow_id, 15);
                back2Menu(borrow_id);
                trial++;
            }
            printf("\nOnly the borrower name can be edited, if you want to change the book borrowed return this book and borrow a new book.\nEnter the name of the borrower: ");
            scanf_s("%[^\n]%*c", borrower_name, 50);
            back2Menu(borrower_name);
            updateCell(BORROW_LIST, 1, borrow_id, 2, borrower_name);
        }
        else if (strcmp(newCommand, "BORROWED_BOOKS") == 0) {
            printf("List of borrowed books\n");
            printBorrowList(1);
        }

    }
}


//get a field(cell) value in a comma separated data
const char* getField(char* str, int index) {
    char* arrayOfString[30];
    char* token;
    int count = 0;

    char temp[1024];
    strcpy_s(temp,1024, str);

    rsize_t strmax = sizeof str;
    char* next_token;
    const char delim[2] = ",";
    token = strtok_s(str, delim, &next_token);
    while (token != NULL)
    {
        arrayOfString[count] = token;
        token = strtok_s(NULL, delim, &next_token);
        count++;
    }
    if (index > count) {
        return NULL;
    }

    char* retVal = malloc(50);
    for (int i = 0;i < count;i++)
        if (i + 1 == index)
            strcpy_s(retVal,52, arrayOfString[i]);

    strcpy_s(str,1024, temp);
    retVal[strcspn(retVal, "\r\n")] = 0; //remove the break line at the last column
    return retVal;
}

//generate random string: small letters only, Capital letters only, Digits only, alpha-numerals and alpha-numerals with special letter
// length should be qualified as const if you follow a rigorous standard
char* randstring(size_t length, int type) {

    char charset[100];
    int charsetlen;
    switch (type) {
    case 0:
        strcpy_s(charset,100, "abcdefghijklmnopqrstuvwxyz");
        charsetlen = 26;
        break;
    case 1:
        strcpy_s(charset, 100, "ABCDEFGHIJKLMNOPQRSTUVWXYZ");
        charsetlen = 26;
        break;
    case 2:
        strcpy_s(charset, 100, "0123456789");
        charsetlen = 10;
        break;
    case 3:
        strcpy_s(charset, 100, "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789");
        charsetlen = 62;
        break;
    default:
        strcpy_s(charset, 100, "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789,.-#'?!");
        charsetlen = 69;
        break;
    }
    char* randomString = malloc(length + 1);   // initializing to NULL isn't necessary as malloc() returns NULL if it couldn't allocate memory as requested

    if (length) {
        //randomString = malloc(length + 1); // I removed your `sizeof(char) * (length +1)` as sizeof(char) == 1, cf. C99

        if (randomString) {
            for (int n = 0;n < length;n++) {
                int key = rand() % charsetlen;//(int) (sizeof(charset) -1);
                randomString[n] = charset[key];
            }
            randomString[length] = '\0';
        }
    }

    return randomString;
}


//search a partuclar column in a file to know if it exist, if it exist return the row ID where it exist or return 0
char* searchDB(char* file_path, int index, char* keyword) {
    int position = 0;
    int seen = 0;
    char *found = malloc(DATALEN);
    FILE* file;
    fopen_s(&file, file_path, "r");
    char line[1024];
    while (fgets(line, 1024, file))
    {
        if (strcmp(getField(line, index), keyword) == 0) {
            strcpy_s(found, DATALEN, line);
            seen = 1;
            break;
        }
        position++;
    }
    fclose(file);
    return (seen)?found:NULL;
}


//add a book to the list of books with a random alpha-numeral as ID
void addBook(char* title, char* author, char* publishedYear)
{
    char book_id[15];
    //generate an ID with random 6 alphabets and random 4 digits
    strcpy_s(book_id, 15, randstring(6, 1));
    strcat_s(book_id, 15, randstring(4, 2));
    char* file_path = BOOK_LIST;
    FILE* file;
    fopen_s(&file, file_path, "a");
    fprintf(file, "%s,%s,%s,%s,_\n", book_id, title, author, publishedYear);
    fclose(file);
    printf("Book added to the library successfully...\n");
}
//borrow a book from the library
void borrowBook(char* book_id, char* borrower_name)
{
    char borrow_id[15];
    //generate an ID with random 6 alphabets and random 4 digits
    strcpy_s(borrow_id, 15, randstring(6, 1));
    strcat_s(borrow_id, 15, randstring(4, 2));
    //generate the current time
    time_t t = time(NULL);   // not a primitive datatype
    char time_str[26];
    ctime_s(time_str, sizeof time_str, &t);
    char* file_path = BORROW_LIST;
    FILE* file;
    fopen_s(&file, file_path, "a");
    time_str[strlen(time_str) - 1] = '\0';
    fprintf(file, "%s,%s,%s,%s,_\n",borrow_id, borrower_name,book_id, time_str);
    fclose(file);
    updateCell(BOOK_LIST, 1, book_id, 5, borrow_id);
    printf("Book borrowed successfully...\n");
}

//update a cell in the database
char* updateCell(char* file_path, int searchIndex, char* searchKeyword, int replaceIndex, char* replaceKeyword)
{
    char lines[1024];
    FILE* file;
    fopen_s(&file, file_path, "r");
    char data[DATALEN];
    int update = 0;

    struct Node* dataList = NULL;
    while (fgets(lines, 1024, file))
    {
        strcpy_s(data, DATALEN, "");
        if (strcmp(getField(lines, searchIndex), searchKeyword) == 0) {
            for (int i = 1; i <= 5; i++) {
                if (i == replaceIndex) {
                    strcat_s(data, DATALEN, replaceKeyword);
                }
                else {
                    strcat_s(data, DATALEN, getField(lines, i));
                }
                (i == 5)? strcat_s(data, DATALEN, "\n"): strcat_s(data, DATALEN, ",");
            }
            push2Node(&dataList, data);
            update = 1;
        }
        else {
            push2Node(&dataList, lines);
        }
    }
    fclose(file);

    node2File(file_path, dataList);

    if (update == 1) {
        printf("database updated successfully....\n");
    }
    else {
        printf("Failed to update database....\n");
    }
}

//print borrow_list.csv file
void printBorrowList(int all) {
    char* file_path = BORROW_LIST;
    FILE* file;
    fopen_s(&file, file_path, "r");
    char line[1024];
    int row = 0;
    while (fgets(line, 1024, file))
    {
        char book_title[50], book_author[50];
        if (row == 0) {
            printf("%*s %*s %*s %*s %*s %*s\n\n", -CLEN, getField(line, 1), -CLEN, getField(line, 2), -CLEN, "Book Title", -CLEN, "Book Author", -CLEN, getField(line, 4), -CLEN, getField(line, 5));
        }
        else {
            strcpy_s(book_title, 50, getField(searchDB(BOOK_LIST, 1, getField(line, 3)), 2));
            strcpy_s(book_author, 50, getField(searchDB(BOOK_LIST, 1, getField(line, 3)), 3));
            if (all == 1) {
                printf("%*s %*s %*s %*s %*s %*s\n\n", -CLEN, getField(line, 1), -CLEN, getField(line, 2), -CLEN, book_title, -CLEN, book_author, -CLEN, getField(line, 4), -CLEN, getField(line, 5));
            }
            else {
                if (strlen(getField(line, 5)) < 9) {
                    printf("%*s %*s %*s %*s %*s %*s\n\n", -CLEN, getField(line, 1), -CLEN, getField(line, 2), -CLEN, book_title, - CLEN, book_author, -CLEN, getField(line, 4), -CLEN, getField(line, 5));
                }
            }
        }
        
        row++;
    }
    fclose(file);
}

//print book_list.csv file
void printBookList() {
    char* file_path = BOOK_LIST;
    FILE* file;
    fopen_s(&file, file_path, "r");
    char line[1024];
    int row = 0;
    while (fgets(line, 1024, file))
    {
        char borrow_by[50], borrow_on[25];
        strcpy_s(borrow_by, 50, getField(line, 5));
        strcpy_s(borrow_on, 25, "_");
        if (strlen(getField(line, 5)) > 9) {
            char data[DATALEN];
            strcpy_s(data, DATALEN,  getField(searchDB(BORROW_LIST, 1, getField(line, 5)), 2));
            if (data != NULL) {
                strcpy_s(borrow_by, 50, data);
                strcpy_s(borrow_on, 25, getField(searchDB(BORROW_LIST, 1, getField(line, 5)), 4));
            }
        }
        if (row == 0) {
            printf("%*s %*s %*s %*s %*s %*s\n\n", -CLEN, getField(line, 1), -CLEN, getField(line, 2), -CLEN, getField(line, 3), -CLEN, getField(line, 4), -CLEN, "Borrowed By", -CLEN, "Borrowed On");
        }
        else {
            printf("%*s %*s %*s %*s %*s %*s\n\n", -CLEN, getField(line, 1), -CLEN, getField(line, 2), -CLEN, getField(line, 3), -CLEN, getField(line, 4), -CLEN, borrow_by, -CLEN, borrow_on);
        }
        row++;
    }
    fclose(file);
}


// update the book details in the book_list.csv file using the book ID
void editBook(char* book_id, char* title, char* author, char* publishedYear) {
    char lines[1024];
    char* file_path = BOOK_LIST;
    FILE* file;
    fopen_s(&file, file_path, "r");
    char data[DATALEN];
    int update = 0;

    struct Node* dataList = NULL;
    while (fgets(lines, 1024, file))
    {
        strcpy_s(data,DATALEN, "");
        if (strcmp(getField(lines, 1), book_id) == 0) {
            strcat_s(data, DATALEN, getField(lines, 1));
            strcat_s(data, DATALEN, ",");
            strcat_s(data, DATALEN, title);
            strcat_s(data, DATALEN, ",");
            strcat_s(data, DATALEN, author);
            strcat_s(data, DATALEN, ",");
            strcat_s(data, DATALEN, publishedYear);
            strcat_s(data, DATALEN, ",");
            strcat_s(data, DATALEN, getField(lines, 5));
            strcat_s(data, DATALEN, "\n");
            push2Node(&dataList, data);
            update = 1;
        }
        else {
            push2Node(&dataList, lines);
        }
    }
    fclose(file);


    node2File(file_path, dataList);

    if (update == 1) {
        printf("Library updated successfully....\n");
    }
    else {
        printf("Failed to update library....\n");
    }
}


//delete a book using the book ID
void deleteBook(char* book_id) {
    char lines[DATALEN];
    char* file_path = BOOK_LIST;
    FILE* file;
    fopen_s(&file, file_path, "r");
    int update = 0;
    struct Node* dataList = NULL;
    while (fgets(lines, DATALEN, file))
    {
        //if the column matches with the book ID skip the row
        if (strcmp(getField(lines, 1), book_id) == 0) {
            update = 1;
            continue;
        }
        else {
            push2Node(&dataList, lines);
        }
    }
    fclose(file);
    //update file after deleting the book
    node2File(file_path, dataList);

    if (update == 1) {
        printf("book removed from the library successfully....\n");
    }
    else {
        printf("Failed to remove book from the library....\n");
    }
}




// Node list functions

/* Function to reverse the linked list */
void printNode(struct Node* head)
{
	// Base case
	if (head == NULL)
		return;

	// print the list after head node
	printNode(head->next);

	// After everything else is printed, print head
	printf("%s \n", head->data);
}
void node2File(char file_path[], struct Node* dataList)
{
    FILE* fileEdit;
	//empty the file
	fopen_s(&fileEdit, file_path, "w");
	fprintf(fileEdit, "%s", "");
	fclose(fileEdit);
	//store data to file
	persistData(file_path, dataList);
}

/* Function to reverse the linked list */
void persistData(char file_path[], struct Node* head)
{
	// Base case
	if (head == NULL)
		return;

	// print the list after head node
	//printReverse(head->next);
	persistData(file_path, head->next);

	// After everything else is printed, print head
	//printf("%s \n", head->data);

    FILE* fileEdit;
    fopen_s(&fileEdit, file_path, "a");
	fprintf(fileEdit, "%s", head->data);
	fclose(fileEdit);
}

/*UTILITY FUNCTIONS*/
/* Push a node to linked list. Note that this function
changes the head */
void push2Node(struct Node** head_ref, char new_data[DATALEN])
{
    size_t nodeSize = sizeof(struct Node);
	/* allocate node */
	struct Node* new_node = (struct Node*)malloc(nodeSize);

	/* put in the data */
	strcpy_s(new_node->data, nodeSize, new_data);

	/* link the old list off the new node */
	new_node->next = (*head_ref);

	/* move the head to point to the new node */
	(*head_ref) = new_node;
}
