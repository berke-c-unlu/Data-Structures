

#include "Graph.h"

/*Do not add new libraries or files*/


Graph::Graph() {

    adjList = HashTable<string, list<Edge>>(); 
    nodeList = HashTable<int, pair<Node, bool>>();
    cycleList = HashTable<int, bool>();
    cost = HashTable<int, Weight>();
    prev = HashTable<int, Node>();
}

Graph::Graph(const Graph& rhs) {

    *this = rhs;
}

Graph& Graph::operator=(const Graph& rhs) {

    if (this != &rhs) {
        adjList.~HashTable();
        nodeList.~HashTable();
        cycleList.~HashTable();
        cost.~HashTable();

        this->nodeList = rhs.nodeList;
        this->adjList = rhs.adjList;
        this->cycleList = rhs.cycleList;
        this->cost = rhs.cost;
    }
    return *this;
}

Graph::~Graph() {

    adjList.~HashTable();
    nodeList.~HashTable();
    cycleList.~HashTable();
    cost.~HashTable();
    prev.~HashTable();
}


void Graph::addNode(const Node& node) {

    list<Edge> adj = list<Edge>();
    adjList.Insert(node.getCountry(), adj);

    nodeList.Insert(node.getVid(), make_pair(node, false));

    cycleList.Insert(node.getVid(), false);

    Weight x = Weight();
    x.node = node;
    cost.Insert(node.getVid(), x);

    prev.Insert(node.getVid(), Node());
}

void Graph::addConnection(const Node& headNode, const Node& tailNode, int import) {

    Edge connection = Edge(tailNode, import);
    adjList.Get(headNode.getCountry()).push_back(connection);

}

list<Node> Graph::getAdjacentNodes(const Node& node) {

    list<Node> adjacents = list<Node>();

    auto iter = adjList.Get(node.getCountry()).begin();
    for (iter = adjList.Get(node.getCountry()).begin(); iter != adjList.Get(node.getCountry()).end(); iter++) {
        adjacents.push_back(iter->getTailNode());
    }

    return adjacents;
}

long Graph::getTotalImports(const Node& node) {

    long sum = 0;

    list<Edge> List = adjList.Get(node.getCountry());

    auto iter = List.begin();
    for (iter = List.begin(); iter != List.end(); iter++) {
        sum += iter->getImport();
    }

    return sum;
}

void Graph::deleteNode(const Node& node) {

    string* countries = new string[adjList.Size()]; // allocate memory for countries
    adjList.getKeys(countries); // active countries

    for (int i = 0; i < adjList.Size(); i++) { // for every country

        auto iter = adjList.Get(countries[i]).begin();
        for (iter = adjList.Get(countries[i]).begin(); iter != adjList.Get(countries[i]).end(); iter++) { // iterate through adj list

            if (iter->getTailNode().getCountry() == node.getCountry()) {
                adjList.Get(countries[i]).erase(iter);
                break;
            }
        }
    }

    adjList.Delete(node.getCountry());
    nodeList.Delete(node.getVid());
    cycleList.Delete(node.getVid());
    cost.Delete(node.getVid());
    prev.Delete(node.getVid());
    delete[] countries;
}

list<string> Graph::findLeastCostPath(const Node& srcNode, const Node& destNode) {
    // TODO: IMPLEMENT THIS FUNCTION.

    //Weights have  nodes with inf weights
    HashTable<int, Weight> old = cost;
    HashTable<int, Node> old_previous = prev;
    priority_queue<Weight, std::vector<Weight>, WeightComp> pq; // priority queue, min value is at the top
    list<string> path = list<string>(); // path list

    Weight source = cost.Get(srcNode.getVid()); // source pair
    source.weight = 0;
    pq.push(source); // insert it into priority queue

    while (pq.size() > 0) { // while pq is not empty

        Weight min = pq.top();
        pq.pop();

        auto it = adjList.Get(min.node.getCountry()).begin(); // iterator for adjacency list 
        for (it; it != adjList.Get(min.node.getCountry()).end(); it++) {

            Node current = it->getTailNode();
            long currDist = it->getImport();

            long distance = min.weight;
            long* oldDist = &(cost.Get(current.getVid()).weight);

            if (*oldDist > currDist + distance) {
                *oldDist = currDist + distance;
                prev.Get(current.getVid()) = min.node;
                Weight x;
                x.node = current;
                x.weight = *oldDist;
                pq.push(x);
            }
        }
    }

    Node curr = destNode;
    while (curr.getVid() != srcNode.getVid()) {
        string country = prev.Get(curr.getVid()).getCountry();
        path.push_front(country);
        curr = prev.Get(curr.getVid());
    }

    path.push_back(destNode.getCountry());
    cost = old;
    prev = old_previous;
    return path;
}


bool Graph::isCyclic() {

    bool flag = false;
    vector<int> s;
    int len = cycleList.Size();
    int* IDs = new int[len];
    cycleList.getKeys(IDs);

    HashTable<int, bool> copy = cycleList;
    for (int j = 0; j < len; j++) {
        if (isCyclicHelper(s, cycleList, IDs[j])) {
            flag = true;
            break;
        }
        cycleList = copy;
    }
    delete[] IDs;
    cycleList = copy;
    return flag;
}


list<string> Graph::getBFSPath(const Node& srcNode, const Node& destNode) {

    queue<Node> nodeQ; // queue for BFS
    list<string> path;  // BFS PATH
    Node myNode;
    HashTable<int, pair<Node, bool>> old = nodeList;

    nodeQ.push(srcNode); // srcNode pushed to the queue
    nodeList.Get(srcNode.getVid()).second = true; // marked as visited in another hashtable
    path.push_back(srcNode.getCountry());
    if (srcNode.getCountry() == destNode.getCountry()) {
        nodeList = old;
        return path;
    }
        
    while (nodeQ.size() > 0) { // while queue is not empty

        myNode = nodeQ.front(); // node at the front of the queue
        nodeQ.pop(); // front pop

        auto it = adjList.Get(myNode.getCountry()).begin(); // iterator through adjacent list

        for (it; it != adjList.Get(myNode.getCountry()).end(); it++) {
            if (it->getTailNode().getCountry() == destNode.getCountry()) { // if we found destination
                nodeList.Get(it->getTailNode().getVid()).second = true; // mark it true
                path.push_back(it->getTailNode().getCountry()); // push to the path list
                return path; // return since we found
            }
            if (nodeList.Get(it->getTailNode().getVid()).second == false) { // if the node is not visited
                nodeList.Get(it->getTailNode().getVid()).second = true; // mark it true
                nodeQ.push(it->getTailNode()); // push to the queue
                path.push_back(it->getTailNode().getCountry()); // push to the list
            }
        }
    }
    nodeList = old;
    return path;
}

bool Graph::isCyclicHelper(vector<int>& s, HashTable<int, bool>& cycleList, const int& id) {
    bool visited = cycleList.Get(id);
    if (!visited) { // if not visited
        //mark it as visited
        cycleList.Get(id) = true;
        //activate stack
        s.push_back(id);
        // adj list of corresponding id's node in cycleList

        list<Node> adjs = getAdjacentNodes(nodeList.Get(id).first);
        auto it = adjs.begin();
        for (it; it != adjs.end(); it++) {

            int currID = it->getVid();
            bool currVis = cycleList.Get(currID);

            if (!currVis && isCyclicHelper(s, cycleList, currID) && currID != id) {
                return true;
            }
            for (int i = 0; i < s.size(); i++) {
                if (s[i] == currID && currID != id) {
                    return true;
                }
            }
        }
    }
    auto it = s.begin();
    for (it; it != s.end(); it++) {
        if (*it == id) {
            s.erase(it);
            break;
        }
    }
    return false;
}