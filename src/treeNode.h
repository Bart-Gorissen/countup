#ifndef TREENODE_H
#define TREENODE_H

#define DEFAULT_ADD_CHILDREN 2
#define DEFAULT_SUB_CHILDREN 2
#define DEFAULT_MULT_CHILDREN 2
#define DEFAULT_DIV_CHILDREN 2

#include <string>
#include <vector>
#include <exception>
#include <tuple>

#include "operation.h"

template <typename T>
class TreeNode {
protected:
    T value;
    OperationType operation;
    
public:
    TreeNode() : value(0), operation(OperationType::None) {}
    TreeNode(const TreeNode<T>& orig) {
        this->value = orig.value;
        this->operation = orig.operation;
    }
    virtual ~TreeNode() {};
    
    T getValue() { return value; }
    void setValue(T value) { this->value = value; }

    void setOperation(OperationType op) { this->operation = op; }

    virtual void compute() = 0;
    virtual bool incOperation() = 0;
    virtual std::vector<TreeNode<T>*> getLeafs() = 0;

    virtual std::vector<std::string> toStringVec() = 0;
    std::string toString() {
        std::vector<std::string> lines = toStringVec();
        std::string result;
        for (size_t i = 0; i < lines.size(); ++i) {
            result += lines[i];
            if (i + 1 < lines.size()) result += "\n";
        }
        return result;
    }
};

template <typename T>
class LeafNode : public TreeNode<T> {
public:
    LeafNode() : TreeNode<T>() {}
    LeafNode(T value) : TreeNode<T>() { this->value = value; }

    void compute () override {}
    bool incOperation() override { return true; }
    std::vector<TreeNode<T>*> getLeafs() override { return {this}; }
    std::vector<std::string> toStringVec() override { return {std::to_string(this->value)}; }
};

template <typename T>
class InternalNode : public TreeNode<T> {
protected:
    std::vector<TreeNode<T>*> children;

public:
    InternalNode() : TreeNode<T>() {}
    InternalNode(const InternalNode& orig) : TreeNode<T>(orig) {
        for (const auto& child: orig.children) {
            if (dynamic_cast<LeafNode<T>*>(child)) {
                children.push_back(new LeafNode<T>(*static_cast<LeafNode<T>*>(child)));
            } else if (dynamic_cast<InternalNode<T>*>(child)) {
                children.push_back(new InternalNode<T>(*static_cast<InternalNode<T>*>(child)));
            } else {
                throw std::logic_error("Unknown child node type");
            }
        }
    }
    ~InternalNode() override {
        for (auto child : children) delete child;
        children.clear();
    }
    void addChild(TreeNode<T>* child) { children.push_back(child); }
    void setOperation(OperationType op) { this->operation = op; }

    void compute() {
        std::vector<T> childValues;

        // compute for all children
        for (auto child : children) {
            child->compute();
            childValues.push_back(child->getValue());
        }

        switch (this->operation) {
            case OperationType::Addition:
                this->value = addition(childValues);
                break;
            case OperationType::Subtraction:
                this->value = subtraction(childValues);
                break;
            case OperationType::Multiplication:
                this->value = multiplication(childValues);
                break;
            case OperationType::Division:
                this->value = division(childValues);
                break;
            case OperationType::None:
            default:
                throw std::logic_error("Unknown operation");
                break;
        }
    }

    bool incOperation() {
        for (auto child : children) {
            if (!child->incOperation()) return false;
        }

        if (this->operation == OPERATION_TYPE_LAST) {
            this->operation = OPERATION_TYPE_FIRST;
            return true;
        }

        this->operation++;
        return false;
    }

    std::vector<TreeNode<T>*> getLeafs() {
        std::vector<TreeNode<T>*> leafs;
        for (auto child : children) {
            auto childLeafs = child->getLeafs();
            leafs.insert(leafs.end(), childLeafs.begin(), childLeafs.end());
        }
        return leafs;
    }

    std::vector<std::string> toStringVec() override {
        // get subtrees
        std::vector<std::vector<std::string>> subtrees;
        for (TreeNode<T>* child : children) {
            subtrees.push_back(child->toStringVec());
        }
        // get widths
        std::vector<size_t> widths(subtrees.size());
        size_t width = 0;
        size_t depth = 0;
        for (size_t i = 0; i < subtrees.size(); ++i) {
            widths[i] = subtrees[i][0].size();
            // for (const auto& line : subtrees[i]) widths[i] = std::max(widths[i], line.size());
            depth = std::max(depth, subtrees[i].size());
            width += widths[i] + 1;
        }

        std::vector<std::string> result(depth+1);
        std::string opLine(width-1, ' ');
        size_t pos = std::max((width / 2) - 1, (size_t)0);
        char opchar;
        switch(this->operation) {
            case OperationType::Addition: opchar = '+'; break;
            case OperationType::Subtraction: opchar = '-'; break;
            case OperationType::Multiplication: opchar = '*'; break;
            case OperationType::Division: opchar = '/'; break;
            case OperationType::None:
            default: opchar = '?'; break;
        }
        opLine[pos] = opchar;
        result[0] = opLine;

        for (size_t d = 0; d < depth; ++d) {
            result[d+1] = "";

            for (size_t i = 0; i < subtrees.size(); ++i) {
                if (d >= subtrees[i].size()) {
                    result[d+1] += std::string(widths[i] + 1, ' ');
                }
                else {
                    result[d+1] += (subtrees[i])[d] + ' ';
                }
            }
        }

        return result;
        }
    };

// template <typename T>
// class AddNode : public InternalNode<T> {
// public:
//     AddNode() : InternalNode<T>() {
//         this->children.reserve(DEFAULT_ADD_CHILDREN);
//     }
//     AddNode(T id) : InternalNode<T>() {
//         this->value = id;
//         this->children.reserve(DEFAULT_ADD_CHILDREN);
//     }
//     void compute() override {
//         for (auto child : this->children) {
//             child->compute();
//             this->value += child->getValue();
//         }
//     }
// };

// template <typename T>
// class SubNode : public InternalNode<T> {
// public:
//     SubNode(T id) : InternalNode<T>() {
//         this->value = id;
//         this->children.reserve(DEFAULT_SUB_CHILDREN);
//     }
//     void compute() override {
//         for (auto child : this->children) {
//             child->compute();
//             this->value -= child->getValue();
//         }
//     }
// };

// template <typename T>
// class MultNode : public InternalNode<T> {
// public:
//     MultNode(T id) : InternalNode<T>() {
//         this->value = id;
//         this->children.reserve(DEFAULT_MULT_CHILDREN);
//     }
//     void compute() override {
//         for (auto child : this->children) {
//             child->compute();
//             this->value *= child->getValue();
//         }
//     }
// };

// template <typename T>
// class DivNode : public InternalNode<T> {
// public:
//     DivNode(T id) : InternalNode<T>() {
//         this->value = id;
//         this->children.reserve(DEFAULT_DIV_CHILDREN);
//     }
//     void compute() override {
//         if (this->children.empty()) {
//             return;
//         }

//         this->value = (this->children)[0]->getValue();
//         for (size_t i = 1; i < (this->children).size(); ++i) {
//             (this->children)[i]->compute();
//             this->value /= (this->children)[i]->getValue();
//         }
//     }
// };

#endif