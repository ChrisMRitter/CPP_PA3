// This is a custom hitbox class, made to better show off collision and because the default size of the 
// sprites I choose bugs me.

#include "Hitbox.h"

// Constructor for the Hitbox class
Hitbox::Hitbox(sf::Vector2f size, sf::Color color) {
    box.setSize(size);
    box.setOrigin(size / 2.f); // center origin
    box.setFillColor(color);
}

// Set the position of the hitbox
void Hitbox::setPosition(const sf::Vector2f& pos) {
    box.setPosition(pos);
}
// Set the size of the hitbox
void Hitbox::setSize(const sf::Vector2f& size) {
    box.setSize(size);
    box.setOrigin(size / 2.f);
}
// Set the color of the hitbox
void Hitbox::setColor(const sf::Color& color) {
    box.setFillColor(color);
}
//  Get the bounds of the hitbox
sf::FloatRect Hitbox::getBounds() const {
    return box.getGlobalBounds();
}
// Draw the hitbox to the window
void Hitbox::draw(sf::RenderWindow& window) {
    window.draw(box);
}
