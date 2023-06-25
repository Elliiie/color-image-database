#ifndef DATABASEOBJECT_H
#define DATABASEOBJECT_H

#endif // DATABASEOBJECT_H

class DatabaseObject {
public:
    virtual int getId() const;
    virtual bool isPersisted() const;
    virtual void setPersisted(bool valid);
protected:
    int id;
    bool persisted;
};
