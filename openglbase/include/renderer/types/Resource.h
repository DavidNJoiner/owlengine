#ifndef RESOURCE_H
#define RESOURCE_H

#include <string>
#include <iostream>

class Resource {
public:
    // Constructor
    Resource(const std::string& path = "")
        : m_Path(path), m_Loaded(false) {}

    // Virtual destructor for proper cleanup
    virtual ~Resource() = default;

    // Pure virtual method to load the resource
    virtual bool Load() { m_Loaded = true; return true; };

    // Virtual method to unload the resource (can be overridden if needed)
    virtual void Unload() {
        m_Loaded = false;
        std::cout << "Unloaded: " << m_Path << std::endl;
    }

    // Virtual method to get the resource path by reference
    virtual const std::string& GetResourcePath() const {
        return m_Path;
    }

    virtual void SetResourcePath(const std::string& path) { m_Path = path; }

    // Check if the resource is loaded
    bool IsLoaded() const {
        return m_Loaded;
    }

protected:
    // Setter for the loaded flag
    void SetLoaded(bool loaded) {
        m_Loaded = loaded;
    }

    // Member variables
    std::string m_Path; // Resource name or path
    bool m_Loaded;      // Flag indicating if the resource is loaded
};

#endif // RESOURCE_H
