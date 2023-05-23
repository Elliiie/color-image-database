#ifndef DATABASEOBJECT_H
#define DATABASEOBJECT_H

#endif // DATABASEOBJECT_H

class DatabaseObject {
public:
    virtual int getId() const;
    virtual bool isValid() const;
protected:
    int id;
    bool valid;
};
