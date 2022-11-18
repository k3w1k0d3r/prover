#include <iostream>
#include <cstring>
#include <proof.h>

using namespace theorem128;

int main() {
    std::vector<Formula> axioms;
    Term zero = Term(Function("0", 0), {});
    Function S = Function("S", 1);
    Variable m = Variable("m");
    Variable n = Variable("n");
    Variable a = Variable("a");
    Variable b = Variable("b");
    Variable c = Variable("c");
    Variable k = Variable("k");
    Function add = Function("+", 2);
    Function mult = Function("\\cdot", 2);
    axioms.push_back(Formula(
                FORALL,
                m,
                Formula(
                    FORALL,
                    n,
                    Formula(
                        EQUIVALENT,
                        Formula(Term(m), Term(n)),
                        Formula(Term(S, {Term(m)}), Term(S, {Term(n)}))
                        )
                    )
                )
            );
    axioms.push_back(Formula(Formula(FORALL, m, Formula(Formula(Term(S, {Term(m)}), zero), 0))));
    axioms.push_back(Formula(FORALL, m, Formula(Term(add, {Term(m), zero}), Term(m))));
    axioms.push_back(Formula(
                FORALL,
                m,
                Formula(
                    FORALL,
                    n,
                    Formula(
                        Term(add, {m, Term(S, {Term(n)})}),
                        Term(S, {Term(add, {Term(m), Term(n)})})
                        )
                    )
                )
            );
    axioms.push_back(Formula(FORALL, m, Formula(Term(mult, {Term(m), zero}), zero)));
    axioms.push_back(Formula(
                FORALL,
                m,
                Formula(
                    FORALL,
                    n,
                    Formula(
                        Term(mult, {Term(m), Term(S, {Term(n)})}),
                        Term(add, {Term(m), Term(mult, {Term(m), Term(n)})})
                        )
                    )
                )
            );
    std::cout << "axioms:" << std::endl;
    for(int i=0;i<axioms.size();++i) {
        std::cout << axioms[i].get_latex() << std::endl;
    }
    Predicate D = Predicate("D", 0);
    Formula induction_formula = Formula(
            IMPLIES,
            Formula(
                AND,
                Formula(
                    EXISTS,
                    m,
                    Formula(AND, Formula(Term(m), zero), Formula(D, {}))
                    ),
                Formula(
                    FORALL,
                    m,
                    Formula(
                        IMPLIES,
                        Formula(D, {}),
                        Formula(
                            EXISTS,
                            n,
                            Formula(
                                AND,
                                Formula(Term(n), Term(S, {m})),
                                Formula(
                                    EXISTS,
                                    m,
                                    Formula(AND, Formula(Term(m), Term(n)), Formula(D, {}))
                                    )
                                )
                            )
                        )
                    )
                ),
                Formula(FORALL, m, Formula(D, {}))
            );
    Schema induction = Schema(
            {D},
            induction_formula,
            {{m}}
            );
    Theory PA = Theory(axioms, {induction});
    Proof proof = Proof(PA);
    void *signature = malloc(sizeof(bool)+sizeof(int)+sizeof(std::vector<Formula>));
    *(bool*)signature = false;
    *(int*)((bool*)signature+1) = 5;
    proof.axiom(axioms[5], signature);
    std::cout << "induction template:" << std::endl;
    std::cout << induction_formula.get_latex() << std::endl;
    std::cout << "axiom truth test:" << std::endl;
    for(int i=0;i<proof.get_truths()[{}].size();++i) {
        std::cout << proof.get_truths()[{}][i].get_latex() << std::endl;
    }
    proof.universal(0, {}, Term(m));
    proof.universal(1, {}, zero);
    *(int*)((bool*)signature+1) = 4;
    proof.axiom(axioms[4], signature);
    proof.universal(3, {}, Term(m));
    *(int*)((bool*)signature+1) = 2;
    proof.axiom(axioms[2], signature);
    proof.universal(5, {}, Term(m));
    proof.identity(4, 2, {}, {}, {1});
    proof.identity(6, 7, {}, {}, {1});
    proof.universal(8, {}, m);
    std::cout << "proven:" << std::endl;
    std::cout << proof.get_truths()[{}][proof.get_truths()[{}].size()-1].get_latex() << std::endl;
    proof.identity(Term(add, {Term(b), zero}));
    proof.universal(5, {}, Term(add, {Term(a), Term(b)}));
    proof.universal(5, {}, Term(b));
    proof.identity(12, 10, {}, {}, {1});
    proof.identity(13, 11, {}, {}, {1, 0});
    proof.hypothesis(
            Formula(
                FORALL,
                a,
                Formula(
                    FORALL,
                    b,
                    Formula(
                        Term(add, {Term(add, {Term(a), Term(b)}), Term(m)}),
                        Term(add, {Term(a), Term(add, {Term(b), Term(m)})})
                        )
                    )
                )
            );
    *(int*)((bool*)signature+1) = 3;
    proof.axiom(axioms[3], signature);
    proof.universal(0, {1}, Term(a));
    proof.universal(1, {1}, Term(b));
    proof.universal(15, {}, Term(add, {Term(a), Term(b)}));
    proof.universal(16, {}, Term(m));
    proof.identity(2, 17, {1}, {}, {1});
    proof.universal(15, {}, Term(a));
    proof.universal(18, {}, Term(add, {Term(b), Term(m)}));
    proof.identity(Term(add, {Term(a), Term(S, {Term(add, {Term(b), Term(m)})})}));
    proof.identity(19, 20, {}, {}, {1});
    proof.identity(21, 3, {}, {1}, {1});
    proof.universal(15, {}, Term(b));
    proof.universal(22, {}, Term(m));
    proof.identity(Term(add, {Term(b), Term(S, {Term(m)})}));
    proof.identity(23, 24, {}, {}, {1});
    proof.identity(25, 4, {}, {1}, {1});
    proof.identity(Term(S, {Term(m)}));
    proof.universal(5, {1}, b);
    proof.universal(6, {1}, a);
    proof.conjunction(26, 7, {}, {1});
    proof.existential(
            8,
            {1},
            Formula(
                EXISTS,
                k,
                Formula(
                    AND,
                    Formula(Term(k), Term(S, {Term(m)})),
                    Formula(
                        FORALL,
                        a,
                        Formula(
                            FORALL,
                            b,
                            Formula(
                                Term(add, {Term(add, {Term(a), Term(b)}), Term(k)}),
                                Term(add, {Term(a), Term(add, {Term(b), Term(k)})})
                                )
                            )
                        )
                    )
                ),
            Term(S, {Term(m)})
            );
    proof.conjunction(26, 9, {}, {1});
    proof.existential(
            10,
            {1},
            Formula(
                EXISTS,
                n,
                Formula(
                    AND,
                    Formula(Term(n), Term(S, {Term(m)})),
                    Formula(
                        EXISTS,
                        k,
                        Formula(
                            AND,
                            Formula(Term(k), Term(n)),
                            Formula(
                                FORALL,
                                a,
                                Formula(
                                    FORALL,
                                    b,
                                    Formula(
                                        Term(add, {Term(add, {Term(a), Term(b)}), Term(k)}),
                                        Term(add, {Term(a), Term(add, {Term(b), Term(k)})})
                                        )
                                    )
                                )
                            )
                        )
                    )
                ),
            Term(S, {Term(m)})
            );
    proof.hypothesis(
            Formula(
                AND,
                Formula(Term(n), Term(S, {Term(m)})),
                Formula(
                    EXISTS,
                    k,
                    Formula(
                        AND,
                        Formula(Term(k), Term(n)),
                        Formula(
                            FORALL,
                            a,
                            Formula(
                                FORALL,
                                b,
                                Formula(
                                    Term(add, {Term(add, {Term(a), Term(b)}), Term(k)}),
                                    Term(add, {Term(a), Term(add, {Term(b), Term(k)})})
                                    )
                                )
                            )
                        )
                    )
                )
            );
    proof.conjunction(0, {0, 1});
    proof.hypothesis(
            Formula(
                AND,
                Formula(Term(k), Term(n)),
                Formula(
                    FORALL,
                    a,
                    Formula(
                        FORALL,
                        b,
                        Formula(
                            Term(add, {Term(add, {Term(a), Term(b)}), Term(k)}),
                            Term(add, {Term(a), Term(add, {Term(b), Term(k)})})
                            )
                        )
                    )
                )
            );
    proof.existential(
            0,
            {0, 0, 1},
            Formula(
                EXISTS,
                m,
                Formula(
                    AND,
                    Formula(Term(m), Term(n)),
                    Formula(
                        FORALL,
                        a,
                        Formula(
                            FORALL,
                            b,
                            Formula(
                                Term(add, {Term(add, {Term(a), Term(b)}), Term(m)}),
                                Term(add, {Term(a), Term(add, {Term(b), Term(m)})})
                                )
                            )
                        )
                    )
                ),
            Term(k)
            );
    proof.cancel_hypothesis(1, 2, {0, 0, 1});
    proof.existential(2, 27, {0, 1}, {});
    proof.conjunction(1, 3, {0, 1}, {0, 1});
    proof.existential(
            4,
            {0, 1},
            Formula(
                EXISTS,
                n,
                Formula(
                    AND,
                    Formula(Term(n), Term(S, {Term(m)})),
                    Formula(
                        EXISTS,
                        m,
                        Formula(
                            AND,
                            Formula(Term(m), Term(n)),
                            Formula(
                                FORALL,
                                a,
                                Formula(
                                    FORALL,
                                    b,
                                    Formula(
                                        Term(add, {Term(add, {Term(a), Term(b)}), Term(m)}),
                                        Term(add, {Term(a), Term(add, {Term(b), Term(m)})})
                                        )
                                    )
                                )
                            )
                        )
                    )
                ),
            Term(n)
            );
    proof.cancel_hypothesis(5, 1, {0, 1});
    proof.existential(11, 28, {1}, {});
    proof.cancel_hypothesis(12, 0, {1});
    proof.universal(29, {}, m);
    proof.identity(zero);
    proof.universal(14, {}, b);
    proof.universal(32, {}, a);
    proof.conjunction(31, 33, {}, {});
    proof.existential(
            34,
            {},
            Formula(
                EXISTS,
                m,
                Formula(
                    AND,
                    Formula(Term(m), zero),
                    Formula(
                        FORALL,
                        a,
                        Formula(
                            FORALL,
                            b,
                            Formula(
                                Term(add, {Term(add, {Term(a), Term(b)}), Term(m)}),
                                Term(add, {Term(a), Term(add, {Term(b), Term(m)})})
                                )
                            )
                        )
                    )
                ),
            zero
            );
    proof.conjunction(35, 30, {}, {});
    *(bool*)signature = true;
    *(int*)((bool*)signature+1) = 0;
    std::vector<Formula> assoc = {
        Formula(
                FORALL,
                a,
                Formula(
                    FORALL,
                    b,
                    Formula(
                        Term(add, {Term(add, {Term(a), Term(b)}), Term(m)}),
                        Term(add, {Term(a), Term(add, {Term(b), Term(m)})})
                        )
                    )
                )
    };
    memcpy((int*)((bool*)signature+1)+1, &assoc, sizeof(assoc));
    Formula assoc_induction = induction_formula;
    assoc_induction.replace(D, assoc[0]);
    proof.axiom(assoc_induction, signature);
    proof.modus_ponens(37, 36, {}, {});
    proof.universal(38, {}, Term(c));
    proof.universal(39, {}, Term(a));
    proof.universal(40, {}, Term(b));
    proof.universal(41, {}, c);
    proof.universal(42, {}, b);
    proof.universal(43, {}, a);
    std::cout << "proven:" << std::endl;
    std::cout << proof.get_truths()[{}][proof.get_truths()[{}].size()-1].get_latex() << std::endl;
    free(signature);
    return 0;
}
