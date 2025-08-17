#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<time.h>

#define Max_Title 100
#define Max_Author 100

typedef struct libeary_management
{
    int id;
    char title[Max_Title];
    char author[Max_Author];
    time_t added_date;
    int quantity;
}Book;

void addBook() {
    FILE *file = fopen("library.dat", "ab"); // append in binary
    if (!file) {
        printf("Error opening file for writing!\n");
        return;
    }

    Book book;
    printf("Enter book ID: ");
    scanf("%d", &book.id);
    getchar(); // clear buffer

    printf("Enter book title: ");
    fgets(book.title, sizeof(book.title), stdin);
    book.title[strcspn(book.title, "\n")] = '\0';

    printf("Enter book author: ");
    fgets(book.author, sizeof(book.author), stdin);
    book.author[strcspn(book.author, "\n")] = '\0';

    printf("Enter book quantity: ");
    scanf("%d", &book.quantity);

    book.added_date = time(NULL);

    // write to file
    fwrite(&book, sizeof(Book), 1, file);
    fclose(file);

    printf("\nBook added successfully!\n");
}


void displayBooks() {
    FILE *file = fopen("library.dat", "rb");
    if (!file) {
        printf("\nNo books found in the library! Please add some first.\n");
        return;
    }

    Book book;
    int count = 0;

    printf("\n--- Books in the Library ---\n");
    while (fread(&book, sizeof(Book), 1, file)) {
        printf("ID: %d | Title: %s | Author: %s | Added: %s | Quantity: %d\n",
               book.id, book.title, book.author,
               ctime(&book.added_date), book.quantity);
        count++;
    }

    if (count == 0)
        printf("Library is empty!\n");

    fclose(file);
}


void search_book(){
    int id;
    printf("Enter book ID to search: ");
    scanf("%d", &id);

    FILE *file = fopen("library.txt", "r");
    if (file == NULL) {
        printf("Error opening file!\n");
        return;
    }

    Book book;
    int found = 0;
    while (fread(&book, sizeof(Book), 1, file)) {
        if (book.id == id) {
            printf("Book found:\n");
            printf("ID: %d\n", book.id);
            printf("Title: %s\n", book.title);
            printf("Author: %s\n", book.author);
            printf("Added Date: %s", ctime(&book.added_date));
            printf("Quantity: %d\n", book.quantity);
            found = 1;
            break;
        }
    }

    if (!found) {
        printf("Book not found.\n");
    }

    fclose(file);
}

void issue_book(){
    int id;
    printf("Enter book ID to issue: ");
    scanf("%d", &id);

    FILE *file = fopen("library.txt", "r+");
    if (file == NULL) {
        printf("Error opening file!\n");
        return;
    }

    Book book;
    int found = 0;
    while (fread(&book, sizeof(Book), 1, file)) {
        if (book.id == id) {
            if (book.quantity > 0) {
                book.quantity--;
                fseek(file, -sizeof(Book), SEEK_CUR);
                fwrite(&book, sizeof(Book), 1, file);
                printf("Book issued successfully.\n");
            } else {
                printf("No copies available for this book.\n");
            }
            found = 1;
            break;
        }
    }

    if (!found) {
        printf("Book not found.\n");
    }

    fclose(file);
}

void return_book(){
    int id;
    printf("Enter book ID to return: ");
    scanf("%d", &id);

    FILE *file = fopen("library.txt", "r+");
    if (file == NULL) {
        printf("Error opening file!\n");
        return;
    }

    Book book;
    int found = 0;
    while (fread(&book, sizeof(Book), 1, file)) {
        if (book.id == id) {
            book.quantity++;
            fseek(file, -sizeof(Book), SEEK_CUR);
            fwrite(&book, sizeof(Book), 1, file);
            printf("Book returned successfully.\n");
            found = 1;
            break;
        }
    }

    if (!found) {
        printf("Book not found.\n");
    }

    fclose(file);
}

void logTransaction(const char *action, int book_id){
    FILE *file = fopen("transaction_log.txt", "a");
    if (file == NULL) {
        printf("Error opening log file!\n");
        return;
    }

    fprintf(file, "Action: %s, Book ID: %d, Date: %s",
            action, book_id, ctime(&(time_t){time(NULL)}));
    fclose(file);
}

void viewTransactionLog(){
    FILE *file = fopen("transaction_log.txt", "r");
    if (file == NULL) {
        printf("Error opening log file!\n");
        return;
    }

    char line[256];
    printf("Transaction Log:\n");
    while (fgets(line, sizeof(line), file)) {
        printf("%s", line);
    }

    fclose(file);
}

int main(){
    printf("Welcome to the Library Management System");
    int choice;
    while (1){
                printf("\n===== LIBRARY MANAGEMENT SYSTEM =====\n");
        printf("1. Add Book\n");
        printf("2. Display All Books\n");
        printf("3. Search Book\n");
        printf("4. Issue Book\n");
        printf("5. Return Book\n");
        printf("6. View Transactions\n");
        printf("7. Exit\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);
        getchar();  // clear input buffer

        switch (choice) {
            case 1: addBook(); break;
            case 2: displayBooks(); break;
            case 3: search_book(); break;
            case 4: issue_book(); break;
            case 5: return_book(); break;
            case 6: viewTransactionLog(); break;
            case 7: printf("Exiting... Goodbye!\nDEVELOPED BY KESHAV SHARMA (REG NO: 2502030005) (MANIPAL UNIVERSITY JAIPUR)"); exit(0);
            default: printf("Invalid choice! Try again.\n");
        }
    }
    return 0;
}