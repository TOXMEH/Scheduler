#include <iostream>
#include <fstream>
#include <math.h>
#include <cstddef>
#include <iomanip>
#include <stack>


using namespace std;


struct stack {
   // struct stack *previous;
    string task;
    int priority;
    struct stack *next;
};



void Showtask(stack *& head){
    stack * previous;
    bool NotFirst=false;

    while (head->priority > 1){
        previous = head;
        head =previous->next;
        NotFirst=true;
    }
    cout << head->task << endl;
    cout<<"Has the task been done?"<<endl;
    string HasBeenDone;
    cin>>HasBeenDone;
    if ((HasBeenDone=="yes") && (NotFirst==false)){
        previous->next= head->next;
        delete head;
    } else if (HasBeenDone=="yes"){
        head=head->next;
    }
}

void AddTask(stack *& tail){
    cout << "Enter the task to add: ";
    string task;
    cin>>task;
    int priority=4;
    while (priority>3 || (priority<1)) {
        cout << "Enter its priority. It can have three ranges: ";
        cin >> priority;
    }
    if (tail->task=="X"){
        tail->task=task;
        tail->priority=priority;
    }else {
        stack *next = new(stack);
        tail->next = next;
        next->task = task;
        next->priority = priority;
        tail = next;
    }
}

void WriteToTheFile(stack *& head){
    ofstream output("tasks.txt");
    while (head->task!="X"){
        output << '\n';
        output << head->task;
        output.seekp(40);
        output << head->priority;
    }

}


int main(){
    stack * tail;
    stack * head=tail;
    tail->task="X";
    string answer;
    while (true) {
        cout << "Enter 'show', if you want to see a task, or enter 'add', if you want to add a task: ";

        cin >> answer;
        if (answer == "show") {
            Showtask(head);
        }
        if (answer == "add") {
            AddTask(tail);
        }
        if (answer == "stop"){
            goto EXIT;
        }
    }
    EXIT:
    //stack * next;
    //next->task=*"X";
    //tail->next=&next;
    WriteToTheFile(head);
    return 0;
}