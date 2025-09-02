#ifndef OPERATION_H
#define OPERATION_H

enum class OperationType {
    None = 0,
    Addition = 1,
    Subtraction = 2,
    Multiplication = 3,
    Division = 4
};
#define OPERATION_TYPE_COUNT 5
#define OPERATION_TYPE_FIRST OperationType::Addition
#define OPERATION_TYPE_LAST OperationType::Division

inline OperationType operator++(OperationType& op, int) {
    OperationType old = op;
    if (op == OperationType::None || op == OPERATION_TYPE_LAST)
        op = OPERATION_TYPE_FIRST;
    else
        op = static_cast<OperationType>(static_cast<int>(op) + 1);
    return old;
}

template<typename HEAD>
HEAD addition(HEAD head) { return head; }
template<typename HEAD, typename... TAIL>
HEAD addition(HEAD head, TAIL... tail) { return head + addition(tail...); }

template<typename HEAD>
HEAD subtraction(HEAD head) { return head; }
template<typename HEAD, typename... TAIL>
HEAD subtraction(HEAD head, TAIL... tail) { return head - addition(tail...); } // result: head - (sum tail)

template<typename HEAD>
HEAD multiplication(HEAD head) { return head; }
template<typename HEAD, typename... TAIL>
HEAD multiplication(HEAD head, TAIL... tail) { return head * multiplication(tail...); }

template<typename HEAD>
HEAD division(HEAD head) { return head; }
template<typename HEAD, typename... TAIL>
HEAD division(HEAD head, TAIL... tail) { return head / multiplication(tail...); } // result: head / (prod tail)

template<typename T>
T addition(std::vector<T> vec) {
    T res = vec[0];
    for (size_t i = 1; i < vec.size(); ++i) {
        res += vec[i];
    }
    return res;
}

template<typename T>
T subtraction(std::vector<T> vec) {
    T res = vec[0];
    for (size_t i = 1; i < vec.size(); ++i) {
        res -= vec[i];
    }
    return res;
}

template<typename T>
T multiplication(std::vector<T> vec) {
    T res = vec[0];
    for (size_t i = 1; i < vec.size(); ++i) {
        res *= vec[i];
    }
    return res;
}

template<typename T>
T division(std::vector<T> vec) {
    T res = vec[0];
    for (size_t i = 1; i < vec.size(); ++i) {
        res /= vec[i];
    }
    return res;
}

#endif // OPERATION_H