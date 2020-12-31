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
    HashTable<int, MesafeVeOncekiDugum> dugumTablosu;

    int *anahtarlar = new int[komsuListeleri.Size()];
    komsuListeleri.getKeys(anahtarlar);

    for (int i = 0; i < komsuListeleri.Size(); ++i) {
        dugumTablosu.Insert(anahtarlar[i], 
            MesafeVeOncekiDugum(std::numeric_limits<int>::max(), -1));
    }
    dugumTablosu.Get(srcNode.getVid()).mesafe = 0;

    std::priority_queue<MesafeVeOncekiDugum, 
        std::vector<MesafeVeOncekiDugum>, MesafeVeOncekiDugum::Kiyasla> pq;


    pq.push(MesafeVeOncekiDugum(0, srcNode.getVid()));


    /* Looping till priority queue becomes empty (or all
    distances are not finalized) */
    while (!pq.empty()) {
        // The first vertex in pair is the minimum distance 
        // vertex, extract it from priority queue. 
        // vertex label is stored in second of pair (it 
        // has to be done this way to keep the vertices 
        // sorted distance (distance must be first item 
        // in pair) 
        int u = pq.top().oncekiDugum;
        pq.pop();

        if (destNode.getVid() == u) {
            break;
        }


        // 'i' is used to get all adjacent vertices of a vertex 
        edgeListItr i;
        edgeListItr iSon = komsuListeleri.Get(u).end();


        for (i = komsuListeleri.Get(u).begin(); i != iSon; ++i) {
            // Get vertex label and weight of current adjacent 
            // of u. 
            int v = i->getTailNode().getVid();
            long kenarUzunlugu = i->getImport();

            

            //  If there is shorted path to v through u.
            long &vUzakligi = dugumTablosu.Get(v).mesafe;
            int &vOncekiDugum = dugumTablosu.Get(v).oncekiDugum;
            long &uUzakligi = dugumTablosu.Get(u).mesafe;
            if (vUzakligi > uUzakligi + kenarUzunlugu) {

                vUzakligi = uUzakligi + kenarUzunlugu;
                vOncekiDugum = u;

                pq.push(MesafeVeOncekiDugum(dugumTablosu.Get(v).mesafe, v));
            }
        }
    }

    list<string> path;
    int dugum;
    for (dugum = destNode.getVid(); dugumTablosu.Get(dugum).oncekiDugum != -1; dugum = dugumTablosu.Get(dugum).oncekiDugum) {
        path.push_front(ulkeIsimleri.Get(dugum));
    }
    path.push_front(ulkeIsimleri.Get(dugum));

    delete[] anahtarlar;
    return path;
}


bool Graph::isCyclic() {
    // bu algoritma muhtemelen 2 dugum birbirine point ediyorsa bunu da cycle kabul ediyor.
    int *anahtarlar = new int[komsuListeleri.Size()];
    komsuListeleri.getKeys(anahtarlar);

    HashTable<int, dugumDurumu> dugumDurumlari;

    for (int i = 0; i < komsuListeleri.Size(); ++i) {
        dugumDurumlari.Insert(anahtarlar[i], dugumDurumu());
    }

    for (int i = 0; i < komsuListeleri.Size(); ++i) {
        if (!dugumDurumlari.Get(anahtarlar[i]).ziyaretEdildi) {
            if (dfsCycleDetect(anahtarlar[i], dugumDurumlari)) {
                delete[] anahtarlar;
                return true;
            }
        }
    }
    delete[] anahtarlar;
    return false;


    
}


list<string> Graph::getBFSPath(const Node &srcNode, const Node &destNode) {
    HashTable<int, bool> ziyaretDurumlari;
    int *anahtarlar = new int[komsuListeleri.Size()];
    komsuListeleri.getKeys(anahtarlar);

    for (int i = 0; i < komsuListeleri.Size(); ++i) {
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
            edgeListItr itr = komsuListesi.begin();
            while (itr != komsuListesi.end()) {
                int komsu = itr->getTailNode().getVid();
                if (komsu == 1) {
                    int a = 0;
                    komsu+= a;
                }

                if (ziyaretDurumlari.Get(komsu) == false) {
                    simdikiDugumler.push(komsu);

                }
                itr++;
            }
        }

    }

}


bool Graph::dfsCycleDetect(int key, HashTable<int, dugumDurumu> & dugumDurumlari) {
    if (dugumDurumlari.Get(key).ziyaretEdildi) {
        return false;
    }
    if (dugumDurumlari.Get(key).kesfediliyor) {
        return true;
    }

    dugumDurumlari.Get(key).kesfediliyor = true;

    list<Edge> &komsuListesi = komsuListeleri.Get(key);
    edgeListItr itr = komsuListesi.begin();
    while (itr != komsuListesi.end()) {
        int komsu = itr->getTailNode().getVid();
        if (komsu != key) {
            if (!dugumDurumlari.Get(komsu).ziyaretEdildi) {
                if (dfsCycleDetect(komsu, dugumDurumlari)) {
                    return true;
                }
            }
        }
        itr++;
        
    }
    dugumDurumlari.Get(key).kesfediliyor = false;
    dugumDurumlari.Get(key).ziyaretEdildi = true;


    return false;
}