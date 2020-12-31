#include "Graph.h"
/*Do not add new libraries or files*/

Graph::Graph() {
    // bos
}

Graph::Graph(const Graph &rhs) {
    *this = rhs;

}

Graph &Graph::operator=(const Graph &rhs) {
    komsuListeleri = rhs.komsuListeleri;
    ulkeIsimleri = rhs.ulkeIsimleri;
    return *this;
}

Graph::~Graph() {
    // bos
}


void Graph::addNode(const Node &node) {
    komsuListeleri.Insert(node.getVid(), list<Edge>());
    ulkeIsimleri.Insert(node.getVid(), node.getCountry());
}

void Graph::addConnection(const Node &headNode, const Node &tailNode, int import) {
    komsuListeleri.Get(headNode.getVid()).push_back(Edge(tailNode, import));
}

list<Node> Graph::getAdjacentNodes(const Node &node) {
    list<Node> adjacentler;
    list<Edge> &kenarListesi = komsuListeleri.Get(node.getVid());
    for (edgeListItr itr = kenarListesi.begin(); itr != kenarListesi.end(); ++itr) {
        adjacentler.push_back(itr->getTailNode());
    }
    return adjacentler;
}

long Graph::getTotalImports(const Node &node) {
    long toplamIthalat = 0;
    list<Edge> &kenarListesi = komsuListeleri.Get(node.getVid());
    for (edgeListItr itr = kenarListesi.begin(); itr != kenarListesi.end(); ++itr) {
        toplamIthalat += itr->getImport();
    }
    return toplamIthalat;
}

void Graph::deleteNode(const Node &node) {
    const int anahtarSayisi = komsuListeleri.Size();
    int *anahtarlar = new int[anahtarSayisi];
    komsuListeleri.getKeys(anahtarlar);
    for (int i = 0; i < anahtarSayisi; ++i) {
        if (anahtarlar[i] == node.getVid()) {
            komsuListeleri.Get(anahtarlar[i]).clear();
            komsuListeleri.Delete(anahtarlar[i]);
        }
        else {
            list<Edge> &kenarListesi = komsuListeleri.Get(anahtarlar[i]);
            for (edgeListItr itr = kenarListesi.begin(); itr != kenarListesi.end(); ++itr) {
                if (itr->getTailNode().getVid() == node.getVid()) {
                    kenarListesi.erase(itr);
                    break;
                }
            }
        }
    }
    delete[] anahtarlar;
}

list<string> Graph::findLeastCostPath(const Node &srcNode, const Node &destNode) {
    const int anahtarSayisi = komsuListeleri.Size();
    int *anahtarlar = new int[anahtarSayisi];
    komsuListeleri.getKeys(anahtarlar);

    HashTable<int, MesafeVeDugum> dugumTablosu;
    for (int i = 0; i < anahtarSayisi; ++i) {
        dugumTablosu.Insert(anahtarlar[i],
            MesafeVeDugum(std::numeric_limits<int>::max(), -1));
    }
    dugumTablosu.Get(srcNode.getVid()).mesafe = 0;

    std::priority_queue<MesafeVeDugum,
        std::vector<MesafeVeDugum>, MesafeVeDugum::Kiyasla> pq;
    pq.push(MesafeVeDugum(0, srcNode.getVid()));

    while (!pq.empty() && pq.top().dugum != destNode.getVid()) {
        int dugum = pq.top().dugum;
        pq.pop();

        edgeListItr iSon = komsuListeleri.Get(dugum).end();
        for (edgeListItr i = komsuListeleri.Get(dugum).begin(); i != iSon; ++i) {
            int komsu = i->getTailNode().getVid();
            long yeniMesafe = dugumTablosu.Get(dugum).mesafe + i->getImport();

            if (dugumTablosu.Get(komsu).mesafe > yeniMesafe) {
                dugumTablosu.Get(komsu).mesafe = yeniMesafe;
                dugumTablosu.Get(komsu).dugum = dugum;
                pq.push(MesafeVeDugum(yeniMesafe, komsu));
            }
        }
    }

    list<string> path;
    int dugum;
    for (dugum = destNode.getVid(); dugumTablosu.Get(dugum).dugum != -1;
        dugum = dugumTablosu.Get(dugum).dugum) {

        path.push_front(ulkeIsimleri.Get(dugum));
    }
    path.push_front(ulkeIsimleri.Get(dugum));

    delete[] anahtarlar;
    return path;
}


bool Graph::isCyclic() {
    // bu algoritma muhtemelen 2 dugum birbirine point ediyorsa bunu da cycle kabul ediyor.
    const int anahtarSayisi = komsuListeleri.Size();
    int *anahtarlar = new int[anahtarSayisi];
    komsuListeleri.getKeys(anahtarlar);

    HashTable<int, dugumDurumu> dugumDurumlari;
    for (int i = 0; i < anahtarSayisi; ++i) {
        dugumDurumlari.Insert(anahtarlar[i], dugumDurumu());
    }

    for (int i = 0; i < anahtarSayisi; ++i) {
        if (!dugumDurumlari.Get(anahtarlar[i]).ziyaretEdildi) {
            if (keydenBaslaDFSCycleVarMi(anahtarlar[i], dugumDurumlari)) {
                delete[] anahtarlar;
                return true;
            }
        }
    }
    delete[] anahtarlar;
    return false;
}


list<string> Graph::getBFSPath(const Node &srcNode, const Node &destNode) {
    const int anahtarSayisi = komsuListeleri.Size();
    int *anahtarlar = new int[anahtarSayisi];
    komsuListeleri.getKeys(anahtarlar);

    HashTable<int, bool> ziyaretDurumlari;
    for (int i = 0; i < anahtarSayisi; ++i) {
        ziyaretDurumlari.Insert(anahtarlar[i], false);
    }
    delete[] anahtarlar;

    list<string> path;
    std::queue<int> simdikiDugumler;
    simdikiDugumler.push(srcNode.getVid());

    while (!simdikiDugumler.empty()) {
        int simdikiDugum = simdikiDugumler.front();
        simdikiDugumler.pop();

        if (ziyaretDurumlari.Get(simdikiDugum) == false) {
            ziyaretDurumlari.Get(simdikiDugum) = true;
            path.push_back(ulkeIsimleri.Get(simdikiDugum));
            if (simdikiDugum == destNode.getVid()) {
                return path;
            }

            list<Edge> &komsuListesi = komsuListeleri.Get(simdikiDugum);
            for (edgeListItr itr = komsuListesi.begin(); itr != komsuListesi.end(); ++itr) {
                int komsu = itr->getTailNode().getVid();
                if (ziyaretDurumlari.Get(komsu) == false) {
                    simdikiDugumler.push(komsu);
                }
            }
        }
    }
}


bool Graph::keydenBaslaDFSCycleVarMi(int key, HashTable<int, dugumDurumu> &dugumDurumlari) const {
    if (dugumDurumlari.Get(key).ziyaretEdildi) {
        return false;
    }
    if (dugumDurumlari.Get(key).kesfediliyor) {
        return true;
    }

    dugumDurumlari.Get(key).kesfediliyor = true;
    list<Edge> &komsuListesi = komsuListeleri.Get(key);
    for (edgeListItr itr = komsuListesi.begin(); itr != komsuListesi.end(); itr++) {
        int komsu = itr->getTailNode().getVid();
        if (komsu != key && !dugumDurumlari.Get(komsu).ziyaretEdildi) {
            if (keydenBaslaDFSCycleVarMi(komsu, dugumDurumlari)) {
                return true;
            }
        }
    }
    dugumDurumlari.Get(key).kesfediliyor = false;
    dugumDurumlari.Get(key).ziyaretEdildi = true;
    return false;
}