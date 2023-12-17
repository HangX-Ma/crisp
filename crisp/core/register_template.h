#ifndef CORE_REGISTER_TEMPLATE_H
#define CORE_REGISTER_TEMPLATE_H

#include <unordered_set>

namespace crisp
{

template <class Object>
class Register
{
public:
    virtual ~Register();

    virtual bool install(Object *object, void *user_data) = 0;
    virtual bool uninstall(Object *object, bool free_mem) = 0;

    virtual void clear(bool free_mem) = 0;

    bool isInstalled(Object *device) { return objects.count(device) != 0; }
    std::size_t size() { return objects.size(); }

protected:
    std::unordered_set<Object *> objects;
};

} // namespace crisp

#endif // CORE_REGISTER_BASE_H