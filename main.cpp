/*
M. Ibrahim Faisal
Abdullah Arshad
Ali Mahoon 23L-0531
*/


#include <iostream>
#include <string>
#include <fstream>
#include <cstdlib>
#include <chrono>
#include <ctime>

using namespace std;

class User;
class Task;
class Badges;
class Categories;

void resizeArray(User* &arr, int &size);
int signup(const string& name,const string& password);
bool logIn(const string& check, int &lineNumber);


string users_file = "users.txt";
string tasks_file = "tasks.txt";
const int MAX_TASKS = 10;
const int MAX_BADGES = 5;

class Categories
{
    int size;
    string *array;
public:
    Categories()
    {
        size = 0;
        array = new string [size];
    }
    void addCategory(string newCatgName,int &size)
    {
        int newSize = size++;
        cout << "Enter the name of category:";
        for(int i=0;i<size;i++) {
            cout << "Category" << i + 1 << ":";
            cin >> array[i];
        }
        string* temp = new string [newSize];

        for (int i = 0; i < size; i++)
        {
            temp[i] = array[i];
        }

        temp[size] = newCatgName;

        delete[] array;

        array = temp;
        size = newSize;
    }

    void displayCategories()
    {
        cout << "Categories:\n";
        for (int i = 0; i < size; i++)
        {
            cout << i + 1 << ". " << array[i] << endl;
        }
    }
    ~Categories()
    {
        delete[] array;
    }


};

tm getCurrentDate()
{
    time_t currentTime = time(nullptr);
    return *localtime(&currentTime);
}

// ***** TASK MANAGEMENT ****


class Task {
private:
    string taskName;
    bool completed; // 0 -> pending task, 1 -> completed task
    int priority{}; // 0pt = simple, 1pt = small, 2pt = medium, 3pt = huge
    int category{};
    tm dueDate{};
    time_t currentTime;
    char dateOption = 'n';
    tm* currentLocalTime = nullptr;
public:

    Task operator=(const Task& other)
    {
        this->taskName = other.taskName;
        this->completed = other.completed;
        this->priority = other.priority;
        this->category= other.category;
        this->dueDate=other.dueDate;
        this->currentTime=other.currentTime;
        if (this->currentLocalTime != nullptr)
        {
            currentLocalTime = nullptr;
        }
        if (other.currentLocalTime != nullptr)
        {
            this->currentLocalTime = new tm(*other.currentLocalTime);
        } else {
            this->currentLocalTime = nullptr;
        }
        return *this;
    }

    Task()
    {
        taskName = "";
        priority = 0;
        category = 0;
        completed = false;
        currentTime = std::time(nullptr);
        currentLocalTime  = std::localtime(&currentTime);
    }

    void addNewTask() {
        cout << "Enter task: ";
        cin.ignore();
        getline(cin, taskName);

        cout << "\nEnter priority: \n";
        cout << "1) Simple (no) priority | 1 point\n";
        cout << "2) Low priority         | 2 points\n";
        cout << "3) Medium priority      | 3 points\n";
        cout << "4) High priority        | 5 points\n\n";
        cin >> priority;

        while (priority <= 0 || priority >= 5) {
            cout << "Invalid priority. Enter again: ";
            cin >> priority;
        }

        cout << "Do you want to add a due date for this task? (y/n): ";
        cin >> dateOption;
        if (dateOption == 'y' || dateOption == 'Y')
        {
            int year, month, day;
            cout << "Enter year for completion: ";
            cin >> year;
            cout << "Enter month for completion: ";
            cin >> month;
            cout << "Enter day for completion: ";
            cin >> day;

            tm dueDate{};
            dueDate.tm_year = year;
            dueDate.tm_mon = month;
            dueDate.tm_mday = day;

            tm currentDate = getCurrentDate();
            time_t time1 = mktime(&dueDate);
            time_t time2 = mktime(currentLocalTime);
            while (difftime(time1, time2) < 0) {
                cout << "Due date is today or in the past." << std::endl;

                cout << "Enter year for completion: ";
                cin >> year;
                cout << "Enter month for completion: ";
                cin >> month;
                cout << "Enter day for completion: ";
                cin >> day;

                dueDate.tm_year = year;
                dueDate.tm_mon = month;
                dueDate.tm_mday = day;
            }

            // Add tasks to file
            ofstream fout(tasks_file, ios::app);

            fout << completed << "," << taskName << "," << priority << "," << category << "," << dueDate.tm_mday << "/"
            <<dueDate.tm_mon<< "/"<<dueDate.tm_year << endl;
            fout.close();
        } else {
            ofstream fout(tasks_file, ios::app);
            fout << completed << "," << taskName << "," << priority << "," << category << endl;
            fout.close();
        }

    }


    // Setters and Getters

    int getDateOption () {
        return dateOption;
    }

    const string &getTaskName() const {
        return taskName;
    }

    void setTaskName(const string &taskName) {
        Task::taskName = taskName;
    }

    bool isCompleted() const {
        return completed;
    }

    void setCompleted(bool completed) {
        Task::completed = completed;
    }

    int getPriority() const {
        return priority;
    }

    void setPriority(int priority) {
        Task::priority = priority;
    }

    int getCategory() const {
        return category;
    }

    void setCategory(int category) {
        Task::category = category;
    }

    const tm &getDueDate() const {
        return dueDate;
    }

    void setDueDate(const tm &dueDate) {
        Task::dueDate = dueDate;
    }

    tm *getCurrentLocalTime() const {
        return currentLocalTime;
    }

    void setCurrentLocalTime(tm *currentLocalTime) {
        Task::currentLocalTime = currentLocalTime;
    }

    ~Task() {
       // delete[] currentLocalTime;
        currentLocalTime = nullptr;
    }
};

class Badges
{
/*
 * Badge idea 1: Master planner: Adding x amount of tasks.
 * Badge idea 2: Early planner: Planning ahead (let's say 1 week) before task completion
 * Badge idea 3: Productive day: Finish 3 tasks in one day
 * Badge idea 4:
 *
 * */
};






// ***** USER MANAGEMENT ****
class User : public Categories
{
private:
    string username;
    int points;
    Task* tasks = nullptr;
    string badges[MAX_BADGES];
    int numTasks;
    int numBadges;

public:
   User()
   {
      username="";
      points=0;
      tasks=nullptr;
      numTasks=0;
   }

   void setName(const string& N)
   {
       username=N;
   }

    void addTask()
    {
        numTasks++;
        Task* temp = new Task[numTasks];

        for (int i = 0; i < numTasks-1; i++)
        {
            temp[i] = this->tasks[i];
        }

        temp[numTasks-1].addNewTask();
        delete[] this->tasks;
        this->tasks = nullptr;

        this->tasks= new Task[numTasks];
        for (int i = 0; i < numTasks; i++)
        {
            this->tasks[i]=temp[i];
        }

        delete [] temp;
        temp = nullptr;
    }

    void removeTask(int index)
    {
       if (index >= 0 && index < numTasks)
        {

            for (int i = index; i < numTasks - 1; i++)
            {
                tasks[i] = tasks[i + 1];
            }
            numTasks--;
            cout << "Task removed successfully!" << endl;
        }
        else
        {
            cout << "Invalid task index!" << endl;
        }
  }
  void completeTask(int index)
  {
        if (index >= 0 && index < numTasks && !tasks[index].isCompleted())
        {
            tasks[index].setCompleted(true);
            points += tasks[index].getPriority();

            if (points >= 100 && numBadges < MAX_BADGES)
            {
                badges[numBadges++] = "Master Planner";
                cout << "Congratulations! You've earned the 'Master Planner' badge!" << endl;
            }
            cout << "Task '" << tasks[index].getTaskName() << "' completed! Points earned: " << tasks[index].getPriority() << endl;
        }
        else
        {
            cout << "Invalid task index or task already completed!" << endl;
        }
  }

  void displayTasks()
  {
        cout << "To-Do List for " << username << ":" << endl;
        for (int i = 0; i < numTasks; i++)
        {
            cout << i + 1 << ". ";
            if (tasks[i].isCompleted())
                cout << "[X] ";
            else
                cout << "[ ] ";
            cout << tasks[i].getTaskName() << " - Points: " << tasks[i].isCompleted() << endl;
        }
        cout << "Total Points: " << points << endl;
        cout << "Earned Badges:" << endl;
        for (int i = 0; i < numBadges; i++)
        {
            cout << "- " << badges[i] << endl;
        }
  }
  ~User()
  {
       delete [] tasks;
       tasks=nullptr;
  }

};

void resizeArray(User* &arr, int &size)
{
    int newSize = size++;
    User* temp = new User[newSize];

    for (int i = 0; i < size; i++) {
        temp[i] = arr[i];
    }

    delete[] arr;

    arr = temp;
    size = newSize;
}




bool logIn(const string& check, int &lineNumber)
{
    ifstream in(users_file);
    string line;
    lineNumber = 0;

    if(!in.is_open())
    {
        cout<<"error opening the file."<<"\a";
        return false;
    }

    else
    {
        while(getline(in,line))
        {
            if(line==check)
            {
                return true;
            }
            lineNumber++;
        }
        return false;
    }


}

int signup(const string& name,const string& password)
{
    string combined = name + ',' + password;


    ifstream check (users_file);
    if (!check.is_open())
    {
        cout << "error opening the file." << "\a";
        return 0;
    }

    string old;
    while (getline(check, old,','))
    {
        if (name == old)
        {
            cout << "username already in use." << endl;
            return 0;
        }
        else
            getline(check,old);
    }
    ofstream write(users_file, ios::app); //opening in append mode so that it writes at the end of the file.
    write << combined << endl;
    write.close();
    cout << "Signed in successfully, you may log in now." << endl;
    return 1;

}







int main()
{
    User* users = nullptr;
    cout<<"1) For sign up" << endl;
    cout<<"2) Log in" << endl;
    int option;
    cin>>option;
    int numberOfUsers=0;
    string name,password,combined;
    users = new User[numberOfUsers];

    tm myDate;

    myDate.tm_year = 2024;
    myDate.tm_mon = 4;
    myDate.tm_mday = 10;


    switch (option)
    {
        case 1:
            cout << "Enter your username: ";
            cin.ignore();
            getline(cin, name);
            cout<< "Enter your password: ";
            getline(cin,password);
            combined = name + ',' + password;
            if(signup(name,password))
            {
                resizeArray(users, numberOfUsers);
                users[numberOfUsers -1].setName(name);
            }

            break;
        case 2:

            cout << "Enter your username: ";
            cin.ignore();
            getline(cin, name);
            cout << "Enter your password: ";
            getline(cin, password);

            combined = name + ',' + password;
            int userID;

            if (logIn(combined, userID)) // Find user in users.txt
            {
                cout << "\033[H\033[2J\033[3J";
                cout << "Logged in successfully :) " << endl;
                cout << "Choose the action u want to perform." << endl << endl << endl;
                cout << "0) Exit the app." << endl;
                cout << "1) Add task." << endl;
                cout << "2) Delete a task." << endl;
                cout << "3) Pin a task." << endl;
                cout << "4) Edit a task." << endl;

                int choice;
                cin >> choice;
                switch (choice)
                {
                    case 0:
                        return 0;
                        break;
                    case 1:
                        users[userID].addTask();
                        break;
                    case 2:
                        break;
                    case 3:
                        break;
                    default:
                        cout << "invalid option" << "\a" << endl;
                }
            }
            break;
            default:
            cout<<"invalid option"<<endl;
    }
}


