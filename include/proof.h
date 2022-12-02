#ifndef PROOF_H
#define PROOF_H

#include <vector>
#include <unordered_map>
#include <theory.h>

namespace theorem128 {
    class Proof {
        public:
            Proof(Theory);
            void axiom(Formula, void*);
            void hypothesis(Formula);
            std::unordered_map<std::vector<bool>, std::vector<Formula>> get_truths();
            std::vector<Formula> get_hypotheses();
            void conjunction(unsigned int, unsigned int, std::vector<bool>, std::vector<bool>);
            void conjunction(unsigned int, std::vector<bool>);
            void disjunction(unsigned int, Formula, std::vector<bool>);
            void disjunction(Formula, unsigned int, std::vector<bool>);
            void disjunction(unsigned int, unsigned int, unsigned int, std::vector<bool>, std::vector<bool>, std::vector<bool>);
            void modus_ponens(unsigned int, unsigned int, std::vector<bool>, std::vector<bool>);
            void explosion(unsigned int, std::vector<bool>, Formula);
            void contradiction(unsigned int, std::vector<bool>);
            void negation(unsigned int, std::vector<bool>);
            void equivalence(unsigned int, unsigned int, std::vector<bool>, std::vector<bool>);
            void equivalence(unsigned int, std::vector<bool>);
            void reductio_ad_absurdum(unsigned int, std::vector<bool>);
            void cancel_hypothesis(unsigned int, unsigned int, std::vector<bool>);
            void identity(Term);
            void identity(unsigned int, unsigned int, std::vector<bool>, std::vector<bool>, std::vector<bool>);
            void universal(unsigned int, std::vector<bool>, Variable);
            void universal(unsigned int, std::vector<bool>, Term);
            void existential(unsigned int, unsigned int, std::vector<bool>, std::vector<bool>);
            void existential(unsigned int, std::vector<bool>, Formula, Term);
        private:
            Theory theory;
            std::unordered_map<std::vector<bool>, std::vector<Formula>> truths;
            std::vector<Formula> hypotheses;
    };
}

#endif
