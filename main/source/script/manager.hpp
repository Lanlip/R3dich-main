#ifndef _MANAGER_HPP
#define _MANAGER_HPP

#include <common.hpp>
#include "script.hpp"

class ScriptManager {
private:
    std::recursive_mutex mutex;
	std::vector<std::unique_ptr<Script>> scripts;
    //std::unique_ptr<Script> script = std::make_unique<Script>();
public:
    static ScriptManager* Instance();
    void Uninitialize();
public:
    void Add(std::unique_ptr<Script> script, const char* scriptName);
    void Clear();
public:
    void Tick();
};

#endif