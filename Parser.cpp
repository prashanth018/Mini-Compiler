#include<bits/stdc++.h>
using namespace std;

struct node                             //DS to hold the content of the string and type of token.
{
    string val;
    string type;
};

map<string,int> c;                      //Used to construct table,to locate the column number.
map<int,pair<string,string> > rule;     //A DS to store rules.
map<string,int> r;                      //Used to construct table,to locate the row number.

void PrintStack(stack<string> st)       //For Printing the Stack Content.
{
    string stri;
    stack<string> st2;
    while(!st.empty())
    {
        st2.push(st.top());
        st.pop();
    }
    while(!st2.empty())
    {
        cout<<st2.top()<<" ";
        st2.pop();
    }
    cout<<endl;
    return;
}

int main()
{
    int i,j,k;
    ifstream infile;
    string line;
    infile.open ("lexer_output2.txt");          //Takes code as an input.
    queue<node> q;
    while(getline(infile, line))
    {
        istringstream iss(line);
        int a;
        string v,t;
        if (!(iss >> a >> v >> t)) { break; }
        else
        {
            node n;
            n.val = v;
            n.type = t;
            q.push(n);
        }
    }
    int m = 54;
    int n = 28;

    string data[m][n];                  //A DS to hold the Parse table.

    ifstream file("parse_table.csv");   //csv files contains the Parse table.


    for(int row = 0; row < m; ++row)    //Populates the "data" DS with parse table.
    {
        string line;
        getline(file, line);
        if ( !file.good() )
            {cout<<"Parse Table Error"<<endl;break;}
        stringstream iss(line);
        for (int col = 0; col < n; ++col)
        {
            string val;
            getline(iss, val, ',');
            if ( !iss.good() )
                break;
            stringstream convertor(val);
            convertor >> data[row][col];
            if(row==0 && col!=0)
            {
                c[data[row][col]]=col;
            }
            else if(col==0 && row!=0)
            {
                r[data[row][col]]=row;
            }
        }
    }
    map<string,int>::iterator it;

    m = 18;
    n = 2;

    ifstream file1("grammar.csv");      //csv file contains the rules.
    int l=0;
    for(int row = 0; row < m; ++row)    //populating the "rule" DS with grammar
    {
        string line;
        getline(file1, line);
        if ( !file1.good() )
            {cout<<"Grammar File Error"<<endl;break;}

        stringstream iss(line);
        string x;
        for (int col = 0; col < n; col++)
        {
            string val;
            getline(iss, val, ',');
            if ( !iss.good() )
                break;

            stringstream convertor(val);
            string p;
            convertor >> p;
            if(col==0)
            {
                x = p;
            }
            else if(col==1)
            {
                rule[l] = make_pair(x,p);
                l++;
            }
        }
    }
    map<int,pair<string,string> >::iterator it1;
    node u;
    string stri;
    queue<node> q2;
    while(!q.empty())               //Converting the input into a form valid for parsing
    {
        u = q.front();
        q.pop();
        if(u.type == "Keyword" || u.type == "Delimiter")
        {
            node temp;
            temp = u;
            temp.type = temp.val;
            q2.push(temp);
        }
        else if(u.type == "Operator")
        {
            node temp;
            temp.val = u.val;
            temp.type = "o";
            q2.push(temp);
        }
        else if(u.type == "Identifier")
        {
            node temp;
            temp.val = u.val;
            temp.type = "v";
            q2.push(temp);
        }
        else if(u.type == "Integer_Literal")
        {
            node temp;
            temp.val = u.val;
            temp.type = "i";
            q2.push(temp);
        }
        else if(u.type == "Floating_Point_Literal")
        {
            node temp;
            temp.val = u.val;
            temp.type = "i";
            q2.push(temp);
        }
    }
    node e;
    e.val = "$";
    e.type = "$";
    q2.push(e);
    stri="";
    while(!q2.empty())
    {
        node x;
        x = q2.front();
        q2.pop();
        stri+=(x.type);
    }
    cout<<stri<<endl;

    stack<string> st;
    i=0;
    st.push("0");
    string err;             //string which holds the error at each stage
    err="";
    string total;
    queue<string> fina;
    while(1)
    {
        PrintStack(st);     //printing stack content after every iteration.
        string xs = st.top();
        if(xs[0]>='0'&&xs[0]<='9')
        {
            string str;
            str+=stri[i];
            string el = data[r[xs]][c[ str ] ];
            if(el=="")
            {
                err+=" [[";
                err+=str;
                i++;
            }
            else if(el[0]=='s')             //shift
            {
                if(err!="")
                {
                    err+="]] ";
                    total+=err;
                    fina.push(err);
                    err="";
                }
                total+=stri[i];
                string op="";
                int j=1;
                for(;j<el.length();j++)
                    op+=el[j];
                st.push(str);
                i++;
                st.push(op);
            }
            else if(el[0]=='r')            //reduce
            {
                if(err!="")
                {
                    err+="]] ";
                    total+=err;
                    fina.push(err);
                    err="";
                }
                string op="";
                int j=1;
                for(;j<el.length();j++)
                    op+=el[j];
                string k = rule[atoi(op.c_str())].second;
                int len = k.length();
                len*=2;
                while(len--)                   //Popping the top 2*n elements of the stack content(As the algorithm says).
                    st.pop();
                st.push(rule[atoi(op.c_str())].first);
            }
        }
        else if(xs[0]>='A' && xs[0]<='Z')      //Goto condition
        {
            st.pop();
            string h = st.top();
            st.push(xs);
            st.push(data[r[h]][c[xs]]);
        }
        string strin;
        strin+=stri[i];
        if(data[r[st.top()]][c[strin]]=="acc") //Condition For accepting the input string.
        {
            cout<<"\nSuccessfully Parsed!!\n"<<endl;
            break;
        }
    }
    int flag=0;                                //keeps note if error has occurred.
    cout<<"Parsed String: Error tokens are enclosed in [[ ]] :"<<endl<<endl;
    for(i=0;i<total.length();i++)              //Detection of errors and printing, If no errors are found it just prints the string without error.
    {
        if(total[i]=='[')
            flag=1;
        else if(total[i]==']')
            flag=0;
        if(total[i]=='v' && flag==0)
            cout<<" variable"<<" ";
        else if(total[i]=='v' && flag==1)
            cout<<"[[variable]]";
        else if(total[i]=='o' && flag==0)
            cout<<" operator"<<" ";
        else if(total[i]=='o' && flag==1)
            cout<<"[[operator]]";
        else if(total[i]=='(')
        {
            cout<<"(";
            while(total[i]!=')')
            {
                i++;
                if(total[i]=='[')
                    flag=1;
                else if(total[i]==']')
                    flag=0;

                if(total[i]=='v' && flag==0)
                    cout<<" variable"<<" ";
                else if(total[i]=='v' && flag==1)
                    cout<<"[[variable]]";
                else if(total[i]=='o' && flag==0)
                    cout<<" operator"<<" ";
                else if(total[i]=='o' && flag==1)
                    cout<<"[[operator]]";
                else if(total[i]=='i' && flag==0)
                    cout<<" integer"<<" ";
                else if(total[i]=='i' && flag==1)
                    cout<<"[[integer]]";
            }
            cout<<")";
        }
        else
            cout<<total[i];
    }
    cout<<endl<<endl;
    return 0;
}
