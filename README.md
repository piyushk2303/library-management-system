# IITJ Library Management System

This program is a simple library management system implemented in C. It allows users to perform various operations such as adding books, adding users, borrowing and returning books, displaying books and users, finding books by author or users by name, and sorting books and users.

## Features

- **User Authentication**: Users can register with a username and password, and then log in to access the system.
- **User Registration**: New users can register with a username and password.
- **Book Management**: Users can add books, display books, and find books by author. Books can also be borrowed and returned.
- **User Management**: Users can be added, removed, and displayed. Users can also be found by name.
- **Search Functionality (sorting)**: Books and users can be sorted by title and name, respectively.

## How to Use

1. **Registration/Login**: 
    - Users can choose to register with a new username and password or log in if already registered.
    - Authentication is done using a text file `users.txt`.

2. **Main Menu**:
    - Users are presented with a menu of options to perform various operations.
    - Options include adding books/users, borrowing/returning books, displaying data, searching, and sorting.

3. **Data Storage**:
    - User credentials are stored in `users.txt`.
    - Book data is stored in dynamically allocated arrays.

4. **Compilation**:
    - Compile the source code using a C compiler like `gcc`.
    - Example: `gcc library_management.c -o library_management`

5. **Run**:
    - Execute the compiled program.
    - Example: `./library_management`


## Compilation and Execution

To compile the program, use a C compiler such as GCC:

To run the program, execute the compiled binary:

## Requirements

- C Compiler (e.g., GCC)

- Standard C Libraries

**------**
