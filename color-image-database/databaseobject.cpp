#include "databaseobject.h"

bool DatabaseObject::isValid() const {
    return this->valid;
}

int DatabaseObject::getId() const {
    return this->id;
}
