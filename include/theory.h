#ifndef THEORY_H
#define THEORY_H

#include <vector>
#include <unordered_set>
#include <formula.h>

namespace theorem128 {
    typedef bool func_t(Formula, void*);

    class Schema {
        public:
            Schema(std::vector<Predicate>, Formula, std::vector<std::unordered_set<Variable>>);
            bool verify(Formula, std::vector<Formula>) const;
        private:
            std::vector<Predicate> dummies;
            Formula generic;
            std::vector<std::unordered_set<Variable>> criteria;
    };

    class Theory {
        public:
            Theory(std::vector<Formula>, std::vector<Schema>);
            Theory(func_t*);
            Theory(){};
            bool check_axiom(Formula, void*) const;
        private:
            std::vector<Formula> axioms;
            std::vector<Schema> schemas;
            func_t *custom_check_axiom = NULL;
    };
}

#endif
