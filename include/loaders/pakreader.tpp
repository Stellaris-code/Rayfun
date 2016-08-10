/*  pakreader %{Cpp:License:ClassName} - Yann BOUCHER (yann) 05/05/2016
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
#ifndef PAKREADER_TPP
#define PAKREADER_TPP

#include <SFML/Graphics/Texture.hpp>
#include <SFML/Graphics/Image.hpp>
#include <SFML/Audio/Music.hpp>
#include <SFML/Audio/SoundBuffer.hpp>
#include <SFML/Graphics/Font.hpp>

#include <Thor/Resources/SfmlLoaders.hpp>
#include <Thor/Resources/ResourceExceptions.hpp>

#include "utility.hpp"
#include "graphicsutility.hpp"

#include "pakcontents.hpp"

namespace Rayfun
{

template <typename T>
inline bool PakReader::fileToSfLoad(const ZipArchive::Ptr t_archive,
                                    const std::string& t_filePath, T& t_sf, bool) const
{
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

    std::vector<char> data;
    data.resize(entry->GetSize());
    decompressionStream->read(data.data(), entry->GetSize());
    return t_sf.loadFromMemory(reinterpret_cast<void*>(data.data()), entry->GetSize());
}

template <>
inline bool PakReader::fileToSfLoad(const ZipArchive::Ptr t_archive,
                                    const std::string& t_filePath, sf::Image& t_sf, bool t_registerInHolder) const
{
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

    std::vector<char> data;
    data.resize(entry->GetSize());
    decompressionStream->read(data.data(), entry->GetSize());

    if (t_registerInHolder)
    {
        try
        {

            t_sf = m_context.resources.imageHolder.acquire(Utility::filenameFromPath(t_filePath),
                                                                thor::Resources::fromMemory<sf::Image>(reinterpret_cast<void*>(data.data()), entry->GetSize()),
                                                                thor::Resources::Reuse);
            return true;
        }
        catch (thor::ResourceLoadingException& e)
        {
            std::cerr << e.what() << "\n";
            return false;
        }
    }
    else
    {
        return t_sf.loadFromMemory(reinterpret_cast<void*>(data.data()), entry->GetSize());
        return false;
    }
}

template <>
inline bool PakReader::fileToSfLoad(const ZipArchive::Ptr t_archive,
                                    const std::string& t_filePath, sf::Texture& t_sf, bool t_registerInHolder) const
{
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

    std::vector<char> data;
    data.resize(entry->GetSize());
    decompressionStream->read(data.data(), entry->GetSize());

    if (t_registerInHolder)
    {
        try
        {

            t_sf = m_context.resources.textureHolder.acquire(Utility::filenameFromPath(t_filePath),
                                                             thor::Resources::fromMemory<sf::Texture>(reinterpret_cast<void*>(data.data()), entry->GetSize()),
                                                             thor::Resources::Reuse);
            return true;
        }
        catch (thor::ResourceLoadingException& e)
        {
            std::cerr << e.what() << "\n";
            return false;
        }
    }
    else
    {
        return t_sf.loadFromMemory(reinterpret_cast<void*>(data.data()), entry->GetSize());
    }
}

template <>
inline bool PakReader::fileToSfLoad(const ZipArchive::Ptr t_archive,
                                    const std::string& t_filePath, sf::Font& t_sf, bool t_registerInHolder) const
{
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

    std::vector<char> data;
    data.resize(entry->GetSize());
    decompressionStream->read(data.data(), entry->GetSize());

    if (t_registerInHolder)
    {
        try
        {

            t_sf = m_context.resources.fontHolder.acquire(Utility::filenameFromPath(t_filePath),
                                                          thor::Resources::fromMemory<sf::Font>(reinterpret_cast<void*>(data.data()), entry->GetSize()),
                                                          thor::Resources::Reuse);
            return true;
        }
        catch (thor::ResourceLoadingException& e)
        {
            std::cerr << e.what() << "\n";
            return false;
        }
    }
    else
    {
        return t_sf.loadFromMemory(reinterpret_cast<void*>(data.data()), entry->GetSize());
    }
}

template <>
inline bool PakReader::fileToSfLoad(const ZipArchive::Ptr t_archive,
                                    const std::string& t_filePath, sf::Music& t_sf, bool t_registerInHolder) const
{
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

    std::vector<char> data;
    data.resize(entry->GetSize());
    decompressionStream->read(data.data(), entry->GetSize());
    return t_sf.openFromMemory(reinterpret_cast<void*>(data.data()), entry->GetSize());
}

template <>
inline bool PakReader::fileToSfLoad(const ZipArchive::Ptr t_archive,
                                    const std::string& t_filePath, sf::SoundBuffer& t_sf, bool t_registerInHolder) const
{
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

    std::vector<char> data;
    data.resize(entry->GetSize());
    decompressionStream->read(data.data(), entry->GetSize());

    if (t_registerInHolder)
    {
        try
        {

            t_sf = m_context.resources.soundHolder.acquire(Utility::filenameFromPath(t_filePath),
                                                           thor::Resources::fromMemory<sf::SoundBuffer>(reinterpret_cast<void*>(data.data()), entry->GetSize()),
                                                           thor::Resources::Reuse);
            return true;
        }
        catch (thor::ResourceLoadingException& e)
        {
            std::cerr << e.what() << "\n";
            return false;
        }
    }
    else
    {
        return t_sf.loadFromMemory(reinterpret_cast<void*>(data.data()), entry->GetSize());
    }
}

}

#endif // PAKREADER_TPP
