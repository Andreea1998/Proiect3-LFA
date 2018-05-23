#include <iostream>
#include <fstream>
#include <cstring>
using namespace std;

ifstream f("date.in");

struct gramatica{
    int nr_prod;
    char t;
    char s[3];
}

gram[100][100];

struct CYK{
    char mult[100];
    int nrt;
}

matr[100][100];

void citire_gramatica(gramatica gram[100][100],int &n){
    int i,j;
    f>>n;
    for(i=0;i<n;i++)
        f>>gram[i][0].t;
    for(i=0;i<n;i++){
        f>>gram[i]->nr_prod;
        for(j=1;j<=gram[i]->nr_prod;j++)
            f>>gram[i][j].s;
    }
}

void reuniune(gramatica gram[100][100],char term[],int n,char v[],int &m){
    int i,j;
    m=0;
    for(i=0;i<n;i++){
        for(j=1;j<=gram[i]->nr_prod;j++)
            if(strcmp(gram[i][j].s,term)==0){ ///verifica daca este tranzitie si adauga terminalul
                v[m]=gram[i][0].t;
                m++;
                v[m]='\0';
            }
    }
}

void reun(char v[],int &q){
    int i,j;
    if(v[0]!='\0'){
        for(i=0;i<strlen(v)-1;i++)
        for(j=i+1;j<strlen(v);j++)
            if(v[i]==v[j]){
                strcpy(v+i,v+i+1); ///scapa de dubluri
                q--;
            }
        if(strlen(v)==2)
            if(v[0]==v[1]){
                strcpy(v+i,v+i+1);
                q--;
            }
    }
}

void reuniune1(CYK matr[100][100],int m,gramatica gram[100][100],int n,int i, int j,int k,int &l,char s[]){
    char v[100],w[100],o[100],u[100];
    int r,q,t,y,ok,x;
    l=0;t=0;
    s[0]='\0';
    strcpy(v,matr[k][i].mult);
    strcpy(w,matr[j-k][i+k].mult);
    for(r=0;r<strlen(v);r++)
        for(q=0;q<strlen(w);q++){
            o[0]=v[r]; ///produs cartezian
            o[1]=w[q];
            o[2]='\0';
            u[0]='\0';
            reuniune(gram,o,n,u,t);///cauta neterminalele din care fac parte elementele din produsul cartezian
            strcpy(s+l,u); ///copiaza terminalele gasite
            l+=t;
            s[l]='\0';
        }
    reun(s,l); ///elimina dublurile
}
int main(){
    int n,i,j,m,k;
    char v[100];
    char w[100];
    citire_gramatica(gram,n);
    for(i=0;i<n;i++){ ///afisare gramatica
        cout<<gram[i][0].t<<"->";
        for(j=1;j<=gram[i]->nr_prod;j++)
            cout<<gram[i][j].s<<"/";
        cout<<endl;
    }
    f>>w;
    m=strlen(w);
    cout<<m<<endl;
    for(i=1;i<=m;i++){ ///formare prima linie
        k=0;
        for(int l=0;l<n;l++)
            for(j=1;j<=gram[l]->nr_prod;j++)
                if(strchr(gram[l][j].s,w[i-1])!=0){
                v[k]=gram[l][0].t;
                k++;
            }
        strcpy(matr[1][i].mult,v);
        matr[1][i].mult[k]='\0';
        matr[1][i].nrt=k;
    }
    for(i=1;i<=m;i++)
        cout<<matr[1][i].mult<<" ";
    cout<<endl<<endl;
    int l,q;
    CYK p;
    for(j=2;j<=m;j++)
        for(i=1;i<=m-j+1;i++){
            q=0;
            for(k=1;k<=j-1;k++){
                reuniune1(matr,m,gram,n,i,j,k,l,v);
                strcpy(matr[j][i].mult+q,v);
                q+=l;
            }
            reun(matr[j][i].mult,q);
            matr[j][i].nrt=q;
        }
    for(j=1;j<=m;j++){
        for(i=1;i<=m-j+1;i++)
            if(matr[j][i].nrt==0)
            cout<<"- ";
        else
            cout<<matr[j][i].mult<<" ";
        cout<<endl;
    }
    if(strchr(matr[m][1].mult,'S')!=0)
        cout<<endl<<"Cuvantul este acceptat";
    else
        cout<<endl<<"Cuvantul nu este acceptat";
    return 0;
}
