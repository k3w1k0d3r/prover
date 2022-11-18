#ifndef PREDICATE_H
#define PREDICATE_H

#include <string>

namespace theorem128 {
    class Predicate {
        public:
            Predicate(std::string, unsigned int);
            Predicate(){};
            std::string get_latex() const;
            unsigned int get_valence() const;
            bool operator==(const Predicate&) const;
            bool operator!=(const Predicate&) const;
        private:
            std::string latex;
            unsigned int valence = 0;
    };
}

#endif
