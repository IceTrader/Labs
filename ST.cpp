#include <iostream>
#include<string>
#include<vector>
#include<fstream>

using namespace std;

class Config
{
private:
    string LOGIN, PASSWORD;
    short int IP [4];
    static Config* PTR;
    bool parse(string boof)
    {
        try
        {
            this->IP[0] = stoi(boof.substr(0, boof.find_first_of('.')));
            boof = boof.substr(boof.find_first_of('.') + 1, boof.length());
            this->IP[1] = stoi(boof.substr(0, boof.find_first_of('.')));
            boof = boof.substr(boof.find_first_of('.') + 1, boof.length());
            this->IP[2] = stoi(boof.substr(0, boof.find_first_of('.')));
            boof = boof.substr(boof.find_first_of('.') + 1, boof.length());
            this->IP[3] = stoi(boof);
        }
        catch (exception e)
        {
            cout << "WRONG IP\n";
            return false;
        }
        return true;
    }
    void print(ostream& out)
    {
        out << IP[0] << '.' << IP[1] << '.' << IP[2] << '.' << IP[3] << '\n'
            << LOGIN << '\n' << PASSWORD << '\n';
    }
public:
    static Config* get_instance()//+
    {
        if(PTR==nullptr)
        {
            PTR = new Config();
            PTR->LOGIN = "";
            PTR->PASSWORD = "";
            for (int i = 0; i < 4; i++)
                PTR->IP[i] = 0;
        }
        return PTR;
    }
    void read()
    {
        string boof;
        fstream in("CONFIG.txt", ios::in);
        if (in.eof())
        {
            cout << "NOTHING TO READ\n";
            return;
        }
        getline(in, boof);
        if (!parse(boof))
            return;
        getline(in, boof);
        PTR->LOGIN = boof;
        getline(in, boof);
        PTR->PASSWORD = boof;
        in.close();
        print(cout);
    }
    void update()//+
    {
        bool flag = false;
        while (!flag)
        {
            string boof;
            cout << "INPUT IP\n";
            cin >> boof;
            flag = parse(boof);
            if (!flag)
                continue;
            cout << "INPUT LOGIN\n";
            cin >> boof;
            PTR->LOGIN = boof;
            cout << "INPUT PASSWORD\n";
            cin >> boof;
            PTR->PASSWORD = boof;
        }
        fstream out("CONFIG.txt", ios::out);
        print(out);
        out.close();
    }
};

 Config * Config::PTR = nullptr;

int main()
{
    auto a = Config::get_instance();
    a->update();
    a->read();
    return 0;
}
