#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_BOOKS 100
#define MAX_USERS 100
#define MAX_NEW_BOOKS 100
#define MAX_TITLE_LENGTH 50
#define MAX_AUTHOR_LENGTH 50
#define MAX_USERNAME_LENGTH 50
#define MAX_PASSWORD_LENGTH 50

// Structures
typedef struct {
    int id;
    char title[MAX_TITLE_LENGTH];
    char author[MAX_AUTHOR_LENGTH];
    int available;
} Book;

typedef struct {
    int id;
    char name[MAX_USERNAME_LENGTH];
    int *borrowedBooks;
    int numBooksBorrowed;
    int maxBooks; // Maximum number of books that can be borrowed
} User;

typedef struct {
    int id;
    char title[50];
    char author[50];
} NewBook;

// Global Variables
Book *books;
User *users;
NewBook *newBooks;
int numBooks = 0;
int numUsers = 0;
int numNewBooks = 0;

// Function prototypes
int authenticateUser();
void registerUser();
void addBook();
void addUser();
void removeUser();
void borrowBook();
void returnBook();
void displayBooks();
void displayUsers();
void displayNewBooks();
void findBooksByAuthor();
void findUserByName();
void cleanup();
void sortBooksByTitle();
void sortUsersByName();
void repeatFunction();
void displayMenu();
void wipe();

// Function to clear console screen
void wipe() {
    #ifdef _WIN32
        system("cls");
    #elif _WIN64
        system("cls");
    #else
        system("clear");
    #endif
}

// Function to authenticate user by checking username and password in a file
int authenticateUser() {
    char username[MAX_USERNAME_LENGTH];
    char password[MAX_PASSWORD_LENGTH];
    printf("Enter username: ");
    scanf("%s", username);
    printf("Enter password: ");
    scanf("%s", password);

    FILE *file = fopen("users.txt", "r+");
    if (file == NULL) {
        printf("Error opening file for authentication.\n");
        return 0;
    }

    char fileUsername[MAX_USERNAME_LENGTH];
    char filePassword[MAX_PASSWORD_LENGTH];
    int userExists = 0;
    while (fscanf(file, "%s %s", fileUsername, filePassword) != EOF) {
        if (strcmp(username, fileUsername) == 0 && strcmp(password, filePassword) == 0) {
            fclose(file);
            return 1;
        }
        if (strcmp(username, fileUsername) == 0) {
            userExists = 1;
        }
    }

    fclose(file);

    if (userExists) {
        // If the user doesn't exist, prompt to create a new username and password
        printf("User not found. Create a new username and password:\n");
        FILE *newUserFile = fopen("users.txt", "a");
        if (newUserFile == NULL) {
            printf("Error opening file for new user registration.\n");
            return 0;
        }
        fprintf(newUserFile, "%s %s\n", username, password);
        fclose(newUserFile);
        printf("New user registered successfully.\n");
        return 1;
    }

    return 0;
}

// Function to register a new user and store their username and password in a file
void registerUser() {
    printf("Register a new username and password:\n");
    char username[MAX_USERNAME_LENGTH];
    char password[MAX_PASSWORD_LENGTH];
    printf("Enter username: ");
    scanf("%s", username);
    printf("Enter password: ");
    scanf("%s", password);

    FILE *file = fopen("users.txt", "a");
    if (file == NULL) {
        printf("Error opening file for user registration.\n");
        return;
    }
    fprintf(file, "%s %s\n", username, password);
    fclose(file);
    printf("User registered successfully.\n");
}

// Function to prompt the user for repeating an operation or exiting the program
void repeatFunction() {
    char choice;
    printf("Do you want to perform another operation? (y/n): ");
    scanf(" %c", &choice);
    if (choice == 'y' || choice == 'Y') {
        return;
    } else if (choice == 'n' || choice == 'N') {
        printf("Exiting program.\n");
        cleanup();
        exit(0);
    } else {
        printf("Invalid choice. Exiting program.\n");
        cleanup();
        exit(1);
    }
}

// Function to add a book to the library
void addBook() {
    // Check if maximum number of books limit is reached
    if (numBooks < MAX_BOOKS) {
        Book newBook;
        printf("Enter book title: ");
        scanf(" %[^\n]s", newBook.title);
        printf("Enter author name: ");
        scanf(" %[^\n]s", newBook.author);
        newBook.id = numBooks + 1;
        newBook.available = 1; // Initially available
        books[numBooks++] = newBook;
        printf("Book added successfully.\n");
        repeatFunction();
    } else {
        printf("Maximum books limit reached.\n");
        repeatFunction();
    }
}

// Function to add a new user to the system
void addUser() {
    // Check if maximum number of users limit is reached
    if (numUsers < MAX_USERS) {
        User newUser;
        printf("Enter user name: ");
        scanf(" %[^\n]s", newUser.name);
        newUser.id = numUsers + 1;
        newUser.numBooksBorrowed = 0;
        newUser.maxBooks = 2; // Example maximum number of books user can borrow
        newUser.borrowedBooks = (int *)malloc(newUser.maxBooks * sizeof(int));
        if (newUser.borrowedBooks == NULL) {
            printf("Memory allocation failed. Exiting program.\n");
            exit(1);
        }
        users[numUsers++] = newUser;
        printf("User added successfully.\n");
        repeatFunction();
    } else {
        printf("Maximum users limit reached.\n");
        repeatFunction();
    }
}

// Function to remove a user from the system
void removeUser() {
    int userId;
    printf("Enter the ID of the user to remove: ");
    scanf("%d", &userId);

     if (userId >= 1 && userId <= numUsers) {
        // Free memory allocated for borrowedBooks
        free(users[userId - 1].borrowedBooks);
        
        // Remove the user by shifting elements
        for (int i = userId - 1; i < numUsers - 1; i++) {
            users[i] = users[i + 1];
        }
        numUsers--;
        printf("User removed successfully.\n");
        repeatFunction();
    } else {
        printf("Invalid user ID.\n");
        repeatFunction();
    }
}

// Function to allow a user to borrow a book
void borrowBook() {
    int userId, bookId;
    printf("Enter user ID: ");
    scanf("%d", &userId);
    printf("Enter book ID: ");
    scanf("%d", &bookId);

    if (userId >= 1 && userId <= numUsers && bookId >= 1 && bookId <= numBooks) {
        User *user = &users[userId - 1];
        Book *book = &books[bookId - 1];

        if (book->available && user->numBooksBorrowed < user->maxBooks ) {
            book->available = 0; // Mark as unavailable
            user->borrowedBooks[user->numBooksBorrowed++] = bookId;
            printf("Book borrowed successfully.\n");
            repeatFunction();
        } else {
            printf("Book not available or user has reached borrowing limit.\n");
            repeatFunction();
        }
    } else {
        printf("Invalid user ID or book ID.\n");
        repeatFunction();
    }
}

// Function to allow a user to return a borrowed book
void returnBook() {
    int userId, bookId;
    printf("Enter user ID: ");
    scanf("%d", &userId);
    printf("Enter book ID: ");
    scanf("%d", &bookId);

    if (userId >= 1 && userId <= numUsers && bookId >= 1 && bookId <= numBooks) {
        User *user = &users[userId - 1];
        Book *book = &books[bookId - 1];

        int i;
        for (i = 0; i < user->numBooksBorrowed; i++) {
            if (user->borrowedBooks[i] == bookId) {
                book->available = 1; // Mark as available
                user->borrowedBooks[i] = user->borrowedBooks[user->numBooksBorrowed - 1];
                user->numBooksBorrowed--;
                printf("Book returned successfully.\n");
                repeatFunction();
                return;
            }
        }
        printf("User has not borrowed this book.\n");
        repeatFunction();
    } else {
        printf("Invalid user ID or book ID.\n");
        repeatFunction();
    }
}

// Function to display all books in the library
void displayBooks() {
    if (numBooks == 0) {
        printf("No books available.\n");
    } else {
        printf("ID\tTitle\tAuthor\tAvailable\n");
        for (int i = 0; i < numBooks; i++) {
            printf("%d\t%s\t%s\t%s\n", books[i].id, books[i].title, books[i].author,
                   books[i].available ? "Yes" : "No");
        }
    }
    repeatFunction();
}

// Function to display all users registered in the system
void displayUsers() {
    if (numUsers == 0) {
        printf("No users registered.\n");
    } else {
        printf("ID\tName\tBooks Borrowed\n");
        for (int i = 0; i < numUsers; i++) {
            printf("%d\t%s\t", users[i].id, users[i].name);
            if (users[i].numBooksBorrowed > 0) {
                for (int j = 0; j < users[i].numBooksBorrowed; j++) {
                    printf("%d ", users[i].borrowedBooks[j]);
                }
            } else {
                printf("None");
            }
            printf("\n");
        }
    }
    repeatFunction();
}

// Function to display all new books
void displayNewBooks() {
    if (numNewBooks == 0) {
        printf("No new books available.\n");
        repeatFunction();
    }

    printf("ID\tTitle\tAuthor\n");
    for (int i = 0; i < numNewBooks; i++) {
        printf("%d\t%s\t%s\n", newBooks[i].id, newBooks[i].title, newBooks[i].author);
    }
    repeatFunction();
}

// Function to find books by a specific author
void findBooksByAuthor() {
    char author[MAX_AUTHOR_LENGTH];
    printf("Enter the author's name: ");
    scanf(" %[^\n]s", author);

    int found = 0;
    printf("Books by %s:\n", author);
    for (int i = 0; i < numBooks; i++) {
        if (strcmp(books[i].author, author) == 0) {
            printf("%d\t%s\t%s\t%s\n", books[i].id, books[i].title, books[i].author,
                   books[i].available ? "Yes" : "No");
            found = 1;
        }
    }
    if (!found) {
        printf("No books found by %s.\n", author);
    }
    repeatFunction();
}

// Function to find users by their name
void findUserByName() {
    char name[MAX_USERNAME_LENGTH];
    printf("Enter the user's name: ");
    scanf(" %[^\n]s", name);

    int found = 0;
    printf("Users with name %s:\n", name);
    for (int i = 0; i < numUsers; i++) {
        if (strcmp(users[i].name, name) == 0) {
            printf("%d\t%s\t", users[i].id, users[i].name);
            if (users[i].numBooksBorrowed > 0) {
                for (int j = 0; j < users[i].numBooksBorrowed; j++) {
                    printf("%d ", users[i].borrowedBooks[j]);
                }
            } else {
                printf("None");
            }
            printf("\n");
            found = 1;
        }
    }
    if (!found) {
        printf("No users found with name %s.\n", name);
    }
    repeatFunction();
}

// Function to free allocated memory
void cleanup() {
    free(books);
    free(users);
    free(newBooks);
}

// Function to perform Quick Sort on books array based on title
void quickSortBooks(int left, int right) {
    int i = left, j = right;
    Book temp;
    char* pivot = books[(left + right) / 2].title;

    while (i <= j) {
        while (strcmp(books[i].title, pivot) < 0) i++;
        while (strcmp(books[j].title, pivot) > 0) j--;
        if (i <= j) {
            temp = books[i];
            books[i] = books[j];
            books[j] = temp;
            i++;
            j--;
        }
    }

    if (left < j) quickSortBooks(left, j);
    if (i < right) quickSortBooks(i, right);
}

// Function to sort books by title
void sortBooksByTitle() {
    if (numBooks <= 1) {
        printf("Not enough books to sort.\n");
        repeatFunction();
        return;
    }

    quickSortBooks(0, numBooks - 1);
    printf("Books sorted successfully by title.\n");
    repeatFunction();
}

// Function to perform Quick Sort on users array based on name
void quickSortUsers(int left, int right) {
    int i = left, j = right;
    User temp;
    char* pivot = users[(left + right) / 2].name;

    while (i <= j) {
        while (strcmp(users[i].name, pivot) < 0) i++;
        while (strcmp(users[j].name, pivot) > 0) j--;
        if (i <= j) {
            temp = users[i];
            users[i] = users[j];
            users[j] = temp;
            i++;
            j--;
        }
    }

    if (left < j) quickSortUsers(left, j);
    if (i < right) quickSortUsers(i, right);
}

// Function to sort users by name
void sortUsersByName() {
    if (numUsers <= 1) {
        printf("Not enough users to sort.\n");
        repeatFunction();
        return;
    }

    quickSortUsers(0, numUsers - 1);
    printf("Users sorted successfully by name.\n");
    repeatFunction();
}

// Function to display the main menu
void displayMenu() {
    wipe();
    printf("\n ------------------------------\n");
    printf(" ******************************\n");
    printf(" ------------------------------\n");
    printf(" IITJ Library Management System\n");
    printf(" ------------------------------\n");
    printf(" ******************************\n");
    printf(" ------------------------------\n");
    printf("1. Add Book\n");
    printf("2. Add User\n");
    printf("3. Remove User\n");
    printf("4. Borrow Book\n");
    printf("5. Return Book\n");
    printf("6. Display Books\n");
    printf("7. Display Users\n");
    printf("8. Display New Books\n");
    printf("9. Find Books by Author\n");
    printf("10. Find User by Name\n");
    printf("11. Sort Books by Title\n");
    printf("12. Sort Users by Name\n");
    printf("0. Exit\n");
    printf(" ------------------------------\n");
    printf(" ******************************\n");
    printf(" ------------------------------\n");
    printf("Enter your choice: ");
}

// Main function
int main() {
    // Register or authenticate user
    wipe();
    printf("\n--------------------*--------------------\n");
    printf("Welcome to the IITJ Library Management System!\n");
    printf("--------------------*--------------------\n");
    printf("1. Register:- \n");
    printf("2. Login:- \n");
    int option;
    scanf("%d", &option);
    if (option == 1) {
        registerUser();
    } else if (option == 2) {
        if (!authenticateUser()) {
            printf("Authentication failed. Exiting program.\n");
            return 1;
        }
    } else {
        printf("Invalid option. Exiting program.\n");
        return 1;
    }

    int maxBooks, maxUsers, maxNewBooks;

    // Prompt the user to input the maximum number of books, users, and new books
    printf("Enter the maximum number of books: ");
    scanf("%d", &maxBooks);
    printf("Enter the maximum number of users: ");
    scanf("%d", &maxUsers);
    printf("Enter the maximum number of new books: ");
    scanf("%d", &maxNewBooks);

    // Allocate memory for arrays based on user input
    books = (Book *)malloc(maxBooks * sizeof(Book));
    users = (User *)malloc(maxUsers * sizeof(User));
    newBooks = (NewBook *)malloc(maxNewBooks * sizeof(NewBook));

    if (books == NULL || users == NULL || newBooks == NULL) {
        printf("Memory allocation failed. Exiting program.\n");
        return 1;
    }

    // Add some example users
    User exampleUsers[] = {
        {1, "AJAY", NULL, 0 , 2},
        {2, "PIYUSH", NULL, 0 , 1},
        {3, "LEE", NULL, 0 , 0},
        {4, "SHIVA" ,NULL, 0, 3}
    };

    for (int j = 0; j < sizeof(exampleUsers) / sizeof(exampleUsers[0]); j++) {
        users[numUsers++] = exampleUsers[j];
    }

    int choice;

    // Main menu loop
    do {
        displayMenu();
        scanf("%d", &choice);

        switch (choice) {
            case 1:
                addBook();
                break;
            case 2:
                addUser();
                break;
            case 3:
                removeUser();
                break;
            case 4:
                borrowBook();
                break;
            case 5:
                returnBook();
                break;
            case 6:
                displayBooks();
                break;
            case 7:
                displayUsers();
                break;
            case 8:
                displayNewBooks();
                break;
            case 9:
                findBooksByAuthor();
                break;
            case 10:
                findUserByName();
                break;
            case 11:
                sortBooksByTitle();
                break;
            case 12:
                sortUsersByName();
                break;
            case 0:
                printf("Exiting program.\n");
                break;
            default:
                printf("Invalid choice. Please enter a number between 0 and 12.\n");
        }
    } while (choice != 0);

    // Clean up allocated memory
    cleanup();

    return 0;
}
