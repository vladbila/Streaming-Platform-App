#ifndef STREAMING_PLATFORM_PROJECT_IOBJECT_H
#define STREAMING_PLATFORM_PROJECT_IOBJECT_H

#include <iostream>
#include <string>

class IObject {
protected:
    const int id;
    inline static int idCounter = 0;
public:
    IObject() : id(++idCounter){};
    virtual ~IObject() = default;

    int getId() const noexcept { return id; }

    virtual std::string toString() const = 0;

    bool operator==(const IObject &o) const {
        return this->id == o.id;
    }

    friend std::ostream& operator<<(std::ostream& os, const IObject &o) {
        os << o.toString() << std::endl;
        return os;
    }
};

#endif