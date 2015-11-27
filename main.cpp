#include <iostream>
#include <fstream>
#include <math.h>
#include <cstddef>
#include <iomanip>
#include <time.h>
#include <Windows.h>


using namespace std;

//русификация перегрузкой оператора вывода
ostream& operator<<(ostream &stream, char *s){
    for (char *ps=s; *ps; ps++){
        if (*ps=='ё')
            stream<<char(241);
        else{
            if (*ps =='Ё')
                stream <<char(240);
            else{
                if (*ps<0)
                    stream<<char(*ps+64+176);
                else
                    stream<<*ps;
            }
        }
    }
    return stream;
}


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
    if ((HasBeenDone == "yes") && (NotFirst)) {
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

    cout << "Enter its priority. It can have three ranges: ";
    cin >> priority;

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
    cout << task << endl;
}

void WriteToTheFile(stack *&head, int &StackSize) {
    if (StackSize > 0) {
        ofstream output("tasks.txt");

        output << StackSize << '\n';
        while (StackSize > 1) {
            output << head->priority << " " << head->task << '\n';

            head = head->next;
            StackSize--;
        }
        output << head->priority << " " << head->task;
    }
}


void ReadFromTheFile(stack *&tail, int &StackSize) {
    ifstream input("tasks.txt");
    string inter;
    getline(input, inter);
    StackSize = stoi(inter);
    //считывем первую строку, дабы не было пустых элементов в стеке
    if (StackSize > 0) {
        int priority;
        input >> priority;
        string task;

        getline(input, task);
        tail->task = task;
        tail->priority = priority;
    }
    if (StackSize > 1) {
        while (!(input.eof())) {
            stack *next = new(stack);
            tail->next = next;

            int priority;
            input >> priority;
            next->priority = priority;

            string task;
            getline(input, task);
            next->task = task;

            tail = next;
        }
    }
}

void IncreasePriority(stack *&head, int StackSize) {
    if (StackSize > 0) {
        int size = StackSize;
        stack *remembered = head;
        while (size > 0) {
            if (head->priority == 1) {
                return;
            }
            head = head->next;
            size--;
        }

        size = StackSize;
        head = remembered;
        while (size > 0) {
            head->priority--;
            head = head->next;
            size--;
        }
        head = remembered;
    }
}


int main() {
    setlocale(LC_ALL, "russian");

    int StackSize = 0;
    stack *tail;
    stack *head = tail;
    ReadFromTheFile(tail, StackSize);

    string answer;
    while (true) {
        cout <<
        "Enter 's', if you want to see a task, or enter 'a', if you want to add a task, or enter '!', if you want to stop working with program: ";

        cin >> answer;
        if ((answer == "s") || (answer == "п")) {
            Showtask(head, StackSize);
        }
        if ((answer == "a") || (answer == "д")) {
            AddTask(tail, StackSize);
        }
        if (answer == "!") {
            goto EXIT;
        }
    }
    EXIT:
    IncreasePriority(head, StackSize);
    WriteToTheFile(head, StackSize);
}