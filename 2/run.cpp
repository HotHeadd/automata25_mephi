#include <iostream>
#include "myre.hpp"
#include <fstream>
#include <set>

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
		case NodeType::KLEENE:
			label = "KLEENE";
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

int generateDotAdvanced(const std::shared_ptr<SyntaxNode>& node, std::ofstream& dotFile, int& counter) {
	if (!node) return -1;

	int currentId = counter++;
	std::string label;

	// Основной тип нода
	switch (node->type) {
		case NodeType::CHAR:
			label = "CHAR: '" + std::string(1, node->value) + "'";
			break;
		case NodeType::CONCAT:
			label = "CONCAT";
			break;
		case NodeType::OR:
			label = "OR";
			break;
		case NodeType::KLEENE:
			label = "KLEENE";
			break;
		case NodeType::EOS:
			label = "$EOS$";
			break;
		case NodeType::EPSYLON:
			label = "EPSYLON";
			break;
		default:
			label = "UNKNOWN";
	}

	// Добавим доп. информацию: number, nullable, first/last pos
	label += "\\nnullable: " + std::string(node->is_nullable ? "true" : "false");
	label += "\\nnumber: " + std::to_string(node->number);

	auto set_to_string = [](const std::unordered_set<unsigned>& s) {
		std::string str = "{";
		for (auto it = s.begin(); it != s.end(); ++it) {
			if (it != s.begin()) str += ",";
			str += std::to_string(*it);
		}
		str += "}";
		return str;
	};

	label += "\\nfirst: " + set_to_string(node->first_pos);
	label += "\\nlast: " + set_to_string(node->last_pos);

	dotFile << "  " << currentId << " [label=\"" << label << "\"];\n";

	if (node->left) {
		int leftId = generateDotAdvanced(node->left, dotFile, counter);
		dotFile << "  " << currentId << " -> " << leftId << " [label=\"L\"];\n";
	}

	if (node->right) {
		int rightId = generateDotAdvanced(node->right, dotFile, counter);
		dotFile << "  " << currentId << " -> " << rightId << " [label=\"R\"];\n";
	}

	return currentId;
}

void visualize_with_sets(const std::shared_ptr<SyntaxNode>& root, const std::string& filename) {
	std::ofstream dotFile(filename);
	dotFile << "digraph SyntaxTree {\n";
	dotFile << "  node [shape=box, fontname=\"Courier\", fontsize=10];\n";
	dotFile << "  edge [arrowhead=none];\n\n";

	int nodeCounter = 0;
	generateDotAdvanced(root, dotFile, nodeCounter);

	// Добавим таблицу followpos
	dotFile << "\n  subgraph cluster_followpos {\n";
	dotFile << "    label=\"Followpos Table\";\n";
	dotFile << "    fontsize=12;\n";
	dotFile << "    style=filled;\n";
	dotFile << "    color=lightgrey;\n";
	dotFile << "    node [shape=plaintext];\n";

	dotFile << "    follow_table [label=<\n";
	dotFile << "      <table border=\"1\" cellborder=\"1\" cellspacing=\"0\" cellpadding=\"4\">\n";
	dotFile << "        <tr><td><b>Node #</b></td><td><b>Followpos</b></td></tr>\n";

	for (const auto& [num, follow] : SetHandler::followpos) {
		dotFile << "        <tr><td>" << num << "</td><td>{";
		bool first = true;
		for (auto f : follow) {
			if (!first) dotFile << ", ";
			dotFile << f;
			first = false;
		}
		dotFile << "}</td></tr>\n";
	}

	dotFile << "      </table>\n";
	dotFile << "    >];\n";
	dotFile << "  }\n";

	dotFile << "}\n";
	dotFile.close();

	std::string command = "dot -Tpng " + filename + " -o " + filename + ".png";
	system(command.c_str());
}



int main(){
	RegexParser parser;
	std::string test = "d(l|d)*";
	std::list<std::shared_ptr<Token>> tokens = parser.tokenize(test);
	for (auto token : tokens){
		if (token->type == TokenType::LPAR){
			std::cout << "(\n";
		}
		if (token->type == TokenType::RPAR){
			std::cout << ")\n";
		}
		if (token->type == TokenType::KLEENE){
			std::cout << "*\n";
		}
		if (token->type == TokenType::OR){
			std::cout << "|\n";
		}
		if (token->type == TokenType::CHAR){
			std::cout << "char\n";
		
		}
		if (token->type == TokenType::CONCAT){
			std::cout << "concat\n";
		}
		if (token->type == TokenType::EPSYLON){
			std::cout << "eps\n";
		}
	}
	std::shared_ptr<SyntaxNode> node = parser.parse(test);
	visualize(node, "ast.dot");
	visualize_with_sets(node, "ast_plus.dot");
	return 0;
}