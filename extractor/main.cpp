#include <NovusTypes.h>
#include <fstream>
#include "MPQ/MPQLoader.h"
#include "DBC/DBCLoader.h"
#include "MAP/MAPLoader.h"
#include "Utils/ServiceLocator.h"

i32 main()
{
    std::shared_ptr<MPQLoader> mpqLoader = std::make_shared<MPQLoader>();
    std::shared_ptr<DBCReader> dbcReader = std::make_shared<DBCReader>();

    ServiceLocator::SetMPQLoader(mpqLoader);
    ServiceLocator::SetDBCReader(dbcReader);

    /* Runs and validaties all 280k files (3.3.5a)
        mpqLoader->__Test__();

        Example usage of GetFIle

        std::string file = "Tileset\\Generic\\Black.blp";
        std::shared_ptr<ByteBuffer> buffer = mpqLoader->GetFile(file);
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

        std::vector<std::string> adtLocations;
        if (DBCLoader::LoadMap(adtLocations))
        {
            MapLoader::LoadMaps(adtLocations);
        }

        DBCLoader::LoadEmotesText();
        DBCLoader::LoadSpell();

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
