#ifndef TERM_H
#define TERM_H

#include <vector>
#include <string>
#include <unordered_set>
#include <function.h>

namespace theorem128 {
    class Variable {
        public:
            Variable(std::string);
            Variable(){};
            std::string get_latex() const;
            bool operator==(const Variable&) const;
            bool operator!=(const Variable&) const;
        private:
            std::string latex;
    };
}

namespace std {
    template <>
    struct hash<theorem128::Variable> {
        std::size_t operator()(const theorem128::Variable& variable) const {
            return std::hash<std::string>()(variable.get_latex());
        }
    };
}

namespace theorem128 {
    class Term {
        public:
            Term(Function, std::vector<Term>);
            Term(Variable);
            Function get_function() const;
            std::vector<Term> get_subterms() const;
            std::unordered_set<Variable> get_variables() const;
            std::string get_latex() const;
            bool get_is_variable() const;
            void replace(Variable, Term);
            void replace(Term, Term, std::vector<bool>&);
            bool operator==(const Term&) const;
        private:
            Function function;
            std::vector<Term> subterms;
            bool is_variable;
            std::unordered_set<Variable> variables;
    };
}

#endif
