#ifndef DATABASEOBJECT_H
#define DATABASEOBJECT_H

#endif // DATABASEOBJECT_H

class DatabaseObject {
public:
    virtual int getId() const;
    virtual bool isValid() const;
    virtual void setValid(bool valid);
protected:
    int id;
    bool valid;
};
