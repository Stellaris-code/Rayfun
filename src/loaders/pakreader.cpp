/*  src/pakreader.cpp PakReader - Yann BOUCHER (yann) 15/04/2016
**
**
**            DO WHAT THE FUCK YOU WANT TO PUBLIC LICENSE
**                    Version 2, December 2004
**
** Copyright (C) 2004 Sam Hocevar <sam@hocevar.net>
**
** Everyone is permitted to copy and distribute verbatim or modified
** copies of this license document, and changing it is allowed as long
** as the name is changed.
**
**            DO WHAT THE FUCK YOU WANT TO PUBLIC LICENSE
**   TERMS AND CONDITIONS FOR COPYING, DISTRIBUTION AND MODIFICATION
**
**  0. You just DO WHAT THE FUCK YOU WANT TO.
*/

#include <type_traits>
#include <thread>

#include <boost/algorithm/clamp.hpp>

#include <json/value.h>
#include <json/reader.h>

#include "ZipLib/ZipFile.h"

#include "chaiscript/chaiscript.hpp"

#include "player.hpp"
#include "billboard.hpp"

#include "loaders/pakreader.hpp"

namespace Rayfun
{

PakReader::PakReader(chaiscript::ChaiScript& t_chai, State::Context &t_context)
    : m_chai(t_chai), m_context(t_context)
{}

PakReader::PakReader(const std::string &t_path, chaiscript::ChaiScript &t_chai, PakContents& t_contents,
                     State::Context &t_context, size_t t_levelIndex)
    : m_chai(t_chai), m_context(t_context)
{
    open(t_path, t_contents, t_levelIndex);
}

void PakReader::open(const std::string& t_path, PakContents& t_contents, size_t t_levelIndex)
{
    m_path = t_path;
    ZipArchive::Ptr archive = ZipFile::Open(m_path);
    if (archive == nullptr)
    {
        throw_pakreader_error(m_path, "cannot open file !");
    }
    try
    {
        loadInfo(archive, t_contents, t_levelIndex);
    }
    catch (const Json::RuntimeError& e)
    {
        throw_pakreader_error(m_currentFile, std::string("JSON error : ") + e.what());
    }
    catch (const chaiscript::exception::eval_error& e)
    {
        throw_pakreader_error(m_currentFile, "Chaiscript error : " + e.pretty_print());
    }
}

Json::Value PakReader::get(const Json::Value& t_root, const std::string &t_key, bool t_nullIfInvalid) const
{
    if (!t_root.isMember(t_key))
    {
        if (t_nullIfInvalid)
        {
            return Json::Value{}; // return null
        }
        else
        {
            throw_pakreader_error(m_currentFile, "cannot find '" + t_key + "' in file " + m_currentFile + " !");
        }
    }
    return t_root[t_key];
}

Directional<std::string> PakReader::getDirectionalString(const Json::Value &t_root, const std::string &t_keyBase, bool& t_found) const
{
    Directional<std::string> data { std::string{} };
    t_found = false;
    data.set(get(t_root, t_keyBase, true).asString());
    if (!get(t_root, t_keyBase, true).isNull())
    {
        t_found = true;
    }

    if (!get(t_root, t_keyBase + "north", true).isNull())
    {
        t_found = true;
        data[Side::North] = get(t_root, t_keyBase + "north").asString();
    }
    if (!get(t_root, t_keyBase + "south", true).isNull())
    {
        t_found = true;
        data[Side::South] = get(t_root, t_keyBase + "south").asString();
    }
    if (!get(t_root, t_keyBase + "east", true).isNull())
    {
        t_found = true;
        data[Side::East] = get(t_root, t_keyBase + "east").asString();
    }
    if (!get(t_root, t_keyBase + "west", true).isNull())
    {
        t_found = true;
        data[Side::West] = get(t_root, t_keyBase + "west").asString();
    }

    return data;
}

Directional<bool> PakReader::getDirectionalBool(const Json::Value &t_root, const std::string &t_keyBase, bool &t_found) const
{
    Directional<bool> data { false };
    t_found = false;
    data.set(get(t_root, t_keyBase, true).asBool());
    if (!get(t_root, t_keyBase, true).isNull())
    {
        t_found = true;
    }

    if (!get(t_root, t_keyBase + "north", true).isNull())
    {
        t_found = true;
        data[Side::North] = get(t_root, t_keyBase + "north").asBool();
    }
    if (!get(t_root, t_keyBase + "south", true).isNull())
    {
        t_found = true;
        data[Side::South] = get(t_root, t_keyBase + "south").asBool();
    }
    if (!get(t_root, t_keyBase + "east", true).isNull())
    {
        t_found = true;
        data[Side::East] = get(t_root, t_keyBase + "east").asBool();
    }
    if (!get(t_root, t_keyBase + "west", true).isNull())
    {
        t_found = true;
        data[Side::West] = get(t_root, t_keyBase + "west").asBool();
    }

    return data;
}

Json::Value PakReader::fileToJson(const ZipArchive::Ptr t_archive,
                                  const std::string &t_filePath)
{
    Json::Value root;
    auto entry = t_archive->GetEntry(t_filePath);
    std::istream * decompressionStream = nullptr;
    if (entry != nullptr)
    {
        decompressionStream = entry->GetDecompressionStream();
    }
    if (entry == nullptr  || decompressionStream == nullptr)
    {
        throw_pakreader_error(m_currentFile, "cannot open '" + t_filePath + "'' !");
    }

    m_currentFile = t_filePath;

    Json::CharReaderBuilder builder;
    builder["allowSingleQuotes"] = true;
    std::string errs;
    bool ok = Json::parseFromStream(builder, *decompressionStream, &root, &errs);
    if (!ok)
    {
        throw_pakreader_error(m_currentFile, "Error during json parsing : " + errs);
    }
    return root;
}

void PakReader::loadInfo(const ZipArchive::Ptr t_archive, PakContents &t_contents, size_t t_levelIndex)
{
    Json::Value root = fileToJson(t_archive, c_infoFile);

    t_contents.title = get(root, "title").asString();
    t_contents.desc = get(root, "desc").asString();
    t_contents.author = get(root, "author").asString();
    t_contents.date = get(root, "date").asString();

    if (!fileToSfLoad(t_archive, c_texPrefix + get(root, "menu_image").asString(), t_contents.menuBkg))
    {
        throw_pakreader_error(m_currentFile, "cannot open '" + get(root, "menu_image").asString() + "' !");
    }

    if (!get(root, "menu_music", true).isNull() && !fileToSfLoad(t_archive,
                                                                 get(root, "menu_music").asString(), t_contents.menuMusic))
    {
        throw_pakreader_error(m_currentFile, "cannot open '" + get(root, "menu_music").asString() + "' !");
    }

    if (!get(root, "weapons", true).isNull())
    {
        if (!get(root, "weapons").isArray())
        {
            throw_pakreader_error(m_currentFile, "'weapons' key has to be an array !");
        }

        for (auto v : get(root, "weapons"))
        {
            loadWeapon(t_archive, fileToJson(t_archive, c_weaponsPrefix + v.asString()), t_contents);
        }
    }

    if (!get(root, "textures", true).isNull())
    {
        if (!get(root, "textures").isArray())
        {
            throw_pakreader_error(m_currentFile, "'textures' key has to be an array !");
        }

        for (auto v : get(root, "textures"))
        {
            sf::Texture tex;
            fileToSfLoad(t_archive, c_texPrefix + v.asString(), tex, true);
        }
    }

    if (!get(root, "images", true).isNull())
    {
        if (!get(root, "images").isArray())
        {
            throw_pakreader_error(m_currentFile, "'images' key has to be an array !");
        }

        for (auto v : get(root, "images"))
        {
            sf::Image img;
            fileToSfLoad(t_archive, c_texPrefix + v.asString(), img, true);
            t_contents.textures.addTexture(v.asString(), img);
        }
    }

    if (!get(root, "sounds", true).isNull())
    {
        if (!get(root, "sounds").isArray())
        {
            throw_pakreader_error(m_currentFile, "'sounds' key has to be an array !");
        }

        for (auto v : get(root, "sounds"))
        {
            sf::SoundBuffer sound;
            fileToSfLoad(t_archive, c_soundsPrefix + v.asString(), sound, true);
        }
    }

    if (!get(root, "levels").isArray())
    {
        throw_pakreader_error(m_currentFile, "'levels' key has to be an array !");
    }

    if (t_levelIndex >= get(root, "levels").size())
    {
        throw_pakreader_error(m_currentFile, "level index is invalid ! (" + std::to_string(t_levelIndex)
                              + ", max is : " + std::to_string(get(root, "levels").size()) + ")");
    }

    loadLevel(t_archive, fileToJson(t_archive, c_levelPrefix +
                                    get(root, "levels")[unsigned(t_levelIndex)].asString()), t_contents);
}

void PakReader::loadLevel(const ZipArchive::Ptr t_archive, const Json::Value &t_root, PakContents &t_contents)
{
    sf::Vector2s mapSize;
    mapSize.x = get(t_root, "width").asUInt();
    mapSize.y = get(t_root, "height").asUInt();

    t_contents.level.map.setSize(mapSize);

    t_contents.level.map.ambientLight = boost::algorithm::clamp(get(t_root, "ambientlight").asUInt(), 0, 255);

    t_contents.level.name = get(t_root, "name").asString();
    t_contents.level.parTime = sf::seconds(get(t_root, "partime").asUInt());
    if (!get(t_root, "music", true).isNull() &&
            !fileToSfLoad(t_archive, get(t_root, "music").asString(), t_contents.level.music))
    {
        throw_pakreader_error(m_currentFile, "cannot open '" + get(t_root, "music").asString() + "' !");
    }

    t_contents.level.beginMessage = get(t_root, "beginmessage", true).asString();
    if (!get(t_root, "beginmessage", true).isNull())
    {
        if (!fileToSfLoad(t_archive, c_texPrefix + get(t_root, "beginmessageimage").asString(), t_contents.level.beginBkg))
        {
            throw_pakreader_error(m_currentFile, "cannot open '" + get(t_root, "beginmessageimage").asString() + "' !");
        }
        else
        {
            t_contents.level.intro = true;
        }
    }

    t_contents.level.endMessage = get(t_root, "endmessage", true).asString();
    if (!get(t_root, "endmessage", true).isNull())
    {
        if (!fileToSfLoad(t_archive, c_texPrefix + get(t_root, "endmessageimage").asString(), t_contents.level.endBkg))
        {
            throw_pakreader_error(m_currentFile, "cannot open '" + get(t_root, "endmessageimage").asString() + "' !");
        }
        else
        {
            t_contents.level.outro = true;
        }
    }

    for (auto tile : get(t_root, "tiles"))
    {
        loadTile(t_archive, tile, t_contents.level.map, t_contents);
    }
    for (auto sector : get(t_root, "sectors", true))
    {
        loadSector(t_archive, sector, t_contents.level.map, t_contents);
    }
    for (auto trigger : get(t_root, "triggers", true))
    {
        loadTrigger(t_archive, trigger, t_contents.level.map);
    }
    for (auto actor : get(t_root, "actors", true))
    {
        if (get(actor, "type") == "Billboard")
        {
            loadBillboard(t_archive, actor, t_contents.level.map);
        }
    }
}

void PakReader::loadTile(const ZipArchive::Ptr, const Json::Value& t_tile, Map& t_map, PakContents& t_contents) const
{
    Tile tile;
    tile.isWall = true;
    bool found = false;
    tile.clip = getDirectionalBool(t_tile, "clip", found);
    if (!found)
    {
        tile.clip = Directional<bool>{ true };
    }
    Directional<std::string> texNames = getDirectionalString(t_tile, "tex", found);
    sf::Image blank;
    blank.create(1, 1, sf::Color::Transparent);
    Directional<sf::Image> textures { blank };
    if (found)
    {
        for (auto side : { Side::North, Side::South, Side::East, Side::West })
        {
            tile.tex[side] = t_contents.textures.getId(texNames[side]);
            tile.brigthnessMap[side].resize(textures[side].getSize().x);
            std::fill(tile.brigthnessMap[side].begin(), tile.brigthnessMap[side].end(), 0);
        }
    }

    Directional<std::string> damageTriggers = getDirectionalString(t_tile, "ondamage", found);
    if (found)
    {
        for (auto side : { Side::North, Side::South, Side::East, Side::West })
        {
            if (!damageTriggers[side].empty())
            {
                auto function = m_chai.eval<std::remove_reference<decltype(tile.ondamage[side])>::type>(damageTriggers[side]);
                tile.ondamage[side] = [function, damageTriggers, side](Actor& act, Tile& intile, size_t dmg, std::string type)
                {
                    std::thread([function, damageTriggers, side, &act, &intile, dmg, type]{
                        function(act, intile, dmg, type);
                    }).detach();
                };
            }
        }
    }

    Directional<std::string> triggers = getDirectionalString(t_tile, "ontrigger", found);
    if (found)
    {
        for (auto side : { Side::North, Side::South, Side::East, Side::West })
        {
            if (!triggers[side].empty())
            {
                auto function = m_chai.eval<std::remove_reference<decltype(tile.ontrigger[side])>::type>(triggers[side]);
                tile.ontrigger[side] = [function, side, triggers](Actor& act, Tile& intile)
                {
                    //                    std::thread([function, triggers, side, &act, &intile]{
                    function(act, intile);
                    //      }).detach();
                };
            }
        }
    }

    if (!get(t_tile, "pos").isArray() || get(t_tile, "pos").size() <= 0)
    {
        throw_pakreader_error(m_currentFile, "key 'pos' has to be an array !");
    }

    for (auto v : get(t_tile, "pos"))
    {
        if (!v.isArray() || v.size() != 2)
        {
            throw_pakreader_error(m_currentFile, "tile position must be a pair of integers !");
        }
        t_map.setTileAt(sf::Vector2s{ v[0].asUInt(), v[1].asUInt() }, tile);
    }
}

void PakReader::loadSector(const ZipArchive::Ptr t_archive, const Json::Value& t_sector, Map& t_map, PakContents& t_contents) const
{
    Sector sector;
    Json::Value jsonRect = get(t_sector, "rect");
    if (!jsonRect.isArray() || jsonRect.size() != 2)
    {
        throw_pakreader_error(m_currentFile, "sector rect must be a pair of pair of integers !");
    }
    if (!jsonRect[0].isArray() || jsonRect[0].size() != 2)
    {
        throw_pakreader_error(m_currentFile, "sector pos must be a pair of integers !");
    }
    if (!jsonRect[1].isArray() || jsonRect[1].size() != 2)
    {
        throw_pakreader_error(m_currentFile, "sector size must be a pair of integers !");
    }

    sector.rect = sf::Rect<size_t>(jsonRect[0][0].asUInt(), jsonRect[0][1].asUInt(),
            jsonRect[1][0].asUInt(), jsonRect[1][1].asUInt());

    sf::Image blank;
    blank.create(1, 1, sf::Color::White);

    //    if (!getImage(get(t_sector, "floortex").asString(), sector.floor, t_contents))
    //    {
    //        throw_pakreader_error(m_currentFile, "cannot open '" + get(t_sector, "floortex").asString() + "' !");
    //    }

    //    if (!getImage(get(t_sector, "ceiltex").asString(), sector.ceiling, t_contents))
    //    {
    //        throw_pakreader_error(m_currentFile, "cannot open '" + get(t_sector, "ceiltex").asString() + "' !");
    //    }

    sector.floor = t_contents.textures.getId(get(t_sector, "floortex").asString());
    sector.ceiling = t_contents.textures.getId(get(t_sector, "ceiltex").asString());

    t_map.sectors.push_back(sector);
}

void PakReader::loadTrigger(const ZipArchive::Ptr, const Json::Value& t_trigger, Map& t_map) const
{
    Trigger trigger;
    Json::Value jsonRect = get(t_trigger, "rect");
    if (!jsonRect.isArray() || jsonRect.size() != 2)
    {
        throw_pakreader_error(m_currentFile, "sector rect must be a pair of pair of integers !");
    }
    if (!jsonRect[0].isArray() || jsonRect[0].size() != 2)
    {
        throw_pakreader_error(m_currentFile, "sector pos must be a pair of integers !");
    }
    if (!jsonRect[1].isArray() || jsonRect[1].size() != 2)
    {
        throw_pakreader_error(m_currentFile, "sector size must be a pair of integers !");
    }

    trigger.rect = sf::Rect<size_t>(jsonRect[0][0].asUInt(), jsonRect[0][1].asUInt(),
            jsonRect[1][0].asUInt(), jsonRect[1][1].asUInt());

    trigger.players = get(t_trigger, "player").asBool();
    trigger.monsters = get(t_trigger, "monsters").asBool();
    trigger.once = get(t_trigger, "once").asBool();

    if (!get(t_trigger, "ontrigger", true).isNull())
    {
        auto function = m_chai.eval<decltype(trigger.ontrigger)>(get(t_trigger, "ontrigger").asString());
        trigger.ontrigger = [function](Actor& act)
        {
            std::thread([function, &act]
            {
                function(act);
            }).detach();
        };
    }

    t_map.triggers.push_back(trigger);
}

void PakReader::loadWeapon(const ZipArchive::Ptr t_archive, const Json::Value &t_root, PakContents& t_contents) const
{
    WeaponStructure weapon;
    weapon.ammoType = get(t_root, "ammotype").asString();
    weapon.magazineSize = get(t_root, "magazine_size").asInt();
    weapon.name = get(t_root, "name").asString();

    if (!fileToSfLoad(t_archive, c_texPrefix + get(t_root, "spritesheet").asString(), weapon.spriteSheet))
    {
        throw_pakreader_error(m_currentFile, "cannot open '" + get(t_root, "spritesheet").asString() + "' !");
    }

    auto json_fire_anim = get(t_root, "fire_anim", true);
    if (!json_fire_anim.isNull())
    {
        thor::FrameAnimation fire_anim;
        float totalDuration { 0 };
        if (!json_fire_anim.isArray())
        {
            throw_pakreader_error(m_currentFile, "'fire_anim' must be an array !");
        }
        for (auto frame : json_fire_anim)
        {
            float duration = get(frame, "duration").asFloat();
            totalDuration += duration;
            auto jsonRect = get(frame, "frame");
            if (!jsonRect.isArray() || jsonRect.size() != 4)
            {
                throw_pakreader_error(m_currentFile, "'frame' must be an array of 4 ints !");
            }
            sf::IntRect rect;
            rect.left = jsonRect[0].asInt();
            rect.top = jsonRect[1].asInt();
            rect.width = jsonRect[2].asInt();
            rect.height = jsonRect[3].asInt();

            sf::Vector2f offset { 0, 0 };
            auto jsonOffset = get(frame, "offset", true);
            if (!jsonOffset.isNull())
            {
                if (!jsonOffset.isArray() || jsonOffset.size() != 2)
                {
                    throw_pakreader_error(m_currentFile, "'offset' must be an array of 2 floats !");
                }
                offset.x = jsonOffset[0].asFloat();
                offset.y = jsonOffset[1].asFloat();
            }
            fire_anim.addFrame(duration, rect, offset);
        }
        weapon.fire_anim = fire_anim;
        weapon.fireAnimDuration = sf::seconds(totalDuration);
    }

    auto json_idle_anim = get(t_root, "idle_anim", true);
    if (!json_idle_anim.isNull())
    {
        thor::FrameAnimation idle_anim;
        float totalDuration { 0 };
        if (!json_idle_anim.isArray())
        {
            throw_pakreader_error(m_currentFile, "'idle_anim' must be an array !");
        }
        for (auto frame : json_idle_anim)
        {
            float duration = get(frame, "duration").asFloat();
            totalDuration += duration;
            auto jsonRect = get(frame, "frame");
            if (!jsonRect.isArray() || jsonRect.size() != 4)
            {
                throw_pakreader_error(m_currentFile, "'idle' must be an array of 4 ints !");
            }
            sf::IntRect rect;
            rect.left = jsonRect[0].asInt();
            rect.top = jsonRect[1].asInt();
            rect.width = jsonRect[2].asInt();
            rect.height = jsonRect[3].asInt();

            sf::Vector2f offset { 0, 0 };
            auto jsonOffset = get(frame, "offset", true);
            if (!jsonOffset.isNull())
            {
                if (!jsonOffset.isArray() || jsonOffset.size() != 2)
                {
                    throw_pakreader_error(m_currentFile, "'offset' must be an array of 2 floats !");
                }
                offset.x = jsonOffset[0].asFloat();
                offset.y = jsonOffset[1].asFloat();
            }
            idle_anim.addFrame(duration, rect, offset);
        }
        weapon.idle_anim = idle_anim;
        weapon.idleAnimDuration = sf::seconds(totalDuration);
    }

    auto json_reload_anim = get(t_root, "reload_anim", true);
    if (!json_reload_anim.isNull())
    {
        thor::FrameAnimation reload_anim;
        float totalDuration { 0 };
        if (!json_reload_anim.isArray())
        {
            throw_pakreader_error(m_currentFile, "'reload_anim' must be an array !");
        }
        for (auto frame : json_reload_anim)
        {
            float duration = get(frame, "duration").asFloat();
            totalDuration += duration;
            auto jsonRect = get(frame, "frame");
            if (!jsonRect.isArray() || jsonRect.size() != 4)
            {
                throw_pakreader_error(m_currentFile, "'idle' must be an array of 4 ints !");
            }
            sf::IntRect rect;
            rect.left = jsonRect[0].asInt();
            rect.top = jsonRect[1].asInt();
            rect.width = jsonRect[2].asInt();
            rect.height = jsonRect[3].asInt();

            sf::Vector2f offset { 0, 0 };
            auto jsonOffset = get(frame, "offset", true);
            if (!jsonOffset.isNull())
            {
                if (!jsonOffset.isArray() || jsonOffset.size() != 2)
                {
                    throw_pakreader_error(m_currentFile, "'offset' must be an array of 2 floats !");
                }
                offset.x = jsonOffset[0].asFloat();
                offset.y = jsonOffset[1].asFloat();
            }
            reload_anim.addFrame(duration, rect, offset);
        }
        weapon.reload_anim = reload_anim;
        weapon.reloadAnimDuration = sf::seconds(totalDuration);
    }
    auto fire_callback = get(t_root, "onfire", true);
    if (!fire_callback.isNull())
    {
        if (!fire_callback.isString())
        {
            throw_pakreader_error(m_currentFile, "'onfire' must be a chaiscript script !");
        }

        auto function = m_chai.eval<std::remove_reference<decltype(weapon.fire_callback)>::type>
                        (fire_callback.asString());
        weapon.fire_callback = [function]
        {
            std::thread([function]
            {
                function();
            }).detach();
        };
    }

    auto reload_callback = get(t_root, "onreload", true);
    if (!reload_callback.isNull())
    {
        if (!reload_callback.isString())
        {
            throw_pakreader_error(m_currentFile, "'o' must be a chaiscript script !");
        }

        auto function = m_chai.eval<std::remove_reference<decltype(weapon.reload_callback)>::type>
                        (reload_callback.asString());
        weapon.reload_callback = [function]
        {
            std::thread([function]
            {
                function();
            }).detach();
        };
    }

    t_contents.weapons.push_back(weapon);
}

void PakReader::loadBillboard(const ZipArchive::Ptr t_archive, const Json::Value &t_root,  Map& t_map) const
{
    for (auto pos : get(t_root, "pos"))
    {
        t_map.sprites.emplace_back(std::make_unique<Billboard>(m_context));
        auto& sprite = *dynamic_cast<Billboard*>(t_map.sprites.back().get());
        sprite.pos = { pos[0].asDouble(), pos[1].asDouble() };
        sprite.angle = get(t_root, "angle").asDouble();
        sprite.clip = get(t_root, "clip").asBool();
        sprite.z = get(t_root, "z").asDouble();
        sprite.scale.x = get(t_root, "scale")[0].asDouble();
        sprite.scale.y = get(t_root, "scale")[1].asDouble();

        for (auto pair : get(t_root, "textures"))
        {
            auto angles = get(pair, "angle");
            double minAngle = angles[0].asDouble();
            double maxAngle = angles[1].asDouble();
            sf::Image tex;
            sf::Image blank;
            blank.create(1, 1, sf::Color::Transparent);
            if (!fileToSfLoad(t_archive, c_texPrefix + get(pair, "tex").asString(), tex))
            {
                tex = blank;
            }
            sprite.setImage(tex, minAngle, maxAngle);
        }

        bool found { false };

        std::string damageTrigger = get(t_root, "ondamage", found).asString();
        if (found)
        {
            if (!damageTrigger.empty())
            {
                auto function = m_chai.eval<std::remove_reference<decltype(sprite.ondamage)>::type>(damageTrigger);
                sprite.ondamage = [function, damageTrigger](Actor& act, DrawableActor& inactor, size_t dmg, std::string type)
                {
                    std::thread([function, damageTrigger, &act, &inactor, dmg, type]{
                        function(act, inactor, dmg, type);
                    }).detach();
                };
            }
        }

        std::string trigger = get(t_root, "ontrigger", found).asString();
        if (found)
        {
            if (!trigger.empty())
            {
                auto function = m_chai.eval<std::remove_reference<decltype(sprite.ontrigger)>::type>(trigger);
                sprite.ontrigger = [function, trigger](Actor& act, DrawableActor& inactor)
                {
                    std::thread([function, trigger, &act, &inactor]{
                        function(act, inactor);
                    }).detach();
                };
            }
        }
    }
}

}
