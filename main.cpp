#include <bits/stdc++.h>
#include <windows.h>
using namespace std;
#define PHONE_SIZE 5


void SetColor(int value){
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),  value);
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

    set<string> autocomplete(string word){
        set<string> l;
        findwords(root,word, l);
        return l;
    }
};

class PhoneDirectory{
    Trie t;
    unordered_map <string, string> m;

public:
    bool Insert(string name, string phone){
        if(!t.Search(phone)){
            m.insert(make_pair(phone, name));
            t.Insert(phone);
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
    cout << "Enter f to find\n";
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
        }else if(c == 'f' || c == 'F'){
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
        }
    }
    return 0;
}
