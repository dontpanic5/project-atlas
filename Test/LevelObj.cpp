#include "LevelObj.h"
#include "BattyUtils.h"

bool LevelObj::s_texLoaded = false;
Texture LevelObj::s_tex;

LevelObj::LevelObj(Vector3 cornerPos, float width, float height, float length)
    : EnvironmentalObject({
    cornerPos.x + width / 2.0f,
    cornerPos.y + height / 2.0f,
    cornerPos.z + length / 2.0f
    })
{
    Mesh mesh = GenMeshCube(width, height, length);
    m_model = LoadModelFromMesh(mesh);

    if (!s_texLoaded)
        s_tex = LoadTexture("resources/aristotle-solar-system.png");
    SetMaterialTexture(&m_model.materials[0], MATERIAL_MAP_DIFFUSE, s_tex);

    m_bb.min = cornerPos;
    m_bb.max = { cornerPos.x + width, cornerPos.y + height, cornerPos.z + length };
}

void LevelObj::drawObj()
{
    EnvironmentalObject::drawObj();
#ifdef DEBUG
    DrawBoundingBox(m_bb, GREEN);
#endif // DEBUG
}

bool LevelObj::collisionCheck(BoundingBox bb) const
{
    return CheckCollisionBoxes(GetBoundingBox(), bb);
}

BoundingBox LevelObj::GetBoundingBox() const
{
    return m_bb;
}

float LevelObj::getOverlapDistance(BoundingBox bb, Vector3 direction) const
{
    return GetOverlapDistanceBoxBox(GetBoundingBox(), bb, direction);
}

float LevelObj::getOverlapDistance(Vector3 center, float radius) const
{
    return GetOverlapDistanceBoxSphere(GetBoundingBox(), center, radius);
}

RayCollision LevelObj::GetRayCollision(Ray ray, bool addBuffer) const
{
    return GetRayCollisionBox(ray, GetBoundingBox());
}
