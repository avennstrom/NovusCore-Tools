#include <NovusTypes.h>
#include "MPQ/MPQLoader.h"
#include "DBC/DBCLoader.h"
#include "MAP/MAPLoader.h"
#include "Interface/InterfaceLoader.h"
#include "FileChunk/ChunkLoader.h"
#include "Utils/ServiceLocator.h"
#include <tracy/Tracy.hpp>

#ifdef TRACY_ENABLE
void* operator new(std::size_t count)
{
    auto ptr = malloc(count);
    TracyAlloc(ptr, count);
    return ptr;
}
void operator delete(void* ptr) noexcept
{
    TracyFree(ptr);
    free(ptr);
}

#endif

i32 main()
{
    ZoneScoped;
    std::shared_ptr<MPQLoader> mpqLoader = std::make_shared<MPQLoader>();
    std::shared_ptr<ChunkLoader> chunkLoader = std::make_shared<ChunkLoader>();
    std::shared_ptr<MapLoader> mapLoader = std::make_shared<MapLoader>();
    std::shared_ptr<DBCReader> dbcReader = std::make_shared<DBCReader>();

    ServiceLocator::SetMPQLoader(mpqLoader);
    ServiceLocator::SetChunkLoader(chunkLoader);
    ServiceLocator::SetDBCReader(dbcReader);

    /* Runs and validaties all 280k files (3.3.5a)
        mpqLoader->__Test__();

        Example usage of GetFIle

        std::string file = "Tileset\\Generic\\Black.blp";
        std::shared_ptr<Bytebuffer> buffer = mpqLoader->GetFile(file);
        if (buffer)
        {
        
        }
    */

    if (mpqLoader->Load())
    {
        // Create Output Folders
        {
            std::filesystem::path basePath = std::filesystem::current_path();
            std::filesystem::path baseFolderPath = basePath.string() + "/ExtractedData";
            std::filesystem::path sqlFolderPath = baseFolderPath.string() + "/Sql";
            std::filesystem::path mapFolderPath = baseFolderPath.string() + "/Maps";
            if (!std::filesystem::exists(baseFolderPath))
            {
                std::filesystem::create_directory(baseFolderPath);
            }

            if (!std::filesystem::exists(sqlFolderPath))
            {
                std::filesystem::create_directory(sqlFolderPath);
            }

            if (!std::filesystem::exists(mapFolderPath))
            {
                std::filesystem::create_directory(mapFolderPath);
            }
        }

        /* Example usage code for the new mpqloader & chunkloader
        std::shared_ptr<Bytebuffer> fileWDT = mpqLoader->GetFile("world\\maps\\Azeroth\\Azeroth.wdt");

        WDT wdt;
        if (!chunkLoader->LoadWDT(fileWDT, wdt))
        {
            // This could happen, but for now I want to assert it in this test scenario
            assert(false);
        }

        // Check if we should load ADTs (If the given flag is set, a single map object is used instead
        if ((wdt.mphd.flags & static_cast<u32>(MPHDFlags::UsesGlobalMapObj)) == 0)
        {
            std::shared_ptr<Bytebuffer> fileADT = mpqLoader->GetFile("world\\maps\\Azeroth\\Azeroth_29_30.adt");
            ADT adt;
            if (!chunkLoader->LoadADT(fileADT, wdt, adt))
            {
                // This could happen, but for now I want to assert it in this test scenario
                assert(false);
            }
        }
        */

        std::vector<std::string> internalMapNames;
        if (DBCLoader::LoadMap(internalMapNames))
        {
            mapLoader->LoadMaps(internalMapNames);
        }

        DBCLoader::LoadEmotesText();
        DBCLoader::LoadSpell();
        InterfaceLoader::LoadInterface();
       
        mpqLoader->Close();
        NC_LOG_SUCCESS("Finished extracting all data");
    }
    else
    {
        NC_LOG_ERROR("Failed to load any MPQs");
    }

    system("pause");
    return 0;
}