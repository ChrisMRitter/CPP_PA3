// This class handles horizontal movement of a spaceship sprite

#include "player.h"
#include <SFML/Graphics.hpp>
#include <iostream>
#include <string>
#include "hitbox.h"
#include "gameconstants.h"


Player::Player(std::string texturePath, float speed) {
    // Load the texture from the file
    if (!texture.loadFromFile(texturePath)) {
        std::cerr << "Error loading texture!" << std::endl;
    }

    // Set the texture to the sprite
    sprite.setTexture(texture);
    sprite.setTextureRect(sf::IntRect(0, 50, 256, 256)); // Adjust if needed

    // Position and scale
    sprite.setPosition(400, 500); // Bottom center of screen
    sprite.setOrigin(32, 32);     // Center the origin
    sprite.setScale(.5, .5);      // Scale up the sprite

    hitbox = new Hitbox(sf::Vector2f(sprite.getGlobalBounds().width * 0.8f,
                                        sprite.getGlobalBounds().height * 0.8f));

    this->speed = speed;
}

//Destructor
Player::~Player() {
    delete hitbox;
}

// Get spaceship's current position
sf::Vector2f Player::getPosition() { return sprite.getPosition(); }

sf::FloatRect Player::getGlobalBounds() { return sprite.getGlobalBounds(); }

void Player::draw(sf::RenderWindow& window) { 
    window.draw(sprite);
    hitbox->draw(window);

}

void Player::update(float dt, std::vector<Laser>& lasers, sf::Vector2f mousePos) {
    timeSinceLastHit += dt; //invuln cooldown iterator
    timeSinceLastLaser += dt; //laser cooldown iterator

     handleInput(dt, lasers, mousePos);

    hitbox->setPosition({sprite.getPosition().x, sprite.getPosition().y + 30});
    
    }

void Player::handleInput(float dt, std::vector<Laser>& lasers, sf::Vector2f mousePos) {
    
    // Handle the input
    sf::Vector2f position = sprite.getPosition();
    float halfWidth = sprite.getGlobalBounds().width / 2.f;
    float halfHeight = sprite.getGlobalBounds().height / 2.f; // For more accurate vertical boundary

    sf::Vector2f movement(0, 0);
    // Vertical movement with screen boundary checks
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::W)) {
        // Why: Don't allow player to move past top edge of screen
        if (position.y - halfHeight > 0) {
            movement.y -= speed;
        }
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::S)) {
        // Why: Don't allow player to move past bottom edge of screen
        if (position.y + halfHeight < VIEW_HEIGHT) {
            movement.y += speed;
        }
    }

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::A)) // moving left
    {
        // Why: Don't allow player to move past left edge of screen
        if (position.x - halfWidth > 0) {
            direction = -1;
            movement.x -= speed;
        }
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::D)) // Moving Right
    {
        // Why: Don't allow player to move past right edge of screen
        if (position.x + halfWidth < VIEW_WIDTH) {
            direction = 1;
            movement.x += speed;
        }	
    }

    sprite.move(movement * dt);

    // if the space bar is pressed, create a laser, and fire it at the mouse
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space) && timeSinceLastLaser > laserCooldown)
    {
        // Get the direction to fire the laser
        sf::Vector2f fireDirection = mousePos - sprite.getPosition();
            
        float temp = sqrt(fireDirection.x * fireDirection.x + fireDirection.y * fireDirection.y);
        if (temp > 0)
        {                
            fireDirection.x /= temp;
            fireDirection.y /= temp;

        }
    
        createLaser(lasers, sprite.getPosition(), fireDirection); // Changed from createSpell
        // Reset the time since the last laser was fired
        timeSinceLastLaser = 0; // Changed from timeSinceLastSpell
    }
        
    // Handle the animation
    handleAnimation(direction, dt);

}
    // Function to create a laser
void Player::createLaser(std::vector<Laser>& lasers, sf::Vector2f playerPos, sf::Vector2f fireDirection) {
        // Create a new laser and add it to the vector
        lasers.push_back(Laser(playerPos.x, playerPos.y, 1000.f, fireDirection));
    }


// Get the player's health
int Player::getHealth() const {
    return health;
}

const Hitbox& Player::getHitbox() const {
    return *hitbox; //Dereference pointer
}

bool Player::canTakeDamage() const {
    return timeSinceLastHit >= damageCooldown; //shorthand if logic
}

void Player::takeDamage(int amount) {
    if (canTakeDamage()) {
        health -= amount;
        timeSinceLastHit = 0.f;
        //TODO: visual feedback code

    }
}

// Function to handle the animation of the player
void Player::handleAnimation(int direction, float dt)
{
    // Increment the elapsed time
    timeSinceLastFrame += dt;

    // If the elapsed time is greater than the time per frame
    if (timeSinceLastFrame > timePerFrame)
    {
        if (curentFrame == 6)
        {
            curentFrame = 0;
        }
        // Set the texture rect to the current frame
        sprite.setTextureRect(sf::IntRect(curentFrame * 64, 0, 64, 64));
        
        // Handle the animation based on the direction
        // 1 being moving right and -1 being moving left
        sprite.setScale(2 * direction, 2);
        
        // Increment the current frame
        curentFrame++;
        
        // Reset the elapsed time
        timeSinceLastFrame = 0;
    }
}
