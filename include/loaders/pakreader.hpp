/*  include/pakreader.hpp PakReader - Yann BOUCHER (yann) 15/04/2016
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
#ifndef PAKREADER_HPP
#define PAKREADER_HPP

#include <string>

#include <ZipLib/ZipArchive.h>

#include <SFML/System/NonCopyable.hpp>

#include "common.hpp"
#include "state.hpp"

namespace Json
{
class Value;
}

namespace chaiscript
{
class ChaiScript;
}

namespace Rayfun
{

class Map;

class pakreader_error : public std::runtime_error
{
        using std::runtime_error::runtime_error;
};

[[noreturn]] inline void throw_pakreader_error(const std::string& t_file, const std::string& t_why)
{
    throw pakreader_error("Error during .pak reading (" + t_file + ") : " + t_why);
}

class PakReader : private sf::NonCopyable
{
    public:
        explicit PakReader(chaiscript::ChaiScript& t_chai, State::Context& t_context);
        PakReader(const std::string& t_path, chaiscript::ChaiScript& t_chai, PakContents& t_contents,
                  State::Context& t_context, size_t t_levelIndex = 0);

    public:
        void open(const std::string& t_path, PakContents& t_contents, size_t t_levelIndex = 0);

    private:
        Json::Value get(const Json::Value& t_root, const std::string& t_key,
                        bool t_nullIfInvalid = false) const;
        Directional<std::string> getDirectionalString(const Json::Value& t_root,
                                                      const std::string& t_keyBase,
                                                      bool& t_found) const;
        Directional<bool> getDirectionalBool(const Json::Value& t_root,
                                             const std::string& t_keyBase,
                                             bool& t_found) const;
        Json::Value fileToJson(const ZipArchive::Ptr t_archive,
                               const std::string& t_filePath);

        template <typename T>
        bool fileToSfLoad(const ZipArchive::Ptr t_archive,
                          const std::string& t_filePath, T& t_sf, bool t_loadInHolder = false) const;

        bool getImage(const std::string& t_filename, sf::Image& t_image, PakContents& t_contents) const;

        void loadInfo(const ZipArchive::Ptr t_archive, PakContents& t_contents, size_t t_levelIndex);
        void loadLevel(const ZipArchive::Ptr t_archive, const Json::Value& t_root, PakContents& t_contents);
        void loadTile(const ZipArchive::Ptr, const Json::Value& t_tile, Map& t_map, PakContents& t_contents) const;
        void loadSector(const ZipArchive::Ptr t_archive, const Json::Value& t_sector, Map& t_map, PakContents& t_contents) const;
        void loadTrigger(const ZipArchive::Ptr, const Json::Value& t_trigger, Map& t_map) const;
        void loadWeapon(const ZipArchive::Ptr t_archive, const Json::Value &t_root, PakContents& t_contents) const;

        void loadBillboard(const ZipArchive::Ptr t_archive, const Json::Value &t_root, Map &t_map) const;

    private:
        const std::string c_infoFile = "info.json";
        const std::string c_texPrefix = "textures/";
        const std::string c_soundsPrefix = "sounds/";
        const std::string c_levelPrefix = "levels/";
        const std::string c_scriptsPrefix = "scripts/";
        const std::string c_weaponsPrefix = c_scriptsPrefix + "weapons/";

    private:
        chaiscript::ChaiScript& m_chai;
        State::Context& m_context;
        std::string m_path;
        std::string m_currentFile;
};

} // namespace Rayfun

#include "pakreader.tpp"

#endif // PAKREADER_HPP
