#include "MOPV.h"
#include "../../ChunkHeader.h"
#include "../../../Wrappers/WMO_ROOT.h"

#include "../../../../Utils/ChunkUtils.h"

bool MOPV::Read(std::shared_ptr<Bytebuffer>& buffer, const ChunkHeader& header, WMO_ROOT& wmo)
{
    // Portals are not required
    if (header.size == 0)
        return true;

    return ChunkUtils::LoadArrayOfStructs(buffer, header.size, wmo.mopv.portalVertexList);
}