#include <iostream>
#include <vector>
#include <string>
#include <map>
#include <memory>
#include <fstream>
#include <sstream>
#include <filesystem>

using namespace std;
class Employee {
protected:
    string name;
    int id;

public:
    Employee(string name, int id) : name(name), id(id) {}

    virtual float calculateSalary() = 0; // ���麯�������ڼ��㹤��
    virtual void displayInfo() = 0; // ��ʾԱ����Ϣ
    string getName() const { return name; }
    int getId() const { return id; }
};

class Teacher : public Employee {
private:
    map<string, int> courses; // �γ����ƺͿ�ʱ��

public:
    // �޸ĺ�Ĺ��캯��
    Teacher(string name, int id, const map<string, int>& courses)
        : Employee(name, id), courses(courses) {}

    void addCourse(string courseName, int hours) {
        courses[courseName] = hours;
    }

    float calculateSalary() override {
        float salary = 0;
        for (auto& course : courses) {
            salary += course.second * 1000; // ÿ�ſ�1000Ԫ
        }
        return salary;
    }

    const map<string, int>& getCourses() const {
        return courses;
    }
    void displayInfo() override {
        cout << "Teacher: " << name << ", ID: " << id << ", Courses: ";
        for (auto& course : courses) {
            cout << course.first << " (" << course.second << " hours), ";
        }
        cout << endl;
    }
};

class Administrator : public Employee {
private:
    string position; // ְ��

public:
    Administrator(string name, int id, string position) : Employee(name, id), position(position) {}

    float calculateSalary() override {
        return 5000; // �̶�����
    }
    string getPosition() const {
        return position;
    }
    void displayInfo() override {
        cout << "Administrator: " << name << ", ID: " << id << ", Position: " << position << endl;
    }
};

class Logistics : public Employee {
private:
    int workingDays; // ��������

public:
    Logistics(string name, int id, int days) : Employee(name, id), workingDays(days) {}

    void setWorkingDays(int days) {
        workingDays = days;
    }

    float calculateSalary() override {
        return workingDays * 100; // ÿ��������100Ԫ
    }

    void displayInfo() override {
        cout << "Logistics: " << name << ", ID: " << id << ", Working Days: " << workingDays << ", Salary: " << calculateSalary() << endl;
    }
    int getWorkingDays() const {
        return workingDays;
    }
};


class StaffManagementSystem {
private:
    vector<shared_ptr<Employee>> employees;

public:
    void addTeacher(const string& name, int id, const map<string, int>& courses) {
        employees.push_back(make_shared<Teacher>(name, id, courses));
    }

    void addAdministrator(const string& name, int id, const string& position) {
        employees.push_back(make_shared<Administrator>(name, id, position));
    }

    void addLogistics(const string& name, int id, int workingDays) {
        employees.push_back(make_shared<Logistics>(name, id, workingDays));
    }

    void displayAllInfo() {
        for (const auto& emp : employees) {
            emp->displayInfo();
        }
    }

    void calculateAllSalaries() {
        for (auto& emp : employees) {
            float salary = emp->calculateSalary();
            cout << "Ա�� " << emp->getName() << " (ID: " << emp->getId() << ") �Ĺ���: " << salary << endl;
        }
    }
    void saveToFile() {
        string filename = "employ.csv";
        ofstream outFile(filename);
        if (!outFile) {
            cerr << "�޷��򿪻򴴽��ļ� " << filename << endl;
            return;
        }
        for (const auto& emp : employees) {
            // ��д�����ͣ�Ȼ����ID������
            if (auto t = dynamic_pointer_cast<Teacher>(emp)) {
                outFile << "Teacher," << emp->getId() << "," << emp->getName() << ",";
                // д��γ���Ϣ
                for (const auto& course : t->getCourses()) {
                    outFile << course.first << ":" << course.second << ";";
                }
            }
            else if (auto a = dynamic_pointer_cast<Administrator>(emp)) {
                outFile << "Administrator," << emp->getId() << "," << emp->getName() << ",";
                // д��ְ����Ϣ
                outFile << a->getPosition();
            }
            else if (auto l = dynamic_pointer_cast<Logistics>(emp)) {
                outFile << "Logistics," << emp->getId() << "," << emp->getName() << ",";
                // д�빤������
                outFile << l->getWorkingDays();
            }
            outFile << endl;
        }

        outFile.close();
    }

    void loadFromFile() {
        string filename = "employ.csv";
        ifstream inFile(filename);

        // ����ļ��Ƿ�ɹ���
        if (!inFile) {
            cout << "�ļ� " << filename << " �����ڣ�������һ�����ļ���" << endl;
            return;
        }

        string line, type, name, position, courseName, temp;
        int id, hours, workingDays;
        map<string, int> courses;

        while (getline(inFile, line)) {
            stringstream ss(line);
            getline(ss, type, ',');
            ss >> id;
            ss.ignore(); // ���Զ���
            getline(ss, name, ',');

            if (type == "Teacher") {
                courses.clear();
                getline(ss, temp, ','); // ��ȡ�γ���Ϣ����
                stringstream courseStream(temp);
                while (getline(courseStream, courseName, ':')) {
                    courseStream >> hours;
                    courseStream.ignore(); // ���Էֺ�
                    courses[courseName] = hours;
                    if (courseStream.peek() == ';') courseStream.ignore();
                }
                addTeacher(name, id, courses);
            }
            else if (type == "Administrator") {
                getline(ss, position);
                addAdministrator(name, id, position);
            }
            else if (type == "Logistics") {
                ss >> workingDays;
                addLogistics(name, id, workingDays);
            }
        }

        inFile.close();
    }

};

int main() {
    StaffManagementSystem sms;
    int choice;
    sms.loadFromFile();
    do {
        cout << "\n��ְ������ϵͳ" << endl;
        cout << "1. ��ӽ�ʦ" << endl;
        cout << "2. ��ӹ�����Ա" << endl;
        cout << "3. ��Ӻ�����Ա" << endl;
        cout << "4. ��ʾ����Ա����Ϣ" << endl;
        cout << "5. ���㲢��ʾ����Ա������" << endl;
        cout << "0. �˳�" << endl;
        cout << "����������ѡ��: ";
        cin >> choice;

        string name, position, courseName;
        int id, workingDays, hours;
        map<string, int> courses;

        switch (choice) {
        case 1: // ��ӽ�ʦ
            cout << "�����ʦ����: ";
            cin >> name;
            cout << "�����ʦ����: ";
            cin >> id;
            cout << "�����ڿ���Ϣ���γ��� ��ʱ���������� 'done' ����: ";
            while (true) {
                cin >> courseName;
                if (courseName == "done") break;
                cin >> hours;
                courses[courseName] = hours;
            }
            sms.addTeacher(name, id, courses);
            break;
        case 2: // ��ӹ�����Ա
            cout << "���������Ա����: ";
            cin >> name;
            cout << "���������Ա����: ";
            cin >> id;
            cout << "���������Աְ��: ";
            cin >> position;
            sms.addAdministrator(name, id, position);
            break;
        case 3: // ��Ӻ�����Ա
            cout << "���������Ա����: ";
            cin >> name;
            cout << "���������Ա����: ";
            cin >> id;
            cout << "���빤������: ";
            cin >> workingDays;
            sms.addLogistics(name, id, workingDays);
            break;
        case 4: // ��ʾ����Ա����Ϣ
            sms.displayAllInfo();
            break;
        case 5: // ���㲢��ʾ����Ա������
            sms.calculateAllSalaries();
            sms.displayAllInfo();
            break;
        case 0: // �˳�
            cout << "�˳�ϵͳ��" << endl;
            sms.saveToFile();
            break;
        default:
            cout << "��Чѡ�����������롣" << endl;
        }
    } while (choice != 0);

    return 0;
}