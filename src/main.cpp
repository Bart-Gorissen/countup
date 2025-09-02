#include <iostream>
#include <vector>
#include <cmath>
#include <algorithm>

#include "operation.h"
#include "treeNode.h"

template<typename T>
std::vector<TreeNode<T>*> constructBinTrees(int nLeafs) {
    if (nLeafs < 1) return {};
    if (nLeafs == 1) return {new LeafNode<T>()};

    std::vector<TreeNode<T>*> result;

    for (int leftLeafs = 1; leftLeafs < nLeafs; ++leftLeafs) {
        std::vector<TreeNode<T>*> leftTrees = constructBinTrees<T>(leftLeafs);
        std::vector<TreeNode<T>*> rightTrees = constructBinTrees<T>(nLeafs - leftLeafs);

        for (TreeNode<T>* lt : leftTrees ) {
            for (TreeNode<T>* rt : rightTrees) {
                InternalNode<T>* internalAddNode = new InternalNode<T>();
                internalAddNode->setOperation(OperationType::Addition);
                TreeNode<T>* aux1 = (dynamic_cast<LeafNode<T>*>(lt) != nullptr)
                    ? static_cast<TreeNode<T>*>(new LeafNode<T>(*static_cast<LeafNode<T>*>(lt)))
                    : static_cast<TreeNode<T>*>(new InternalNode<T>(*static_cast<InternalNode<T>*>(lt)));
                TreeNode<T>* aux2 = (dynamic_cast<LeafNode<T>*>(rt) != nullptr)
                    ? static_cast<TreeNode<T>*>(new LeafNode<T>(*static_cast<LeafNode<T>*>(rt)))
                    : static_cast<TreeNode<T>*>(new InternalNode<T>(*static_cast<InternalNode<T>*>(rt)));
                internalAddNode->addChild(aux1);
                internalAddNode->addChild(aux2);
                result.push_back(internalAddNode);
            }
        }
    }

    return result;
}

template<typename T, typename U> // T is the type of evalutation, U is the type used for computation
TreeNode<U>* findSolution(const std::vector<T>& constants, T target, std::function<bool(T, U)> equals, const std::vector<std::function<bool(U)>>& intermediaryConditions = {}) {
    if (constants.empty()) return nullptr;

    TreeNode<U>* res = nullptr;
    std::vector<U> constantsLocal(constants.size());
    for (size_t i = 0; i < constants.size(); ++i) {
        constantsLocal[i] = static_cast<U>(constants[i]);
    }

    std::vector<TreeNode<U>*> trees = constructBinTrees<U>(constants.size());

    for (TreeNode<U>* tree : trees) {
        std::vector<TreeNode<double>*> leafs = tree->getLeafs();

        do {
            do {
                for (size_t i = 0; i < leafs.size(); ++i) {
                    leafs[i]->setValue(constantsLocal[i]);
                }

                try {
                    tree->compute(intermediaryConditions);
                    if (!tree->hasComputed()) continue;
                    if (equals(target, tree->getValue())) {
                        res = (dynamic_cast<LeafNode<U>*>(tree))
                            ? static_cast<TreeNode<U>*>(new LeafNode<U>(*static_cast<LeafNode<U>*>(tree)))
                            : static_cast<TreeNode<U>*>(new InternalNode<U>(*static_cast<InternalNode<U>*>(tree)));
                        goto findSolution_CLEANUP;
                    }
                }
                catch (const std::exception& e) {
                    std::cerr << "Error: " << e.what() << std::endl;
                }

            } while (std::next_permutation(constantsLocal.begin(), constantsLocal.end()));
        } while (!tree->incOperation());
    }

findSolution_CLEANUP:
    for (auto tree : trees) {
        delete tree;
    }

    return res;
}



int main(int argc, char* argv[]) {
    if (argc < 2) {
        std::cerr << "Usage: " << argv[0] << " <target> <constants...>" << std::endl;
        return 1;
    }

    int target; 
    std::vector<int> constants; 

    try {
        target = std::stoi(argv[1]);
        constants.resize(argc - 2);
        for (int i = 2; i < argc; ++i) {
            constants[i - 2] = std::stoi(argv[i]);
        }
    }
    catch (const std::invalid_argument& e) {
        std::cerr << "Invalid argument: " << e.what() << std::endl;
        return 1;
    }
    catch (const std::out_of_range& e) {
        std::cerr << "Out of range: " << e.what() << std::endl;
        return 1;
    }

    std::cout << "Target: " << target << std::endl;
    std::cout << "Constants: ";
    for (const auto& c : constants) {
        std::cout << c << " ";
    }
    std::cout << std::endl;

    TreeNode<double>* solution = findSolution<int, double>(constants, target, [](int a, double b) {
        double aux;
        return std::modf(b, &aux) == 0.0 && static_cast<int>(aux) == a;
    }, {[](double a) {
        double aux;
        return std::modf(a, &aux) == 0.0;
    }});

    if (solution) {
        std::cout << "Solution found: " << target << std::endl << solution->toString([](double a) { return std::to_string(static_cast<int>(a)); }) << std::endl;
        delete solution;
    } else {
        std::cout << "No solution found." << std::endl;
    }

    return 0;
}