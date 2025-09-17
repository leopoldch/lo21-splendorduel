#include "privilege.h"

int Privilege::instanceCount = 0;

std::vector<const Privilege *> initPrivileges() {
	std::vector<const Privilege *> tab;
	for (int i = 0; i < 3; ++i) {
		tab.push_back(Privilege::GetInstance());
	}
	return tab;
}
