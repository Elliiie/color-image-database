#include "databaseobject.h"

bool DatabaseObject::isPersisted() const {
    return this->persisted;
}

int DatabaseObject::getId() const {
    return this->id;
}

void DatabaseObject::setPersisted(bool valid) {
    this->persisted = valid;
}
