
#include <unoredered_map>
#include <vector>

struct Key{
	std::string name;
	Node* locations;
}

struct Node{
	std::string name;
	std::vector<const Key*> acsessToKeys; // If A -> B && A has Key => B has Key

	bool visited; //it's for when 
	
	Node(const string& name) : name(name), visited(false) {}
}

//template <class NodeType, class EdgeType>
class GameMap{
	typedef std::pair<Node, const Key*> Edge;
	
	typedef std::unordered_map<Node, Edge> EdgeMap;
	typedef std::unordered_map<Node, EdgeMap> NodeMap;
	
	typedef typename NodeMap::iterator NodeIter;
	typedef typename EdgesMap::iterator EdgeIter;

	std::unordered_map<Node> nodes;
	

	NodeMap nodes;
	size_t nodeCnt;
	size_t edgeCnt;
	
public:
	GameMap() : nodeCnt(0), edgeCnt(0) {};
	
	void addNode(const Node& node);
	bool removeNode(const Node& node);
	
	bool addEdge(const Node& from, const Key* key, const Node& to);
	// It's not a multygraph
	bool removeEdge(const Node& from, const Node& to);
	
	void traverse(const Node& start);
	
	
}

void GameMap::addNode(const Node& node){
	NodeIter it = nodes.find(node);
	if(it == nodes.end()){
		nodes.insert(std::make_pair(node, EdgeMap());
		++nodeCnt;
	}
}


bool GameMap::removeNode(const Node& node){
		NodeIter nIt = nodes.find(node);
		if(nIt == nodes.end()) return false;
	
		// The edges from the deleted node
		int removedEdges = nIt->second.size();
	
		for(NodeIter it = nodes.begin();
			it != nodes.end();
			++it){
				
			EdgeMap& adjacent = it->second();
			EdgeIter edgeIt = adjeacent.find(node);
			
			if(edgeIt != adjecent.end()){
				adjeacent.erase(node);
				//the edges going into the deleted node
				++removedEdges;
			}
		}
		
		edgeCnt -= removedEdges;
		
		nodes.erase(node);
		--nodeCnt;
		
		return true;
}

bool GameMap::addEdge(const Node& from, const Key* key, const Node& to){
	
	NodeIter fromIt = nodes.find(from);
	if(fromIt == nodes.end()) return false;
	
	NodeIter toIt = nodes.find(to);
	if(toIt == nodes.end()) return false;
	
	fromIt->second.insert(std::make_pair(to, key));
	//fromIt->second[to] = key;
	++edgeCnt;	
	
	return true;
}

bool GameMap::removeEdge(const Node& from, const Node& to){
	NodeIter fromIt = nodes.find(from);
	if(fromIt == nodes.end()) return false;
	
	NodeIter toIt = nodes.find(to);
	if(toIt == nodes.end()) return false;
	
	//check!!!
	return fromIt->second.erase(to) ? --edgeCnt : false;

}




