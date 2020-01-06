// made by Gabi

#include <iostream>
#include <string>
#include <fstream>
#include <stdlib.h>
#include <errno.h>
#include <cstring>
#include <direct.h>
#include <windows.h>
#include <tchar.h>
#include <thread>
#include <chrono>
#include <cstdio>

using namespace std;

bool online = false;
int id;

void check_update()
{
    const string version_formatted = "1.0.1.7";
    char update_status;
    char choice;
    cout << "Checking for updates... \n";
    _chdir("wget/bin");
    string cmd = "wget.exe -O update.txt \"http://gabi-api.000webhostapp.com/checkupdate.php?version=" + version_formatted + "\" -q";
    system(cmd.c_str());
    ifstream file("update.txt");
    file >> update_status;
    file.close();
    remove("update.txt");
    if (update_status == '0') {
        cout << "The application is up to date!\n";
        this_thread::sleep_for(chrono::milliseconds(2000));
        system("cls");
    }
    else if (update_status == '1') {
        cout << "Update available!\n";
        cout << "Download update? (y/n): ";
        cin >> choice;
        if (choice == 'y' || choice == 'Y') {
            cout << "Downloading update...\n";
            cmd = "wget.exe \"http://gabi-api.000webhostapp.com/windows-client.zip\"";
            system(cmd.c_str());
            rename("windows-client.zip", "../../windows-client-update.zip");
            remove("windows-client.zip");
            cout << "Update downloaded.\n";
            cout << "You can find the file in the root of the application folder. Extract it and delete the old folder.\n";
            system("pause");
            system("cls");
        }
        system("cls");
    }
    _chdir("../..");
}

void download_quiz()
{
    online = true;
    system("cls");
    cout << "Note: You can find IDs on the site.\n";
    cout << "Enter quiz pack ID to download: ";
    cin >> id;
    _chdir("wget/bin");
    cout << "\nDownloading...\n";
    string cmd = "wget.exe \"http://gabi-api.000webhostapp.com/get_quiz/" + to_string(id) + "/quiz_data.zip\" -q -nc";
    system(cmd.c_str());
    cout << "Extracting...\n";
    _chdir("../../7z");
    cmd = "7za.exe e ../wget/bin/quiz_data.zip -oextracted";
    system(cmd.c_str());
    _chdir("extracted");
    cout << "Done.\n";
    system("pause");
}

void upload_results(int score, int questions, int question_number)
{
    if (questions == question_number) {
        string user;
        string password;
        string status_msg;
        char choice;
        cout << "Enter username: ";
        cin >> user;
        cout << "\nEnter password(if it is the first time choose your password): ";
        cin >> password;
        if (online)
            _chdir("../..");
        else
            id = 0;
        string cmd = "wget.exe -O output.txt \"http://gabi-api.000webhostapp.com/quiz_app_api.php?user=" + user + "&questions=" + to_string(questions) + "&score=" + to_string(score) + "&id=" + to_string(id) + "&pwd=" + password + "\" -q";
        _chdir("wget/bin");
        system(cmd.c_str());
        ifstream file("output.txt");
        getline(file, status_msg);
        file.close();
        cout << status_msg;
        remove("quiz_data.zip");
        remove("output.txt");
        cout << "\nDone.\n";
        cout << "Open quiz results databse? (y/n): ";
        cin >> choice;
        if (choice == 'y' || choice == 'Y')
            system("start https://gabi-api.000webhostapp.com/quiz_app_db.html");
    }
}

string load_question(int question_id, int mode)
{
    string data;
    string data_processed[3]; // 0 = question, 1 = options, 2 = answer
    int data_index = 0;
    string file_name = "quiz_data_" + to_string(question_id) + ".dat";
    ifstream file;
    file.open(file_name);
    if (file.fail())
        cout << "Error opening file! Error code # " << strerror(errno) << "\n";
    else {
        getline(file, data);
        file.close();

        for (unsigned int i = 0; i <= data.length(); i++) {
                if (data[i] != ';')
                    data_processed[data_index] += data[i];
                else
                    data_index++;
        }

        if (mode == 1)
            return data_processed[0];
        else if (mode == 2)
            return data_processed[1];
        else if (mode == 3)
            return data_processed[2];
    }
    return "error";
}

int main()
{
    bool running = true;
    unsigned int question_number = 0, n;
    const string bar = "-----------------------------------------\n";
    char answer;
    string returned_answer;
    const string version = " v1.0.1 build 7";
    const string svfile_name = "quiz_save.dat";
    unsigned int score = 0;
    unsigned int choice;

    SetConsoleTitle(_T("Gabi's Quiz App"));
    check_update();
    cout << "Gabi's quiz program" << version << "\n";
    cout << "This program is a quiz game that loads questions from a file or from the server.\n";
    cout << bar;
    cout << "[1] Download quiz packs online\n";
    cout << "[2] Load quiz packs locally(put the files in the main folder)\n";
    cout << bar;
    cout << "Select an option: ";
    cin >> choice;
    cout << "Enter the number of questions: ";
    cin >> n;

    if (choice == 1)
        download_quiz();

    while (running) {
        system("cls");
        question_number++;
        cout << "Question " << question_number << "\n";
        cout << bar;
        cout << load_question(question_number, 1) << "\n";
        cout << bar;
        cout << load_question(question_number, 2) << "\n";
        cout << bar;
        cout << "Answer (letter only): ";
        cin >> answer;
        returned_answer = load_question(question_number, 3);

		for (unsigned int i = 0; i <= returned_answer.length(); i++) {
        	if (answer == returned_answer[i])
            score++;
		}
        if (question_number >= n)
            running = false;
    }
    system("cls");
    cout << "Your score: " << score << "\n";

    upload_results(score, n, question_number);

    return 0;
}
