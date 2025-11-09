#include "voxelship/VoxelBlock.h"

#include <algorithm>

namespace fresh
{
namespace voxelship
{

VoxelBlock::VoxelBlock() : position(0.0f), size(1.0f), material() {}

VoxelBlock::VoxelBlock(const glm::vec3& pos, const glm::vec3& sz, const VoxelMaterial& mat)
    : position(pos), size(sz), material(mat)
{
}

bool VoxelBlock::intersects(const VoxelBlock& other) const
{
    glm::vec3 thisMin, thisMax, otherMin, otherMax;
    getBounds(thisMin, thisMax);
    other.getBounds(otherMin, otherMax);

    return (thisMin.x <= otherMax.x && thisMax.x >= otherMin.x) &&
           (thisMin.y <= otherMax.y && thisMax.y >= otherMin.y) &&
           (thisMin.z <= otherMax.z && thisMax.z >= otherMin.z);
}

bool VoxelBlock::contains(const glm::vec3& point) const
{
    glm::vec3 min, max;
    getBounds(min, max);

    return (point.x >= min.x && point.x <= max.x) && (point.y >= min.y && point.y <= max.y) &&
           (point.z >= min.z && point.z <= max.z);
}

void VoxelBlock::getBounds(glm::vec3& min, glm::vec3& max) const
{
    glm::vec3 halfSize = size * 0.5f;
    min = position - halfSize;
    max = position + halfSize;
}

} // namespace voxelship
} // namespace fresh
