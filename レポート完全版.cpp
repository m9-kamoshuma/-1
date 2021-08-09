#include <iostream>
#include <fstream>
#include <sstream>
#include <math.h>
#include <vector>
#include <stdlib.h>
#include <stdio.h>
#include <regex>

using namespace std;

class LoginManager  //ログインに関するクラス●(変更点あり）
{
public:

    LoginManager()
    {
        accessGranted = 0;
    }
    int login(int i) //ログイン操作に関する関数●（変更点：ログインとログアウトを変数を使って区別できるようにした）
    {
        cout << "IDを入力してください. \nID:";
        cin >> userNameAttempt;

        int usrID = checkFile(userNameAttempt, "users.txt");
        if (usrID != 0)
        {
            cout << "パスワードを入力してください.\nパスワード:";
            cin >> passwordAttempt;

            int pwdID = checkFile(passwordAttempt, "pswds.txt");
            if (usrID == pwdID)
            {
                if (i == 1)
                    cout << "ログインしました. \n" << endl;


                if (i == 0)
                    cout << "ログアウトしました. \n" << endl;



            }
            else
            {
                cout << "パスワードが違います.\n" << endl;
                if (i == 1)  login(1);
                if (i == 0) login(0);
            }
        }
        else
        {
            cout << "このIDは登録されていません.\n" << endl;
            if (i == 1)  login(1);
            if (i == 0) login(0);
        }
        if (i == 1)return 1;
        if (i == 0)return 0;
    }

    void addUser()//ユーザー登録をする関数●（変更点：登録するIDとパスワードをプログラム実行後に入力できるようにcin関数を使った）
    {
        string username;
        string password;

        if (checkFile(username, "users.txt") != 0)
        {
            cout << "このIDは利用できません" << endl;
            return;
        }

        int id = 1 + getLastID();
        cout << "使用するIDを打ち込んでください" << endl;
        cin >> username;
        saveFile(username, "users.txt", id);

        cout << "使用するパスワードを打ち込んでください" << endl;
        cin >> password;
        saveFile(password, "pswds.txt", id);
        cout << "登録が完了しました.\n" << endl;
    }

    int getLastID() //現在有るidの最大値を調べる関数●
    {
        fstream file;
        file.open("users.txt", ios::in);
        file.seekg(0, ios::end);

        if (file.tellg() == -1)
            return 0;

        string s;

        for (int i = -1; s.find("#") == string::npos; i--)
        {
            file.seekg(i, ios::end);
            file >> s;
        }

        file.close();
        s.erase(0, 4);

        int id;
        istringstream(s) >> id;

        return id;
    }

    string getusername() //打ち込まれたIDを返す関数☆
    {
        return userNameAttempt;
    }
 
    int getid() //打ち込まれたIDをint型で返す関数☆
    {
        int id = stoi(getusername());
        return id;
    }

    int checkFile(string attempt, const char* p_fileName) //打ち込まれたIDがファイルにあるか確認する関数●
    {
        string line;
        fstream file;

        string currentChar;
        long long eChar;

        file.open(p_fileName, ios::in);

        while (1)
        {
            file >> currentChar;
            if (currentChar.find("#ID:") != string::npos)
            {
                if (attempt == line)
                {
                    file.close();
                    currentChar.erase(0, 4);
                    int id;
                    istringstream(currentChar) >> id;
                    return id;
                }
                else
                {
                    line.erase(line.begin(), line.end());
                }
            }
            else
            {
                istringstream(currentChar) >> eChar;
                line += (char)decrypt(eChar);
                currentChar = "";
            }

            if (file.peek() == EOF)
            {
                file.close();
                return 0;
            }
        }
    }

    void saveFile(string p_line, const char* p_fileName, const int& id)//打ち込まれた文字列を暗号化してファイルに書き込む関数●
    {
        fstream file;
        file.open(p_fileName, ios::app);
        file.seekg(0, ios::end);

        if (file.tellg() != 0)
            file << "\n";

        file.seekg(0, ios::beg);

        for (int i = 0; i < p_line.length(); i++)
        {
            file << encrypt(p_line[i]);
            file << "\n";
        }

        file << "#ID:" << id;
        file.close();
    }

    long long encrypt(int p_letter)
    {
        return powf(p_letter, 5) * 4 - 14;
    }
    int decrypt(long long p_letter)
    {
        return powf((p_letter + 14) / 4, 1 / 5.f);
    }

private:
    string userNameAttempt;
    string passwordAttempt;
    bool accessGranted;
};

class TimeTable : public LoginManager//ログイン、ログアウトにあわせて時間を管理するクラス☆
{
public:

    void timeTable(string when, string time,string who)//退勤表に情報を書き込む元の関数☆
    {
        ofstream outfile(who, ios_base::app);
        int id = getid();
        outfile << id << "," << when << time << endl;
        outfile.close();
    }

    void time(string job)//打ち込んだ文字列を退勤表に書き込む関数☆
    {
        cout << job << "時間を入力してください(例10時7分→10:7)" << endl;
        string time;
        cin >> time;
        timeTable(job, time,"timetable.txt");
        timeTable(job, time, "mytimetable.txt");
    }

    void showTT()//メニューで退勤表を開く操作を示す関数☆
    {
        cout << "どちらを閲覧しますか\n1:全体の退勤表\n2:個人の退勤表\n";
        int Num;
        cin >> Num;
        if (Num == 1)opentimetable("timetable.txt");
        if (Num == 2)
        {
            login(1);
            opentimetable("mytimetable.txt");
        }
          
    }

    void opentimetable(const char* whose)//退勤表を開く関数●(変更点：変数を定義して任意のファイルを開けるようにした）
    {
        int rv;
        const char* cmd = whose;
        if ((rv = system(cmd)) == -1)
        {
            fprintf(stderr, "execute failed %s\n", cmd);
        }
    }

    int getlineNum()//ファイルの行数を数える関数☆
    {
        fstream infile("mytimetable.txt");
        string line;
        int i = 0;
        while (getline(infile, line))
        {
            i++;
        }
        infile.close();
        return i;
    }

    int getjobtime()//勤務時間を計算する関数☆
    {
        vector<int> v(4);
        fstream infile("mytimetable.txt");
       // getlineNum();
        string line;
        int i = 0;
        int l = 0;
        while (getline(infile, line))
        {
            ++l;
            if (l == getlineNum() or l+1 == getlineNum())
            {
                regex rx1(R"((\d+):(\d+))");
                sregex_iterator it1(line.begin(), line.end(), rx1);
                sregex_iterator end;
                while (it1 != end)
                {
                    string str= (it1++)->str();
                    regex rx2(R"(\d+)");
                    sregex_iterator it2(str.begin(), str.end(), rx2);
                    sregex_iterator end;                
                
                    while (it2 != end)
                    {
                        v[i] = stoi(it2++->str());
                        i++;
                        
                    }                                                
                }                 
            }
        }
        infile.close();
        
        int Rh;
        int Rm;
        if (v[2] >= v[0], v[3] >= v[1])
        {
            Rh = v[2] - v[0];
            Rm = v[3] - v[1];
        }

        else
        {
            Rh = v[2] - v[0] + 1;
            Rm = 60 - v[1] + v[3];
        }

        cout <<"本日の勤務時間は" << Rh << "時間" << Rm << "分です。お疲れ様！！" << endl;
        ofstream outfile("mytimetable.txt", ios_base::app);
        outfile << "勤務時間：" << Rh << "時間" << Rm << "分" << endl;
        outfile.close();

        return 0;
        
    }

        int Menu()//画面操作の関数☆
        {
            int Num;
            cout << "メニュー\n1:ログイン\n2:ログアウト\n3:登録\n4:退勤表\n5:終了" << endl;
            cin >> Num;
            if (Num == 1)
            {
                login(1);
                time("出勤");
                Menu();
            }

            if (Num == 2)
            {
                login(0);
                time("退勤");
                getjobtime();
                Menu();
            }

            if (Num == 3)
            {
                addUser();
                Menu();
            }

            if (Num == 4)
            {
                showTT();
                Menu();
            }

            if (Num == 5)
                exit;

            return 0;
        }
};

int main()
{
   TimeTable app;
     //app.Menu();
}