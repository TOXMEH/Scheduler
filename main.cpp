#include <iostream>
#include <fstream>
#include <cstddef>
#include <iomanip>
//#include <string>
//#include <Windows.h>


using namespace std;

//Число дней в месяцах года с 0 элементом не работаем
int DayInMonth[] = {-1, 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};

class date {
public:
    //Мне захотелось год, месяц, день int сделать
    int _Year;
    int _Month;
    int _Day;
public :
    //Конструктор преобразования с начальным инитом private переменных
    //говорят очень эффективно, ну значит так и запишем
    date(int iYear = 2015, int iMonth = 1, int iDay = 1) {
        _Day = iDay;
        _Month = iMonth;
        _Year = iYear;
    };

    bool isintYear() { return (_Year % 4 == 0); }//Високостный год нацело делиться на 4

    //Устанавливает месяц события
    void SetMonth();

    //Устанавливает день месяца события
    void SetDay();

    //Возвращает число дней в году
    int GetDaysInYear();

    //Преобразует дату в число дней от начала года
    int GetDayOfYear();

    //Возвращает Число дней между событиями
    int GetDaysBetween(date Today);
};

void date::SetMonth() {
    std::cout << "Month:  ";
    std::cin >> _Month;
    if (_Month < 1 || 12 < _Month)
        std::cout << "Wrong value of Month!Program set it as " << (_Month = 1) << "\r\n";
}

void date::SetDay() {
    std::cout << "Day: ";
    std::cin >> _Day;
    if (DayInMonth[_Month] + (isintYear() ? 1 : 0) < _Day)
        std::cout << "Wrong value of Day!Program set it as 1 " << (_Day = 1) << "\r\n";
}


int date::GetDaysInYear() {
    return 365 + (isintYear() ? 1 : 0);
}

int date::GetDayOfYear() {
    int Days = _Day;
    for (int i = 1; i < _Month; i++)
        Days += DayInMonth[i];
    if (isintYear())
        Days++;//+ 29 feb
    return Days;
}

int date::GetDaysBetween(date Today) {
    int DaysTillEvent = GetDaysInYear() - GetDayOfYear();
    int DaysTillToday = Today.GetDaysInYear() - Today.GetDayOfYear();
    return DaysTillToday - DaysTillEvent;
}


struct stack {
    string Task;
    int Priority;
    int Month;
    int Day;
    struct stack *next;
};

void Showtask(stack *&head, int &StackSize) {
    stack *remembered = head;
    int size = StackSize;

    if (StackSize == 0) {
        cout << "There are no tasks" << endl;
        return;
    }

    stack *previous;

    for (int p = 1; p <= 3; p++) {
        while ((size > 0) && (head->Priority > p)) {
            previous = head;
            head = previous->next;
            size--;
        }
        if (size > 0)
            goto EXIT;
        else {
            head = remembered;
            size = StackSize;
        }
    }

    EXIT:
    cout << head->Task << endl;
    cout << "Has the Task been done? ";
    string HasBeenDone;
    cin >> HasBeenDone;
    if ((HasBeenDone == "yes") && (size == StackSize)) {//если это задание находится на вершине стэка
        head = head->next;
        StackSize--;
    }
    else if ((HasBeenDone == "yes") && (size > 1)) {
        previous->next = head->next;
        delete head;
        head = remembered;
        StackSize--;
    } else if ((HasBeenDone == "yes") && (size == 1)) {
        delete head;
        head = remembered;
        StackSize--;
    }

}

void AddTask(stack *&tail, int &StackSize, date Today) {
    cout << "Enter the Task to add: ";
    string Task;
    cin >> Task;

    int Priority;
    date Deadline;
    cout << "Enter deadline`s date:" << endl;
    Deadline.SetMonth();
    Deadline.SetDay();
    int DaysLeft = Deadline.GetDaysBetween(Today);
    switch (DaysLeft) {
        case 0:
        case 1:
            Priority = 1;
            break;
        case 2:
        case 3:
        case 4:
        case 5:
        case 6:
            Priority = 2;
            break;
        default:
            Priority = 3;
    }
    if (DaysLeft < 0)
        Priority = 1;

    if (StackSize == 0) {
        tail->Task = Task;
        tail->Priority = Priority;
        tail->Month = Deadline._Month;
        tail->Day = Deadline._Day;

        StackSize++;
    } else {
        stack *next = new(stack);
        tail->next = next;
        next->Task = Task;
        next->Priority = Priority;
        next->Month = Deadline._Month;
        next->Day = Deadline._Day;
        tail = next;

        StackSize++;
    }
}

void WriteToTheFile(stack *&head, int &StackSize) {
    if (StackSize > 0) {
        ofstream output("tasks.txt");

        output << StackSize << '\n';
        while (StackSize > 1) {
            output << head->Month << " " << head->Day << " " << head->Task << '\n';

            head = head->next;
            StackSize--;
        }
        output << head->Month << " " << head->Day << " " << head->Task;
    }
}


void ReadFromTheFile(stack *&tail, int &StackSize, date Today) {
    ifstream input("tasks.txt");
    if (!input.is_open()) {
        cout << "tasks.txt cannot be found" << endl;
        return;
    }
    string inter;
    getline(input, inter);
    StackSize = stoi(inter);

    bool IsFirst = true;

    if (StackSize > 0) {
        while (!(input.eof())) {
            if ((StackSize > 1) && (!(IsFirst))) {
                //если в файле больше 1 записи то выделяем память для каждой новой записи
                stack *next = new(stack);
                tail->next = next;
                tail = next;
            }
            IsFirst = false;

            date Deadline;
            input >> Deadline._Month;
            input >> Deadline._Day;

            string task;
            getline(input, task);
            tail->Task = task;
            tail->Month = Deadline._Month;
            tail->Day = Deadline._Day;

            int DaysLeft = Deadline.GetDaysBetween(Today);
            switch (DaysLeft) {
                case 0:
                case 1:
                    tail->Priority = 1;
                    break;
                case 2:
                case 3:
                case 4:
                case 5:
                case 6:
                    tail->Priority = 2;
                    break;
                default:
                    tail->Priority = 3;
            }

        }
    }
}

int main() {

    cout << "Enter today`s date" << endl;
    date Today;
    Today.SetMonth();
    Today.SetDay();

    int StackSize = 0;
    stack *tail = new(stack);
    stack *head = new(stack);
    head = tail;
    ReadFromTheFile(tail, StackSize, Today);

    char answer;
    while (true) {
        cout << "Enter 's', if you want to see a Task" << endl;
        cout << "Enter 'a', if you want to add a Task" << endl;
        cout << "Enter '!', if you want to stop working with program: ";

        cin >> answer;
        cout << endl;
        if (answer == 's') {
            Showtask(head, StackSize);
        }
        if (answer == 'a') {
            AddTask(tail, StackSize, Today);
        }
        if (answer == '!') {
            goto EXIT;
        }
        cout << endl;
    }
    EXIT:
    WriteToTheFile(head, StackSize);
    delete (head);
}