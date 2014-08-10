#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

// MySQL includes and initialization block -JS 8/9

// System header include.
#include <mysql/mysql.h>

// MySQL wrapper automates error reporting.
int mysql_wrapper(MYSQL *mysql, char *query);

// MySQL variables.
MYSQL *mysql;
MYSQL_RES *res;
MYSQL_ROW row;

// MySQL query construction buffer.
char querybuf[20000];

// End MySQL includes and init block.

// Magic numbers are bad! Pre-defining constants makes everyone's life easier.
#define MAX_NAME_LENGTH 20
#define MAX_PWD_LENGTH 15

#define OPT_UNSET 0
#define OPT_I_OWE_THEM 1
#define OPT_THEY_OWE_ME 2
#define OPT_ADD_DEBT 3
#define OPT_CLEAR_DEBT 4
#define OPT_EXIT 5

#define ACCESS_UNAUTHENTICATED 0
#define ACCESS_AUTHENTICATED 1

#define QUERY_DEBUG 1

void getUsername(char[]);
void getName(char[]);

// Struct for storage of user data, designed with overflow in mind.
// Note: While these items are contiguous in code, they may not be contiguous in memory due to data structure padding.
// See http://stackoverflow.com/questions/8377667/layout-in-memory-of-a-struct-struct-of-arrays-and-array-of-structs-in-c-c for details.
struct userdata {
  // Uses +1 to account for the C-style '\0'-terminated string.
  char username[MAX_NAME_LENGTH + 1];
  char password[MAX_PWD_LENGTH + 1];
  int access;
};

int main(int argc, char* argv[])
{
  struct userdata user;
  memset(user.username, 0, sizeof(user.username));
  memset(user.password, 0, sizeof(user.password));
  user.access = ACCESS_UNAUTHENTICATED;
  
  char name[MAX_NAME_LENGTH + 1];
  int amount, option;
  if (argc > 1) {
    // What is this? We never use the arguments, and I don't know what a.out is supposed to point to. -JS 8/9
    printf("Sorry. You have entered too many arguments.\n");
    printf("To run the debt-tracker, you must use \"a.out\" as the only argument.\n");
    printf("Goodbye!");
    return 1;
  }
  
  /* Initialize the MySQL instance. */
  mysql = mysql_init(NULL);
  if (!mysql_real_connect(mysql, "lucien.twilightparadox.com", "ics491a3", "SUPER_SECURE_ICS", "ICS491A3", 20000, NULL, 0)) {
    fprintf(stderr, "Error while connecting to DB: %s\r\n", mysql_error(mysql));
    return 1;
  }
  
  /* Obtain authentication from user. Several exploits exist here to force authentication. */
  while (user.access == ACCESS_UNAUTHENTICATED) {
    // EXPLOIT - SQL INJECTION: if username is "VALID_USERNAME';--", password will not be verified.
    printf("Please enter your username: ");
    getUsername(user.username);
    
    // EXPLOIT - BUFFER OVERFLOW: Use of unbounded gets() on a bounded array allows for overflow into the ACCESS variable, granting access.
    printf("Please enter your password: ");
    gets(user.password);
    
    // Actual SQL lookup is carried out here.
    sprintf(querybuf, "SELECT uid FROM users WHERE uid='%s' AND pwd='%s';", user.username, user.password);
    if (mysql_wrapper(mysql, querybuf)) {
      fprintf(stderr, "FATAL SQL DB ERROR. Program will now terminate.\r\n");
      fprintf(stderr, "%s\r\n", mysql_error(mysql));
      return 1;
    }
    res = mysql_use_result(mysql);
    row = mysql_fetch_row(res);
    if (!row)
      printf("That username and password combination is invalid.\r\n");
    else {
      user.access = ACCESS_AUTHENTICATED;
    }
    mysql_free_result(res);
  }
  printf("Welcome %s!\n", user.username);
  
  option = OPT_UNSET;
  while (option != OPT_EXIT) {
    printf("Type the number corresponding to your choice:\n");
    printf("1. View people who owe you money\n");
    printf("2. View people to whom you owe money\n");
    printf("3. Add a debt\n");
    printf("4. Remove a debt\n");
    printf("5. Exit the debt tracker\n");
    printf("> ");
    scanf("%d", &option);
    if (option == OPT_I_OWE_THEM) {
      option = OPT_UNSET;
      while (option != 1 && option != OPT_EXIT) {
        // Compose query:
        sprintf(querybuf, "SELECT owed_by, amt FROM debts WHERE owed_to='%s' AND paid=0", user.username);
        if (mysql_wrapper(mysql, querybuf)) {
          // Query failed.
          printf("Nobody currently owes you money.\r\n");
          option = 1;
          continue;
        }
        res = mysql_store_result(mysql);
        if (mysql_num_rows(res) < 1) {
          // Query succeeded with no results returned.
          mysql_free_result(res);
          printf("Nobody currently owes you money.\r\n");
          option = 1;
          continue;
        }
        printf("These people owe you money:\n");
        while ((row = mysql_fetch_row(res)))
          printf("  %s: $%s\n", row[0], row[1]);
        mysql_free_result(res);
        printf("\n");
        printf("Type the number corresponding to your choice:\n");
        printf("1. Return to main screen\n");
        printf("2. Exit the debt tracker\n");
        printf("> ");
        scanf("%d", &option);
        if (option != 1 && option != 2) {
          printf("That is not a valid option.\n");
        }
        else if (option == 2) {
          option = OPT_EXIT;
        }
      }
    }
    else if (option == OPT_THEY_OWE_ME) {
      option = OPT_UNSET;
      while (option != 1 && option != OPT_EXIT) {
        // Compose query:
        sprintf(querybuf, "SELECT owed_to, amt FROM debts WHERE owed_by='%s' AND paid=0", user.username);
        if (mysql_wrapper(mysql, querybuf)) {
          // Query failed.
          printf("You don't currently owe anyone any money.\r\n");
          option = 1;
          continue;
        }
        res = mysql_store_result(mysql);
        if (mysql_num_rows(res) < 1) {
          // Query succeeded with no results returned.
          mysql_free_result(res);
          printf("You don't currently owe anyone any money.\r\n");
          option = 1;
          continue;
        }
        printf("You owe these people money:\n");
        while ((row = mysql_fetch_row(res)))
          printf("  %s: $%s\n", row[0], row[1]);
        mysql_free_result(res);
        printf("\n");
        printf("Type the number corresponding to your choice:\n");
        printf("1. Return to main screen\n");
        printf("2. Exit the debt tracker\n");
        printf("> ");
        scanf("%d", &option);
        if (option != 1 && option != 2) {
          printf("That is not a valid option.\n");
        }
        else if (option == 2) {
          option = OPT_EXIT;
        }
      }
    }
    /*need to add if-else stuffies*/
    else if (option == OPT_ADD_DEBT) {
      option = OPT_UNSET;
      while (option != 3 && option != OPT_EXIT) {
        printf("Type the number corresponding to your choice:\n");
        printf("1. You owe somebody money\n");
        printf("2. Somebody owes you money\n");
        printf("3. Return to main screen\n");
        printf("4. Exit the debt tracker\n");
        printf("> ");
        scanf("%d", &option);
        if (option == 1) {
          printf("Type the name of the person to whom you owe money: ");
          getName(name);
          printf("How much do you owe %s? ", name);
          /*no number checking*/
          printf("> ");
          scanf("%d", &amount);
          /*do other stuff (need to add this debt to the database..)*/
        }
        else if (option == 2) {
          printf("Type the name of the person who owes you money: ");
          getName(name);
          printf("How much does %s owe you? ", name);
          /*no number checking*/
          printf("> ");
          scanf("%d", &amount);
          /*do other stuff (need to add this debt to the database..)*/
        }
        else if (option == 4) {
          option = 5;
        }
        else if (option != 3) {
          printf("That is not a valid option.\n");
        }
      }
    }
    else if (option == OPT_CLEAR_DEBT)
    {
      option = OPT_UNSET;
      while (option != 2 && option != 5)
      {
        printf("Type the number corresponding to your choice:\n");
        printf("1. Someone paid you back\n");
        printf("2. Return to main screen\n");
        printf("3. Exit the debt tracker\n");
        printf("> ");
        scanf("%d", &option);
        
        if (option == 1)
        {
          printf("Type the name of the person who paid you back: ");
          getName(name);
          /*remove this debt from the database*/
        }
        else if (option == 3)
        {
          option = 5;
        }
        else if (option != 2)
        {
          printf("That is not a valid option.\n");
        }
      }
    }
  }
  printf("Goodbye.\r\n");
}

void getUsername(char input[])
{
  char tempUserName[MAX_NAME_LENGTH + 1];
  char character;
  int index = 0;
  
  // this while loop trims any non-letter characters at the beginning of the line
  while (!isalpha((character = fgetc(stdin))));
  
  // this while loop reads in characters until the end of the line is reached, or until the space reserved for the username is filled
  while (character != '\n' && index < MAX_NAME_LENGTH)
  {
    tempUserName[index++] = character;
    character = fgetc(stdin);
  }
  
  tempUserName[index] = 0;
  strcpy(input, tempUserName);
}

void getName(char input[])
{
  char tempUserName[MAX_NAME_LENGTH];
  char character;
  int length;
  character = fgetc(stdin);
  length = 0;
  /*this while loop trims any white space at the beginning of the line*/
  while (character == ' ' || character == '\n')
  {
    character = fgetc(stdin);
  }
  /*this while loop reads in characters until the end of the line is reached,
   or until the space reserved for the username is filled*/
  while (character != '\n' && strlen(tempUserName) < MAX_NAME_LENGTH)
  {
    length = strlen(tempUserName);
    tempUserName[length] = character;
    tempUserName[length + 1] = '\0';
    character = fgetc(stdin);
  }
  strcpy(input, tempUserName);
}

// MySQL wrapper to automate some of the error reporting. -JS 8/9
int mysql_wrapper(MYSQL *mysql, char *query)
{
  if (QUERY_DEBUG)
    printf("QUERY: %s\r\n", query);
  if (mysql_errno(mysql))
    fprintf(stderr, "MYSQLERROR: %s (generated from query %s)\r\n", mysql_error(mysql), query);
  return mysql_query(mysql, query);
}