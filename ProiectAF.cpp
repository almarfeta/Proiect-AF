#include <iostream>
#include <fstream>
#include <vector>
#include <queue>
#include <stack>
#include <algorithm>

using namespace std;

struct cost {
    int first;
    int second;
    int weight;
};

class Graf
{
    int tip;
    int nr_noduri;
    int nr_muchii;
    vector <vector <int>> lista_vecini;
    vector <cost> lista_muchii;

public:
    Graf() {
        tip = 0;
        nr_noduri = 0;
        nr_muchii = 0;
    }
    Graf(int tip, int nr_noduri, int nr_muchii) {
        this->tip = tip;
        this->nr_noduri = nr_noduri;
        this->nr_muchii = nr_muchii;
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
    Graf(int tip, int nr_noduri, int nr_muchii, vector <cost> lista_muchii) {
        this->tip = tip;
        this->nr_noduri = nr_noduri;
        this->nr_muchii = nr_muchii;
        this->lista_muchii = lista_muchii;
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
    int reprez_kruskal(int nod, vector <int>& tata);
    void reuneste_kruskal(int ru, int rv, vector <int>& tata, vector <int>& h);
    int BFS_darb(int &start);
    bool bfs_flux(int s, int t, vector <int> &tata, vector <vector <int>> &flux);

public:
    vector <int> BFS(int start);
    int comp_conexe();
    bool havel_hakimi(int seq_len, vector <int> seq);
    vector <vector <int>> criticalConnections();
    vector <vector <int>> tare_conex();
    vector <int> sortare_top();
    vector <vector <pair <int, int>>> biconex();
    vector <pair <int, int>> kruskal();
    vector <bool> disjoint();
    vector <int> bellman_ford(vector <vector <pair <int, int>>> lista_costuri);
    vector <int> dijkstra(vector <vector <pair <int, int>>> lista_costuri);
    void floyd_warshall(vector <vector <int>> &matrice_costuri);
    int diametru();
    int edmond_karp(vector <vector <int>>& flux);
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

void infoarena_bfs() {
    ifstream in("bfs.in");
    ofstream out("bfs.out");

    int n, m, s;
    vector <vector <int>> v;

    in >> n >> m >> s;
    v.resize(n + 1);
    for (int i = 0; i < m; i++) {
        int x, y;
        in >> x >> y;
        v[x].push_back(y);
    }
    Graf g(1, n, m, v);

    vector <int> solutie;
    solutie = g.BFS(s);
    for (int i = 1; i <= n; i++)
        out << solutie[i] << " ";

    in.close();
    out.close();
}
void infoarena_dfs() {
    ifstream in("dfs.in");
    ofstream out("dfs.out");

    int n, m;
    vector <vector <int>> v;

    in >> n >> m;
    v.resize(n + 1);
    for (int i = 0; i < m; i++) {
        int x, y;
        in >> x >> y;
        v[x].push_back(y);
        v[y].push_back(x);
    }
    Graf g(0, n, m, v);

    out << g.comp_conexe();

    in.close();
    out.close();
}
void rezolva_havel() {
    ifstream in("havel.in");
    ofstream out("havel.out");

    int n;
    vector <int> v;

    in >> n;
    for (int i = 0; i < n; i++) {
        int x;
        in >> x;
        v.push_back(x);
    }
    Graf g;

    if (g.havel_hakimi(n, v) == true)
        out << "Da";
    else
        out << "Nu";

    in.close();
    out.close();
}
void leetcode_criticalCon() {
    ifstream in("critcon.in");
    ofstream out("critcon.out");

    int n, m;
    vector <vector <int>> v;

    in >> n >> m;
    v.resize(n + 1);
    for (int i = 0; i < m; i++) {
        int x, y;
        in >> x >> y;
        v[x].push_back(y);
        v[y].push_back(x);
    }
    Graf g(0, n, m, v);

    vector <vector <int>> solutie;
    solutie = g.criticalConnections();
    for (int i = 0; i < solutie.size(); i++)
        out << solutie[i][0] << " " << solutie[i][1] << "\n";

    in.close();
    out.close();
}
void infoarena_ctc() {
    ifstream in("ctc.in");
    ofstream out("ctc.out");

    int n, m;
    vector <vector <int>> v;

    in >> n >> m;
    v.resize(n + 1);
    for (int i = 0; i < m; i++) {
        int x, y;
        in >> x >> y;
        v[x].push_back(y);
    }
    Graf g(1, n, m, v);

    vector <vector <int>> solutie;
    solutie = g.tare_conex();
    int nr_comp = solutie[0][0];

    out << nr_comp << "\n";
    for (int i = 1; i <= nr_comp; i++) {
        for (int j = 0; j < solutie[i].size(); j++)
            out << solutie[i][j] << " ";
        out << "\n";
    }

    in.close();
    out.close();
}
void infoarena_sortaret() {
    ifstream in("sortaret.in");
    ofstream out("sortaret.out");

    int n, m;
    vector <vector <int>> v;

    in >> n >> m;
    v.resize(n + 1);
    for (int i = 0; i < m; i++) {
        int x, y;
        in >> x >> y;
        v[x].push_back(y);
    }
    Graf g(1, n, m, v);

    vector <int> solutie;
    solutie = g.sortare_top();
    for (int i = solutie.size() - 1; i >= 0; i--)
        out << solutie[i] << " ";

    in.close();
    out.close();
}
void infoarena_biconex() {
    ifstream in("biconex.in");
    ofstream out("biconex.out");

    int n, m;
    vector <vector <int>> v;

    in >> n >> m;
    v.resize(n + 1);
    for (int i = 0; i < m; i++) {
        int x, y;
        in >> x >> y;
        v[x].push_back(y);
        v[y].push_back(x);
    }
    Graf g(0, n, m, v);

    vector <vector <pair <int, int>>> solutie;
    solutie = g.biconex();
    int nr_comp = solutie[0][0].first;
    out << nr_comp << "\n";
    for (int i = 1; i <= nr_comp; i++) {
        vector <int> noduri(n + 1, 0);

        for (int j = 0; j < solutie[i].size(); j++) {
            if (noduri[solutie[i][j].first] == 0) {
                out << solutie[i][j].first << " ";
                noduri[solutie[i][j].first] = 1;
            }
            if (noduri[solutie[i][j].second] == 0) {
                out << solutie[i][j].second << " ";
                noduri[solutie[i][j].second] = 1;
            }
        }
        out << "\n";
    }

    in.close();
    out.close();
}
void infoarena_apm() {
    ifstream in("apm.in");
    ofstream out("apm.out");

    int n, m;
    vector <cost> v;

    in >> n >> m;
    for (int i = 0; i < m; i++) {
        int x, y, c;
        in >> x >> y >> c;
        v.push_back({ x,y,c });
    }
    Graf g(0, n, m, v);

    vector <pair <int, int>> solutie;
    solutie = g.kruskal();
    int cost_total = solutie[0].first;
    int nr_muchiiSol = solutie[0].second;
    out << cost_total << "\n" << nr_muchiiSol << "\n";
    for (int i = 1; i <= nr_muchiiSol; i++)
        out << solutie[i].first << " " << solutie[i].second << "\n";

    in.close();
    out.close();
}
void infoarena_disjoint() {
    ifstream in("disjoint.in");
    ofstream out("disjoint.out");

    int n, m;
    vector <cost> v;

    in >> n >> m;
    for (int i = 0; i < m; i++) {
        int x, y, cod;
        in >> cod >> x >> y;
        v.push_back({ x,y,cod });
    }
    Graf g(0, n, m, v);

    vector <bool> solutie;
    solutie = g.disjoint();
    for (int i = 0; i < solutie.size(); i++)
        if (solutie[i] == true)
            out << "DA\n";
        else
            out << "NU\n";

    in.close();
    out.close();
}
void infoarena_bellmanford() {
    ifstream in("bellmanford.in");
    ofstream out("bellmanford.out");

    int n, m;
    vector <vector <pair<int, int>>> v;

    in >> n >> m;
    v.resize(n + 1);
    for (int i = 0; i < m; i++) {
        int x, y, c;
        in >> x >> y >> c;
        v[x].push_back(make_pair(y, c));
    }
    Graf g(1, n, m);

    vector <int> solutie;
    solutie = g.bellman_ford(v);
    if (!solutie.empty())
        for (int i = 2; i <= n; i++)
            out << solutie[i] << " ";
    else
        out << "Ciclu negativ!";

    in.close();
    out.close();
} 
void infoarena_dijkstra() {
    ifstream in("dijkstra.in");
    ofstream out("dijkstra.out");

    int n, m;
    vector <vector <pair <int, int>>> v;

    in >> n >> m;
    v.resize(n + 1);
    for (int i = 0; i < m; i++) {
        int x, y, c;
        in >> x >> y >> c;
        v[x].push_back(make_pair(y,c));
    }
    Graf g(1, n, m);

    vector <int> solutie;
    solutie = g.dijkstra(v);
    int inf = 0x3f3f3f3f;
    for (int i = 2; i <= n; i++)
        if (solutie[i] == inf)
            out << 0 << " ";
        else
            out << solutie[i] << " ";

    in.close();
    out.close();
}
void infoarena_floydwarshall() {
    ifstream in("royfloyd.in");
    ofstream out("royfloyd.out");

    int n;
    in >> n;
    vector <vector <int>> d(n + 1, vector <int>(n + 1, 0));
    for (int i = 1; i <= n; i++)
        for (int j = 1; j <= n; j++)
            in >> d[i][j];
    Graf g(1, n, -1);

    g.floyd_warshall(d);

    for (int i = 1; i <= n; i++) {
        for (int j = 1; j <= n; j++)
            out << d[i][j] << " ";
        out << "\n";
    }

    in.close();
    out.close();
}
void infoarena_diametru() {
    ifstream in("darb.in");
    ofstream out("darb.out");

    int n, m;
    in >> n;
    m = n - 1;
    vector <vector <int>> v;
    v.resize(n + 1);
    for (int i = 0; i < m; i++) {
        int x, y;
        in >> x >> y;
        v[x].push_back(y);
        v[y].push_back(x);
    }
    Graf g(0, n, m, v);
    
    int d = g.diametru();
    out << d;

    in.close();
    out.close();
}
void infoarena_maxflow() {
    ifstream in("maxflow.in");
    ofstream out("maxflow.out");

    int n, m;
    vector <vector <int>> v;
    in >> n >> m;
    v.resize(n + 1);
    vector <vector <int>> c(n + 1, vector <int>(n + 1, 0));
    for (int i = 0; i < m; i++) {
        int x, y, z;
        in >> x >> y >> z;
        v[x].push_back(y);
        v[y].push_back(x);
        c[x][y] = z;
    }
    Graf g(1, n, m, v);

    out << g.edmond_karp(c);

    in.close();
    out.close();
}

int main()
{
    infoarena_maxflow();
    return 0;
}



vector <int> Graf::BFS(int start) {

    queue <int> bfs_tree;
    vector <int> viz(nr_noduri + 1, -1);

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

    return viz;
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
int Graf::comp_conexe() {
    vector <int> viz(nr_noduri + 1, 0);
    int nr_comp = 0;

    for (int i = 1; i <= this->nr_noduri; i++)
        if (viz[i] == 0) {
            DFS(i, viz);
            nr_comp++;
        }

    return nr_comp;
}
bool Graf::havel_hakimi(int seq_len, vector <int> seq) {
    int sum = 0;
    for (int i = 0; i < seq_len; i++) {
        sum = sum + seq[i];
        if (seq[i] > seq_len - 1)
            return false;
    }
    if (sum % 2 != 0)
        return false;
    else {
        sort(seq.begin(), seq.end(), greater<int>());
        while (seq[0] != 0) {
            int nod_curent = seq[0];
            seq.erase(seq.begin());
            for (int i = 0; i < nod_curent; i++) {
                seq[i]--;
                if (seq[i] < 0)
                    return false;
            }
            sort(seq.begin(), seq.end(), greater<int>());
        }
        return true;
    }
}
void Graf::dfs_muchiiIntoarcere(int i, vector <int>& viz, vector <int>& niv, vector<int>& niv_min, vector <vector<int>>& muchii_crit) {
    viz[i] = 1;
    niv_min[i] = niv[i];
    for (int j = 0; j < lista_vecini[i].size(); j++)
        if (viz[lista_vecini[i][j]] == 0) {
            niv[lista_vecini[i][j]] = niv[i] + 1;
            dfs_muchiiIntoarcere(lista_vecini[i][j], viz, niv, niv_min, muchii_crit);
            niv_min[i] = min(niv_min[i], niv_min[lista_vecini[i][j]]);
            if (niv_min[lista_vecini[i][j]] > niv[i]) {
                muchii_crit.push_back({ i,lista_vecini[i][j] });
            }
        }
        else {
            if (niv[lista_vecini[i][j]] < niv[i] - 1)
                niv_min[i] = min(niv_min[i], niv[lista_vecini[i][j]]);
        }
}
vector <vector <int>> Graf::criticalConnections() {
    vector <vector<int>> muchii_crit;
    vector <int> viz(nr_noduri + 1, 0);
    vector <int> niv;
    vector <int> niv_min;

    niv.resize(nr_noduri + 1);
    niv_min.resize(nr_noduri + 1);
    niv[1] = 1;
    dfs_muchiiIntoarcere(1, viz, niv, niv_min, muchii_crit);

    return muchii_crit;
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
vector <vector <int>> Graf::tare_conex() {
    vector <int> niv(nr_noduri + 1, 0);
    vector <int> niv_min;
    stack <int> noduri;
    vector <int> onStack(nr_noduri + 1, 0);
    vector <vector<int>> solutie;

    int nr_comp = 0;
    niv_min.resize(nr_noduri + 1);
    solutie.resize(nr_noduri + 1);
    int index = 1;
    for (int i = 1; i <= nr_noduri; i++)
        if (niv[i] == 0)
            dfs_tare_conex(i, onStack, niv, niv_min, index, noduri, nr_comp, solutie);

    solutie[0].push_back(nr_comp);
    return solutie;
}
void Graf::dfs_sortare(int nod_curent, vector <int>& viz, vector <int>& solutie) {
    viz[nod_curent] = 1;
    for (int i = 0; i < lista_vecini[nod_curent].size(); i++)
        if (viz[lista_vecini[nod_curent][i]] == 0)
            dfs_sortare(lista_vecini[nod_curent][i], viz, solutie);
    solutie.push_back(nod_curent);
}
vector <int> Graf::sortare_top() {
    vector <int> solutie;
    vector <int> viz(nr_noduri + 1, 0);

    for (int i = 1; i <= nr_noduri; i++)
        if (viz[i] == 0)
            dfs_sortare(i, viz, solutie);

    return solutie;
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
                nr_comp++;
                while (muchii.top().first != i || muchii.top().second != lista_vecini[i][j]) {
                    solutie[nr_comp].push_back(muchii.top());
                    muchii.pop();
                }
                solutie[nr_comp].push_back(muchii.top());
                muchii.pop();
            }
        }
        else if (lista_vecini[i][j] != tata[i]) {
            niv_min[i] = min(niv_min[i], niv[lista_vecini[i][j]]);
            if (niv[lista_vecini[i][j]] < niv[i])
                muchii.push(make_pair(i, lista_vecini[i][j]));
        }
    }
}
vector <vector <pair <int, int>>> Graf::biconex() {
    vector <int> niv(nr_noduri + 1, 0);
    vector <int> niv_min;
    vector <int> tata(nr_noduri + 1, 0);
    stack <pair <int, int>> muchii;
    vector <vector <pair <int, int>>> solutie;
    niv_min.resize(nr_noduri + 1);
    solutie.resize(nr_noduri);

    int index = 1;
    int nr_comp = 0;

    for (int i = 1; i <= nr_noduri; i++)
        if (niv[i] == 0)
            dfs_biconex(i, index, niv, niv_min, tata, muchii, nr_comp, solutie);

    if (!muchii.empty()) {
        nr_comp++;
        while (!muchii.empty()) {
            solutie[nr_comp].push_back(muchii.top());
            muchii.pop();
        }
    }
    solutie[0].push_back(make_pair(nr_comp, 0));

    return solutie;
}
int Graf::reprez_kruskal(int nod, vector <int> &tata) {
    while (tata[nod] != 0)
        nod = tata[nod];
    return nod;
}
void Graf::reuneste_kruskal(int ru, int rv, vector <int>& tata, vector <int>& h) {
    if (h[ru] > h[rv])
        tata[rv] = ru;
    else {
        tata[ru] = rv;
        if (h[ru] == h[rv])
            h[rv]++;
    }
}
bool criteriuSort(cost c1, cost c2) { return (c1.weight < c2.weight); }
vector <pair <int, int>> Graf::kruskal() {
    vector <pair <int, int>> solutie;
    solutie.resize(nr_noduri);
    vector <int> tata(nr_noduri + 1, 0);
    vector <int> h(nr_noduri + 1, 0);

    sort(lista_muchii.begin(), lista_muchii.end(), criteriuSort);

    int cost_total = 0;
    int nr_muchiiSol = 0;
    for (int i = 0; (i < lista_muchii.size()) && (nr_muchiiSol < nr_noduri - 1); i++) {
        int ru = reprez_kruskal(lista_muchii[i].first, tata);
        int rv = reprez_kruskal(lista_muchii[i].second, tata);
        if (ru != rv) {
            nr_muchiiSol++;
            solutie[nr_muchiiSol].first = lista_muchii[i].first;
            solutie[nr_muchiiSol].second = lista_muchii[i].second;
            cost_total = cost_total + lista_muchii[i].weight;
            reuneste_kruskal(ru, rv, tata, h);
        }
    }

    solutie[0].first = cost_total;
    solutie[0].second = nr_muchiiSol;
    return solutie;
}
vector <bool> Graf::disjoint() {
    vector <bool> solutie;
    vector <int> tata(nr_noduri + 1, 0);
    vector <int> h(nr_noduri + 1, 0);

    for (int i = 0; i < nr_muchii; i++)
        if (lista_muchii[i].weight == 1) {
            int ru = reprez_kruskal(lista_muchii[i].first, tata);
            int rv = reprez_kruskal(lista_muchii[i].second, tata);
            reuneste_kruskal(ru, rv, tata, h);
        }
        else if (lista_muchii[i].weight == 2) {
            int ru = reprez_kruskal(lista_muchii[i].first, tata);
            int rv = reprez_kruskal(lista_muchii[i].second, tata);
            if (ru == rv)
                solutie.push_back(true);
            else
                solutie.push_back(false);
        }

    return solutie;
}
vector <int> Graf::bellman_ford(vector <vector <pair <int, int>>> lista_costuri) {
    int inf = 0x3f3f3f3f;
    vector <int> distanta(nr_noduri + 1, inf);
    queue <int> coada;
    vector <bool> inCoada(nr_noduri + 1, false);
    vector <int> pasi(nr_noduri + 1, 0);

    distanta[1] = 0;
    coada.push(1);
    inCoada[1] = true;
    while (!coada.empty()) {
        int u = coada.front();
        coada.pop();
        inCoada[u] = false;
        for (int i = 0; i < lista_costuri[u].size(); i++) {
            int v = lista_costuri[u][i].first;
            int c = lista_costuri[u][i].second;
            if (distanta[u] + c < distanta[v]) {
                distanta[v] = distanta[u] + c;
                pasi[v]++;
                if (pasi[v] == nr_noduri)
                    return {};
                if (inCoada[v] == false) {
                    coada.push(v);
                    inCoada[v] = true;
                }
            }
        }
    }
    
    return distanta;
}
vector <int> Graf::dijkstra(vector <vector <pair <int, int>>> lista_costuri) {
    int inf = 0x3f3f3f3f;
    vector <int> distanta(nr_noduri + 1, inf);
    priority_queue <pair <int, int>> min_heap;

    distanta[1] = 0;
    min_heap.push(make_pair(0, 1));
    while (!min_heap.empty()) {
        int u = min_heap.top().second;
        min_heap.pop();
        for (int i = 0; i < lista_costuri[u].size(); i++) {
            int v = lista_costuri[u][i].first;
            int c = lista_costuri[u][i].second;
            if (distanta[u] + c < distanta[v]) {
                distanta[v] = distanta[u] + c;
                min_heap.push(make_pair(-distanta[v], v));
            }
        }
    }

    return distanta;
}
void Graf::floyd_warshall(vector <vector <int>> &matrice_costuri) {
    for (int k = 1; k <= nr_noduri; k++)
        for (int i = 1; i <= nr_noduri; i++)
            for (int j = 1; j <= nr_noduri; j++)
                if (matrice_costuri[i][k] != 0 && matrice_costuri[k][j] != 0 && i != j)
                    if(matrice_costuri[i][j] > matrice_costuri[i][k] + matrice_costuri[k][j] || matrice_costuri[i][j] == 0)
                        matrice_costuri[i][j] = matrice_costuri[i][k] + matrice_costuri[k][j];
}
int Graf::BFS_darb(int &start) {
    vector <int> dist(nr_noduri + 1, 0);
    queue <int> coada;
    int diam;

    coada.push(start);
    dist[start] = 1;
    diam = dist[start];

    while (!coada.empty()) {
        int nod_curent = coada.front();
        start = nod_curent;
        coada.pop();
        for (int i = 0; i < lista_vecini[nod_curent].size(); i++)
            if (dist[lista_vecini[nod_curent][i]] == 0) {
                coada.push(lista_vecini[nod_curent][i]);
                dist[lista_vecini[nod_curent][i]] = dist[nod_curent] + 1;
                diam = dist[lista_vecini[nod_curent][i]];
            }
    }

    return diam;
}
int Graf::diametru() {
    int start = 1;
    int diam = BFS_darb(start);
    diam = BFS_darb(start);
    return diam;
}
bool Graf::bfs_flux(int s, int t, vector <int> &tata, vector <vector <int>>& flux) {
    vector <int> viz(nr_noduri + 1, 0);
    queue <int> coada;

    coada.push(s);
    viz[s] = 1;
    while (!coada.empty()) {
        int nod_curent = coada.front();
        coada.pop();
        for (int i = 0; i < lista_vecini[nod_curent].size(); i++) {
            if (viz[lista_vecini[nod_curent][i]] == 0 && flux[nod_curent][lista_vecini[nod_curent][i]] > 0) {
                if (lista_vecini[nod_curent][i] == t) {
                    tata[t] = nod_curent;
                    return true;
                }
                coada.push(lista_vecini[nod_curent][i]);
                viz[lista_vecini[nod_curent][i]] = 1;
                tata[lista_vecini[nod_curent][i]] = nod_curent;
            }
        }
    }
    return false;
}
int Graf::edmond_karp(vector <vector <int>>& flux) {
    int flux_max = 0;
    vector <int> tata(nr_noduri + 1, 0);
    while (bfs_flux(1, nr_noduri, tata, flux) == true) {
        int nod = nr_noduri;
        int flux_min = 0;
        while (nod != 1) {
            if (flux_min == 0 || flux[tata[nod]][nod] < flux_min)
                flux_min = flux[tata[nod]][nod];
            nod = tata[nod];
        }
        nod = nr_noduri;
        while (nod != 1) {
            flux[tata[nod]][nod] = flux[tata[nod]][nod] - flux_min;
            flux[nod][tata[nod]] = flux_min;
            nod = tata[nod];
        }
        flux_max = flux_max + flux_min;
    }
    return flux_max;
}