#include <iostream>
#include <vector>
#include <algorithm>
#include <cmath>
#include <stack>
using namespace std;

vector<vector<int>> grafo;
vector<int> dfs;
vector<vector<int>> edges;
vector<vector<int>> circuitos;
int t = 0;
bool staCk[100000];
vector<int> low;
static stack<int> Stackk;

bool comparaDor(const vector<int> &a, const vector<int> b){
    return a[2]<b[2];
}

void print_grafo(){
    for(int i = 0; i<(int)grafo.size();i++){
        for(int j=0; j<(int)grafo.size();j++){
            printf("%d  ",grafo[i][j]);
        }
        printf("\n");
    }
}

void print_circuitos(){
    for(int i = 0; i<(int)circuitos.size();i++){
        for(int j=0; j<(int)circuitos[i].size();j++){
            printf("%d  ",circuitos[i][j]);
        }
        printf("\n");
    }
}

void print_circuito(vector<int> &circuitos){
        for(int j=0; j<(int)circuitos.size();j++){
            printf("%d  ",circuitos[j]);
        }
        printf("\n");
}

void print_edges(){
    for(int i = 0; i<(int)edges.size();i++){
        for(int j=0; j<(int)edges[0].size();j++){
            printf("%d  ",edges[i][j]);
        }
        printf("\n");
    }
}



int find_set(int s, vector<int> &S){
    if(S[s]!=s){
        S[s]=find_set(S[s],S);
    }
    return S[s];
}

void uniOn(int a, int b, vector<int> &S, vector<int> &R){  
    if (R[a] > R[b])
        S[b] = a;
    else
        S[a] = b;

    if (R[a] == R[b])
        R[b]++;
}

int kruskal(vector<int> circuito, int nNodes){
    vector<int> S = vector<int>(nNodes);
    vector<int> R = vector<int>(nNodes);

    for (int i = 0; i < nNodes; i++){
        S[i]=i;
        R[i]=0;
    }

    //cout << edges.size() << " tam edges\n";

    sort(edges.begin(),edges.end(),comparaDor);
    int distancia = 0;
    for (auto &i : edges){
        if(find(circuito.begin(), circuito.end(), i[0]) != circuito.end() && (find(circuito.begin(), circuito.end(), i[1]) != circuito.end())){
            int a = find_set(i[0],S);
            int b = find_set(i[1],S);
            if(a != b){
                uniOn(b,a,S,R);
                distancia += i[2];
            }
        }
    }
    //cout << distancia << "  distancia\n";
    return distancia;    
}


void tarjan(int v){

    vector<int> circuito;

    low[v] = dfs[v] = t;
    t = t + 1;
    Stackk.push(v);
    staCk[v] = true;

    for (auto& w: grafo[v]){
        if (dfs[w] == -1){
            tarjan(w);
            low[v] = min(low[v], low[w]);
        } else if(staCk[w]){
            low[v] = min(low[v], dfs[w]);
        }
    }
    
    if( low[v] == dfs[v]){
        circuito = vector<int>();
        int aux;
        do{
            aux = Stackk.top();
            Stackk.pop();
            staCk[aux] = false;
            //cout << "adicionei "<< aux << "\n";
            circuito.push_back(aux);
        }while(aux != v);

        if(circuito.size() >= 2){
            circuitos.push_back(circuito);
        }
    }
}

void resposta(int nNodes, int nPerguntas){
    vector<int> dist;
    int maximo =0;
    int soma = 0;
    int tam = 0;
    int aux;

    dfs = vector<int>(nNodes,-1);
    low = vector<int>(nNodes,-1);
    
    for (int i = 0; i < nNodes; i++){
        if(dfs[i] == -1){
           tarjan(i);
        }
    }
    
   //printf("circuitos size = %d\n",circuitos.size());

    for(int i=0;i<(int)circuitos.size();i++){
        aux = kruskal(circuitos[i], nNodes);
        dist.push_back(aux);
        //cout << aux << "  dist\n";
        maximo = max((int)circuitos[i].size(), maximo);
        soma += aux;
        //cout << tam << "tam\n" << aux << "aux\n";       
        if(tam<aux){
            tam=aux;
        }

    }

    //responder
    if(nPerguntas>=1){
        cout << circuitos.size();
    }
    if(nPerguntas>=2){
        cout << " " << maximo;
    }
    if(nPerguntas>=3){
        cout << " " << tam;
    }
    if(nPerguntas>=4){
        cout << " " << soma;
    }
    cout << "\n";
}


int main(){

    std::ios_base::sync_with_stdio(0);
    std::cin.tie(0);

    // n -> n pontos de interesse
    // m -> n conexoes entre pontos de interesse
    // q -> n questoes que o mayor pergunta
    // D -> distancia

    int n_testes;
    int n, m, q, D, A, B;

    cin >> n_testes;

    for (int i = 0; i < n_testes; i++){
        cin >> n >> m >> q;
        grafo = vector<vector<int>>(n+1, vector<int> ());
        for (int j = 0; j < m; j++){
            cin >> A >> B >> D;
            grafo[A-1].push_back(B-1);
            edges.push_back({A-1,B-1,D});
        }
        resposta(n,q);
        circuitos.clear();
        Stackk=stack<int>();
        grafo.clear();
        edges.clear();
    }


}
