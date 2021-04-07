#include<stdio.h>
#include<string.h>
#include<stdlib.h>
char oper[12]={'+','-','*','/','=','%','>','<','?','!','&','|'};
char delim[9]={'}','{',';','(',')','[',']',',',':'};
char *keyword[]={"with","std.io","gnu.io","array","character","string","float","if","put","get","return","elif","else","integer","while","of","boolean","true","false","function"}; //16
char *tokenlist[] = {"Keyword","Operator","Delimiter","Identifier","Integer_Literal","Floating_Point_Literal","String_Literal","Section_Separator"};   //7
struct node
{
    int line_num;
    char str[30];
    char type[30];
};
typedef struct node node;
int identifier(char c);
int isdelim(char c);
int isop(char c);
int main()
    {

    FILE *fp;
    char str[256];
    fp = fopen("input_program.txt" , "r");
    if(fp == NULL)
    {
        perror("Error opening file");
        return(-1);
    }
    int ct=0;
    int it=0;
    node a[100];
    int f=0;
    while( fgets (str,sizeof(str), fp))
    {
        ct++;
        int i,j,k;
        i=j=k=0;
        while(i<strlen(str))
        {
            if(f==1)
            {
                int d=0;
                while(str[i]!='*'||str[i+1]!='/')          //To recognize end of comment
                    {
                        i++;
                        if(str[i+1]=='\n'||str[i+1]==EOF)
                        {
                            d=1;
                            break;
                        }
                    }
                if(str[i]=='*'&&str[i+1]=='/')
                    { i+=2;f=0;}
            }
            else if(str[i]=='@' && str[i+1]=='@')               //To recognize section separator
            {
                j=0;
                a[it].str[j] = str[i];
                a[it].str[j+1] = str[i+1];
                a[it].str[j+2]=0;
                a[it].line_num = ct;
                strcpy(a[it].type,tokenlist[7]);
                it++;
                i+=2;
            }
            else if(str[i]=='@')                                //To give an error when a single @ found
            {
                strcpy(a[it].type,"Error");
                strcpy(a[it].str,"@");
                a[it].line_num = ct;
                it++;
                i++;
            }
            else if(str[i]=='-'&&str[i+1]=='-')                //Single line comment
            {
                j=i;
                while(str[j]!='\n')
                    {j++; /*printf("hello")*/;}
                i=j+1;
            }
            else if(str[i]=='/' && str[i+1]=='*')              //To recognize start of multiLine comment
            {
                j=i;
                j+=2;
                i=j;
                f=1;
            }
            else if(identifier(str[i]) && !isdigit(str[i]))    //To recognize identifiers
            {
                j=i;
                k=0;
                while(identifier(str[j]))
                {
                    a[it].str[k] = str[j];
                    j++;
                    k++;
                }
                a[it].str[k]=0;
                a[it].line_num = ct;
                strcpy(a[it].type,tokenlist[3]);
                it++;
                i=j;
            }
            else if(str[i]=='"')                //To recognize String literals
            {
                k=0;
                j=i;
                a[it].str[k] = str[j];
                k++;
                j++;
                while(str[j]!='"')
                {
                    a[it].str[k]=str[j];
                    k++;
                    j++;
                }
                a[it].str[k]='"';
                k++;
                a[it].str[k]=0;
                a[it].line_num = ct;
                strcpy(a[it].type,tokenlist[6]);
                it++;
                i=j+1;
            }
            else if(isop(str[i]))            //To recognize Operators
            {
                j=i;
                if(str[j]=='&'&&str[j+1]=='&')
                {
                    a[it].str[0] = str[j];
                    a[it].str[1] = str[j+1];
                    a[it].str[2] = 0;
                    a[it].line_num = ct;
                    strcpy(a[it].type,tokenlist[1]);
                    i+=2;
                }
                else if(str[j]=='|'&&str[j+1]=='|')
                {
                    a[it].str[0] = str[j];
                    a[it].str[1] = str[j+1];
                    a[it].str[2] = 0;
                    i+=2;
                    a[it].line_num = ct;
                    strcpy(a[it].type,tokenlist[1]);
                }
                else if(str[j]=='>'||str[j]=='<'||str[j]=='='||str[j]=='!')
                {
                    if(str[j+1]=='=')
                    {
                        a[it].str[0]=str[j];
                        a[it].str[1]=str[j+1];
                        a[it].str[2]=0;
                        i=j+2;
                    }
                    else
                    {
                        a[it].str[0]=str[j];
                        a[it].str[1]=0;
                        i=j+1;
                    }
                    a[it].line_num = ct;
                    strcpy(a[it].type,tokenlist[1]);
                }
                else if(str[j]=='+'||str[j]=='-'||str[j]=='*'||str[j]=='/'||str[j]=='%'||str[j]=='?'||str[j]=='!'||str[j]==':')
                {
                    a[it].str[0] = str[j];
                    a[it].str[1]=0;
                    a[it].line_num = ct;
                    strcpy(a[it].type,tokenlist[1]);
                    i=j+1;
                }
                it++;
            }
            else if(isdelim(str[i]))            //To recognize delimiters
            {
                j=i;
                a[it].str[0] = str[j];
                a[it].str[1]=0;
                a[it].line_num=ct;
                strcpy(a[it].type,tokenlist[2]);
                i++;
                it++;
            }
            else if(isdigit(str[i]))            //To recognize Integer and Floating point literals
            {
                j=i;
                k=0;
                char temp[30];
                char temp2[30];
                while(isdigit(str[j]))
                {
                    a[it].str[k] = str[j];
                    temp[k] = str[j];
                    k++;
                    j++;
                }
                strcpy(temp2,temp);
                if(isalpha(str[j]))
                {
                    while(identifier(str[j]))
                    {
                        temp[k] = str[j];
                        j++;
                        k++;
                    }
                    temp[k]=0;
                    strcpy(a[it].type,"Error");
                    strcpy(a[it].str,temp);
                    a[it].line_num = ct;
                    it++;
                    i=j;
                    strcpy(temp2,temp);
                    continue;

                }
                else if(str[j]=='.')
                {
                    int z=0;
                    while(isdigit(str[j])||str[j]=='.')
                    {
                        if(str[j]=='.')
                            z++;
                        a[it].str[k] = str[j];
                        temp2[k] = str[j];
                        j++;
                        k++;
                    }
                    if(z>1)
                    {
                        strcpy(a[it].type,"Error");
                        strcpy(a[it].str,temp2);
                        a[it].line_num = ct;
                        it++;
                        i=j;
                        continue;
                    }
                    if(isalpha(str[j]))
                    {
                        while(isalpha(str[j]))
                        {
                            temp2[k] = str[j];
                            k++;
                            j++;
                        }
                        temp2[k]=0;
                        strcpy(a[it].type,"Error");
                        strcpy(a[it].str,temp2);
                        a[it].line_num = ct;
                        it++;
                        i=j;
                        continue;
                    }
                    a[it].str[k]=0;
                    strcpy(a[it].type,tokenlist[5]);
                }
                else
                {
                    a[it].str[k]=0;
                    strcpy(a[it].type,tokenlist[4]);
                }
                a[it].line_num = ct;
                i=j;
                it++;
            }
            else
                i++;
        }
    }
    int i,j,k;
    for(i=0;i<it;i++)                   //To recognize Keywords
    {
        for(j=0;j<20;j++)
        {
            char temp[40];
            strcpy(temp,a[i].str);
            if(strcmp(strlwr(temp),keyword[j])==0)
            {
                strcpy(a[i].type,tokenlist[0]);
                break;
            }
        }
    }
    FILE *zp;
    zp = fopen("output2.txt","w");
    for(i=0;i<it;i++)                               //Printing tokens and Errors if any
    {
        if(!strcmp(a[i].type,"Error"))
            printf("Error at line number %d, Found an unidentified token %s\n",a[i].line_num,a[i].str);
        else
            fprintf(zp,"%d     %s     %s\n",a[i].line_num,a[i].str,a[i].type);
    }
    fclose(zp);
    fclose(fp);
    return 0;
}

int identifier(char c)                          //function to recognize identifiers
{

    if(isalpha(c)||isdigit(c)||c=='_'||c=='.')
        return 1;
    else
        return 0;
}
int isdelim(char c)                             //function to recognize delimiters
{
    int i;
    for(i=0;i<9;i++)
        if(delim[i]==c)
            return 1;
    return 0;
}
int isop(char c)                                //function to recognize Operators
{
    int i;
    for(i=0;i<12;i++)
        if(oper[i]==c)
            return 1;
    return 0;
}
