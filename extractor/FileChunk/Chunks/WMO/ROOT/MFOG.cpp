#include "MFOG.h"
#include "../../ChunkHeader.h"
#include "../../../Wrappers/WMO_ROOT.h"

#include "../../../../Utils/ChunkUtils.h"

bool MFOG::Read(std::shared_ptr<Bytebuffer>& buffer, const ChunkHeader& header, WMO_ROOT& wmo)
{
    return ChunkUtils::LoadArrayOfStructs(buffer, header.size, wmo.mfog.data);
}