#include <stdio.h>
#include <stdlib.h>
#include <string.h>


void getUsername(char[]);
void getName(char[]);

int main(int argc, char* argv[])
{
  char character;
  char username[10];
  char name[20];
  int amount, option, access;
  char password[15];
  access = 0;
  if (argc > 1)
  {
    printf("Sorry. You have entered too many arguments.\n");
    printf("To run the debt-tracker, you must use \"a.out\" as the only argument.\n");
	 printf("Goodbye!");
  }
  else
  {
	 printf("Please enter your username: ");
	 getUsername(username);
	 /*if the username is valid, then move on..otherwise print an error message*/
	 
	 printf("Please enter your password: ");
	 gets(password);
	 
	 if (strcmp(password, "correct"))
	 {
	   printf("Sorry, that is incorrect.\n");
		printf("Sorry. %p is not a valid argument.\n", &password);
		printf("%p \n", &access);
    }
	 else
	 {
	   printf("Welcome %s!\n", username);
		access = 1;
	 }
	 if(access)
	 {
	   option = 0;
		while (option != 5)
		{
	     printf("Type the number corresponding to your choice:\n");
		  printf("1. View people who owe you money\n");
		  printf("2. View people to whom you owe money\n");
		  printf("3. Add a debt\n");
		  printf("4. Remove a debt\n");
		  printf("5. Exit the debt tracker\n");
		  scanf("%d", &option);
		  if (option == 1)
		  {
		    option = 0;
			 while (option != 1 && option != 5)
			 {
		      printf("These people owe you money.\n");
			   printf("Type the number corresponding to your choice:\n");
			   printf("1. Return to main screen\n");
		      printf("2. Exit the debt tracker\n");
			   scanf("%d", &option);
			   if (option != 1 && option != 2)
		  	   {
			     printf("That is not a valid option.\n");
		      }
			   else if (option == 2)
			   {
			     option = 5;
		      }
			 }			 
		  }
		  else if (option == 2)
		  {
		    option = 0;
		    while (option != 1 && option != 5)
			 {
		      printf("You owe these people money.\n");
			   printf("Type the number corresponding to your choice:\n");
			   printf("1. Return to main screen\n");
		      printf("2. Exit the debt tracker\n");
			   scanf("%d", &option);
			   if (option != 1 && option != 2)
		  	   {
			     printf("That is not a valid option.\n");
		      }
			   else if (option == 2)
			   {
			     option = 5;
		      }
			 }
		  }
		  /*need to add if-else stuffies*/
		  else if (option == 3)
		  {
		    option = 0;
			 while (option != 3 && option != 5)
			 {
		      printf("Type the number corresponding to your choice:\n");
			   printf("1. You owe somebody money\n");
			   printf("2. Somebody owes you money\n");
			   printf("3. Return to main screen\n");
			   printf("4. Exit the debt tracker\n");
			 
			   scanf("%d", &option);
			   if (option == 1)
			   {
			     printf("Type the name of the person to whom you owe money: ");
				  getName(name);
				  printf("How much do you owe %s? ", name);
				  /*no number checking*/
				  scanf("%d", &amount);
				  /*do other stuff (need to add this debt to the database..)*/
		      }
			   else if (option == 2)
			   {
			     printf("Type the name of the person who owes you money: ");
				  getName(name);
				  printf("How much does %s owe you? ", name);
				  /*no number checking*/
				  scanf("%d", &amount);
				  /*do other stuff (need to add this debt to the database..)*/
		      }
			   else if (option == 4)
			   {
			     option = 5;
		      }
			   else if (option != 3)
			   {
			     printf("That is not a valid option.\n");
		      }	
		    }
		  }
		  else if (option == 4)
		  {
		    option = 0;
			 while (option != 2 && option != 5)
			 {
		      printf("Type the number corresponding to your choice:\n");
			   printf("1. Someone paid you back\n");
			   printf("2. Return to main screen\n");
			   printf("3. Exit the debt tracker\n");
				
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
    }
	     
    /*at this point, we need to connect to the database
	 if the input compared to the password is not 0
	   printf("Sorry, but that was an incorrect username/password combination")
    else
	   printf("Welcome username!")
		access = 1
		
    if(access)
	   login successful*/
  }
}
	 
	 
	 
	 
void getUsername(char input[])
{
  char tempUserName[10];
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
  while (character != '\n' && strlen(tempUserName) < 10)
  {
    length = strlen(tempUserName);
    tempUserName[length] = character;
    tempUserName[length + 1] = '\0';
	 character = fgetc(stdin);
  }
  strcpy(input, tempUserName);
}

void getName(char input[])
{
  char tempName[20];
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
  while (character != '\n' && strlen(tempName) < 20)
  {
    tempName[length] = character;
    tempName[length + 1] = '\0';
	 length += 1;
	 character = fgetc(stdin);
  }
  strcpy(input, tempName);
}

