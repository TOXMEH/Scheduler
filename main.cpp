#include <iostream>
#include <fstream>
#include <math.h>
#include <cstddef>
#include <iomanip>


using namespace std;


struct stack {
    // struct stack *previous;
    string task;
    int priority;
    struct stack *next;
};

void Queue(stack *&st, string symbol) {
    stack *top = new(stack);
    top->task = symbol;
    st->next = top;
    st = top;
}


void Showtask(stack *&head, int &StackSize) {
    stack *previous;
    bool NotFirst = false;

    while (head->priority > 1) {
        previous = head;
        head = previous->next;
        NotFirst = true;
    }
    cout << head->task << endl;
    cout << "Has the task been done?";
    string HasBeenDone;
    cin >> HasBeenDone;
    if ((HasBeenDone == "yes") && (NotFirst == false)) {
        previous->next = head->next;
        delete head;
    } else if (HasBeenDone == "yes") {
        head = head->next;
    }
}

void AddTask(stack *&tail, int &StackSize) {
    cout << "Enter the task to add: ";
    string task;
    cin.sync();
    getline(cin, task);
    int priority = 4;
    while (priority > 3 || (priority < 1)) {
        cout << "Enter its priority. It can have three ranges: ";
        cin >> priority;
    }
    if (StackSize == 0) {
        tail->task = task;
        tail->priority = priority;
        StackSize++;
    } else {
        stack *next = new(stack);
        tail->next = next;
        next->task = task;
        next->priority = priority;
        tail = next;
        StackSize++;
    }
}

void WriteToTheFile(stack *&head, int &StackSize) {
    ofstream output("tasks.txt");
    while (StackSize > 0) {
        output << head->priority << " " << head->task << '\n';

        head = head->next;
        StackSize--;
    }
}

void ReadFromTheFile(stack *&head, int &StackSize) {
    ifstream input("tasks.txt");
    while (!(input.eof())){

    }
}


int main() {
    setlocale(LC_ALL, "russian");

    int StackSize = 0;
    stack *tail;
    stack *head = tail;

    string answer;
    while (true) {
        cout <<
        "Enter 's', if you want to see a task, or enter 'a', if you want to add a task, or enter '!', if you want to stop working with program: ";

        cin >> answer;
        if (answer == "s") {
            Showtask(head, StackSize);
        }
        if (answer == "a") {
            AddTask(tail, StackSize);
        }
        if (answer == "!") {
            goto EXIT;
        }
    }
    EXIT:
    WriteToTheFile(head, StackSize);
}