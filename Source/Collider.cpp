#include "SDL/include/SDL.h"
#include "Collider.h"
#include "Gameobject.h"
#include "Transform.h"
#include "Vector3.h"
#include "Application.h"
#include "Render.h"
#include "Log.h"
#include "CollisionSystem.h"
#include "Map.h"
#include "Point.h"
#include "JuicyMath.h"


Collider::Collider(Gameobject* go, RectF coll, ColliderType t, ColliderTag tg, RectF off ,CollisionLayer lay, ComponentType ty) : Component(ty, go)
{
    /*std::pair<float, float> localPos = Map::F_MapToWorld(coll.x, coll.y, 1.0f);
	boundary.x = localPos.first;
    boundary.y = localPos.second;
    boundary.w = coll.w * 64;
    boundary.h = coll.h * 64;*/
    //vec s = game_object->GetTransform()->GetGlobalScale();
    tileSize = Map::GetTileSize_F();
    //std::pair<float, float> pos = Map::F_MapToWorld(coll.x, coll.y);
    boundary = coll;
    boundary.w *= tileSize.first;// *= coll.w;
    boundary.h *= tileSize.second;// *= coll.h;

    collType = t;
    layer = lay;
    tag = tg;
    App->collSystem.Add(this);
    offset = off;
}

Collider::~Collider()
{}

void Collider::SetPosition()
{
    vec localpos = game_object->GetTransform()->GetGlobalPosition();
    std::pair<float, float> pos = Map::F_MapToWorld(localpos.x, localpos.y, localpos.z);
    //std::pair<float, float> localPos = Map::F_MapToWorld(pos.x, pos.y, pos.z);
    //LOG("Coll pos X:%f/Y:%f", pos.x, pos.y);
   // boundary.x = localPos.first - (boundary.w / 2) + offset.x;
   // boundary.y = localPos.second - (boundary.h / 2) + offset.y;
    boundary.x = pos.first; //- (boundary.w / 2) + offset.x;
    boundary.y = pos.second; //- (boundary.h / 2) + offset.y;
    ConvertToIsoPoints();
    //LOG("Bound pos X:%f/Y:%f",boundary.x,boundary.y);
}

void Collider::ConvertToIsoPoints()
{
    vec s = game_object->GetTransform()->GetGlobalScale();
    /*isoDraw.left = { boundary.x, boundary.y + (boundary.h * 0.5f) - (boundary.h * s.z) };
    isoDraw.bot = { boundary.x + (boundary.w * 0.5f), boundary.y + boundary.h - (boundary.h * s.z) };
    isoDraw.right = { boundary.x + boundary.w, boundary.y + (boundary.h * 0.5f) - (boundary.h * s.z) };
    isoDraw.top = { boundary.x + (boundary.w * 0.5f) , boundary.y - (boundary.h * s.z) };*/
    isoDraw.left = { boundary.x, boundary.y + (boundary.h * 0.5f) };
    isoDraw.bot = { boundary.x + (boundary.w * 0.5f), boundary.y + boundary.h };
    isoDraw.right = { boundary.x + boundary.w, boundary.y + (boundary.h * 0.5f) };
    isoDraw.top = { boundary.x + (boundary.w * 0.5f) , boundary.y };
    isoDraw.right.first += offset.x;
    isoDraw.left.first += offset.x;
    isoDraw.top.first += offset.x;
    isoDraw.bot.first += offset.x;
    isoDraw.right.second += offset.y;
    isoDraw.left.second += offset.y;
    isoDraw.top.second += offset.y;
    isoDraw.bot.second += offset.y;
}

IsoLinesCollider Collider::GetIsoPoints() { return isoDraw; }

Manifold Collider::Intersects(Collider* other)
{
    Manifold m;
    //const RectF thisColl = GetWorldColliderBounds();
    //const RectF otherColl = other->GetWorldColliderBounds();
    m.colliding = false;
    m.other = other->GetIsoPoints();
    //LOG("This coll W:%f/H:%f",thisColl.w,thisColl.h);
    //LOG("Other coll W:%f/H:%f", otherColl.w, otherColl.h);

    IsoLinesCollider otherColl = other->GetIsoPoints();

    /*fPoint topRight1(thisColl.x+thisColl.w, thisColl.y);
    fPoint topRight2(otherColl.x+otherColl.w, otherColl.y);
    fPoint bottomLeft1(thisColl.x, thisColl.y + thisColl.h);
    fPoint bottomLeft2(otherColl.x, otherColl.y + otherColl.h);
    //LOG("This coll tr X:%f/Y:%f    bl X:%f/Y:%f", topRight1.x, topRight1.y, bottomLeft1.x, bottomLeft1.y);
    //LOG("Other  coll tr X:%f/Y:%f    bl X:%f/Y:%f", topRight2.x, topRight2.y, bottomLeft2.x, bottomLeft2.y);
    if (topRight1.y > bottomLeft2.y || bottomLeft1.y < topRight2.y ||
        topRight1.x < bottomLeft2.x || bottomLeft1.x > topRight2.x) //Non colliding
    {
        //LOG("Non colliding");
        m.colliding = false;
        m.other = nullptr;
    }*/

    //LOG("This coll top X:%f/Y:%f     bot X:%f/Y:%f", isoDraw.top.first, isoDraw.bot.second, isoDraw.bot.first, isoDraw.bot.second);
    //LOG("Other  coll top X:%f/Y:%f    bot X:%f/Y:%f", otherColl.top.first, otherColl.top.second, otherColl.bot.first, otherColl.bot.second);
    if (JMath::PointInsideTriangle(isoDraw.top,otherColl.top,otherColl.left,otherColl.right))
    {
        m.colliding = true;
        m.other = other->GetIsoPoints();
    }
    else if (JMath::PointInsideTriangle(isoDraw.top, otherColl.bot, otherColl.left, otherColl.right))
    {
        m.colliding = true;
        m.other = other->GetIsoPoints();
    }
    if (JMath::PointInsideTriangle(isoDraw.bot, otherColl.top, otherColl.left, otherColl.right))
    {
        m.colliding = true;
        m.other = other->GetIsoPoints();
    }
    else if (JMath::PointInsideTriangle(isoDraw.bot, otherColl.bot, otherColl.left, otherColl.right))
    {
        m.colliding = true;
        m.other = other->GetIsoPoints();
    }
    if (JMath::PointInsideTriangle(isoDraw.right, otherColl.top, otherColl.left, otherColl.right))
    {
        m.colliding = true;
        m.other = other->GetIsoPoints();
    }
    else if (JMath::PointInsideTriangle(isoDraw.right, otherColl.bot, otherColl.left, otherColl.right))
    {
        m.colliding = true;
        m.other = other->GetIsoPoints();
    }
    if (JMath::PointInsideTriangle(isoDraw.left, otherColl.top, otherColl.left, otherColl.right))
    {
        m.colliding = true;
        m.other = other->GetIsoPoints();
    }
    else if (JMath::PointInsideTriangle(isoDraw.left, otherColl.bot, otherColl.left, otherColl.right))
    {
        m.colliding = true;
        m.other = other->GetIsoPoints();
    }
    /*if (isoDraw.top.first > otherColl.top.first || isoDraw.bot.second < otherColl.top.second ||
        isoDraw.top.first < otherColl.bot.first || isoDraw.bot.first > otherColl.top.first) //Non colliding
    {
        LOG("Non colliding");
        m.colliding = false;
    }*/

   /* if (topRight1.y > bottomLeft2.y)
    {
        LOG("Bottom");
    }
    if (bottomLeft1.y < topRight2.y)
    {
        LOG("TOP");
    }
    if (topRight1.x < bottomLeft2.x)
    {
        LOG("LEFT");
    }
    if (bottomLeft1.x > topRight2.x)
    {
        LOG("RIGHT");
    }*/

 
    return m;
}

void Collider::ResolveOverlap(Manifold& m)
{
    if (collType != TRIGGER)
    {
        Transform* t = game_object->GetTransform();
        vec pos = t->GetGlobalPosition();
        //const RectF& rect1 = GetColliderBounds();
        const RectF rect1 = GetColliderBounds();
        //const RectF* rect2 = m.other;
        /*std::pair<float,float> otherRect = Map::F_WorldToMap(m.other->x,m.other->y);
        float res = 0;
        float xDif = (rect1.x + (rect1.w * 0.5f)) - (otherRect.first + (m.other->w * 0.5f));
        float yDif = (rect1.y + (rect1.h * 0.5f)) - (otherRect.second + (m.other->h * 0.5f));*/
        IsoLinesCollider otherColl = m.other;
        float res = 0;
        float xDif = isoDraw.top.first - otherColl.top.first;
        float yDif = isoDraw.left.second - otherColl.left.second;


        LOG("Xdif: %f/Ydif:%f",xDif,yDif);
        /*if (xDif > 0) // Colliding on the left.
        {
            res = 9.5f;//(rect2->x + rect2->w) - rect1.x;
        }
        else // Colliding on the right.
        {
            res = 9.5f;//((rect1.x + rect1.w) - rect2->x);
        }*/
        //LOG("Move res %f",res);
        //pos.x += res;
        //if(App->pathfinding.ValidTile(int(pos.x), int(pos.y)) == true) t->MoveX(res * 0.5 * App->time.GetGameDeltaTime());//Move x      
        t->MoveX(xDif * App->time.GetGameDeltaTime());//Move x      

        /*if (yDif > 0) // Colliding above.
        {
            res = -9.5f;//(rect2->y + rect2->h) - rect1.y;
        }
        else // Colliding below
        {
            res = 9.5f;// ((rect1.y + rect1.h) - rect2->y);
        }*/
        //LOG("Move res %f", res);
        //pos.y += res;
        //if(App->pathfinding.ValidTile(int(pos.x), int(pos.y)) == true) t->MoveY(res * 0.5 * App->time.GetGameDeltaTime());//Move y
        t->MoveY(yDif * App->time.GetGameDeltaTime());//Move y
        
        //vec pos = t->GetGlobalPosition();
        //LOG("New pos X:%f/Y:%f",pos.x,pos.y);
    }  
}

void Collider::SaveCollision(double id)
{    
    if (!GetCollisionState(id))
    {
        collisions.push_back(id);
    }    
}

bool Collider::GetCollisionState(double ID)
{
    bool ret = false;
    for (std::vector<double>::iterator it = collisions.begin(); it != collisions.end(); ++it)
    {
        if (ID == *it) ret = true;
    }
    return ret;
}

void Collider::DeleteCollision(double ID)
{
    for (std::vector<double>::iterator it = collisions.begin(); it != collisions.end(); ++it)
    {
        if (ID == *it)
        {
            collisions.erase(it);
            break;
        }
    }
}

RectF Collider::GetISOColliderBounds()
{ 
    std::pair<float, float> pos = Map::F_WorldToMap(boundary.x, boundary.y);
    std::pair<float, float> length = Map::F_WorldToMap(boundary.x + boundary.w, boundary.y + boundary.h);
    return  RectF({ pos.first,pos.second,length.first,length.second});
}

RectF Collider::GetWorldColliderBounds()
{
    std::pair<float, float> pos = Map::F_MapToWorld(boundary.x, boundary.y, 1.0f);
    //std::pair<float, float> length = Map::F_MapToWorld(boundary.x + boundary.w, boundary.y + boundary.h, 1.0f);
    return  RectF({ pos.first,pos.second,boundary.w,boundary.h });
}

void Collider::SetLayer(CollisionLayer lay) { layer = lay; }

CollisionLayer Collider::GetCollLayer() { return layer; }

void Collider::SetColliderBounds(RectF& rect) 
{
    vec s = game_object->GetTransform()->GetGlobalScale();
    std::pair<float, float> tile_size = Map::GetTileSize_F();

    boundary = rect;
    boundary.w = tile_size.first *= s.x;
    boundary.h = tile_size.second *= s.y;
}

RectF Collider::GetColliderBounds() { return boundary; }

void Collider::SetOffset(RectF off) { offset = off;}

void Collider::SetCollType(ColliderType t) { collType = t; }

ColliderType Collider::GetCollType() { return collType; }

void Collider::SetColliderTag(ColliderTag tg) { tag = tg; }

ColliderTag Collider::GetColliderTag() { return tag; }