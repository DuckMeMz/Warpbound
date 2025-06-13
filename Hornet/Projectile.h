#pragma once
#include "GameObject.h"
class Projectile :
    public GameObject
{
public:
    Projectile(const Vector2D& position, const Vector2D& velocity, int damage,
        const std::string& imagePath, ObjectType objType, GameObject* owner);

    virtual ~Projectile() {}

    virtual void Update(double frametime) override;

    virtual void ProcessCollision(GameObject& otherObject) override;

    Vector2D getVelocity() { return m_velocity; }
    void SetVelocity(Vector2D velocity) { m_velocity = velocity; }

    int GetDamage() { return m_damage; }

    GameObject* GetOwner() const { return m_owner; }

    virtual double GetMovementSpeed() = 0;
    virtual double GetLifetime() = 0;

    IShape2D& GetCollisionShape() override { return m_collisionShape; }

protected:
    GameObject* m_owner = nullptr;
    int m_damage = 1;
    PictureIndex m_image;
    Vector2D m_velocity = { 0,0 };
    Circle2D m_collisionShape;
  
    double m_lifetimeTimer = -1.0;

    bool m_hasHit = false;
};

