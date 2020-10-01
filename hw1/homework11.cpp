#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <type_traits>
#include <queue>
#include <map>
#include <algorithm>

using namespace std;

enum class Act : int
{
	x_plus = 1,
	x_minus = 2,
	y_plus = 3,
	y_minus = 4,
	z_plus = 5,
	z_minus = 6,
	x_plus_y_plus = 7,
	x_plus_y_minus = 8,
	x_minus_y_plus = 9,
	x_minus_y_minus = 10,
	x_plus_z_plus = 11,
	x_plus_z_minus = 12,
	x_minus_z_plus = 13,
	x_minus_z_minus = 14,
	y_plus_z_plus = 15,
	y_plus_z_minus = 16,
	y_minus_z_plus = 17,
	y_minus_z_minus = 18,
};

ostream& operator<< (std::ostream& output, Act act)
{
	output << static_cast<std::underlying_type<Act>::type>(act);
	return output;
}

class Vec3
{
public:
	unsigned int x;
	unsigned int y;
	unsigned int z;

	Vec3(int xi, int yi, int zi) 
		: x(xi), y(yi), z(zi)
	{

	}

	Vec3(const Vec3& v) 
		: x(v.x), y(v.y), z(v.z)
	{
	}

	Vec3() 
		: x(-1), y(-1), z(-1)
	{
	}

	bool operator==(const Vec3& other) const
	{
		if (this->x != other.x || this->y != other.y || this->z != other.z)
		{
			return false;
		}
		else
		{
			return true;
		}
	}

	bool operator!=(const Vec3& other) const
	{
		if (this->x != other.x || this->y != other.y || this->z != other.z)
		{
			return true;
		}
		else
		{
			return false;
		}
	}

	bool operator<(const Vec3& other) const
	{
		if (this->x < other.x)
			return true;
		else if (this->x == other.x && this->y < other.y)
			return true;
		else if (this->x == other.x && this->y == other.y && this->z < other.z)
			return true;
		else
			return false;
	}

	friend istream& operator>>(istream& input, Vec3& v) {
		input >> v.x >> v.y >> v.z;
		return input;
	}

	friend ostream& operator<<(ostream& output, const Vec3& v) {
		output << "(" << v.x << ", " << v.y << ", " << v.z << ")";
		return output;
	}
};

class Node
{
public:
	Vec3 point;
	Vec3 pre;
	vector<Act> acts;
	unsigned int cost;
	unsigned int future;
	bool visited;

	Node()
		: point(-1, -1, -1), pre(-1, -1, -1), acts({}), cost(-1), future(-1), visited(false)
	{

	}

	friend ostream& operator<<(ostream& output, const Node& n) {
		output << "Point: (" << n.point.x << ", " << n.point.y << ", " << n.point.z <<
			")  Pre: (" << n.pre.x << ", " << n.pre.y << ", " << n.pre.z <<
			")  Cost: " << n.cost << "  future: " << n.future << "  visited: " << n.visited;
		return output;
	}

	Vec3 PointAfterAct(Act act) 
	{
		Vec3 v(this->point);
		switch (act)
		{
		case Act::x_plus:
			++v.x;
			break;
		case Act::x_minus:
			--v.x;
			break;
		case Act::y_plus:
			++v.y;
			break;
		case Act::y_minus:
			--v.y;
			break;
		case Act::z_plus:
			++v.z;
			break;
		case Act::z_minus:
			--v.z;
			break;
		case Act::x_plus_y_plus:
			++v.x;
			++v.y;
			break;
		case Act::x_plus_y_minus:
			++v.x;
			--v.y;
			break;
		case Act::x_minus_y_plus:
			--v.x;
			++v.y;
			break;
		case Act::x_minus_y_minus:
			--v.x;
			--v.y;
			break;
		case Act::x_plus_z_plus:
			++v.x;
			++v.z;
			break;
		case Act::x_plus_z_minus:
			++v.x;
			--v.z;
			break;
		case Act::x_minus_z_plus:
			--v.x;
			++v.z;
			break;
		case Act::x_minus_z_minus:
			--v.x;
			--v.z;
			break;
		case Act::y_plus_z_plus:
			++v.y;
			++v.z;
			break;
		case Act::y_plus_z_minus:
			++v.y;
			--v.z;
			break;
		case Act::y_minus_z_plus:
			--v.y;
			++v.z;
			break;
		case Act::y_minus_z_minus:
			--v.y;
			--v.z;
			break;
		default:
			cout << "wrong type!" << endl;
			break;
		}
		return v;
	}
};

unsigned int ABS(unsigned a, unsigned b)
{
	return (a > b ? (a - b) : (b - a));
}

// Global data structure
map<Vec3, Node> m;

// BFS
bool BFS(Node& Nentrace, const Vec3& exit)
{	
	queue<Node> q;
	// init queue
	q.push(Nentrace);
	Nentrace.visited = true;
	unsigned int cost = 0;

	// loop
	while (!q.empty())
	{
		int size = q.size();
		for (int i = 0; i < size; i++) 
		{
			Node& Ncur = q.front();
			m[Ncur.point].cost = cost;

			// cout << "Ncur: " << Ncur.point << " " << Ncur.visited << endl;

			if (Ncur.point == exit)
			{
				// return success with all the parameters needed
				// cout << "SUCCESS: " << cost << endl;
				return true;
			}
			
			for (Act a : Ncur.acts)
			{
				Vec3 pointNext = Ncur.PointAfterAct(a);
				if (m.find(pointNext) == m.end())
				{
					continue;
				}
				Node& nodeNext = m.at(pointNext);
				if (!nodeNext.visited)
				{
					nodeNext.pre = Ncur.point; // only if not visited
					q.push(nodeNext);
					nodeNext.visited = true;
				}
			}
			
			q.pop();
		}
		++cost;
	}
	
	// cout << "FAIL" << endl;
	return false;
}


// UCS
bool UCS(Node& Nentrace, const Vec3& exit)
{
	auto cmp = [](Node nl, Node nr) {
		return nl.cost > nr.cost;
	};
	priority_queue<Node, vector<Node>, decltype(cmp)> q(cmp);

	// init queue
	Nentrace.cost = 0;
	Nentrace.visited = true;
	q.push(Nentrace);

	// loop
	while (!q.empty())
	{
		Node Ncur = q.top();
		if (Ncur.point == exit)
		{
			return true;
		}
		for (Act a : Ncur.acts)
		{
			Vec3 pointNext = Ncur.PointAfterAct(a);
			if (m.find(pointNext) == m.end())
			{
				continue;
			}
			Node& nodeNext = m.at(pointNext);
			if (!nodeNext.visited)
			{
				nodeNext.pre = Ncur.point; // only if not visited
				if (a == Act::x_minus || a == Act::x_plus || a == Act::y_minus || a == Act::y_plus || a == Act::z_minus || a == Act::z_plus)
				{
					nodeNext.cost = Ncur.cost + 10;
				}
				else
				{
					nodeNext.cost = Ncur.cost + 14;
				}
				q.push(nodeNext);
				nodeNext.visited = true;
			}
		}
		q.pop();
	}
	// cout << "FAIL" << endl;
	return false;
}

// A*
bool A_Star(Node& Nentrace, const Vec3& exit)
{
	auto cmp = [](Node nl, Node nr) {
		return (nl.cost + nl.future) > (nr.cost + nr.future);
	};
	priority_queue<Node, vector<Node>, decltype(cmp)> q(cmp);

	// define h(n)
	auto h = [&](Node& node, const Vec3& exit) -> unsigned int{
		unsigned int result = 0;
		vector<unsigned int> d{ ABS(node.point.x, exit.x), ABS(node.point.y, exit.y), ABS(node.point.z, exit.z) };
		sort(d.begin(), d.end());
		result += d[1] * 14;
		unsigned int t = d[2] - d[1];
		if (t > d[0])
		{
			result += (d[0] * 14);
			result += ((t - d[0]) * 10);
		}
		else
		{
			result += (t * 14);
			result += ((d[0] - t) * 10);
		}
		return result;
	};

	// init queue
	Nentrace.cost = 0;
	Nentrace.future = h(Nentrace, exit);
	Nentrace.visited = true;
	q.push(Nentrace);

	// loop
	while (!q.empty())
	{
		Node Ncur = q.top();
		if (Ncur.point == exit)
		{
			return true;
		}
		for (Act a : Ncur.acts)
		{
			Vec3 pointNext = Ncur.PointAfterAct(a);
			if (m.find(pointNext) == m.end())
			{
				continue;
			}
			Node& nodeNext = m.at(pointNext);
			if (!nodeNext.visited)
			{
				nodeNext.pre = Ncur.point; // only if not visited
				if (a == Act::x_minus || a == Act::x_plus || a == Act::y_minus || a == Act::y_plus || a == Act::z_minus || a == Act::z_plus)
				{
					nodeNext.cost = Ncur.cost + 10;
				}
				else
				{
					nodeNext.cost = Ncur.cost + 14;
				}
				nodeNext.future = h(nodeNext, exit);
				q.push(nodeNext);
				nodeNext.visited = true;
			}
		}
		q.pop();
	}
	// cout << "FAIL" << endl;
	return false;
}



int main()
{
	unsigned int X, Y, Z, N;
	Vec3 entrance, exit;
	string algo;

	// Deal with input
	ifstream fin("input.txt");

	fin >> algo >> X >> Y >> Z >> entrance >> exit >> N;
	fin.get(); // for '\n'
	
	for (int i = 0; i < N; i++)
	{
		Vec3 v;
		Node node;
		string s;
		getline(fin, s);
		istringstream ss(s);
		
		ss >> v;
		node.point = v;

		int tmp;
		while (ss >> tmp)
		{
			node.acts.push_back(Act(tmp));
		}
		m[v] = node;
	}

	// if exit does not exist in acts node
	if (m.find(exit) == m.end())
	{
		Node tmp;
		tmp.point = exit;
		m[exit] = tmp;
	}

	// Choose algo
	bool result = false;
	if (algo == "BFS")
	{
		/*
		cout << "\nList Map Before BFS: " << endl;
		for (auto p : m)
		{
			cout << "Pont: " << p.first << "  Node: " << p.second << endl;
		}
		cout << endl;
		*/
		result = BFS(m[entrance], exit);
		/*
		cout << "\nList Map After BFS: " << endl;
		for (auto p : m)
		{
			cout << "Pont: " << p.first << "  Node: " << p.second << endl;
		}
		cout << endl;
		*/
	}
	else if (algo == "UCS")
	{
		result = UCS(m[entrance], exit);
	}
	else if (algo == "A*")
	{
		/*
		cout << "\nList Map Before A*: " << endl;
		for (auto p : m)
		{
			cout << "Point: " << p.first << "  Node: " << p.second << endl;
		}
		cout << endl;
		*/
		result = A_Star(m[entrance], exit);
		/*
		cout << "\nList Map After A*: " << endl;
		for (auto p : m)
		{
			cout << "Point: " << p.first << "  Node: " << p.second << endl;
		}
		cout << endl;
		*/
	}
	else
	{
		cout << "input error" << endl;
	}

	// Deal with output
	ofstream fout("output.txt");
	if (result)
	{
		fout << m[exit].cost << '\n';

		vector<Vec3> v;
		v.push_back(exit);
		while (v.back() != entrance)
		{
			v.push_back(Vec3(m[v.back()].pre));
		}

		fout << v.size() << '\n';
		fout << entrance.x << ' ' << entrance.y << ' ' << entrance.z << ' ' << 0 << '\n';
		for (int i = v.size() - 2; i >= 0; i--)
		{
			fout << v[i].x << ' ' << v[i].y << ' ' << v[i].z << ' ' << (m[v[i]].cost - m[v[i + 1]].cost) << '\n';
		}
	}
	else
	{
		fout << "FAIL";
	}
	fout.close();


	// system("pause");
	return 0;
}