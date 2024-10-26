#include "variables.hpp"
#include <modules/config/config.hpp>
#include <utils.hpp>

#include <Geode/binding/LevelEditorLayer.hpp>
#include <Geode/binding/GameManager.hpp>
#include <Geode/binding/GJGameLevel.hpp>
#include <Geode/binding/PlayerObject.hpp>
#include <Geode/binding/PlayLayer.hpp>
#include <Geode/loader/Mod.hpp>
#include <Geode/Loader.hpp>
#include <Geode/modify/PlayLayer.hpp>

#include <Geode/modify/PlayLayer.hpp>

namespace eclipse::labels {

    VariableManager& VariableManager::get() {
        static VariableManager instance;
        static bool initialized = false;
        if (!initialized) {
            instance.init();
            initialized = true;
        }
        return instance;
    }

    void VariableManager::init() {
        auto* mod = geode::Mod::get();
        auto* loader = geode::Loader::get();

        // Mod variables
        m_variables["modVersion"] = rift::Value::string(mod->getVersion().toNonVString());
        m_variables["platform"] = rift::Value::string(
            GEODE_WINDOWS("Windows")
            GEODE_ANDROID("Android")
            GEODE_MACOS("macOS")
            GEODE_IOS("iOS")
        );
        m_variables["gameVersion"] = rift::Value::string(loader->getGameVersion());
        auto allMods = loader->getAllMods();
        m_variables["totalMods"] = rift::Value::integer(static_cast<int>(allMods.size()));
        m_variables["enabledMods"] = rift::Value::integer(static_cast<int>(std::ranges::count_if(allMods, [](auto* mod) {
            return mod->shouldLoad();
        })));

        // Emojis :D
        m_variables["starEmoji"] = rift::Value::string("⭐");
        m_variables["moonEmoji"] = rift::Value::string("🌙");
        m_variables["heartEmoji"] = rift::Value::string("❤️");
        m_variables["checkEmoji"] = rift::Value::string("✅");
        m_variables["crossEmoji"] = rift::Value::string("❌");
        m_variables["exclamationEmoji"] = rift::Value::string("❗");
        m_variables["questionEmoji"] = rift::Value::string("❓");
        m_variables["fireEmoji"] = rift::Value::string("🔥");
        m_variables["snowflakeEmoji"] = rift::Value::string("❄️");
        m_variables["catEmoji"] = rift::Value::string("🐱");
        m_variables["dogEmoji"] = rift::Value::string("🐶");
        m_variables["speakingHeadEmoji"] = rift::Value::string("🗣️");
        m_variables["robotEmoji"] = rift::Value::string("🤖");
        m_variables["alienEmoji"] = rift::Value::string("👽");
        m_variables["ghostEmoji"] = rift::Value::string("👻");
        m_variables["skullEmoji"] = rift::Value::string("💀");
        m_variables["babyEmoji"] = rift::Value::string("👶");

        // Fetch everything else
        m_variables["fps"] = rift::Value::floating(0.f);
        m_variables["realFps"] = rift::Value::floating(0.f);
        refetch();
    }

    void VariableManager::setVariable(const std::string& name, const rift::Value& value) {
        m_variables[name] = value;
    }

    rift::Value VariableManager::getVariable(const std::string& name) const {
        auto it = m_variables.find(name);
        if (it == m_variables.end()) return {};
        return it->second;
    }

    bool VariableManager::hasVariable(const std::string& name) const {
        return m_variables.contains(name);
    }

    void VariableManager::removeVariable(const std::string& name) {
        m_variables.erase(name);
    }

    enum class LevelDifficulty {
        NA, Auto, Easy,
        Normal, Hard, Harder,
        Insane, EasyDemon, MediumDemon,
        HardDemon, InsaneDemon, ExtremeDemon
    };

    static LevelDifficulty getLevelDifficulty(GJGameLevel* level) {
        if (level->m_autoLevel) return LevelDifficulty::Auto;
        auto diff = level->m_difficulty;

        if (level->m_ratingsSum != 0)
            diff = static_cast<GJDifficulty>(level->m_ratingsSum / 10);

        if (level->m_demon > 0) {
            switch (level->m_demonDifficulty) {
                case 3: return LevelDifficulty::EasyDemon;
                case 4: return LevelDifficulty::MediumDemon;
                case 5: return LevelDifficulty::InsaneDemon;
                case 6: return LevelDifficulty::ExtremeDemon;
                default: return LevelDifficulty::HardDemon;
            }
        }

        switch (diff) {
            case GJDifficulty::Easy: return LevelDifficulty::Easy;
            case GJDifficulty::Normal: return LevelDifficulty::Normal;
            case GJDifficulty::Hard: return LevelDifficulty::Hard;
            case GJDifficulty::Harder: return LevelDifficulty::Harder;
            case GJDifficulty::Insane: return LevelDifficulty::Insane;
            case GJDifficulty::Demon: return LevelDifficulty::HardDemon;
            default: return LevelDifficulty::NA;
        }
    }

    const char* getLevelDifficultyString(LevelDifficulty diff) {
        switch (diff) {
            case LevelDifficulty::NA: return "N/A";
            case LevelDifficulty::Auto: return "Auto";
            case LevelDifficulty::Easy: return "Easy";
            case LevelDifficulty::Normal: return "Normal";
            case LevelDifficulty::Hard: return "Hard";
            case LevelDifficulty::Harder: return "Harder";
            case LevelDifficulty::Insane: return "Insane";
            case LevelDifficulty::EasyDemon: return "Easy Demon";
            case LevelDifficulty::MediumDemon: return "Medium Demon";
            case LevelDifficulty::HardDemon: return "Hard Demon";
            case LevelDifficulty::InsaneDemon: return "Insane Demon";
            case LevelDifficulty::ExtremeDemon: return "Extreme Demon";
            default: return "Unknown";
        }
    }

    const char* getLevelDifficultyKey(LevelDifficulty diff) {
        switch (diff) {
            case LevelDifficulty::NA: return "na";
            case LevelDifficulty::Auto: return "auto";
            case LevelDifficulty::Easy: return "easy";
            case LevelDifficulty::Normal: return "normal";
            case LevelDifficulty::Hard: return "hard";
            case LevelDifficulty::Harder: return "harder";
            case LevelDifficulty::Insane: return "insane";
            case LevelDifficulty::EasyDemon: return "easy_demon";
            case LevelDifficulty::MediumDemon: return "medium_demon";
            case LevelDifficulty::HardDemon: return "hard_demon";
            case LevelDifficulty::InsaneDemon: return "insane_demon";
            case LevelDifficulty::ExtremeDemon: return "extreme_demon";
            default: return "unknown";
        }
    }

    static std::string formatTime(int millis) {
        if (millis == 0) return "N/A";
        double seconds = millis / 1000.0;
        return utils::formatTime(seconds);
    }

    float getFPS() {
        static std::chrono::time_point<std::chrono::system_clock> s_lastUpdate;
        auto now = std::chrono::system_clock::now();
        auto duration = now - s_lastUpdate;
        s_lastUpdate = now;
        double micros = static_cast<double>(std::chrono::duration_cast<std::chrono::microseconds>(duration).count());
        return static_cast<float>(1'000'000.0 / micros);
    }

    float accumulateFPS(float fps) {
        static std::deque<float> s_fps;
        constexpr size_t maxFPS = 100;
        s_fps.push_back(fps);
        if (s_fps.size() > maxFPS) s_fps.pop_front();
        return std::accumulate(s_fps.begin(), s_fps.end(), 0.f) / static_cast<float>(s_fps.size());
    }

    void VariableManager::updateFPS() {
        auto fps = getFPS();
        m_variables["realFps"] = rift::Value::floating(fps);
        m_variables["fps"] = rift::Value::floating(accumulateFPS(fps));
    }

    void VariableManager::refetch() {
        // Game variables
        auto* gameManager = GameManager::get();
        m_variables["username"] = rift::Value::string(gameManager->m_playerName);
        m_variables["cubeIcon"] = rift::Value::integer(utils::getPlayerIcon(PlayerMode::Cube));
        m_variables["shipIcon"] = rift::Value::integer(utils::getPlayerIcon(PlayerMode::Ship));
        m_variables["ballIcon"] = rift::Value::integer(utils::getPlayerIcon(PlayerMode::Ball));
        m_variables["ufoIcon"] = rift::Value::integer(utils::getPlayerIcon(PlayerMode::UFO));
        m_variables["waveIcon"] = rift::Value::integer(utils::getPlayerIcon(PlayerMode::Wave));
        m_variables["robotIcon"] = rift::Value::integer(utils::getPlayerIcon(PlayerMode::Robot));
        m_variables["spiderIcon"] = rift::Value::integer(utils::getPlayerIcon(PlayerMode::Spider));
        m_variables["swingIcon"] = rift::Value::integer(utils::getPlayerIcon(PlayerMode::Swing));

        // Hack states (only the important ones)
        m_variables["isCheating"] = rift::Value::boolean(config::getTemp("hasCheats", false));
        m_variables["noclip"] = rift::Value::boolean(config::get("player.noclip", false));
        m_variables["speedhack"] = rift::Value::boolean(config::get("global.speedhack.toggle", false));
        m_variables["speedhackSpeed"] = rift::Value::floating(config::get("global.speedhack", 1.f));
        m_variables["framestepper"] = rift::Value::boolean(config::get("player.framestepper", false));

        // Time
        auto time = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
        auto localTime = std::localtime(&time);
        m_variables["hour"] = rift::Value::integer(localTime->tm_hour);
        m_variables["minute"] = rift::Value::integer(localTime->tm_min);
        m_variables["second"] = rift::Value::integer(localTime->tm_sec);
        m_variables["day"] = rift::Value::integer(localTime->tm_mday);
        m_variables["month"] = rift::Value::integer(localTime->tm_mon + 1);
        m_variables["monthName"] = rift::Value::string(utils::getMonthName(localTime->tm_mon));
        m_variables["year"] = rift::Value::integer(localTime->tm_year + 1900);
        m_variables["clock"] = rift::Value::string(utils::getClock());
        m_variables["clock12"] = rift::Value::string(utils::getClock(true));

        // Game state
        if (auto* gjbgl = gameManager->m_gameLayer) {
            // Level
            auto* level = gjbgl->m_level;
            auto levelID = level->m_levelID;
            bool isRobtopLevel = (levelID > 0 && levelID < 100) || (levelID >= 3001 && levelID <= 6000);
            auto levelDifficulty = getLevelDifficulty(level);
            m_variables["levelID"] = rift::Value::integer(levelID);
            m_variables["levelName"] = rift::Value::string(level->m_levelName);
            m_variables["author"] = rift::Value::string(isRobtopLevel ? "RobTop" : gjbgl->m_level->m_creatorName);
            m_variables["isRobtopLevel"] = rift::Value::boolean(isRobtopLevel);
            m_variables["levelAttempts"] = rift::Value::integer(level->m_attempts);
            m_variables["levelStars"] = rift::Value::integer(level->m_stars);
            m_variables["difficulty"] = rift::Value::string(getLevelDifficultyString(levelDifficulty));
            m_variables["difficultyKey"] = rift::Value::string(getLevelDifficultyKey(levelDifficulty));
            m_variables["practicePercent"] = rift::Value::integer(level->m_practicePercent);
            m_variables["bestPercent"] = rift::Value::integer(level->m_normalPercent);
            m_variables["bestTime"] = rift::Value::floating(static_cast<float>(level->m_bestTime));
            if (gjbgl->m_isPlatformer)
                m_variables["best"] = rift::Value::string(formatTime(level->m_bestTime));
            else
                m_variables["best"] = rift::Value::integer(level->m_normalPercent);

            // Player
            m_variables["playerX"] = rift::Value::floating(gjbgl->m_player1->m_position.x);
            m_variables["playerY"] = rift::Value::floating(gjbgl->m_player1->m_position.y);
            m_variables["player2X"] = rift::Value::floating(gjbgl->m_player2->m_position.x);
            m_variables["player2Y"] = rift::Value::floating(gjbgl->m_player2->m_position.y);
            m_variables["attempt"] = rift::Value::integer(gjbgl->m_attempts);
            m_variables["isTestMode"] = rift::Value::boolean(gjbgl->m_isTestMode);
            m_variables["isPracticeMode"] = rift::Value::boolean(gjbgl->m_isPracticeMode);
            m_variables["isPlatformer"] = rift::Value::boolean(gjbgl->m_isPlatformer);
            m_variables["levelTime"] = rift::Value::floating(static_cast<float>(gjbgl->m_gameState.m_levelTime));
            m_variables["levelLength"] = rift::Value::floating(gjbgl->m_levelLength);
            m_variables["levelDuration"] = rift::Value::floating(gjbgl->m_level->m_timestamp / 240.f);
            m_variables["time"] = rift::Value::string(utils::formatTime(gjbgl->m_gameState.m_levelTime));
            m_variables["frame"] = rift::Value::integer(static_cast<int>(gjbgl->m_gameState.m_levelTime * 240.f));
            m_variables["isDead"] = rift::Value::boolean(gjbgl->m_player1->m_isDead);
            m_variables["isDualMode"] = rift::Value::boolean(gjbgl->m_player2 != nullptr && gjbgl->m_player2->isRunning()); // can m_isDualMode be added already
            m_variables["noclipDeaths"] = rift::Value::integer(config::getTemp("noclipDeaths", 0));
            m_variables["noclipAccuracy"] = rift::Value::floating(config::getTemp("noclipAccuracy", 100.f));
            m_variables["progress"] = rift::Value::floating(utils::getActualProgress(gjbgl));
            if (auto* pl = gameManager->m_playLayer) {
                m_variables["editorMode"] = rift::Value::boolean(false);
                m_variables["realProgress"] = rift::Value::floating(pl->getCurrentPercent());
                m_variables["objects"] = rift::Value::integer(level->m_objectCount);
            } else if (auto* ed = gameManager->m_levelEditorLayer) {
                m_variables["editorMode"] = rift::Value::boolean(true);
                m_variables["realProgress"] = rift::Value::floating(0.f);
                m_variables["objects"] = rift::Value::integer(static_cast<int>(ed->m_objects->count()));
            }

            // Player icon
            auto gamemode = utils::getGameMode(gjbgl->m_player1);
            m_variables["gamemode"] = rift::Value::string(utils::gameModeName(gamemode));
            m_variables["playerIcon"] = rift::Value::integer(utils::getPlayerIcon(gamemode));
        } else {
            removeVariable("levelID");
            removeVariable("levelName");
            removeVariable("author");
            removeVariable("isRobtopLevel");
            removeVariable("levelAttempts");
            removeVariable("levelStars");
            removeVariable("difficulty");
            removeVariable("difficultyKey");
            removeVariable("practicePercent");
            removeVariable("bestPercent");
            removeVariable("bestTime");
            removeVariable("best");
            removeVariable("playerX");
            removeVariable("playerY");
            removeVariable("player2X");
            removeVariable("player2Y");
            removeVariable("attempt");
            removeVariable("isTestMode");
            removeVariable("isPracticeMode");
            removeVariable("isPlatformer");
            removeVariable("levelTime");
            removeVariable("levelLength");
            removeVariable("levelDuration");
            removeVariable("time");
            removeVariable("frame");
            removeVariable("isDead");
            removeVariable("isDualMode");
            removeVariable("noclipDeaths");
            removeVariable("noclipAccuracy");
            removeVariable("editorMode");
            removeVariable("progress");
            removeVariable("realProgress");
            removeVariable("objects");
            removeVariable("runFrom");
            removeVariable("bestRun");
            removeVariable("lastDeath");

            auto gamemode = utils::getGameMode(nullptr);
            m_variables["gamemode"] = rift::Value::string(utils::gameModeName(gamemode));
            m_variables["playerIcon"] = rift::Value::integer(utils::getPlayerIcon(gamemode));
        }
    }

    class $modify(BestRunPLHook, PlayLayer) {
        struct Fields {
            float m_runFrom = 0.f;
            float m_lastRunFrom = 0.f;
            float m_bestRun = 0.f;
            float m_lastBestRun = 0.f;
        };

        bool init(GJGameLevel* level, bool useReplay, bool dontCreateObjects) {
            if (!PlayLayer::init(level, useReplay, dontCreateObjects)) return false;

            auto& manager = VariableManager::get();
            manager.setVariable("runFrom", rift::Value::floating(0.f));
            manager.setVariable("bestRun", rift::Value::floating(0.f));

            return true;
        }

        void saveBestRun() {
            auto fields = m_fields.self();
            fields->m_bestRun = utils::getActualProgress(this);
            if ((fields->m_bestRun - fields->m_runFrom) >= (fields->m_lastBestRun - fields->m_lastRunFrom)) {
                fields->m_lastBestRun = fields->m_bestRun;
                fields->m_lastRunFrom = fields->m_runFrom;
                auto& manager = VariableManager::get();
                manager.setVariable("runFrom", rift::Value::floating(fields->m_runFrom));
                manager.setVariable("bestRun", rift::Value::floating(fields->m_bestRun));
            }
        }

        void levelComplete() {
            PlayLayer::levelComplete();
            saveBestRun();
        }
        
        void destroyPlayer(PlayerObject* player, GameObject* object) override {
            auto percentage = utils::getActualProgress(this);
            PlayLayer::destroyPlayer(player, object);
            if (object != m_anticheatSpike) {
                VariableManager::get().setVariable("lastDeath", rift::Value::from(percentage));
                saveBestRun();
            }
        }

        void resetLevel() {
            PlayLayer::resetLevel();
            m_fields->m_runFrom = utils::getActualProgress(this);
        }
    };
}
