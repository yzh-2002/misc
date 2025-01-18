#include <unordered_map>
#include <memory>

class ApplicationContext
{
private:
    ApplicationContext() {}
    std::unordered_map<std::string, std::shared_ptr<void>> object_map;

public:
    static ApplicationContext &instance()
    {
        static ApplicationContext instance;
        return instance;
    }

    template <typename T>
    std::shared_ptr<T> get()
    {
        auto it = object_map.find(typeid(T).name());
        if (it != object_map.end())
        {
            return std::static_pointer_cast<T>(it->second);
        }
        auto obj = std::make_shared<T>();
        object_map[typeid(T).name()] = obj;
        return obj;
    }
};
