#include "manager.hpp"
#include "../utility/logger/logger.hpp"

static ScriptManager* scriptManagerInstance = nullptr;

ScriptManager* ScriptManager::Instance() {
	if (!scriptManagerInstance)
		scriptManagerInstance = new ScriptManager();

	return scriptManagerInstance;
}

void ScriptManager::Uninitialize() {
    scriptManagerInstance = nullptr;
    delete this;
}

void ScriptManager::Add(std::unique_ptr<Script> script, const char* scriptName) {
    //Logger::Instance()->Log("Scripts", scriptName);
    //std::lock_guard lock(mutex);
    std::lock_guard lock(mutex);

    try {
        std::lock_guard lock(mutex);
        scripts.push_back(std::move(script));
    }
    catch (const std::bad_alloc& e) {
        std::cerr << "Memory allocation failed: " << e.what() << std::endl;
    }
    catch (const std::exception& e) {
        std::cerr << "An exception occurred: " << e.what() << std::endl;
    }
}

void ScriptManager::Clear() {
    std::lock_guard lock(mutex);
	scripts.clear();
}

void ScriptManager::Tick() {
    bool ensureMainFiber = (ConvertThreadToFiber(nullptr), true);
    if (!ensureMainFiber)
        return;

    std::lock_guard lock(mutex);
    for (auto const &script : scripts)
        script->Tick();
}