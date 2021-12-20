/*
   Created by Vitaliy Eroshin on 20.12.2021.
*/

#pragma once
#include <set>
#include <string>
#include <utility>
#include <iostream>
#include <vector>

#define TRUTHLiteral Literal("⊤")
#define FALSELiteral Literal("⊥")

struct Literal {
  Literal() = default;

  explicit Literal(std::string alias): variable(std::move(alias)), sign(Sign::POSITIVE) {};

  Literal(const Literal& other) = default;

  ~Literal() = default;

  Literal& operator=(const Literal& other) = default;

  bool operator==(const Literal& other) const { return variable == other.variable && sign == other.sign; }

  bool operator<(const Literal& other) const;

  void changeSign() { sign = (sign == Sign::POSITIVE ? Sign::NEGATIVE : Sign::POSITIVE); }

  Literal inverted() const;

  friend std::istream& operator>>(std::istream& in, Literal& literal);

  enum class Sign {NEGATIVE, POSITIVE};
  std::string variable;
  Sign sign;
};

bool Literal::operator<(const Literal& other) const {
  if (variable == other.variable)
    return sign < other.sign;

  return variable < other.variable;
}

Literal Literal::inverted() const {
  Literal literal(*this);
  literal.changeSign();
  return literal;
}

std::ostream& operator<<(std::ostream& out, const Literal& literal) {
  if (literal.sign == Literal::Sign::NEGATIVE)
    out << "¬";

  out << literal.variable;
  return out;
}

std::istream& operator>>(std::istream& in, Literal& literal) {
  std::string raw;
  in >> raw;
  bool sign = true;
  std::string alias;
  for (const auto& x : raw) {
    alias.push_back(x);
    if (alias == "¬") {
      sign = false;
      alias.clear();
    }
  }
  literal.variable = alias;
  literal.sign = (sign ? Literal::Sign::POSITIVE : Literal::Sign::NEGATIVE);
  return in;
}

struct Disjunct {
public:
  Disjunct() = default;

  Disjunct(const Disjunct& other) = default;

  ~Disjunct() = default;

  void push(const Literal& literal) { literals.insert(literal); };

  bool isEmpty() const { return *literals.begin() == FALSELiteral; };

  bool isTrue() const { return *literals.begin() == TRUTHLiteral; };

  std::set<Literal> literals;
};

std::ostream& operator<<(std::ostream& out, const Disjunct& disjunct) {
  out << "{";
  for (const auto& literal : disjunct.literals)
    out << literal << ' ';

  out << "}";

  return out;
}

class CNF {
public:
  bool isFalse = false;
  std::set<Disjunct> uniqueDisjuncts;
  std::vector<Disjunct> disjuncts;

  bool process(bool verbose = false);

  void push(const Disjunct& disjunct);

  Disjunct resolvent(const Disjunct& first, const Disjunct& other) const;
};

bool CNF::process(bool verbose) {
  static int iterations = 1;
  if (verbose)
    std::cout << "Iteration " << iterations << ":\n";

  bool producedNewDisjunct = false;
  for (size_t i = 0; i < disjuncts.size(); ++i) {
    for (size_t j = i + 1; j < disjuncts.size(); ++j) {
      Disjunct product = resolvent(disjuncts[i], disjuncts[j]);
      if (uniqueDisjuncts.count(product)) {
        continue;
      } else if (product.isEmpty()) {
        if (verbose)
          std::cout << "False disjunct produced from " << i << "'th and " << j << "'th!\n";
        isFalse = true;
        return false;
      } else if (product.isTrue()) {
        continue;
      } else {
        uniqueDisjuncts.insert(product);
        disjuncts.push_back(product);
        producedNewDisjunct = true;
      }
    }
  }
  if (verbose) {
    int orderCounter = 0;
    for (const auto& disjunct : disjuncts) {
      std::cout << "(" << orderCounter++ << ")" << disjunct << " ";
    }
  }
  return producedNewDisjunct;
}

Disjunct CNF::resolvent(const Disjunct& first, const Disjunct& other) const {
  Disjunct result;
  std::set<Literal>& combination = result.literals;
  combination = first.literals;
  bool applied = false;
  for (const auto& literal : other.literals) {
    Literal inverted = literal.inverted();
    if (!applied && combination.count(inverted)) {
      combination.erase(inverted);
      applied = true;
      continue;
    }
    if (applied && combination.count(inverted)) {
      combination = {TRUTHLiteral};
      return result;
    }
    combination.insert(literal);
  }

  if (combination.empty())
    combination = {FALSELiteral};

  return result;
}

void CNF::push(const Disjunct& disjunct) {
  if (uniqueDisjuncts.count(disjunct) == 0) {
    uniqueDisjuncts.insert(disjunct);
    disjuncts.push_back(disjunct);
  }
}

std::ostream& operator<<(std::ostream& out, const CNF& cnf) {
  int orderCounter = 0;
  for (const auto& disjunct : cnf.disjuncts) {
    std::cout << "(" << orderCounter++ << ")" << disjunct << " ";
  }

  return out;
}
