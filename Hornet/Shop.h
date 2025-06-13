#pragma once
#include "GameObject.h"
#include "ShopItem.h"

class SpaceShip;
class Galaxy;

class Shop : public GameObject
{
public:

    Shop();
    ~Shop();

    void Initialise(SpaceShip* player, Galaxy* galaxy);

    void Update(double frametime) override;

    void Render() override;

    void UponEnter();
    void UponLeave();

    void ForceLeave();

private:

    Rectangle2D m_outline;

    int m_shopScene = 10;

    //Vector storing pointers to each of the shop items (Buy Buttons)
    std::vector<ShopItem*> m_items;

    bool m_uponEnterFlag = false;

    Vector2D PlayerRestockAnchor = Vector2D(0, 400);

};  