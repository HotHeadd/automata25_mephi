#include <iostream>
#include "myre.hpp"
#include <fstream>

using namespace myre;

int generateDot(const std::shared_ptr<SyntaxNode>& node, std::ofstream& dotFile, int& counter) {
	if (!node) return -1;

	int currentId = counter++;
	std::string label;

	switch (node->type) {
		case NodeType::CHAR:
			label = std::string("CHAR: '") + node->value + "'";
			break;
		case NodeType::CONCAT:
			label = "CONCAT";
			break;
		case NodeType::OR:
			label = "OR";
			break;
		case NodeType::RANGE:
			label = "RANGE: " + std::to_string(node->start) + 
				   (node->finish == 0 ? "+" : 
				   ("-" + std::to_string(node->finish)));
			break;
		case NodeType::EOS:
			label = "$EOS$";
			break;
		case NodeType::EPSYLON:
			label = "EPSYLON";
			break;
	}

	dotFile << "  " << currentId << " [label=\"" << label << "\"];\n";

	if (node->left) {
		int leftId = generateDot(node->left, dotFile, counter);
		dotFile << "  " << currentId << " -> " << leftId << " [label=\"L\"];\n";
	}

	if (node->right) {
		int rightId = generateDot(node->right, dotFile, counter);
		dotFile << "  " << currentId << " -> " << rightId << " [label=\"R\"];\n";
	}

	return currentId;
}

void visualize(const std::shared_ptr<SyntaxNode>& root, const std::string& filename) {
	std::ofstream dotFile(filename);
	dotFile << "digraph AST {\n";
	dotFile << "  node [shape=box, fontname=\"Courier\"];\n";
	dotFile << "  edge [arrowhead=none];\n\n";

	int nodeCounter = 0;
	generateDot(root, dotFile, nodeCounter);

	dotFile << "}\n";
	dotFile.close();

	// Генерация PNG (требуется установленный Graphviz)
	std::string command = "dot -Tpng " + filename + " -o " + filename + ".png";
	system(command.c_str());
}

int main(){
	RegexParser parser;
	std::shared_ptr<SyntaxNode> node = parser.parse("(a|)(|)");
	visualize(node, "ast.dot");
	return 0;
}