#ifndef LO21_SPLENDOR_DUEL_PRIVILEGE_H
#define LO21_SPLENDOR_DUEL_PRIVILEGE_H
#include "Exception.h"
#include "carte.h"
#include <iostream>
#include <stdexcept>
#include <vector>

class Privilege {
private:
  const int id;
  static int instanceCount; // Compteur d'instances actuelles
  static const int maxInstances = 3;
  Privilege(const Privilege &) = delete;
  Privilege &operator=(const Privilege &) = delete;
  Privilege() : id(instanceCount) {}
  Privilege(int id) : id(id) {}

public:
  json toJson() const {
    json j;
    j["id"] = id;
    return j;
  }

  static Privilege *getForJson(int id) {
    if (instanceCount < maxInstances) {
      ++instanceCount;
      return new Privilege(id);
    }
    throw SplendorException("Nombres d'instances de privileges depasse");
  }

  const int get_id() const { return id; }
  static int getCount() { return instanceCount; }

  // Méthode statique pour obtenir une instance de la classe
  static const Privilege *GetInstance() {
    if (instanceCount < maxInstances) {
      ++instanceCount;
      return new Privilege();
    }
    throw SplendorException("Nombres d'instances de privileges depassé");
  }

  ~Privilege() { instanceCount = 0; }

  static int get_max_instance() { return maxInstances; }
};

inline std::ostream &operator<<(std::ostream &f, const Privilege &p) {
  f << "Id : " << p.get_id();
  return f;
}
std::vector<const Privilege *> initPrivileges();

#endif // LO21_SPLENDOR_DUEL_PRIVILEGE_H
