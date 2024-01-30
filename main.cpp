#include <iostream>
#include <map>  
#include <set>
#include <unordered_map>
#ifdef _WIN32
#include <windows.h>
#endif
using namespace std;
#define PHONE_SIZE 5
#define ANSI_RESET "\033[0m"
#define ANSI_COLOR_RED "\033[31m"
#define ANSI_COLOR_GREEN "\033[32m"
#define ANSI_COLOR_YELLOW "\033[33m"
#define ANSI_COLOR_BLUE "\033[34m"
#define ANSI_COLOR_MAGENTA "\033[35m"
#define ANSI_COLOR_CYAN "\033[36m"
#define ANSI_COLOR_WHITE "\033[37m"


void SetColor(int value){
    // 2, 6, 7, 8, 10, 12, 15
    #ifdef _WIN32
        // Code that works on Windows operating system
        SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),  value);
    #else
        // Code that works on Mac/Unix operating system
        switch (value) {
            case 2:
                std::cout << ANSI_COLOR_RED;
                break;
            case 6:
                std::cout << ANSI_COLOR_GREEN;
                break;
            case 7:
                std::cout << ANSI_COLOR_MAGENTA;
                break;
            case 8:
                std::cout << ANSI_COLOR_CYAN;
                break;
            case 10:
                std::cout << ANSI_COLOR_YELLOW;
                break;
            case 12:
                std::cout << ANSI_COLOR_BLUE;
                break;
            case 15:
                std::cout << ANSI_COLOR_WHITE;
                break;
            default:
                // Reset to default color if an invalid code is provided
                std::cout << ANSI_RESET;
                break;
        }   
    #endif
}


class Trie{
private:
    struct TrieNode{
        map <char, TrieNode*> children;
        bool endOfWord;
    };
    typedef TrieNode* TN;
    TN root;
public:
    Trie(){
        root = new TrieNode();
        root->endOfWord = false;
    }

    void Insert(string word){
        TN current = root;
        for(int i=0;i<word.length();i++){
            char c = word[i];
            TN node = current->children[c];
            if(node == nullptr){
                node = new TrieNode();
                current->children[c] = node;
            }
            current = node;
        }
        current->endOfWord = true;
    }

    bool Search(string word){
        TN current = root;
        for(int i=0;i<word.length();i++){
            char ch = word[i];
            TN node = current->children[ch];
            if(node == nullptr){
                return false;
            }
            current = node;
        }
        return current->endOfWord;
    }

    void Delete(string word){
        deleteUtil(root, word, 0);
    }

    bool deleteUtil(TN current, string word, int index){
        if(index == word.length()){
            if(!current->endOfWord){
                return false;
            }
            current->endOfWord = false;
            return (current->children.size() == 0);
        }
        char ch = word[index];
        TN node = current->children[ch];
        if(node == nullptr){
            return false;
        }
        bool shouldDeleteCurrentNode = deleteUtil(node, word, index+1);

        if(shouldDeleteCurrentNode){
            current->children.erase(ch);
            //current->children.erase(current->children.find(ch));
            return (current->children.size() == 0);
        }
        return false;
    }

    // auto complete

    void findwordUtil(TN curr, string temp, set<string> &l){
        if(curr->endOfWord){
            l.insert(temp);
        }

        for(int i=0;i<10;i++){
            char c = i+'0';
            if(curr->children[c] != nullptr){
                //cout << "test : " << temp + c << endl;
                findwordUtil(curr->children[c], temp + c, l);
            }
        }
    }

    void findwordUtilName(TN curr, string temp, set<string> &l){
        if(curr->endOfWord){
            l.insert(temp);
        }

        for(int i=0;i<26;i++){
            char c = i+'a';
            if(curr->children[c] != nullptr){
                //cout << "test : " << temp + c << endl;
                findwordUtilName(curr->children[c], temp + c, l);
            }
        }
    }


    void findwords(TN curr, string str, set<string> &l){
        string temp = "";
        for(int i=0;i<str.length();i++){
            char c = str[i];
            temp += c;
            if(curr->children[c] == nullptr){
                return;
            }
            curr = curr->children[c];
        }
        //cout << temp << endl;
        findwordUtil(curr, temp, l);

        for(auto w : l){
            w = temp + w;
        }
    }

    void findwordsName(TN curr, string str, set<string> &l){
        string temp = "";
        for(int i=0;i<str.length();i++){
            char c = str[i];
            temp += c;
            if(curr->children[c] == nullptr){
                return;
            }
            curr = curr->children[c];
        }
        //cout << temp << endl;
        findwordUtilName(curr, temp, l);

        for(auto w : l){
            w = temp + w;
        }
    }



    set<string> autocomplete(string word){
        set<string> l;
        findwords(root,word, l);
        return l;
    }

    set<string> autocompleteName(string word){
        set<string> l;
        findwordsName(root,word,l);
        return l;
    }
};

class PhoneDirectory{
    Trie t,t_name;
    unordered_map <string, string> m;
    unordered_map <string, string> m_inverse;
public:
    bool Insert(string name, string phone){
        if(!t.Search(phone)){
            m.insert(make_pair(phone, name));
            m_inverse.insert(make_pair(name, phone));
            t.Insert(phone);
            t_name.Insert(name);
            return true;
        }
        return false;
    }

    bool isAvail(string phone){
        return t.Search(phone);
    }

    void aCompletePrint(string ph){
        set<string> l;
        l = t.autocomplete(ph);

        for(auto ph : l){
            cout << ph << endl;
        }
    }

    void aCompletePrintName(string name){
        set<string> n;
        n = t.autocompleteName(name);

        for(auto nam : n){
            cout << nam << endl;
        }
    }

    void del(string ph){
        if(t.Search(ph)){
            t.Delete(ph);
            SetColor(6);
            cout << "Number has been removed from phone directory\n";
            SetColor(7);
        }else{
            SetColor(6);
            cout << "Number not found in the phone directory\n";
            SetColor(7);
        }
    }

    bool findPrint(string ph){
        set<string> l = t.autocomplete(ph);
        bool found = false;

        for(auto pNo : l){
            if(m.find(pNo) != m.end()){
                cout << pNo << " : " << m[pNo] << endl;
                found = true;
            }
        }
        return found;
    }

    bool findPrintName(string name){
        set<string> l = t_name.autocompleteName(name);
        bool found = false;

        for(auto nam : l){
            //cout << nam << endl;
            if(m_inverse.find(nam) != m_inverse.end()){
                cout << nam << " : " << m_inverse[nam] << endl;
                found = true;
            }
        }
        return found;
    }

};

bool isValidPhone(string p){
    if(p.size() == PHONE_SIZE){
        for(int i=0;i<p.size();i++){
            if(p[i] >= '0' && p[i] <= '9'){
                continue;
            }else{
                return false;
            }
        }
        return true;
    }else{
        return false;
    }
}

int main()
{
    PhoneDirectory pd;

    SetColor(10);
    cout << "Welcome to Phone Directory 1.0\n";
    cout << "Press escape/e key anytime to exit the program\n\n";
    SetColor(8);


    cout << "Enter i to insert \n";
    cout << "Enter d to delete \n";
    cout << "Enter f to find by Phone\n";
    cout << "Enter F to find by Name\n";
    cout << "\n";
    SetColor(7);

    while(1){
        cout << endl;
        char c;
        SetColor(4);
        cout << "Enter input : ";
        SetColor(12);
        cin >> c;
        SetColor(7);
        if(c == 'e' || c == 'E'){
            break;
        }
        if(c == 'i' || c == 'I'){
            string n, p;
            cout << "Enter name : " << flush;
            SetColor(15);
            cin.ignore();
            getline(cin, n, '\n');
            SetColor(7);
            while(1){
                while(1){
                    cout << "Enter " << PHONE_SIZE << " digit phone number : ";
                    SetColor(15);
                    cin >> p;
                    SetColor(7);
                    if(!isValidPhone(p)){
                        SetColor(2);
                        cout << "Enter a valid phone number \n";
                        SetColor(7);
                    }else{
                        break;
                    }
                }
                if(pd.Insert(n, p)){
                    SetColor(6);
                    cout << "Number added to the phone directory\n";
                    SetColor(7);
                    break;
                }else{
                    SetColor(2);
                    cout << "Number already exists in phone directory\n";
                    SetColor(7);
                }
            }
        }else if(c == 'f'){
            string f;
            cout << "Enter the number to find : ";
            SetColor(15);
            cin >> f;
            if(!pd.findPrint(f)){
                SetColor(2);
                cout << "No Matching number\n";
                SetColor(7);
            }
            SetColor(7);
        }else if(c == 'd' || c == 'D'){
            string d;
            cout << "Enter a number to delete : ";
            SetColor(15);
            cin >> d;
            SetColor(7);
            pd.del(d);
        }else if(c == 'F'){
            string f;
            cout << "Enter the name to find : ";
            SetColor(15);
            cin >> f;
            if(!pd.findPrintName(f)){
                SetColor(2);
                cout << "No Matching name\n";
                SetColor(7);
            }
            SetColor(7);
        }
    }
    return 0;
}
