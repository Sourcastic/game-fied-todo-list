#include <iostream>
#include <string>
#include <fstream>
#include <ctime>
#include <iomanip>
#include <unistd.h>

using namespace std;

class User;

class Task;

class Badges;

class Category;


// ***** TASK MANAGEMENT ****
template<class T>
void resizeArray(T *&arr, int &size);

int signup(const string &name, const string &password);

bool logIn(const string &check, int &lineNumber);


string users_file = "users.txt";
string tasks_file = "tasks.txt";
string category_file = "category.txt";
string badges_file = "badges.txt";

class Category {
private:
    int size;
    string *categoryList;
public:
    Category() {
        size = 0;
        categoryList = new string[size];
    }

    void loadCategory(const string &username) {
        ofstream fout(category_file, ios::app);
        ifstream fin(category_file);
        string line;
        bool existsInFile;
        bool fileEmpty;
        char ch;
        fin >> noskipws >> ch;

        if (fin.eof()) {
            fileEmpty = true;
        } else {
            fileEmpty = false;
        }
        fin.close();
        if (fout.is_open()) {
            for (int i = 0; i < size; i++) {
                existsInFile = false;
                fin.open(category_file);
                if (fileEmpty) {
                } else {
                    while (!fin.eof()) {
                        getline(fin, line);
                        if ((username + ':' + categoryList[i]) == line) {
                            existsInFile = true;
                        }
                    }
                }
                fin.close();
                if (existsInFile == false) {
                    fout << username << ":" << categoryList[i] << endl;
                }
            }
        } else {
            cout << "File not found.\n";
        }
        fout.close();
    }

    void addCategory(string newCatgName) {
        int newSize = size + 1;

        string *temp = new string[newSize];

        for (int i = 0; i < size; i++) {
            temp[i] = categoryList[i];
        }

        temp[size] = newCatgName;
        delete[] categoryList;
        categoryList = new string[newSize];

        for (int i = 0; i < newSize; i++) {
            categoryList[i] = temp[i];
        }

        size = newSize;
        cout << "Category successfully added. :)\n";
    }


    ~Category() {
        delete[] categoryList;
    }


};

tm getCurrentDate() {
    time_t currentTime = time(nullptr);
    return *localtime(&currentTime);
}

bool isValidDate(int day, int month, int year) { // This shit used to be an entire ass PF assignment. Good times.
    if (month < 1 || month > 12) {
        return false;
    }

    if (day < 1 || day > 31) {
        return false;
    } else if (month == 4 || month == 6 || month == 9 || month == 11) {
        if (day > 30) {
            return false;
        }
    }

    if (month == 2) {
        if (year % 4 == 0 && (year % 100 != 0 || year % 400 == 0)) {
            if (day > 29) {
                return false;
            }
        } else {
            if (day > 28) {
                return false;
            }
        }
    }
    return true;
}

bool doesNotContainComma(const string &str) {
    for (char c: str) {
        if (c == ',' || c == ':') {
            return false;
        }
    }
    return true;
}

class Task {
private:
    string taskName;
    bool completed; // 0 -> pending task, 1 -> completed task
    bool dateEntered;
    int priority{}; // 0pt = simple, 1pt = small, 2pt = medium, 3pt = huge
    int category{};
    tm dueDate{};
    time_t currentTime;
    char dateOption;
    tm *currentLocalTime = nullptr;

public:

    Task operator=(const Task &other) {
        this->taskName = other.taskName;
        this->completed = other.completed;
        this->priority = other.priority;
        this->category = other.category;
        this->dueDate = other.dueDate;
        this->currentTime = other.currentTime;
        if (this->currentLocalTime != nullptr) {
            currentLocalTime = nullptr;
        }
        if (other.currentLocalTime != nullptr) {
            this->currentLocalTime = new tm(*other.currentLocalTime);
        } else {
            this->currentLocalTime = nullptr;
        }
        return *this;
    }


    Task() {
        taskName = "";
        priority = 0;
        category = 5;
        completed = false;
        dateOption = 'n';
        dateEntered = false;
        currentTime = std::time(nullptr);
        currentLocalTime = std::localtime(&currentTime);
    }

    void displayCategories(const string &username) {
        ifstream fin(category_file);
        int i = 0;
        while (!fin.eof()) {
            string line;
            getline(fin, line, ':');
            if (line == username) {
                i++;
            }
            getline(fin, line);
        }
        int j = 0;
        fin.close();
        fin.open(category_file);
        while (j < i) {
            string line;
            getline(fin, line, ':');
            if (line == username) {
                getline(fin, line);
                cout << " " << j << ") " << line << endl;
                j++;
            }
        }
    }

    void addNewTask(const string &username) {
        cout << "Enter task: ";
        cin.ignore();
        getline(cin, taskName);
        while(taskName.empty()) {
            cout << "Task name can't be empty. Try again: ";
            getline(cin, taskName);
        }
        while (!doesNotContainComma(taskName)) {
            cout << "Sorry, name can not contain a comma or colon. Enter again: ";
            cin.ignore();
            getline(cin, taskName);
        }

        cout << "\nEnter priority: \n";
        cout << "0) Simple (no) priority | 1 point\n";
        cout << "1) Low priority         | 2 points\n";
        cout << "2) Medium priority      | 3 points\n";
        cout << "3) High priority        | 5 points\n\n";
        cin >> priority;


        while (priority < 0 || priority > 3) {
            cout << "Invalid priority. Enter again: ";
            cin >> priority;
        }

        cout << "Category list:\n";
        cout << "-1) No category\n";
        displayCategories(username);
        cout << "\nWhat category is the task from: ";
        cin >> category;
        if(category == -1) {
            category = 5;
        }

        cout << "Do you want to add a due date for this task? (y/n): ";
        cin >> dateOption;

        if (dateOption == 'y' || dateOption == 'Y') {
            dateEntered = true;
            int year, month, day;

            cout << "Enter day for completion: ";
            cin >> day;
            cout << "Enter month for completion: ";
            cin >> month;
            cout << "Enter year for completion: ";
            cin >> year;


            while (!isValidDate(day, month, year)) {
                cout << "\nNot a valid date, enter again: \n";
                cout << "Enter day for completion: ";
                cin >> day;
                cout << "Enter month for completion: ";
                cin >> month;
                cout << "Enter year for completion: ";
                cin >> year;

            }

            dueDate.tm_mday = day;
            dueDate.tm_mon = month;
            dueDate.tm_year = year;

            time_t time1 = mktime(&dueDate);
            time_t time2 = mktime(currentLocalTime);
            while (difftime(time1, time2) < 0) {
                cout << "\nDue date is either today or in the past.\n\n";

                cout << "Enter day for completion: ";
                cin >> day;
                cout << "Enter month for completion: ";
                cin >> month;
                cout << "Enter year for completion: ";
                cin >> year;

                dueDate.tm_year = year;
                dueDate.tm_mon = month;
                dueDate.tm_mday = day;
            }

            // Add tasks to file
            ofstream fout(tasks_file, ios::app);
            if (!fout.is_open()) {
                cout << "file not found";
            } else {

                fout << username << ":" << completed << "," << taskName << "," << priority << "," << category << ","
                     << dueDate.tm_mday << "/"
                     << dueDate.tm_mon << "/" << dueDate.tm_year << endl;
                fout.close();
            }

        } else {
            ofstream fout(tasks_file, ios_base::app);
            if (!fout.is_open()) {
                cout << "file not found";
            } else {
                fout << username << ":" << completed << "," << taskName << "," << priority << "," << category << endl;
                fout.close();
            }

        }

    }

    void displayTask(int MaxLength, int index, const string &username) {
        const char fillChar = ' ';
        const string red = "\033[31m";
        const string yellow = "\033[33m";
        const string green = "\033[32m";
        const string resetFormatting = "\033[0m";
        string priorityName;
        if (priority == 0) {
            priorityName = "None";
        } else if (priority == 1) {
            priorityName = "Low";
        } else if (priority == 2) {
            priorityName = "Medium";
        } else {
            priorityName = "High";
        }
        int colorLength = 3; // Length of ANSI color escape codes

        string categoryName;

        categoryName = "Uncategorised";
        if (category != 5) {
            ifstream fin(category_file);
            int i = 0;

            while (!fin.eof()) {
                string tempName;
                getline(fin, tempName, ':');
                if (tempName == username) {
                    if (i == category) {
                        getline(fin, categoryName);
                        break;
                    }
                    i++;
                }
                getline(fin, categoryName);
            }
            fin.close();
        }

        bool dateNotEntered = true;
        if (dateOption == 'y' || dateOption == 'Y') {
            dateNotEntered = false;
        }


        if (completed == 0) { // If task is incomplete
            time_t time1 = mktime(&dueDate);
            time_t time2 = mktime(currentLocalTime);

            // If task is not missing
            if (difftime(time1, time2) > 0) {
                cout << setw(MaxLength + colorLength) << left << (yellow + to_string(index)) << setfill(fillChar);
                cout << " | ";
                cout << setw(MaxLength + colorLength) << left << (yellow + taskName) << setfill(fillChar);
                cout << " | ";
                cout << setw(MaxLength + colorLength) << left << (yellow + categoryName) << setfill(fillChar);
                cout << " | ";
                cout << setw(MaxLength + colorLength) << left << (yellow + priorityName) << setfill(fillChar);
                cout << " | ";
                if (dateNotEntered) {
                    cout << setw(MaxLength + colorLength) << left << yellow << "|" << setfill(fillChar);
                } else {
                    string date = to_string(dueDate.tm_mday) + "/" + to_string(dueDate.tm_mon) + "/" +
                                  to_string(dueDate.tm_year);
                    cout << setw(MaxLength + colorLength) << left << (yellow + date) << setfill(fillChar) << "|";
                }
                // If task is missing
            } else {
                cout << setw(MaxLength + colorLength) << left << (yellow + to_string(index)) << setfill(fillChar);
                cout << " | ";
                cout << setw(MaxLength + colorLength) << left << (yellow + taskName) << setfill(fillChar);
                cout << " | ";
                cout << setw(MaxLength + colorLength) << left << (yellow + categoryName) << setfill(fillChar);
                cout << " | ";
                cout << setw(MaxLength + colorLength) << left << (yellow + priorityName) << setfill(fillChar);
                cout << " | ";
                if (dateOption == 'y' || dateOption == 'Y') {
                    cout << setw(MaxLength + colorLength) << left << yellow << "|" << setfill(fillChar);
                } else {
                    string date = to_string(dueDate.tm_mday) + "/" + to_string(dueDate.tm_mon) + "/" +
                                  to_string(dueDate.tm_year);
                    cout << setw(MaxLength + colorLength) << left << (yellow + date) << setfill(fillChar) << "|";
                }
            }
            // If task is complete
        } else {
            cout << setw(MaxLength + colorLength) << left << (green + to_string(index)) << setfill(fillChar);
            cout << " | ";
            cout << setw(MaxLength + colorLength) << left << (green + taskName) << setfill(fillChar);
            cout << " | ";
            cout << setw(MaxLength + colorLength) << left << (green + categoryName) << setfill(fillChar);
            cout << " | ";
            cout << setw(MaxLength + colorLength) << left << (green + priorityName) << setfill(fillChar);
            cout << " | ";
            if (dateOption == 'y' || dateOption == 'Y') {
                string date =
                        to_string(dueDate.tm_mday) + "/" + to_string(dueDate.tm_mon) + "/" + to_string(dueDate.tm_year);
                cout << setw(MaxLength + colorLength) << left << (green + date) << setfill(fillChar);
                cout << " | ";
            } else {
                cout << setw(MaxLength + colorLength) << left << green << "|" << setfill(fillChar);
            }
        }
        cout << resetFormatting;
    }

    void fillTaskFile(string username) {
        ofstream fout(tasks_file, ios_base::app);
        if (dateEntered) {
            fout << username << ":" << completed << "," << taskName << "," << priority << "," << category << ","
                 << dueDate.tm_mday << "/"
                 << dueDate.tm_mon << "/" << dueDate.tm_year << endl;
        } else {
            fout << username << ":" << completed << "," << taskName << "," << priority << "," << category << endl;
        }
        fout.close();
    }


    void setAttributes(const string &add) {
        int i = 0;

        for (i; add[i] != ':'; i++) {}
        i++;

        if (add[i] == '0') {
            completed = false;
        } else
            completed = true;

        i += 2;
        string substring;
        char delimiter = ',';
        int namePosition = add.find(delimiter, i);
        substring = add.substr(i, namePosition - i);
        taskName = substring;
        i = namePosition + 1;

        priority = stoi(&add[i]);

        i += 2;

        category = stoi(&add[i]);
        i++;


        if (add[i] == ',') {

            dateOption = 'y';

            i++;
            delimiter = '/';
            namePosition = add.find(delimiter, i);
            substring = add.substr(i, namePosition - i);
            dueDate.tm_mday = stoi(substring);
            i = namePosition + 1;

            namePosition = add.find(delimiter, i);
            substring = add.substr(i, namePosition - i);
            dueDate.tm_mon = stoi(substring);
            i = namePosition + 1;

            int len = add.length();
            substring = add.substr(i, len - 1);
            dueDate.tm_year = stoi(substring);
        }
    }



    // Setters and Getters


    const string &getTaskName() const {
        return taskName;
    }

    const tm &getDueDate() const {
        return dueDate;
    }

    void setCompleted(bool completed) {
        Task::completed = completed;
    }

    int getPriority() const {
        return priority;
    }

    bool getCompleted() {
        return Task::completed;
    }

    ~Task() {
        // delete[] currentLocalTime;
        currentLocalTime = nullptr;
    }
};

class Badges {
/*
 * Badge idea 1 : Level up! Every 10 points, a new badge
 * Badge idea 2 : Early planner: If the date set is a week ahead
 * Badge idea 3 : Productive Day: Mark more than 2 tasks as complete in one day
 * Badge idea 4 : Master Planner: More than 2 tasks added in a single day!
 * */
protected:
    int numBadgesEarned[4];
    const string badgeNames[4] = {"Level up", "Early planner", "Productive Day", "Master Planner"};
    int levelUpPoints;
    time_t master_oldTime;
    tm *master_oldLocalTime = nullptr;
    time_t productive_oldTime;
    tm *productive_oldLocalTime = nullptr;
public:
    Badges() {
        levelUpPoints = 0;
        for (int i = 0; i < 4; i++) {
            numBadgesEarned[i] = 0;
        }
        master_oldTime = std::time(nullptr);
        master_oldLocalTime = std::localtime(&master_oldTime);
        productive_oldTime = std::time(nullptr);
        productive_oldLocalTime = std::localtime(&productive_oldTime);
    }

    void loadFromBadgesFile(const string &username) {
        string temp;
        ifstream fin(badges_file);
        int lineNum = 0;

        while (getline(fin, temp, ':')) {
            if (temp == username) {
                break;
            } else {
                lineNum++;
            }
            getline(fin, temp);
        }

        fin.close();
        fin.open(badges_file);

        if (fin.is_open()) {
            int i = 0;
            while (i != lineNum) {
                getline(fin, temp);
                i++;
            }
            getline(fin, temp, ':');
            getline(fin, temp, ',');
            numBadgesEarned[0] = stoi(temp);
            getline(fin, temp);
            levelUpPoints = stoi(temp);

            getline(fin, temp, ':');
            getline(fin, temp);
            numBadgesEarned[1] = stoi(temp);

            getline(fin, temp, ':');
            getline(fin, temp, ',');
            numBadgesEarned[2] = stoi(temp);
            getline(fin, temp, '/');
            productive_oldLocalTime->tm_mday = stoi(temp);
            getline(fin, temp, '/');
            productive_oldLocalTime->tm_mon = stoi(temp);
            getline(fin, temp);
            productive_oldLocalTime->tm_year = stoi(temp);

            getline(fin, temp, ':');
            getline(fin, temp, ',');
            numBadgesEarned[3] = stoi(temp);
            getline(fin, temp, '/');
            master_oldLocalTime->tm_mday = stoi(temp);
            getline(fin, temp, '/');
            master_oldLocalTime->tm_mon = stoi(temp);
            getline(fin, temp);
            master_oldLocalTime->tm_year = stoi(temp);


        }
        fin.close();
    }


    void earnLevelUp(int newPoints) {
        ifstream fin(badges_file);
        string line;
        getline(fin, line, ',');
        getline(fin, line);
        levelUpPoints = stoi(line);
        levelUpPoints = newPoints + levelUpPoints;
        if (levelUpPoints >= 10) {
            numBadgesEarned[0]++;
            levelUpPoints = (levelUpPoints - 10);
        }
        fin.close();
    }

    void earnMasterPlanner(int &tasksAdded) {
        if (tasksAdded == 1) {
            master_oldTime = std::time(nullptr);
            master_oldLocalTime = std::localtime(&master_oldTime);

        } else if (tasksAdded >= 2) {
            time_t master_currentTime = std::time(nullptr);
            tm *master_currentLocalTime = std::localtime(&master_currentTime);
            time_t time1 = mktime(master_oldLocalTime);
            time_t time2 = mktime(master_currentLocalTime);
            if (difftime(time1, time2) > 60.00) { // Actual: one day, for evaluation, one minute
                tasksAdded = 0;
            } else if (tasksAdded == 2) {
                tasksAdded++;
            } else if (tasksAdded > 2) {
                numBadgesEarned[3]++;
                tasksAdded = 0;
            }

        }
    }

    void earnEarlyPlanner(tm dueDate) {
        time_t early_currentTime = std::time(nullptr);
        tm *early_currentLocalTime = std::localtime(&early_currentTime);
        time_t time1 = mktime(&dueDate);
        time_t time2 = mktime(early_currentLocalTime);
        double diff = difftime(time1, time2);
        double weekTime = 7 * 24 * 60 * 60;
        if (diff > weekTime) {
            numBadgesEarned[1]++;
        }
    }

    void earnProductiveDay(int &tasksCompleted) {
        if (tasksCompleted == 1) {
            productive_oldTime = std::time(nullptr);
            productive_oldLocalTime = std::localtime(&productive_oldTime);

        } else if (tasksCompleted >= 2) {
            time_t productive_currentTime = std::time(nullptr);
            tm *productive_currentLocalTime = std::localtime(&productive_currentTime);
            time_t time1 = mktime(productive_oldLocalTime);
            time_t time2 = mktime(productive_currentLocalTime);
            if (difftime(time1, time2) > 60.00) { // Actual: one day, for evaluation, one minute
                tasksCompleted = 0;
            } else if (tasksCompleted == 2) {
                tasksCompleted++;
            } else if (tasksCompleted > 2) {
                numBadgesEarned[2]++;
                tasksCompleted = 0;
            }
        }
    }


    void displayBadges() {
        for (int i = 0; i < 4; i++) {
            cout << badgeNames[i] << " badge earned a total of... x" << numBadgesEarned[i] << " times!\n";
        }
        system("read");
    }


};

void overwriteLine(const string &filename, int lineNumber, const string &newText) {
    ifstream finBadges(filename);
    ofstream foutBadges(filename);
    const string tempBadgeFile = "temp" + filename;
    ofstream foutTemp(tempBadgeFile);
    ifstream finTemp(tempBadgeFile);
    int numLines = 0;
    while (!finBadges.eof()) {
        string temp;
        getline(finBadges, temp);
        numLines++;
    }

    finBadges.close();
    finBadges.open(filename);

    for (int i = 0; i < numLines; i++) {
        string temp;
        getline(finBadges, temp);
        if (i == lineNumber) {
            foutTemp << newText;
        } else {
            foutTemp << temp;
        }

    }
    finBadges.close();
    foutTemp.close();
    foutBadges.open(filename);
    for (int i = 0; i < numLines; i++) {
        string temp;
        getline(finTemp, temp);
        foutBadges << temp;
    }
    foutBadges.close();
    finBadges.close();
}


// ***** USER MANAGEMENT ****
class User : public Category, public Badges {
private:
    string username;
    int points;
    Task *task_list = nullptr;
    int numTasks;
    int tasksAdded = 0;
    int newPoints = 0;
    int taskCompleted = 0;
public:
    User() {
        username = "";
        points = 0;
        task_list = nullptr;
        numTasks = 0;
    }

    User operator=(const User &other) {
        this->username = other.username;
        this->points = other.points;
        this->task_list = other.task_list;
        this->numTasks = other.numTasks;
        return *this;
    }

    void addTask() {
        numTasks++;
        Task *temp = new Task[numTasks];

        for (int i = 0; i < numTasks - 1; i++) {
            temp[i] = this->task_list[i];
        }

        temp[numTasks - 1].addNewTask(username);
        delete[] this->task_list;
        this->task_list = nullptr;

        this->task_list = new Task[numTasks];
        for (int i = 0; i < numTasks; i++) {
            this->task_list[i] = temp[i];
        }

        delete[] temp;
        temp = nullptr;

        tasksAdded++;
        earnMasterPlanner(tasksAdded);
        fillBadgesFile();
        earnEarlyPlanner(task_list[numTasks].getDueDate());
        fillBadgesFile();
    }

    void loadTaskList() {
        ifstream fin(tasks_file);
        string line;

        delete[] task_list;
        task_list = new Task[numTasks];

        int addedTask = 0;

        if (!fin.is_open()) {
            cout << "File not open.";
        } else {
            while (addedTask < numTasks) {
                getline(fin, line);
                string substring;

                char delimeter = ':';
                int namePosition = line.find(delimeter);
                substring = line.substr(0, namePosition);

                if (substring == username) {
                    task_list[addedTask].setAttributes(line);
                    addedTask++;
                }
            }
        }
    }

    void removeTask() {
        int index;
        cout << "Hi " << username << ", which task number do you want to delete? ";
        cin >> index;
        if (index >= 0 && index < numTasks) {

            for (int i = index; i < numTasks - 1; i++) {
                task_list[i] = task_list[i + 1];
            }

            numTasks--;
            Task *temp = new Task[numTasks];

            for (int i = 0; i < numTasks; i++) {
                temp[i] = task_list[i];
            }

            delete[] task_list;
            task_list = new Task[numTasks];

            for (int i = 0; i < numTasks; i++) {
                task_list[i] = temp[i];
            }
            delete[] temp;

            ofstream fout(tasks_file);
            fout.close();
            for (int i = 0; i < numTasks; i++) {
                task_list[i].fillTaskFile(username);
            }

            cout << "Task number removed successfully!" << endl;
        } else {
            cout << "Invalid task index!" << endl;
        }
    }

    void fillBadgesFile() {
        ifstream fin(badges_file);
        int firstNameOccurance = 0;
        string line;
        bool fileEmpty;
        char ch;
        fin >> noskipws >> ch;

        if (fin.eof()) {
            fileEmpty = true;
        } else {
            fileEmpty = false;
        }
        fin.close();

        bool namePresent;
        if (fileEmpty == false) {
            fin.open(badges_file);
            while (!fin.eof()) {
                string temp;
                getline(fin, temp, ':');
                if (username == temp) {
                    namePresent = true;
                }
                getline(fin, temp);
            }
            fin.close();
        }

        if (fileEmpty == false && namePresent == true) {
            fin.open(badges_file);
            while (getline(fin, line, ':')) {
                if (line == username) {
                    break;
                } else {
                    firstNameOccurance++;
                }
            }
            fin.close();

            string temp;
            temp = username + ":" + to_string(numBadgesEarned[0]) + "," + to_string(levelUpPoints) + "\n";
            overwriteLine(badges_file, firstNameOccurance, temp);

            temp = username + ":" + to_string(numBadgesEarned[1]) + "\n";
            overwriteLine(badges_file, firstNameOccurance + 1, temp);

            temp = username + ":" + to_string(numBadgesEarned[2]) + "," + to_string(productive_oldLocalTime->tm_mday) +
                   "/" + to_string(productive_oldLocalTime->tm_mon) + "/" +
                   to_string(productive_oldLocalTime->tm_year) + "\n";
            overwriteLine(badges_file, firstNameOccurance + 2, temp);

            temp = username + ":" + to_string(numBadgesEarned[3]) + "," + to_string(productive_oldLocalTime->tm_mday) +
                   "/" + to_string(productive_oldLocalTime->tm_mon) + "/" +
                   to_string(productive_oldLocalTime->tm_year) + "\n";
            overwriteLine(badges_file, firstNameOccurance + 3, temp);
        } else if (!fileEmpty && !namePresent) {
            string temp;
            ofstream fout(badges_file, ios::app);
            fout << username << ":" << numBadgesEarned[0] << "," << levelUpPoints << endl;
            fout << username << ":" << numBadgesEarned[1] << endl;
            fout << username << ":" << numBadgesEarned[2] << "," << productive_oldLocalTime->tm_mday << "/"
                 << productive_oldLocalTime->tm_mon << "/" << productive_oldLocalTime->tm_year << endl;
            fout << username << ":" << numBadgesEarned[3] << "," << productive_oldLocalTime->tm_mday << "/"
                 << productive_oldLocalTime->tm_mon << "/" << productive_oldLocalTime->tm_year << endl;
            fout.close();
        } else {
            string temp;
            ofstream fout(badges_file);
            fout << username << ":" << numBadgesEarned[0] << "," << levelUpPoints << endl;
            fout << username << ":" << numBadgesEarned[1] << endl;
            fout << username << ":" << numBadgesEarned[2] << "," << productive_oldLocalTime->tm_mday << "/"
                 << productive_oldLocalTime->tm_mon << "/" << productive_oldLocalTime->tm_year << endl;
            fout << username << ":" << numBadgesEarned[3] << "," << productive_oldLocalTime->tm_mday << "/"
                 << productive_oldLocalTime->tm_mon << "/" << productive_oldLocalTime->tm_year << endl;
            fout.close();
        }
    }

    void toggleCompletionTask(int index) {
        if (index >= 0 || index < numTasks) {
            bool completed = task_list[index].getCompleted();

            if (completed == false) {
                task_list[index].setCompleted(true);
                ifstream fin(tasks_file);
                int i = 0;
                while (!fin.eof()) {
                    string temp;
                    getline(fin, temp, ':');
                    if (temp == username) {
                        i++;
                    }
                    if (index == i) {
                        string line;
                        getline(fin, line);
                        string replace = username + ':' + line;
                        overwriteLine(tasks_file, i, replace);
                    }
                    getline(fin, temp);
                }
                int priority = task_list[index].getPriority();
                if (priority == 0) {
                    newPoints = 1;
                    earnLevelUp(newPoints);
                    fillBadgesFile();
                    points += 1;
                } else if (priority == 1) {
                    newPoints = 2;
                    earnLevelUp(newPoints);
                    fillBadgesFile();
                    points += 2;
                } else if (priority == 2) {
                    newPoints = 3;
                    earnLevelUp(newPoints);
                    fillBadgesFile();
                    points += 3;
                } else if (priority == 3) {
                    newPoints = 5;
                    earnLevelUp(newPoints);
                    fillBadgesFile();
                    points += 5;
                }
                taskCompleted++;
                earnProductiveDay(taskCompleted);
                fillBadgesFile();
            } else {
                task_list[index].setCompleted(false);
            }
        } else {
            cout << "Invalid task index!" << endl;
        }
    }

    void displayTasks() {
        int maxLength = 25;

        for (int i = 0; i < numTasks; i++) {
            if (task_list[i].getTaskName().length() > maxLength) {
                maxLength = task_list[i].getTaskName().length();
            }
        }


        const char fillChar = ' ';

        cout << points << " Points Earned\n\n";

        for (int i = 0; i < 5 * (maxLength + 2); i++) {
            cout << "-";
        }
        cout << endl;
        cout << left << setw(maxLength - 1) << "ID" << setfill(fillChar) << "| ";
        cout << left << setw(maxLength - 1) << "Name" << setfill(fillChar) << "| ";
        cout << left << setw(maxLength - 1) << "Category" << setfill(fillChar) << "| ";
        cout << left << setw(maxLength - 1) << "Priority" << setfill(fillChar) << "| ";
        cout << left << setw(maxLength - 2) << "Date (if any)" << setfill(fillChar) << "| ";

        cout << endl;
        for (int i = 0; i < 5 * (maxLength + 2); i++) {
            cout << "-";
        }
        cout << endl;
        for (int i = 0; i < numTasks; i++) {
            task_list[i].displayTask(maxLength, i, username);
            cout << endl;
        }
        for (int i = 0; i < 5 * (maxLength + 2); i++) {
            cout << "-";
        }
        cout << endl;
        cout << endl << endl;
    }

    void fillTaskFile() {
        ofstream fout(tasks_file);
        fout.close();
        for (int i = 0; i < numTasks; i++) {
            task_list[i].fillTaskFile(username);
        }
    }

    const string &getUsername() const {
        return username;
    }

    int getNumTasks() const {
        return numTasks;
    }

    void setUsername(const string &username) {
        User::username = username;
    }

    void setNumTasks(int numTasks) {
        User::numTasks = numTasks;
    }

    ~User() {
        delete[] task_list;
        task_list = nullptr;
    }

};

template<class T>
void resizeArray(T *&arr, int &size) {
    int newSize = size + 1;
    T *temp = new T[newSize];

    for (int i = 0; i < size; i++) {
        temp[i] = arr[i];
    }

    delete[] arr;
    arr = temp;
    size = newSize;
}

bool logIn(const string &check, int &lineNumber) {
    ifstream fin(users_file);
    string line;
    lineNumber = 0;

    if (!fin.is_open()) {
        cout << "error opening the file." << "\a";
        return false;
    } else {
        while (!fin.eof()) {
            getline(fin, line);
            if (line == check) {

                return true;
            }
            lineNumber++;
        }
        return false;
    }
}

int signup(const string &password, const string &name) {
    string combined = name + ',' + password;
    ifstream check(users_file);
    if (!check.is_open()) {
        cout << "error opening the file." << "\a";
        return 0;
    }

    string old;
    while (getline(check, old, ',')) {
        if (name == old) {
            cout << "username already in use." << endl;
            return 0;
        } else
            getline(check, old);
    }
    check.close();
    ofstream write(users_file, ios::app); //opening in append mode so that it writes at the end of the file.
    write << combined << endl;
    write.close();
    cout << "Signed up successfully, you may log in now." << endl;
    return 1;

}

void fillUserList(User *&user_list, int &numberOfUsers) {
    ifstream finUser(users_file);
    if (!finUser.is_open()) {
        cout << "File not open.";
    }
    ifstream finTask;

    numberOfUsers = 0;
    string temp;
    while (getline(finUser, temp)) {
        numberOfUsers++;
    }

    user_list = new User[numberOfUsers];
    finUser.close();
    finUser.open(users_file);

    for (int i = 0; i < numberOfUsers; i++) {
        string userLine;
        string taskLine;

        getline(finUser, userLine, ',');
        user_list[i].setUsername(userLine);

        finTask.open(tasks_file);
        if (!finTask.is_open()) {
            cout << "File not open.";
        }
        int numTasks = 0;
        while (!finTask.eof()) {
            getline(finTask, taskLine, ':');
            if (taskLine == userLine) {
                numTasks++;
            }
            getline(finTask, taskLine);
        }
        finTask.close();

        user_list[i].setNumTasks(numTasks);

        getline(finUser, taskLine);

    }

    finUser.close();
}


int main() {
    std::system("clear");
    User *user_list = nullptr;
    string categoryAdd = "";
    cout << "1) For sign up" << endl;
    cout << "2) Log in" << endl;
    int option;
    cin >> option;
    int numberOfUsers = 0;
    string name, password, combined;

    fillUserList(user_list, numberOfUsers);

    switch (option) {

        case 1:
            cout << "Enter your username: ";
            cin.ignore();
            getline(cin, name);
            while (!doesNotContainComma(name)) {
                cout << "Sorry, name can not contain a comma or colon. Enter again: ";
                cin.ignore();
                getline(cin, name);
            }

            while (name.empty()) {
                cout << "Name can not be empty: ";
                cin.ignore();
                getline(cin, name);
            }

            cout << "Enter your password: ";
            getline(cin, password);
            while (!doesNotContainComma(password)) {
                cout << "Sorry, name can not contain a comma or colon. Enter again: ";
                cin.ignore();
                getline(cin, name);
            }

            while (password.empty()) {
                cout << "Password can not be empty, enter again: ";
                cin.ignore();
                getline(cin, name);
            }

            if (signup(password, name)) {
                resizeArray(user_list, numberOfUsers);
                user_list[numberOfUsers - 1].setUsername(name);
                user_list[numberOfUsers - 1].fillBadgesFile();
            }


            return 0;
            break;

        case 2:
            cout << "Enter your username: ";
            cin.ignore();
            getline(cin, name);
            cout << "Enter your password: ";
            getline(cin, password);

            combined = name + ',' + password;
            int userID;

            if (logIn(combined, userID)) { // Find user in users.txt

                std::system("clear");
                cout << "Logged in successfully :) " << endl;
                sleep(1);
                std::system("clear");

                user_list[userID].loadTaskList();
                user_list[userID].loadFromBadgesFile(user_list[userID].getUsername());

                while (1) {
                    std::system("clear");
                    if (user_list[userID].getNumTasks() > 0) {
                        user_list[userID].displayTasks();
                    }


                    cout << "Choose the action u want to perform." << endl << endl;
                    cout << "0) Log out" << endl;
                    cout << "1) Add task" << endl;
                    cout << "2) Delete a task." << endl;
                    cout << "3) Add a category" << endl;
                    cout << "4) Mark as done/undone" << endl;
                    cout << "5) View earned badges and points" << endl;
                    int index;
                    int choice;
                    cin >> choice;
                    switch (choice) {
                        case 0:
                            delete[] user_list;
                            user_list = nullptr;
                            return 0;
                        case 1:
                            user_list[userID].addTask();
                            user_list[userID].loadTaskList();
                            break;
                        case 2:
                            std::system("clear");
                            if (user_list[userID].getNumTasks() > 0) {
                                user_list[userID].displayTasks();
                            }
                            user_list[userID].removeTask();
                            user_list[userID].loadTaskList();
                            break;
                        case 3:

                            cout << "Enter the name of the category you wish to add: ";
                            cin.ignore();
                            getline(cin, categoryAdd, '\n');
                            user_list[userID].addCategory(categoryAdd);
                            user_list[userID].loadCategory(user_list[userID].getUsername());
                            user_list[userID].loadTaskList();
                            break;
                        case 4:
                            std::system("clear");
                            if (user_list[userID].getNumTasks() > 0) {
                                user_list[userID].displayTasks();
                            }
                            cout << "\nEnter id for task to mark done/undone: ";
                            cin >> index;
                            user_list[userID].toggleCompletionTask(index);
                            break;
                        case 5:
                            user_list[userID].displayBadges();
                            break;
                        default:
                            cout << "invalid option" << "\a" << endl;
                    }
                }
                default:
                    cout << "invalid option" << endl;
            } else {
                cout << "Username or password not found.";
            }
    }
    delete[] user_list;
    user_list = nullptr;

}
