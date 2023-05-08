
#include <stdio.h>
#include <string.h>
#include <stdlib.h>


int Ticket_N=0;

typedef struct tag{
  int ID;
  char Name[80];
  int ticket_number;
  struct tag* next;
}advisee;

void menu(){
  printf("\n                Menu:                 ");
  printf("\n<<__________________________________>>");
  printf("\n\t1-Start");
  printf("\n\t2-Add a new Advisee");
  printf("\n\t3-Serve an Advisee");
  printf("\n\t4-Check who is Waiting?");
  printf("\n\t5-Leave the Queue");
  printf("\n\t6-Change Place");
  printf("\n\t7-Advising Session is Over");
  printf("\n -----------------------------\n");
}
//creating and adding a student to a queue
advisee* Create_Fill_Node(){
  char temp;
  advisee* new_node;
  new_node=(advisee*)malloc(sizeof(advisee));
  printf("\nPlease enter the student's ID: \t");
  scanf("%d", &new_node->ID);
  printf("\nPlease enter the student's Name: \t");
  scanf("%c", &temp);
  gets(new_node->Name);
  new_node->ticket_number=++Ticket_N;
  new_node->next=NULL;
  return new_node;
}

void Add_To_Queue(advisee* ToAdd, advisee** front, advisee** rear){
  if(*front==NULL){
    *front=ToAdd;
    *rear=ToAdd;
  }

  else{
    (*rear)->next=ToAdd;
    *rear=ToAdd;
    ToAdd->next=NULL;
  }
}
// serving a student
void Serve(advisee** front, advisee** rear, int* Status, int* T_N, char* Name){
  advisee *temp;
  
  if(*front==NULL){
    *Status=0;
    
  }
  else{
    temp=*front;
    if(*front==*rear)
      *rear=NULL;
    
    *front=temp->next;
    *T_N=temp->ticket_number;
    strcpy(Name,temp->Name);
    *Status=1;
    free(temp);

  }


}
// check who is waiting
void Check_Queue(advisee** front){
  advisee* walker;
  walker=*front;
  if(*front==NULL)
    printf("The queue is empty");
  else{
    while(walker!=NULL){
      printf("\n%d %s %d", walker->ID ,walker->Name, walker->ticket_number);
      walker=walker->next;
    }
  }
}
// removing a student from the queue
void Leave_Queue(int ID_To_delete, advisee** front, advisee** rear, int* status, char* Name){
  
  advisee* walker;
  advisee* temp;
  walker=(*front);
  
  if(*front==NULL){
    *status=2;
    return;
  }
    
  else if(ID_To_delete==(*front)->ID){
    temp=*front;
    (*front)=temp->next;
    if(*front==NULL)
      *rear=NULL;
    strcpy(Name, temp->Name );
    *status=1;
    return;
  }

  else{
    while(walker->next!=NULL){
      if(ID_To_delete==(walker->next)->ID){
        if(walker->next==(*rear)){
          temp=*rear;
          (*rear)= walker;
          strcpy(Name, temp->Name );
          *status=1;
          return;
        }
        else{
          temp=walker->next;
          walker->next= temp->next;
          strcpy(Name, temp->Name );
          *status=1;
          return;
        }
      }
      walker=walker->next;
    }
  }

  
}
// swaping places
void switch_places(int ID_Student1, int ID_Student2, advisee** front, char* Name1, char* Name2, int* status){
  advisee* walker1,*walker2, *temp;
  walker1=*front;
  temp=(advisee*)malloc(sizeof(advisee));
  while(walker1!=NULL && *status!=1){
    if(walker1->ID==ID_Student1 || walker1->ID==ID_Student2 ){
      walker2=walker1->next;
      while(walker2!=NULL && *status!=1){
          if(walker2->ID==ID_Student1 ||walker2->ID==ID_Student2){
              strcpy(Name1, walker1->Name);
              strcpy(Name2, walker2->Name);
              
              temp->ID=walker1->ID;
              strcpy(temp->Name, walker1->Name);
              

              walker1->ID=walker2->ID;
              strcpy(walker1->Name, walker2->Name);
              
                  
              walker2->ID=temp->ID;
              strcpy(walker2->Name, temp->Name);
        
              *status=1;
          }
          walker2=walker2->next;
      }
    walker1=walker1->next;
    }

  }

}
// ending the session
void End_session(FILE* outfp, advisee** front, advisee** rear){
  Ticket_N=0;
  fseek(outfp, 0,0);
  if(*front==NULL)
    printf("\nThe queue is empty.");
  else{
  while((*front)!=NULL){
    fprintf(outfp, "%d %d ", (*front)->ticket_number, (*front)->ID);
    fputs((*front)->Name, outfp);
    fprintf(outfp, "\n");
    (*front)=(*front)->next;
  }
  }

  (*front)=NULL;
  (*rear)=NULL;
  printf("\nThe advising session is over.\n");
}
// starting the advising session
void Start(FILE* outfp, advisee** front, advisee** rear){
  advisee* student;
  Ticket_N=0;
  *front=NULL;
  *rear=NULL;
  
  fseek(outfp,0,0);
    while(!feof(outfp)){
      student=(advisee*)malloc(sizeof(advisee));
      fscanf(outfp,"%d %d ", &student->ticket_number, &student->ID);
      fgets(student->Name, 50, outfp);
      if(feof(outfp))
        break;
      student->ticket_number=++Ticket_N;
      Add_To_Queue(student, front, rear);


    }
  
}

int main(){
  advisee *front=NULL;
  advisee *rear=NULL;
  advisee* new_node;
  char answer;
  int choice;
  FILE* outfp;
  char Name[50];
  int T_N;
  int Status;
  int status1;
  int status2;
  int ID_To_delete;
  int ID_Student1, ID_Student2;
  char Name1[50], Name2[50];

  

  do{
    menu();
    printf("\nWhat option would you like to choose? ");
    scanf("%d", &choice);
    switch(choice){
      case 1:
        outfp=fopen("To_serve.txt", "r");
        
        if(outfp==NULL)
          printf("There are no students left in the queue");
        else{
          Start(outfp,  &front, &rear);
        }
        break;
        
      case 2:
        new_node=Create_Fill_Node();
        Add_To_Queue( new_node, &front, &rear);
        break;

      case 3:
        Status=0;
        Serve(&front, &rear, &Status, &T_N, Name);
        if(Status==1)
          printf("\nThe student %s with the ticket number %d has been served", Name ,T_N);
        else
          printf("\nThe queue is empty");
        break;

      case 4:
        Check_Queue(&front);
        break;

      case 5:
        status1=0;
        printf("\nPlease enter the ID of the Student that wants to leave the queue:\t");
        scanf("%d", &ID_To_delete);
        Leave_Queue(ID_To_delete, &front, &rear, &status1, Name);
        if(status1==1)
            printf("\nSorry but the student %s has left the queue", Name);
        else if(status1==0)
            printf("\nSorry couldn't find the student");
        else if(status1==2)
            printf("\nSorry but the queue is empty");
        break;
      
      case 6:
        status2=0;
        printf("\nplease enter the ID's of the students that want to switch places: ");
        scanf("%d %d", &ID_Student1, &ID_Student2);
        switch_places(ID_Student1, ID_Student2, &front, Name1, Name2, &status2);
        if(status2==0)
          printf("\nSorry couldn't find the students that wanted to switch places\n");
        else
          printf("\nThe student %s and %s swaped places", Name1, Name2);
        break;

      case 7:
        outfp=fopen("To_serve.txt", "w");
        End_session(outfp, &front, &rear);
        fclose(outfp);
        break;

      default:
        printf("wrong entry");
        break;
    }
    printf("\nwould you like to continue? Y/N: ");
    scanf(" %c", &answer);
  }while(answer!='N');


  fclose(outfp);
  return 0;
}