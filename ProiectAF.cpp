#include <iostream>
#include <vector>
#include <queue>
#include <stack>
#include <algorithm>

using namespace std;

class Graf
{
    int tip;
    int nr_noduri;
    int nr_muchii;
    vector <vector <int>> lista_vecini;

public:
    Graf() {
        tip = 0;
        nr_noduri = 0;
        nr_muchii = 0;
    }
    Graf(int tip, int nr_noduri, int nr_muchii, vector <vector<int>> lista_vecini) {
        this->tip = tip;
        this->nr_noduri = nr_noduri;
        this->nr_muchii = nr_muchii;
        this->lista_vecini.resize(nr_noduri + 1);
        for (int i = 1; i <= nr_noduri; i++)
            for (int j = 0; j < lista_vecini[i].size(); j++)
                this->lista_vecini[i].push_back(lista_vecini[i][j]);
    }
    ~Graf() { }

    friend istream& operator>>(istream& in, Graf& g);
    friend ostream& operator<<(ostream& out, const Graf& g);
   
private:
    void DFS(int start, vector <int>& viz);
    void dfs_muchiiIntoarcere(int i, vector <int>& viz, vector <int>& niv, vector<int>& niv_min, vector <vector<int>>& solutie);
    void dfs_tare_conex(int i, vector <int>& onStack, vector <int>& niv, vector <int>& niv_min, int& index, stack <int>& noduri, int& nr_comp, vector <vector<int>>& solutie);
    void dfs_sortare(int nod_curent, vector <int>& viz, vector <int>& solutie);
    void dfs_biconex(int i, int& index, vector <int>& niv, vector <int>& niv_min, vector <int>& tata, stack <pair<int, int>>& muchii, int& nr_comp, vector <vector <pair<int, int>>>& solutie);

public:
    void BFS(int start);
    void comp_conexe();
    void havel_hakimi();
    void criticalConnections();
    void tare_conex();
    void sortare_top();
    void biconex();

};

istream& operator>>(istream& in, Graf& g)
{
    cout << "\nIntroduceti tipul grafului (0-Neorientat 1-Orientat):";
    in >> g.tip;
    if (g.tip == 0) {
        cout << "\nIntroduceti numarul de noduri: ";
        cin >> g.nr_noduri;
        cout << "\nIntroduceti numarul de muchii: ";
        cin >> g.nr_muchii;
        cout << "\nIntroduceti lista muchiilor: ";
        g.lista_vecini.resize(g.nr_noduri + 1);
        for (int i = 0; i < g.nr_muchii; i++) {
            int x, y;
            cin >> x >> y;
            g.lista_vecini[x].push_back(y);
            g.lista_vecini[y].push_back(x);
        }
    }
    else if(g.tip == 1) {
        cout << "\nIntroduceti numarul de noduri: ";
        cin >> g.nr_noduri;
        cout << "\nIntroduceti numarul de arce: ";
        cin >> g.nr_muchii;
        cout << "\nIntroduceti lista arcelor: ";
        g.lista_vecini.resize(g.nr_noduri + 1);
        for (int i = 0; i < g.nr_muchii; i++) {
            int x, y;
            cin >> x >> y;
            g.lista_vecini[x].push_back(y);
        }
    }
    else {
        cout << "\nTip gresit";
    }

    return in;
}
ostream& operator<<(ostream& out, const Graf& g)
{
    cout << "\nTip graf: ";
    if (g.tip == 0) {
        cout << "neorientat";
        cout << "\nNumar de noduri: " << g.nr_noduri;
        cout << "\nNumar de muchii: " << g.nr_muchii;
        cout << "\nLista de vecini: ";
        for (int i = 1; i <= g.nr_noduri; i++)
        {
            for (int j = 0; j < g.lista_vecini[i].size(); j++)
                cout << g.lista_vecini[i][j] << " ";
            cout << "\n";
        }
    }
    else if (g.tip == 1) {
        cout << "orientat";
        cout << "\nNumar de noduri: " << g.nr_noduri;
        cout << "\nNumar de arce: " << g.nr_muchii;
        cout << "\nLista de vecini: ";
        for (int i = 1; i <= g.nr_noduri; i++)
        {
            for (int j = 0; j < g.lista_vecini[i].size(); j++)
                cout << g.lista_vecini[i][j] << " ";
            cout << "\n";
        }
    }

    return out;
}

int main()
{
    int n, m, s;
    cin >> n >> m;
    vector <vector <int>> v;
    v.resize(n + 1);
    for (int i = 0; i < m; i++)
    {
        int x, y;
        cin >> x >> y;
        v[x].push_back(y);
        v[y].push_back(x);
    }
    Graf G(0, n, m, v);
    G.criticalConnections();
    return 0;
}



void Graf::BFS(int start) {

    queue <int> bfs_tree;
    vector <int> viz;
    for (int i = 0; i <= this->nr_noduri; i++)
        viz.push_back(-1);

    bfs_tree.push(start);
    viz[start] = 0;

    while (!bfs_tree.empty()) {
        int nod_curent = bfs_tree.front();
        bfs_tree.pop();

        for (int i = 0; i < lista_vecini[nod_curent].size(); i++)
            if (viz[lista_vecini[nod_curent][i]] == -1) {
                viz[lista_vecini[nod_curent][i]] = viz[nod_curent] + 1;
                bfs_tree.push(lista_vecini[nod_curent][i]);
            }
    }

    cout << "\nVectorul distantelor: ";
    for (int i = 1; i <= this->nr_noduri; i++)
        cout << viz[i] << " ";
}
void Graf::DFS(int start, vector <int>& viz) {
    stack <int> dfs_tree;

    dfs_tree.push(start);
    viz[start] = 1;

    while (!dfs_tree.empty()) {
        int nod_curent = dfs_tree.top();
        int vecin = 0;
        for (int i = 0; i < lista_vecini[nod_curent].size(); i++)
            if (viz[lista_vecini[nod_curent][i]] == 0) {
                vecin = lista_vecini[nod_curent][i];
                break;
            }
        if (vecin == 0)
            dfs_tree.pop();
        else {
            dfs_tree.push(vecin);
            viz[vecin] = 1;
        }
    }

}
void Graf::comp_conexe() {
    vector <int> viz;
    for (int i = 0; i <= this->nr_noduri; i++)
        viz.push_back(0);

    int nr_comp = 0;

    for (int i = 1; i <= this->nr_noduri; i++)
        if (viz[i] == 0) {
            DFS(i, viz);
            nr_comp++;
        }

    cout << "\nNumar de componente conexe: " << nr_comp << " ";
}
void Graf::havel_hakimi() {
    int seq_len;
    vector <int> seq;
    int sum = 0;
    int ok = 1;
    cin >> seq_len;
    for (int i = 0; i < seq_len; i++) {
        int aux;
        cin >> aux;
        seq.push_back(aux);
        sum = sum + aux;
        if (aux > seq_len - 1)
            ok = 0;
    }
    if (sum % 2 != 0 || ok == 0)
        cout << "Nu";
    else {
        sort(seq.begin(), seq.end(), greater<int>());
        while (seq[0] != 0) {
            int nod_curent = seq[0];
            seq.erase(seq.begin());
            for (int i = 0; i < nod_curent; i++) {
                seq[i]--;
                if (seq[i] < 0) {
                    cout << "Nu";
                    ok = 0;
                    break;
                }
            }
            if (ok == 0)
                break;
            sort(seq.begin(), seq.end(), greater<int>());
        }
        if (ok == 1)
            cout << "Da";
    }

}
void Graf::dfs_muchiiIntoarcere(int i, vector <int>& viz, vector <int>& niv, vector<int>& niv_min, vector <vector<int>>& solutie) {
    viz[i] = 1;
    niv_min[i] = niv[i];
    for (int j = 0; j < lista_vecini[i].size(); j++)
        if (viz[lista_vecini[i][j]] == 0) {
            niv[lista_vecini[i][j]] = niv[i] + 1;
            dfs_muchiiIntoarcere(lista_vecini[i][j], viz, niv, niv_min, solutie);
            niv_min[i] = min(niv_min[i], niv_min[lista_vecini[i][j]]);
            if (niv_min[lista_vecini[i][j]] > niv[i]) {
                solutie.push_back({ i,lista_vecini[i][j] });
            }
        }
        else {
            if (niv[lista_vecini[i][j]] < niv[i] - 1)
                niv_min[i] = min(niv_min[i], niv[lista_vecini[i][j]]);
        }
}
void Graf::criticalConnections() {
    vector <vector<int>> solutie;
    vector <int> viz;
    vector <int> niv;
    vector <int> niv_min;

    for (int i = 0; i <= nr_noduri; i++)
        viz.push_back(0);
    niv.resize(nr_noduri + 1);
    niv_min.resize(nr_noduri + 1);
    niv[1] = 1;
    dfs_muchiiIntoarcere(1, viz, niv, niv_min, solutie);

    for (int i = 0; i < solutie.size(); i++)
        cout << solutie[i][0] << " " << solutie[i][1] << "\n";
}
void Graf::dfs_tare_conex(int i, vector <int>& onStack, vector <int>& niv, vector <int>& niv_min, int& index, stack <int>& noduri, int& nr_comp, vector <vector<int>>& solutie) {
    niv[i] = index;
    niv_min[i] = index;
    index++;
    noduri.push(i);
    onStack[i] = 1;

    for (int j = 0; j < lista_vecini[i].size(); j++)
        if (niv[lista_vecini[i][j]] == 0) {
            dfs_tare_conex(lista_vecini[i][j], onStack, niv, niv_min, index, noduri, nr_comp, solutie);
            niv_min[i] = min(niv_min[i], niv_min[lista_vecini[i][j]]);
        }
        else if (onStack[lista_vecini[i][j]] == 1)
            niv_min[i] = min(niv_min[i], niv[lista_vecini[i][j]]);

    if (niv_min[i] == niv[i]) {
        nr_comp++;
        while (noduri.top() != i) {
            solutie[nr_comp].push_back(noduri.top());
            onStack[noduri.top()] = 0;
            noduri.pop();
        }
        solutie[nr_comp].push_back(noduri.top());
        onStack[noduri.top()] = 0;
        noduri.pop();
    }
}
void Graf::tare_conex() {
    vector <int> niv;
    vector <int> niv_min;
    stack <int> noduri;
    vector <int> onStack;
    vector <vector<int>> solutie;
    int nr_comp = 0;
    niv_min.resize(nr_noduri + 1);
    solutie.resize(nr_noduri + 1);
    for (int i = 0; i <= nr_noduri; i++) {
        onStack.push_back(0);
        niv.push_back(0);
    }

    int index = 1;
    for (int i = 1; i <= nr_noduri; i++)
        if (niv[i] == 0)
            dfs_tare_conex(i, onStack, niv, niv_min, index, noduri, nr_comp, solutie);


    cout << nr_comp << "\n";
    for (int i = 1; i <= nr_comp; i++) {
        for (int j = 0; j < solutie[i].size(); j++)
            cout << solutie[i][j] << " ";
        cout << "\n";
    }
}
void Graf::dfs_sortare(int nod_curent, vector <int>& viz, vector <int>& solutie) {
    viz[nod_curent] = 1;
    for (int i = 0; i < lista_vecini[nod_curent].size(); i++)
        if (viz[lista_vecini[nod_curent][i]] == 0)
            dfs_sortare(lista_vecini[nod_curent][i], viz, solutie);
    solutie.push_back(nod_curent);
}
void Graf::sortare_top() {
    vector <int> solutie;
    vector <int> viz;
    for (int i = 0; i <= nr_noduri; i++)
        viz.push_back(0);

    for (int i = 1; i <= nr_noduri; i++)
        if (viz[i] == 0)
            dfs_sortare(i, viz, solutie);

    for (int i = solutie.size() - 1; i >= 0; i--)
        cout << solutie[i] << " ";
    cout << "\n";
}
void Graf::dfs_biconex(int i, int& index, vector <int>& niv, vector <int>& niv_min, vector <int>& tata, stack <pair<int, int>>& muchii, int& nr_comp, vector <vector <pair<int, int>>>& solutie) {
    niv[i] = index;
    niv_min[i] = index;
    index++;
    int nr_copii = 0;

    for (int j = 0; j < lista_vecini[i].size(); j++) {
        if (niv[lista_vecini[i][j]] == 0) {
            nr_copii++;
            tata[lista_vecini[i][j]] = i;
            muchii.push(make_pair(i, lista_vecini[i][j]));

            dfs_biconex(lista_vecini[i][j], index, niv, niv_min, tata, muchii, nr_comp, solutie);

            niv_min[i] = min(niv_min[i], niv_min[lista_vecini[i][j]]);
            if ((niv[i] == 1 && nr_copii > 1) || (niv[i] > 1 && niv_min[lista_vecini[i][j]] >= niv[i])) {
                while (muchii.top().first != i || muchii.top().second != lista_vecini[i][j]) {
                    solutie[nr_comp].push_back(muchii.top());
                    muchii.pop();
                }
                solutie[nr_comp].push_back(muchii.top());
                muchii.pop();
                nr_comp++;
            }
        }
        else if (lista_vecini[i][j] != tata[i]) {
            niv_min[i] = min(niv_min[i], niv[lista_vecini[i][j]]);
            if (niv[lista_vecini[i][j]] < niv[i])
                muchii.push(make_pair(i, lista_vecini[i][j]));
        }
    }
}
void Graf::biconex() {
    vector <int> niv;
    vector <int> niv_min;
    vector <int> tata;
    stack <pair <int, int>> muchii;
    vector <vector <pair <int, int>>> solutie;
    niv_min.resize(nr_noduri + 1);
    solutie.resize(nr_noduri);
    for (int i = 0; i <= nr_noduri; i++) {
        niv.push_back(0);
        tata.push_back(0);
    }
    int index = 1;
    int nr_comp = 0;

    for (int i = 1; i <= nr_noduri; i++)
        if (niv[i] == 0)
            dfs_biconex(i, index, niv, niv_min, tata, muchii, nr_comp, solutie);

    int ok = 0;
    while (!muchii.empty()) {
        solutie[nr_comp].push_back(muchii.top());
        muchii.pop();
        ok = 1;
    }
    if (ok == 1)
        nr_comp++;

    cout << nr_comp << "\n";
    for (int i = 0; i < solutie.size(); i++) {
        vector <int> noduri;
        for (int i = 0; i <= nr_noduri; i++)
            noduri.push_back(0);
        for (int j = 0; j < solutie[i].size(); j++) {
            if (noduri[solutie[i][j].first] == 0) {
                cout << solutie[i][j].first << " ";
                noduri[solutie[i][j].first] = 1;
            }
            if (noduri[solutie[i][j].second] == 0) {
                cout << solutie[i][j].second << " ";
                noduri[solutie[i][j].second] = 1;
            }
        }
        cout << "\n";
    }

}