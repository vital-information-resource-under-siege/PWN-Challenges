#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <ctype.h>

#define SERVER_HOST "127.0.0.1"
#define SERVER_PORT 65432

#define MAX_USERS 3
#define MAX_LETTERS 10

#define REGISTER 1
#define LOGIN 2
#define EXIT 3

#define WRITE 1
#define DELETE 2
#define SEND 3
#define LOGOUT 4

struct __attribute__((packed)) User {
    char username[128];
    char password[128];
} typedef User;

struct __attribute__((packed)) Letter {
    char content[256];
    size_t id;
} typedef Letter;

User *users[MAX_USERS];
Letter *letters[MAX_LETTERS];
int user_count = 0;
int letter_count = 0;
User *logged_in_user = NULL; 

int read_integer() 
{
  int temp = 0;
  scanf("%d", &temp);
  while (getchar() != '\n');
  return temp;
}

int register_account() 
{
  if (user_count >= MAX_USERS) 
  {
    printf("Maximum users reached.\n");
    return 0;
  }

  char username[128];
  char password[128];

  printf("Enter username: \n");
  fgets(username, sizeof(username), stdin);
  username[strcspn(username, "\n")] = 0;  

  printf("Enter password: \n");
  fgets(password, sizeof(password), stdin);
  password[strcspn(password, "\n")] = 0;  

  User *new_user = (User *)malloc(sizeof(User));
  strncpy(new_user->username, username, sizeof(new_user->username) - 1);
  strncpy(new_user->password, password, sizeof(new_user->password) - 1);
  users[user_count++] = new_user;

  printf("User %s registered successfully.", new_user->username);
  return 1;
}

int login() 
{
  char username[128];
  char password[128];

  printf("Enter username: \n");
  fgets(username, sizeof(username), stdin);
  username[strcspn(username, "\n")] = 0;

  printf("Enter password: \n");
  fgets(password, sizeof(password), stdin);
  password[strcspn(password, "\n")] = 0;

  for (int i = 0; i < user_count; i++) 
  {
    if (strcmp(users[i]->username, username) == 0 && strcmp(users[i]->password, password) == 0) 
    {
      logged_in_user = users[i];
      printf("Login successful! Welcome, %s.", logged_in_user->username);
      return 1;
    }
  }
  printf("Login failed. Invalid username or password.\n");
  return 0;
}

int is_alphanumeric(const char *str) 
{
  for (size_t i = 0; str[i] != '\0'; i++) 
  {
    if (!isalnum((unsigned char)str[i])) 
    {
      return 0;
    }
  }
  return 1;
}

void create_letter() 
{
  if (letter_count >= MAX_LETTERS) 
  {
    printf("Maximum letters reached.\n");
    return;
  }

  char content[256];
  printf("Enter letter content: \n");
  fgets(content, sizeof(content), stdin);
  content[strcspn(content, "\n")] = 0;  

  if (!is_alphanumeric(content))
  {
    printf("Invalid characters in letter content!\n");
    return;
  }

  Letter *new_letter = (Letter *)malloc(sizeof(Letter));
  strncpy(new_letter->content, content, sizeof(new_letter->content) - 1);
  new_letter->id = letter_count;
  letters[letter_count] = new_letter;
  letter_count++;

  printf("Letter created: %s (ID: %zu)", new_letter->content, new_letter->id);
}

void delete_letter() 
{
  printf("Enter letter ID to delete: \n");
  int id = read_integer();

  if (letters[id] != NULL && letters[id]->id == id) 
  {
    printf("Deleting letter: %s (ID: %zu)", letters[id]->content, letters[id]->id);
    free(letters[id]);
    printf("Letter deleted successfully.\n");
    return;
  }
  printf("Letter with ID %d not found.", id);
}

void send_all_letters() {
  int sock;
  struct sockaddr_in server_addr;

  if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0) 
  {
      printf("Socket creation error\n");
      return;
  }
  
  server_addr.sin_family = AF_INET;
  server_addr.sin_port = htons(SERVER_PORT);
  
  if (inet_pton(AF_INET, SERVER_HOST, &server_addr.sin_addr) <= 0) {
    printf("Invalid address / Address not supported\n");
    close(sock);
    return;
  }

  if (connect(sock, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
    printf("Connection failed\n");
    close(sock);
    return;
  }
  
  printf("\n--- Sending All letters to Admin ---\n");
  for (int i = 0; i < letter_count; i++) {
    if (letters[i] != NULL) 
    {
      printf("Sending letter ID: %zu, Content: %s\n", letters[i]->id, letters[i]->content);
      
      if (send(sock, letters[i]->content, strlen(letters[i]->content), 0) < 0) {
          printf("Failed to send letter ID %zu\n", letters[i]->id);
      }
    }
  }
  close(sock);
}

void printUnAuthOptions() {
  puts("What can I do for you?");
  puts("[1] Register");
  puts("[2] Login");
  puts("[3] Exit");
  fputs("> ", stdout);
}

void printAuthOptions() {
  puts("What can I do for you?");
  puts("[1] Write letter");
  puts("[2] Delete letter");
  puts("[3] Send all letters");
  puts("[4] Logout");
  fputs("> ", stdout);
}

int main() 
{
  int decision = 0;
  while (decision != EXIT) 
  {
    if (logged_in_user == NULL) 
    {
      printUnAuthOptions();
      decision = read_integer();
      switch (decision) 
      {
        case REGISTER:
          register_account();
          decision = 0;
          break;
        case LOGIN:
          login();
          decision = 0;
          break;
        case EXIT:
          printf("Exiting...\n");
          break;
        default:
          printf("Invalid option.\n");
          break;
      }
    }
    else 
    {
      printAuthOptions();
      decision = read_integer();
      switch (decision) 
      {
        case WRITE:
          create_letter();
          decision = 0;
          break;
        case DELETE:
          delete_letter();
          decision = 0;
          break;
        case SEND:
          send_all_letters();
          decision = 0;
          break;
        case LOGOUT:
          printf("Logging out...\n");
          logged_in_user = NULL;
          decision = 0;
          break;
        default:
          printf("Invalid option.\n");
          break;
      }
    }
  }

  for (int i = 0; i < user_count; i++) {
    if (users[i] != NULL) free(users[i]);
  }

  return 0;
}
