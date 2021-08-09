#include <iostream>
#include <fstream>
#include <sstream>
#include <math.h>
#include <vector>
#include <stdlib.h>
#include <stdio.h>
#include <regex>

using namespace std;

class LoginManager  //���O�C���Ɋւ���N���X��(�ύX�_����j
{
public:

    LoginManager()
    {
        accessGranted = 0;
    }
    int login(int i) //���O�C������Ɋւ���֐����i�ύX�_�F���O�C���ƃ��O�A�E�g��ϐ����g���ċ�ʂł���悤�ɂ����j
    {
        cout << "ID����͂��Ă�������. \nID:";
        cin >> userNameAttempt;

        int usrID = checkFile(userNameAttempt, "users.txt");
        if (usrID != 0)
        {
            cout << "�p�X���[�h����͂��Ă�������.\n�p�X���[�h:";
            cin >> passwordAttempt;

            int pwdID = checkFile(passwordAttempt, "pswds.txt");
            if (usrID == pwdID)
            {
                if (i == 1)
                    cout << "���O�C�����܂���. \n" << endl;


                if (i == 0)
                    cout << "���O�A�E�g���܂���. \n" << endl;



            }
            else
            {
                cout << "�p�X���[�h���Ⴂ�܂�.\n" << endl;
                if (i == 1)  login(1);
                if (i == 0) login(0);
            }
        }
        else
        {
            cout << "����ID�͓o�^����Ă��܂���.\n" << endl;
            if (i == 1)  login(1);
            if (i == 0) login(0);
        }
        if (i == 1)return 1;
        if (i == 0)return 0;
    }

    void addUser()//���[�U�[�o�^������֐����i�ύX�_�F�o�^����ID�ƃp�X���[�h���v���O�������s��ɓ��͂ł���悤��cin�֐����g�����j
    {
        string username;
        string password;

        if (checkFile(username, "users.txt") != 0)
        {
            cout << "����ID�͗��p�ł��܂���" << endl;
            return;
        }

        int id = 1 + getLastID();
        cout << "�g�p����ID��ł�����ł�������" << endl;
        cin >> username;
        saveFile(username, "users.txt", id);

        cout << "�g�p����p�X���[�h��ł�����ł�������" << endl;
        cin >> password;
        saveFile(password, "pswds.txt", id);
        cout << "�o�^���������܂���.\n" << endl;
    }

    int getLastID() //���ݗL��id�̍ő�l�𒲂ׂ�֐���
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

    string getusername() //�ł����܂ꂽID��Ԃ��֐���
    {
        return userNameAttempt;
    }
 
    int getid() //�ł����܂ꂽID��int�^�ŕԂ��֐���
    {
        int id = stoi(getusername());
        return id;
    }

    int checkFile(string attempt, const char* p_fileName) //�ł����܂ꂽID���t�@�C���ɂ��邩�m�F����֐���
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

    void saveFile(string p_line, const char* p_fileName, const int& id)//�ł����܂ꂽ��������Í������ăt�@�C���ɏ������ފ֐���
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

class TimeTable : public LoginManager//���O�C���A���O�A�E�g�ɂ��킹�Ď��Ԃ��Ǘ�����N���X��
{
public:

    void timeTable(string when, string time,string who)//�ދΕ\�ɏ����������ތ��̊֐���
    {
        ofstream outfile(who, ios_base::app);
        int id = getid();
        outfile << id << "," << when << time << endl;
        outfile.close();
    }

    void time(string job)//�ł����񂾕������ދΕ\�ɏ������ފ֐���
    {
        cout << job << "���Ԃ���͂��Ă�������(��10��7����10:7)" << endl;
        string time;
        cin >> time;
        timeTable(job, time,"timetable.txt");
        timeTable(job, time, "mytimetable.txt");
    }

    void showTT()//���j���[�őދΕ\���J������������֐���
    {
        cout << "�ǂ�����{�����܂���\n1:�S�̂̑ދΕ\\n2:�l�̑ދΕ\\n";
        int Num;
        cin >> Num;
        if (Num == 1)opentimetable("timetable.txt");
        if (Num == 2)
        {
            login(1);
            opentimetable("mytimetable.txt");
        }
          
    }

    void opentimetable(const char* whose)//�ދΕ\���J���֐���(�ύX�_�F�ϐ����`���ĔC�ӂ̃t�@�C�����J����悤�ɂ����j
    {
        int rv;
        const char* cmd = whose;
        if ((rv = system(cmd)) == -1)
        {
            fprintf(stderr, "execute failed %s\n", cmd);
        }
    }

    int getlineNum()//�t�@�C���̍s���𐔂���֐���
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

    int getjobtime()//�Ζ����Ԃ��v�Z����֐���
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

        cout <<"�{���̋Ζ����Ԃ�" << Rh << "����" << Rm << "���ł��B�����l�I�I" << endl;
        ofstream outfile("mytimetable.txt", ios_base::app);
        outfile << "�Ζ����ԁF" << Rh << "����" << Rm << "��" << endl;
        outfile.close();

        return 0;
        
    }

        int Menu()//��ʑ���̊֐���
        {
            int Num;
            cout << "���j���[\n1:���O�C��\n2:���O�A�E�g\n3:�o�^\n4:�ދΕ\\n5:�I��" << endl;
            cin >> Num;
            if (Num == 1)
            {
                login(1);
                time("�o��");
                Menu();
            }

            if (Num == 2)
            {
                login(0);
                time("�ދ�");
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