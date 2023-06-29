#ifndef DATABASEOBJECT_H
#define DATABASEOBJECT_H

#endif // DATABASEOBJECT_H

class DatabaseObject {
public:
    /*!
     * \brief Return the object's id (which is supposed to mirror the id in the DB)
     * \return Object's id
    */
    virtual int getId() const;

    /*!
     * \brief Return whether the object in its current state is persisted to the DB

     * \return bool Is the object persisted
    */
    virtual bool isPersisted() const;

    /*!
     * \brief Set the state (persisted or not) of the object
     * \param bool value to set
    */
    virtual void setPersisted(bool valid);
protected:
    int id;
    bool persisted;
};
