#include "display.hpp"
#include <iostream>

static constexpr ContextIndex npos = ContextIndex(-1);

static std::string set_to_str(const std::set<unsigned>& s) {
    std::string out = "{";
    bool first = true;
    for (auto v : s) {
        if (!first) out += ",";
        out += std::to_string(v);
        first = false;
    }
    out += "}";
    return out;
}

void visualize(Context& ctx, const std::string& filename) {
    std::ofstream dotFile(filename);
    dotFile << "digraph AST {\n";
    dotFile << "  node [shape=box, fontname=\"Courier\"];\n";
    dotFile << "  edge [arrowhead=none];\n\n";

    int counter = 0;

    // вспомогательная функция
    std::function<int(ContextIndex)> generateDot = [&](ContextIndex index) -> int {
        if (index == ContextIndex(-1)) return -1;

        const SyntaxNode& node = ctx.nodes[index];
        int currentId = counter++;

        std::string label;
        switch (node.type) {
            case NodeType::CHAR:
                label = "CHAR: '" + std::string(1, node.value) + "'";
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

        dotFile << "  " << currentId << " [label=\"" << label << "\"];\n";

        if (node.left != ContextIndex(-1)) {
            int leftId = generateDot(node.left);
            dotFile << "  " << currentId << " -> " << leftId << " [label=\"L\"];\n";
        }
        if (node.right != ContextIndex(-1)) {
            int rightId = generateDot(node.right);
            dotFile << "  " << currentId << " -> " << rightId << " [label=\"R\"];\n";
        }

        return currentId;
    };

    // Стартуем с корня — это должен быть последний добавленный узел
    if (!ctx.nodes.empty()) {
        generateDot(ctx.nodes.size() - 1);
    }

    dotFile << "}\n";
    dotFile.close();

    // Генерация картинки через Graphviz
    std::string command = "dot -Tpng " + filename + " -o " + filename + ".png";
    system(command.c_str());
}


// Расширённая версия с first/last/nullable/followpos/symbols
void generateDotAdvanced(const Context& ctx, std::ofstream& dotFile) {
    // Сначала выведем все узлы
    for (size_t i = 0; i < ctx.nodes.size(); ++i) {
        const auto& node = ctx.nodes[i];

        std::string label;
        switch (node.type) {
            case NodeType::CHAR:
                label = "CHAR: '" + std::string(1, node.value) + "'";
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

        label += "\\nnullable: " + std::string(node.is_nullable ? "true" : "false");
        label += "\\nnumber: " + std::to_string(node.number);

        auto set_to_str = [](const std::set<unsigned>& s) {
            std::string out = "{";
            bool first = true;
            for (auto v : s) {
                if (!first) out += ",";
                out += std::to_string(v);
                first = false;
            }
            out += "}";
            return out;
        };

        label += "\\nfirst: " + set_to_str(node.first_pos);
        label += "\\nlast: " + set_to_str(node.last_pos);

        dotFile << "  " << i << " [label=\"" << label << "\"];\n";
    }

    // Теперь выведем связи между узлами
    for (size_t i = 0; i < ctx.nodes.size(); ++i) {
        const auto& node = ctx.nodes[i];
        if (node.left != static_cast<size_t>(-1)) {
            dotFile << "  " << i << " -> " << node.left << " [label=\"L\"];\n";
        }
        if (node.right != static_cast<size_t>(-1)) {
            dotFile << "  " << i << " -> " << node.right << " [label=\"R\"];\n";
        }
    }
}


void visualize_with_sets(const Context& ctx, const std::string& filename) {
    std::ofstream dotFile(filename);
    dotFile << "digraph SyntaxTree {\n";
    dotFile << "  node [shape=box, fontname=\"Courier\", fontsize=10];\n";
    dotFile << "  edge [arrowhead=none];\n\n";

    generateDotAdvanced(ctx, dotFile);

    // Добавляем followpos таблицу
    dotFile << "\n  subgraph cluster_followpos {\n";
    dotFile << "    label=\"Followpos Table\";\n";
    dotFile << "    fontsize=12;\n";
    dotFile << "    style=filled;\n";
    dotFile << "    color=lightgrey;\n";
    dotFile << "    node [shape=plaintext];\n";

    dotFile << "    follow_table [label=<\n";
    dotFile << "      <table border=\"1\" cellborder=\"1\" cellspacing=\"0\" cellpadding=\"4\">\n";
    dotFile << "        <tr><td><b>Node #</b></td><td><b>Followpos</b></td></tr>\n";

    auto set_to_str = [](const std::set<unsigned>& s) {
        std::string out = "{";
        bool first = true;
        for (auto v : s) {
            if (!first) out += ",";
            out += std::to_string(v);
            first = false;
        }
        out += "}";
        return out;
    };

    for (const auto& [num, fset] : ctx.followpos) {
        dotFile << "        <tr><td>" << num << "</td><td>" << set_to_str(fset) << "</td></tr>\n";
    }

    dotFile << "      </table>\n";
    dotFile << "    >];\n";
    dotFile << "  }\n";

    // Добавляем symbols таблицу
    dotFile << "\n  subgraph cluster_symbols {\n";
    dotFile << "    label=\"Symbols Table\";\n";
    dotFile << "    fontsize=12;\n";
    dotFile << "    style=filled;\n";
    dotFile << "    color=lightgrey;\n";
    dotFile << "    node [shape=plaintext];\n";

    dotFile << "    symbols_table [label=<\n";
    dotFile << "      <table border=\"1\" cellborder=\"1\" cellspacing=\"0\" cellpadding=\"4\">\n";
    dotFile << "        <tr><td><b>Symbol</b></td><td><b>Node #s</b></td></tr>\n";

    for (const auto& [symbol, nodes] : ctx.symbols) {
        char opt = symbol == '\0' ? '$' : symbol;
        dotFile << "        <tr><td>" << opt << "</td><td>" << set_to_str(nodes) << "</td></tr>\n";
    }

    dotFile << "      </table>\n";
    dotFile << "    >];\n";
    dotFile << "  }\n";

    dotFile << "}\n";
    dotFile.close();

    std::string command = "dot -Tpng " + filename + " -o " + filename + ".png";
    system(command.c_str());
}



void dump_dfa_dot(const DFA& dfa, std::ostream& out) {
    out << "digraph DFA {\n";
    out << "  rankdir=LR;\n";

    // Принимающие состояния
    out << "  node [shape = doublecircle];\n";
    for (unsigned state : dfa.accepting_states) {
        if (state == DFA::null_state) {
            out << "  null;\n";  // null_state — особый случай
        } else {
            out << "  " << state << ";\n";
        }
    }

    // Остальные состояния
    out << "  node [shape = circle];\n";

    // Стартовая стрелка
    out << "  start [shape=point];\n";
    out << "  start -> " << dfa.start_state << ";\n";

    // Узел null_state
    out << "  null [label=\"⊘\", style=dashed";
    if (!dfa.accepting_states.count(DFA::null_state)) {
        out << ", shape=circle";  // непринимающий null_state — обычный кружок
    }
    out << "];\n";

    // Переходы
    for (size_t from = 0; from < dfa.transitions.size(); ++from) {
        for (const auto& [symbol, to] : dfa.transitions[from]) {
            std::string to_state = (to == DFA::null_state) ? "null" : std::to_string(to);
            out << "  " << from << " -> " << to_state << " [label=\"" << symbol << "\"];\n";
        }
    }

    out << "}\n";
}
