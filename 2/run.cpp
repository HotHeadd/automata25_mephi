#include <iostream>
#include "myre.hpp"
#include <fstream>
#include <set>
#include <cstdlib>

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

	auto set_to_string = [](const std::set<unsigned>& s) {
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

void visualize_with_sets(const std::shared_ptr<SyntaxNode>& root, const std::string& filename, Context& context) {
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

    for (const auto& [num, follow] : context.followpos) {
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

    // Добавим таблицу symbols
    dotFile << "\n  subgraph cluster_symbols {\n";
    dotFile << "    label=\"Symbols Table\";\n";
    dotFile << "    fontsize=12;\n";
    dotFile << "    style=filled;\n";
    dotFile << "    color=lightgrey;\n";
    dotFile << "    node [shape=plaintext];\n";

    dotFile << "    symbols_table [label=<\n";
    dotFile << "      <table border=\"1\" cellborder=\"1\" cellspacing=\"0\" cellpadding=\"4\">\n";
    dotFile << "        <tr><td><b>Symbol</b></td><td><b>Node #s</b></td></tr>\n";

    for (const auto& [symbol, nodes] : context.symbols) {
		char opt = symbol;
		if (symbol == '\0'){
			opt = '$';
		}
        dotFile << "        <tr><td>" << opt << "</td><td>{";
        bool first = true;
        for (auto node : nodes) {
            if (!first) dotFile << ", ";
            dotFile << node;
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


void dump_dfa_dot(const DFA& dfa, std::ostream& out = std::cout) {
    out << "digraph DFA {\n";
    out << "  rankdir=LR;\n";

    // Принимающие состояния
    out << "  node [shape = doublecircle];\n";
    for (unsigned state : dfa.accepting_states) {
        out << "  " << state << ";\n";
    }

    out << "  node [shape = circle];\n";

    // Стартовая стрелка
    out << "  start [shape=point];\n";
    out << "  start -> " << dfa.start_state << ";\n";

    // Переходы
    for (size_t from = 0; from < dfa.transitions.size(); ++from) {
        for (const auto& t : dfa.transitions[from]) {
            out << "  " << from << " -> " << t.to << " [label=\"" << t.symbol << "\"];\n";
        }
    }

    out << "}\n";
}


int main(){
	std::string test = "a{2,3}{0,2}{3,4}*";

	// TODO: test new searches

	RegexParser parser;
	Context context;

	std::shared_ptr<SyntaxNode> node = parser.parse(test, context);
	visualize(node, "visuals/ast.dot");
	visualize_with_sets(node, "visuals/ast_plus.dot", context);

	DFABuilder builder;
	DFA dfa = builder.buildDFA(node, context);
	std::ofstream dotFile("visuals/automaton.dot");
	dump_dfa_dot(dfa, dotFile);
	
	DFA min_dfa = builder.minimize_dfa(dfa, context);
	std::ofstream minfile("visuals/minimal.dot");\
	dump_dfa_dot(min_dfa, minfile);
	std::string expr = "vvv";
	if (search(expr, dfa)){
		std::cout << "\nTRUE\n";
	}
	else{
		std::cout << "\nFALSE\n";
	}
	return 0;
}