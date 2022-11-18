#ifndef FORMULA_H
#define FORMULA_H

#include <vector>
#include <string>
#include <unordered_set>
#include <predicate.h>
#include <term.h>

namespace theorem128 {
    enum Quantifier {FORALL, EXISTS};
    enum Connective {AND, OR, IMPLIES, EQUIVALENT};
    class Formula {
        public:
            Formula(Predicate, std::vector<Term>);
            Formula(Quantifier, Variable, Formula);
            Formula(Connective, Formula, Formula);
            Formula(Formula, int);
            Formula(Term, Term);
            Formula(int);
            Formula(const Formula&);
            Formula(){};
            Predicate get_predicate() const;
            Quantifier get_quantifier() const;
            Connective get_connective() const;
            Variable get_quantified() const;
            Formula *get_subformula(int) const;
            std::vector<Term> get_subterms() const;
            std::unordered_set<Variable> get_variables() const;
            int get_type() const;
            std::string get_latex() const;
            void replace(Predicate, Formula);
            int replace(Variable, Term);
            void replace(Term, Term, std::vector<bool>&);
            Formula& operator=(const Formula&);
            bool operator==(const Formula&) const;
            bool operator!=(const Formula&) const;
            ~Formula();
        private:
            int type = 0;
            Predicate predicate;
            Quantifier quantifier = FORALL;
            Connective connective = AND;
            Variable quantified;
            Formula *subformula1 = NULL;
            Formula *subformula2 = NULL;
            std::vector<Term> subterms;
            std::unordered_set<Variable> variables;
    };
}

namespace std {
    template <>
    struct hash<theorem128::Formula> {
        std::size_t operator()(const theorem128::Formula& formula) const {
            return std::hash<std::string>()(formula.get_latex());
        }
    };
}

#endif
