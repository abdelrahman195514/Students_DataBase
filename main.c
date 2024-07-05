#include <stdio.h>
#include <stdlib.h>
#include<string.h>
#define NAMESIZE 30
#define DEPARTMENTSIZE 20
#define MAXPASSWORD 30

typedef struct
{
    char userName[MAXPASSWORD];
    char password[MAXPASSWORD];
} Login;
typedef struct student {
    long int ID;
    char name[NAMESIZE];
    char department[DEPARTMENTSIZE];
    float degree;
}Student;
typedef struct node {
    Student data;
    struct node* next;
}Node;
typedef struct linked {
    Node* top;
    int size;
}Linked;
//nagy
void insert(Student* record, Linked* pl);
void createLinked(Linked* pl);
int linkedEmpty(Linked* pl);
void deleteAll(Linked* pl);
void deleteStudent(const long int id, Linked* pl);
//hegazy
void update(const long int id, Linked* pl);
void display(Student* record);
//kashsh
void traverseLinked(Linked* pl, void (*pf)(Student *record));
//hegazy
void save(FILE* fPtr, Linked* pl,Login *pUser);
//nagy
void readFile(FILE* fptr, Linked* pl,Login *pUser, void (*fp)(Student* record, Linked* pl));
void query(const long int  id, Linked* pl);
void instructions(void);
void enterChoice(int* ch);
//nagy
void getPassword(Login* userCheck);
//hegazy
int signIn(Login* userCheck, Login* user);
int changePassword(Login* userCheck, Login* user);
int main() {
    //nagy => files
    Student record;
    Linked student;
    Login user = { "admin","admin" }, userCheck;
    int choice;
    //if the file exist, Linked will read the file else it will do nothing beacuse it's first time
    FILE* fptr = fopen("students.dat","rb");
    createLinked(&student);
    //to read the file data alwaya in the beginning and make it easy to deal with the data
    if (fptr != NULL) {
        readFile(fptr, &student,&user, insert);
        fclose(fptr);
    }
   puts("\t\t\t\tWELCOME TO YOUR STUDENTS DATABASE");
    if ((signIn(&userCheck, &user)) >= 3) {
        puts("\nyou failed to sign in");
        return 0;
    }
    system("clear");
    instructions();
    enterChoice(&choice);
     while(choice !=11){
        puts("\t\t\t\t*******************************");
         switch (choice) {
                 case 1:
            //nagy
            printf("enter student ID(to end enter 0): ");
            scanf("%ld", &record.ID);
            while (record.ID < 0)
            {
                puts("wrong input, ID must be bigger than zero ");
                printf("enter student ID: ");
                scanf("%ld", &record.ID);
            }
            while (record.ID != 0) {
                //to clean input buffer
                while ((getchar()) != '\n');
                printf("%s", "enter student name: ");
                gets(record.name);
                printf("%s", "enter student deprtemnt: ");
                gets(record.department);
                printf("enter student last year degree: ");
                scanf("%f", &record.degree);
                insert(&record, &student);
                printf("enter student ID(to end enter 0): ");
                scanf("%ld", &record.ID);

            }
            break;

                 case 2:
            //abdelrahman
            printf("enter student ID: ");
            scanf("%ld", &record.ID);
            while (record.ID <= 0)
            {
                puts("wrong input, ID must be bigger than zero ");
                printf("enter student ID: ");
                scanf("%ld", &record.ID);
            }
                //to clean input buffer
                while ((getchar()) != '\n');
                printf("%s", "enter student name: ");
                gets(record.name);
                printf("%s", "enter student deprtemnt: ");
                gets(record.department);
                printf("enter student last year degree: ");
                scanf("%f", &record.degree);
                insert(&record, &student);
                break;

        case 3:
            //kashs
            //check that the Linked is not empty
            if (linkedEmpty(&student)) {
                puts("database already free, there's no students to delete");
            }
            else {
                deleteAll(&student);
                remove("students.dat");
                if (student.top == NULL)
                {
                    puts("all students removed");
                }
                //to save the password user had change
                fptr = fopen("students.dat", "wb");
                    save(fptr, &student, &user);
                    fclose(fptr);
            }
            break;
         case 4:
            //osama
            //check that the Linked is not empty
            if (linkedEmpty(&student)) {
                puts("database already free, there's no students to delete");

            }
            else {
                printf("enter student ID you want to remove: ");
                scanf("%ld", &record.ID);
                deleteStudent(record.ID, &student);
            }
            break;
          case 5:
            //afaf
            if (linkedEmpty(&student)) {
                puts("database is empty, no students to search");
            }
            else {
                printf("enter student ID to search: ");
                scanf("%ld", &record.ID);
                query(record.ID, &student);
            }
            break;

        case 6:
            //afaf
            if (linkedEmpty(&student)) {
                puts("database is empty, no students to display");
            }
            else {
                printf("%-15s%-25s%-25s%-25s\n\n", "ID", "name", "department","degree");
                traverseLinked(&student, display);
            }
            break;
         case 7:
            //osama
            if (linkedEmpty(&student)) {
                puts("database is empty, no students to update his data");
            }
            else {
                printf("enter student ID to search and update: ");
                scanf("%ld", &record.ID);
                update(record.ID, &student);
            }
            break;
         case 8:
            //osama
            printf("you saved %d student successfully\n", student.size);
            break;
         case 9:
            //nagy
            fptr=fopen("students.dat","wb");
            if (fptr == NULL) {
                perror("error");
            }
            else
            {
                save(fptr, &student,&user);
                puts("data saved");
                fclose(fptr);
            }
            break;
         case 10:
            //hegazy
            if ((changePassword(&userCheck, &user)) >= 3) {
                puts("\npassword can't be changed");
           }
            else {
                //to save the new password in the file
                fptr = fopen("students.dat", "wb");
                if (fptr == NULL) {
                    puts("\ncan't change the password");
                }
                else {
                    save(fptr, &student, &user);
                    fclose(fptr);
                    puts("\npassword changed succssfully");
                }
            }
            break;
        default :
            puts("wrong answer");
        }
        puts("\t\t\t\t*******************************");
        system("pause");
        system("clear");
        instructions();
        enterChoice(&choice);
    }
    //to free the memory
    deleteAll(&student);
    puts("\n\t\t\t\t*******************************\n\n\t\t\t\tGOODBYE,SEE YOU SOON <3\n\n\t\t\t\t*******************************");
    return 0;
}
//order is function of the name
void insert(Student* record, Linked* pl) {
    Node* pn = (Node*)malloc(sizeof(Node));
    pn->next = NULL;
    pn->data.ID = record->ID;
    strcpy(pn->data.name, record->name);
    strcpy(pn->data.department, record->department);
    pn->data.degree = record->degree;
     Node* temp = pl->top;
    while (temp)
    {
        if (temp->data.ID == record->ID)
        {
            free(pn);
            return 0;
        }
        temp = temp->next;
    }
    //if program reach this point the id doesn't exist
    Node* currentPtr = pl->top;
    Node* previousPtr = NULL;
    //to insert  node in order
    while (currentPtr != NULL && strcmp(currentPtr->data.name, record->name) == -1)
    {
        previousPtr = currentPtr;
        currentPtr = currentPtr->next;
    }
    if (pl->top == NULL || previousPtr == NULL) {
        pn->next = pl->top;
        pl->top = pn;
    }
    else {
        previousPtr->next = pn;
        pn->next = currentPtr;
    }
    pl->size++;
}
//afaf
int linkedEmpty(Linked* pl)
{
    return pl->top == NULL;
}
 //kashsh
void deleteAll(Linked* pl) {
    Node* pn = pl->top;
    while (pl->top != NULL) {
        pl->top = pl->top->next;
        free(pn);
        pn = pl->top;
    }
    pl->size = 0;
}
 //afaf
 void display(Student* record) {
        printf("%-15ld%-25s%-25s%-25.2f\n", record->ID, record->name, record->department,record->degree);
}
 //afaf
void enterChoice(int* ch)
{
    printf("%s", "enter your choice: ");
    scanf("%d", ch);
    //to make sure user enter correct input
    while (*ch < 1 || *ch >11) {
        puts("wrong input, choices must be between \'1\' and \'11\'");
        printf("%s", "enter your choice: ");
        scanf("%d", ch);
    }
}
    //hegazy
void save(FILE* fPtr, Linked* pl,Login *pUser)
{
    //kept the Linked ,dont destroy the data
    Node* pn = pl->top;
    //to save the password in the file
    fwrite(pUser, sizeof(Login), 1, fPtr);
    while (pn) {
        fwrite(&pn->data, sizeof(Student), 1, fPtr);
        pn = pn->next;
    }
}
void update(const long int id, Linked* pl)
{
    Node* pn = pl->top;
    Node* previous = NULL;
    //to find where is the student
    while (pn != NULL && pn->data.ID != id)
    {
        previous = pn;
        pn = pn->next;
    }
    if (pn) {
        printf("the student data is\nstudent ID: %ld\nstudent name: %s\nstudent department: %s\nstudent degree: %0.2f\nenter student new data::", pn->data.ID, pn->data.name, pn->data.department,pn->data.degree);
        printf("enter student new ID: ");
        scanf_s("%ld", &pn->data.ID);
        while ((getchar()) != '\n');//clear input buffer
        printf("%s", "student name: ");
        gets(pn->data.name);
        printf("%s", "student department: ");
        gets(pn->data.department);
        printf("enter student last year degree: ");
        scanf_s("%f", &pn->data.degree);
        puts("data updated successfully");
        //to remove that node from the linked list,,, to change the position by the new name
        if (previous) {
            previous->next = pn->next;
        }
        else {
            pl->top = pn->next;
        }
        insert(&pn->data, pl);
        //to not save one more student
        pl->size--;
    }
    else if (pn == NULL)
    {
        puts("sorry,student doesn't exist");
    }
}
void deleteStudent(const long int id, Linked* pl)
{
    Node* pn = pl->top, * pre = NULL;
    //to find student
    while (pn != NULL && pn->data.ID != id) {
        pre = pn;
        pn = pn->next;
    }
    if (pn)
    {
        if (pre) {
            pre->next = pn->next;
        }
        else {
            pl->top = pn->next;
        }
        free(pn);
        puts("student removed successfully");
    }
    else if (pn == NULL) {
        puts("student doesn't exist");
    }
    pl->size--;
}
void instructions(void) {
    puts("\n\t\t\t\t*****************************\nenter \'1\' to enter more than one student data\nenter \'2\' to enter student data\nenter \'3\' to delete all data\nenter \'4\' to search about student by ID and delete his/her data \nenter \'5\' to search about student by his ID and display his/her data\nenter \'6\' to display all student data\nenter \'7\' to search about student by his/her ID and update name , department and degree\nenter \'8\' to know how many students you saved\nenter \'9\' to save\nenter \'10\' to change the userName and the password\nenter \'11\' to end the program\n\t\t\t\t*******************************");
    //puts("\"tip\" this database designed to read automatically from the file in the beginning of the run\n\t\t\t\t*******************************");
}
void query(const long int  id, Linked* pl) {
    Node* pn = pl->top;
    //to find where is the Node
    while ((pn != NULL)&&(pn->data.ID != id) )
    {
        pn = pn->next;
    }
    if (pn != NULL) {
        printf("student exist\nstudent name: %s\nstudent department: %s\nstudent ID: %ld\nstudent degree in last year: %0.2f\n", pn->data.name, pn->data.department, pn->data.ID,pn->data.degree);
    }
    else if (pn == NULL)
    {
        puts("student doesn't exist\n");
    }
}
void createLinked(Linked* pl) {
    pl->top = NULL;
    pl->size = 0;
}
void readFile(FILE* fptr, Linked* pl,Login *pUser,void (*fp)(Student* record, Linked* pl))
{
    Student record = { 0,"","" };
    fread(pUser, sizeof(Login), 1, fptr);
    while (!feof(fptr))
    {
       if( (fread(&record, sizeof(Student), 1, fptr))==1)
        (*fp)(&record, pl);
    }
}
//abdelrahman
void  traverseLinked(Linked* pl, void (*pf)(Student* record))
{
    Node* pn = pl->top;
    while (pn != NULL)
    {
        (*pf)(&pn->data);
        pn = pn->next;
    }
}
void getPassword(Login* userCheck) {
    char ch;
    int i = 0;
    while (1) {
        ch = _getch();	//get key
        if (ch == ENTER || ch == TAB) {
            userCheck->password[i] = '\0';
            break;
        }
        else if (ch == BKSP) {
            if (i > 0) {
                i--;
                printf("\b \b");		//for backspace
            }
        }
        else {
            userCheck->password[i++] = ch;
            printf("*");				//to replace password character with *
        }
    }
}
int signIn(Login* userCheck, Login* user)
{
    int times = 1;
    puts("\ntip,the default userName is "admin", default password is "admin"\nLOGIN");
    printf("%s", "username: ");
    gets(userCheck->userName);
    printf("%s", "password: ");
    //gets(userCheck->password);
    getPassword(userCheck);
    //to check for password correctness
    while (times < 3 && ( (strcmp(user->userName, userCheck->userName)) != 0 || (strcmp(user->password, userCheck->password)) != 0 )) {
        puts("\nwrong user name or password,please try login again");
        printf("%s", "username: ");
        gets(userCheck->userName);
        printf("%s", "password: ");
        getPassword(userCheck);
        times++;
    }
    return times;
}
int changePassword(Login* userCheck, Login* user) {
    int times = 1;
    while ((getchar()) != '\n');
    printf("%s", "enter current password: ");
    getPassword(userCheck);
    while (times<3 && (strcmp(userCheck->password, user->password)) != 0)
    {
        puts("\nwrong password, enter password again");
        printf("%s", "password: ");
        getPassword(userCheck);
        times++;
    }
    if(times<3){
        printf("\n%s", "enter new userName: ");
        gets(user->userName);
        printf("\n%s", "enter new password: ");
        getPassword(user);//to sign the password in user not USERCHECK
    }
    return times;
}
