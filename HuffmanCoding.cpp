/* -------------------
	Author: Arxivory
   ------------------- */
#include <iostream>
#include <queue>
#include <vector>
#include <map>
#include <algorithm>

using namespace std;

class Node 
{
	public:
		string text;
		int frequency;
		Node *left, *right;

		Node() {}

		Node(string text, int frequency)
		{
			this->text = text;
			this->frequency = frequency;
			this->left = this->right = nullptr;
		}
};

void infixTraversal(Node *root)
{
	if (root != nullptr)
	{
		infixTraversal(root->left);
		cout << root ->text << ": " << root->frequency << ' ';
		infixTraversal(root->right);
	}
}

map<string, int> getFrequencies(string text)
{
	map<string, int> table;
	for (char character : text)
	{
		string str;
		str += character;
		table[str]++;
	}
	return table;
}

Node *buildTree(Node *x, Node* y)
{
	Node *root = new Node(x->text + y->text, x->frequency + y->frequency);
	root->left = x;
	root->right = y;
	return root;
}

bool compare(const Node *a, const Node *b)
{
	if (a->frequency == b->frequency)
	{
		return a->text > b->text;
	}
	return a->frequency > b->frequency;
}

void treeBuildingSteps(vector<Node *> &minHeap)
{
	for (int i = minHeap.size() - 1; i >= 0; i--)
	{
		cout << '{' << minHeap[i]->text << ", " << minHeap[i]->frequency << '}' << ' ';
	}
	cout << endl;
}

Node *buildHuffmanTree(vector<Node*> &minHeap, char buildSteps)
{
	Node *z;
	if (buildSteps == 'y') cout << "Tree Building Steps: " << endl;
	while (!minHeap.empty() && minHeap.size() > 1)
	{
		int n = minHeap.size();
		sort(minHeap.begin(), minHeap.end(), compare);

		if (buildSteps == 'y') treeBuildingSteps(minHeap);

		Node *x = minHeap[n - 1];
		Node *y = minHeap[n - 2];

		z = buildTree(x, y);
		minHeap.pop_back();
		minHeap.pop_back();

		minHeap.push_back(z);
	}
	return z;
}

Node *queuingProcess(map<string, int> &frequencyTable, char buildSteps)
{
	vector<Node*> minHeap;
	for (auto entry: frequencyTable)
	{
		Node *character = new Node(entry.first, entry.second);
		minHeap.push_back(character);
	}

	Node *tree = buildHuffmanTree(minHeap, buildSteps);

	return tree;
} 

bool codeHelper(Node *root, string character, vector<bool> &binaryCode)
{
	if (!root) return false;

	if (root->text == character) return true;

	if (codeHelper(root->left, character, binaryCode)) 
	{
		binaryCode.push_back(0);
		return true;
	}

	if (codeHelper(root->right, character, binaryCode)) 
	{
		binaryCode.push_back(1);
		return true;
	}

	if (!binaryCode.empty()) binaryCode.pop_back();	
	return false;
}

void presentStatistics(map<string, int> &frequencyTable, Node* huffmanTree)
{
	map<string, pair<int, pair<vector<bool>, int>>> characterCodes;
	
	for (auto entry : frequencyTable)
	{
		vector<bool> binaryCode;
		codeHelper(huffmanTree, entry.first, binaryCode);
		characterCodes[entry.first] = {entry.second, {binaryCode, binaryCode.size() * entry.second}};
	}

	int overallBits = 0;

	cout << "Table: " << endl;
	cout << "+----------+----------+----------+----------+" << endl;
	cout << "| Character| frequency|   code   | weighted |" << endl;
	cout << "+----------+----------+----------+----------+" << endl;
	for (auto entry : characterCodes)
	{
		cout << "|     " << entry.first << "    |    " << entry.second.first << "     |   ";
		for (int i = entry.second.second.first.size() - 1; i >= 0; i--)
		{
			cout << entry.second.second.first[i];
		}
		cout << "     |     " << entry.second.second.second << "   |" << endl;
		cout << "+----------+----------+----------+----------+" << endl;
		overallBits += entry.second.second.second;
	}
	
	cout << "Your Huffman Compression has overall " << overallBits << " bits." << endl;
}

int main()
{
	cout << "Welcome to Huffman Coding Calculator!" << endl <<
		"Please Input Your Text to Compress: " << endl;
	string text;
	cin >> text;

	if (text == "" || text == " ") cout << "You did not input any text." << endl;

	cout << "Do you want to show the step by step Tree Building? y/n" << endl;
	char buildSteps;
	cin >> buildSteps;
	
	map<string, int> frequencyTable = getFrequencies(text);
	Node *huffmanTree = queuingProcess(frequencyTable, buildSteps);

	presentStatistics(frequencyTable, huffmanTree);

	return 0;
}